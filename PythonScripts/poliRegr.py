import numpy as np 
import h5py 
from sklearn.linear_model import LinearRegression
import json
from time import process_time_ns
from time import perf_counter_ns
import os
import argparse
import matplotlib.pyplot as plt
import sys

#Set Argument Parser for input information
parser = argparse.ArgumentParser(description='Train NN for Datastructure index')
parser.add_argument('-i', '--input', dest="inputFile", default="logn03.sorted",
                    help='Input file name')
parser.add_argument('-id', '--inputDir', dest="inputDir", default="./Resource",
help='Input file path')
parser.add_argument('-n', '--name', dest="fileName", default="logn03.sorted",
                    help='Input file name')
parser.add_argument('-o', '--output', dest="outputFile", default="polinomial_regression.json",
                    help='Output file name')
parser.add_argument('-e', '--exp', dest="poliExp", default=2,
                    help='Polynomial degree')
parser.add_argument('-od', '--outputDir', dest="outputDir", default="./Result",
                    help='Output file path')


args = parser.parse_args()


if(os.path.exists(os.path.join(args.outputDir, args.outputFile))):
    with open(os.path.join(args.outputDir, args.outputFile)) as json_file:
        resJson = json.load(json_file)
else:
    resJson = {}

if(args.fileName not in resJson.keys()):
    resJson[args.fileName] = {}
    resJson[args.fileName][args.poliExp] = {}
elif(args.poliExp not in resJson[args.fileName].keys()):
    resJson[args.fileName][args.poliExp] = {}

print("Reading "+args.inputFile+".mat")
with h5py.File(os.path.join(args.inputDir, args.inputFile+".mat"),'r') as f:
    data = f.get('M')
    bin_data = np.array(data, dtype=np.float64)
    bin_data = np.transpose(bin_data)

dim_set = len(bin_data)
set_byte = bin_data.nbytes
#bin_data = np.concatenate((bin_data, np.ones((dim_set,1))), axis=1)
#print(np.array_str(bin_data, precision=2, suppress_small=True))

labels_index = np.linspace(1, len(bin_data), num=len(bin_data), dtype=np.float64)
labels = labels_index/len(bin_data)
labels = np.reshape(labels, (-1, 1))
#j = 0
#for i in labels:
    #if(int(i[0]) == 8198720063600608256):
#    print("{}: {:f}".format(j, i[0]))
#    j = j+1

#for i in labels:
#    print("{:f}".format(i[0]))

#plt.plot(bin_data, labels)
#plt.show()

m = np.max(bin_data)
#print(m)

#p = np.random.permutation(dim_set)

#bin_data = bin_data[p]
#labels_index = labels_index[p]
#labels = labels[p]

X=[]
bin_data = np.reshape(bin_data, (dim_set))
for i in range(int(args.poliExp), -1, -1):
    X.append(bin_data**i)
X = np.array(X)
X = np.transpose(X)

del bin_data

#j=0
#for i in X:
    #if(int(i[0]) == 8198720063600608256):
#    print("{}: [{:f}, 1]".format(j, i[0]))
#    j = j+1


#print(np.array_str(X, precision=2, suppress_small=True))

#print(labels.shape)
#print(bin_data.shape)
print(X.shape)

print("Performing Polinomial Regression...")
#start = process_time_ns()
start = perf_counter_ns()
w = np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(labels)
end = perf_counter_ns()
#end = process_time_ns()

print("TEMPO PRIMA:{}".format(start))
print("TEMPO DOPO:{}".format(end))

print(print("TEMPO Trascorso:{}".format(end - start)))

#print(w.shape)
#print(labels)
#print(X.shape)
#print(w.shape)
pred = np.matmul(X,w)
#print(pred.shape)
#pred = np.transpose(pred)
#print(pred)
#j = 0
#for i in pred:
    #if(int(i[0]) == 8198720063600608256):
    #print("{}: [{:f}, 1]".format(j, i[0]))
    #j = j+1

#print(w.size)
#print(X.size)
#plt.plot(bin_data, pred)
#plt.show()

l_pred = np.floor(pred * dim_set)
l_pred=np.reshape(l_pred,(dim_set))
#print(pred[0])
#print(l_pred[0])

#for i in range(0, dim_set):
    #print(X[i,0])
#    if(int(X[i,0]) == 8198720063600608256):
#    	print("PRED:{} REAL: {} ERR: {}".format(l_pred[i], labels_index[i], l_pred[i]-labels_index[i]))
#print(np.amax(np.abs(labels_index - l_pred)))
maximum = 0
for i in np.abs(labels_index - l_pred):
   if(i>maximum): 
       maximum = i
print(maximum)
print(np.abs(labels_index - l_pred))
max_err = np.amax(np.abs(labels_index - l_pred))
print("Errore:{}".format(max_err))
resJson[args.fileName][args.poliExp]["Overhead"] = max_err/dim_set*100
resJson[args.fileName][args.poliExp]["Space"] = w.nbytes/set_byte 
resJson[args.fileName][args.poliExp]["epsilon"] = max_err
resJson[args.fileName][args.poliExp]["Reduction Factor %"] = (1 - (2*max_err/dim_set))*100
resJson[args.fileName][args.poliExp]["time"] = (end - start)/10**9
resJson[args.fileName][args.poliExp]["meanTime"] = resJson[args.fileName][args.poliExp]["time"]/dim_set

print("Epsilon: {}".format(resJson[args.fileName][args.poliExp]["epsilon"]))
print("Reduction Factor: {}".format(resJson[args.fileName][args.poliExp]["Reduction Factor %"]))
print("Training Time: {}".format(resJson[args.fileName][args.poliExp]["time"]))
#prova = [ 8198720063600608256, 1]
#pred = np.transpose(w).dot(np.transpose(prova))

print((pred))
print(np.floor(pred * dim_set))
with open(os.path.join(args.outputDir, "pr_w"+str(args.poliExp)+"_"+args.fileName+".dat"),'w') as f:
    for weight in w:
        print(weight[0])
        f.write(str(weight[0])+"\n")

with open(os.path.join(args.outputDir, args.outputFile), 'w') as outfile:
    json.dump(resJson, outfile)
