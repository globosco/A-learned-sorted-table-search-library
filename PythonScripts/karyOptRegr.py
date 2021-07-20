import numpy as np
import h5py
import math
from sklearn.linear_model import LinearRegression
import json
from time import process_time_ns
from time import perf_counter_ns
import os
import argparse
import matplotlib.pyplot as plt

#Set Argument Parser for input information
parser = argparse.ArgumentParser(description='Train NN for Datastructure index')
parser.add_argument('-i', '--input', dest="inputFile", default="fb_200M_uint64_L4",
                    help='Input file name')
parser.add_argument('-id', '--inputDir', dest="inputDir", default="./inputfileshdf5/",
help='Input file path')
parser.add_argument('-n', '--name', dest="fileName", default="L4_fb_200M_uint64",
                    help='Input file name')
parser.add_argument('-o', '--output', dest="outputFile", default="L4_fb_200M_uint64_kary",
                    help='Output file name')
parser.add_argument('-od', '--outputDir', dest="outputDir", default="./outputjson/",
                    help='Output file path')
parser.add_argument('-k', '--exponent', dest="k", default="1",
                    help='Output file path')


args = parser.parse_args()
k = int(args.k)
outputFile = args.outputFile + "_" + str(k) + ".json"
if(os.path.exists(os.path.join(args.outputDir, outputFile))):
    with open(os.path.join(args.outputDir, outputFile)) as json_file:
        resJson = json.load(json_file)
else:
    resJson = {}

if(args.fileName not in resJson.keys()):
    resJson[args.fileName] = {}

print("Reading "+args.inputFile+".mat")
with h5py.File(os.path.join(args.inputDir, args.inputFile+".mat"),'r') as f:
    data = f.get('M')
    bin_data = np.array(data, dtype=np.float64)
    bin_data = np.transpose(bin_data)

dim_set = len(bin_data)
#bin_data = np.concatenate((bin_data, np.ones((dim_set,1))), axis=1)
#print(np.array_str(bin_data, precision=2, suppress_small=True))

labels_index = np.linspace(1, len(bin_data), num=len(bin_data), dtype=np.float64)
labels = labels_index/len(bin_data)
labels = np.reshape(labels, (-1, 1))

plt.plot(bin_data, labels)
#plt.show()

m = np.max(bin_data)
#print(m)

#p = np.random.permutation(dim_set)

#bin_data = bin_data[p]
#labels_index = labels_index[p]
#labels = labels[p]
#k = 4
exp = [1,2,3]
rfBestList = []
timeBestList = []
segDim = 0
timeList = []
for j in range(1,k+1):
    if j == 1:
        segmentLeft = 0
    else:
        segmentLeft = segmentRight
    print("{}, {}, {}, {}".format(j,dim_set,k,j * (dim_set) / k))
    segmentRight = math.ceil(j * (dim_set) / k)+1
    if(segmentRight >= dim_set):
        segmentRight = dim_set

    print("Seg {}: [{},{}]".format(j, segmentLeft, segmentRight))
    segDim += segmentRight-segmentLeft+1

    segLabels = labels[segmentLeft:segmentRight]
    segLabels_index = labels_index[segmentLeft:segmentRight]
    segData = np.reshape(bin_data[segmentLeft:segmentRight], (segmentRight-segmentLeft))
    resJson[args.fileName][str(j)] = {}
    rfList = []

#Contiene tutti i tempi
    for e in exp:
        X=[]
        for i in range(e, -1, -1):
            X.append(segData**i)
        X = np.array(X)
        X = np.transpose(X)

        #print(np.array_str(X, precision=2, suppress_small=True))

        #print(labels.shape)
        #print(bin_data.shape)
        #print(X.shape)

        print("Performing Polinomial Regression...")
        start = perf_counter_ns()
        w = np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(segLabels)
        end  = perf_counter_ns()

        print("TEMPO PRIMA:{}".format(start))
        print("TEMPO DOPO:{}".format(end))

        print("TEMPO Trascorso:{}".format(end - start))



        #print(w.shape)
        #print(labels)
        pred = np.transpose(w).dot(np.transpose(X))
        pred = np.transpose(pred)

        #if(2001-segmentLeft <= len(pred)):
            #print("Pred[2001]: {}".format(pred[2001-segmentLeft]*len(pred)))

        #plt.plot(bin_data, pred)
        #plt.show()

        l_pred = np.floor(pred * dim_set)
        l_pred=np.reshape(l_pred,(len(segData)))
        print(np.amax(np.abs(segLabels_index - l_pred)))
        print(np.amax(np.abs(segLabels_index - l_pred)))
        max_err = np.amax(np.abs(segLabels_index - l_pred))

        # if(j == 5):
        #     for i in range(0,len(segData)):
        #         print("ERRORE: {} -> {} -> {}".format(segLabels_index[i], l_pred[i], segLabels_index[i] - l_pred[i]))

        print("MAX ERROR Seg{} with exp {}:{}".format(j, e, max_err))
        rf = (1 - (2*max_err/dim_set))*100
        if(rf < 0):
            rf = 0
        rfList.append(rf)
        if(j in resJson[args.fileName].keys() and e in resJson[args.fileName][j].keys()):
            del resJson[args.fileName][j][e]
            del resJson[args.fileName][j]
        elif(j in resJson[args.fileName].keys()):
            del resJson[args.fileName][j]


        resJson[args.fileName][str(j)][str(e)] = {}

        resJson[args.fileName][str(j)][str(e)]["Overhead"] = max_err/len(bin_data)*100
        resJson[args.fileName][str(j)][str(e)]["Space"] = w.nbytes/bin_data.nbytes
        resJson[args.fileName][str(j)][str(e)]["epsilon"] = max_err
        resJson[args.fileName][str(j)][str(e)]["Reduction Factor %"] = rf
        resJson[args.fileName][str(j)][str(e)]["time"] = (end - start)/10**9
        timeList.append((end - start)/10**9)
        #Check bug
        # print("CIAOOOOOOOOO {}".format(e))
        # if(j == 5 and e == 2):
        #     prova = [5183352367578909696**2, 5183352367578909696,1]
        #     pred = np.transpose(w).dot(np.transpose(prova))
        #     print(("PROVA: {}".format(np.floor(pred*len(bin_data)))))

        resJson[args.fileName][str(j)][str(e)]["w"] = {}
        wi = e
        for weight in w:
            resJson[args.fileName][str(j)][str(e)]["w"][str(wi)] = weight[0]
            wi -= 1

        print("Epsilon: {}".format(resJson[args.fileName][str(j)][str(e)]["epsilon"]))
        print("Reduction Factor: {}".format(resJson[args.fileName][str(j)][str(e)]["Reduction Factor %"]))
        print("Training Time: {}".format(resJson[args.fileName][str(j)][str(e)]["time"]))

    modelBest = np.argmax(rfList)+1
    rfBestList.append(resJson[args.fileName][str(j)][str(modelBest)]["Reduction Factor %"])
    timeBestList.append(resJson[args.fileName][str(j)][str(modelBest)]["time"])
    resJson[args.fileName][str(j)]["best_model"]= int(modelBest)
print(timeBestList)
print(np.sum(timeBestList))
print(rfBestList)
print(np.mean(rfBestList))
meanRf = np.mean(rfBestList)
#timeTot = np.sum(timeBestList)
timeTot = np.sum(timeList)
resJson[args.fileName]["Mean Readuction Factor"] = meanRf
resJson[args.fileName]["Time"] = timeTot
resJson[args.fileName]["meanTime"] = timeTot / dim_set

with open(os.path.join(args.outputDir, outputFile), 'w') as outfile:
    json.dump(resJson, outfile)

print("DIM:{}".format(dim_set))
print("DIM With SEG:{}".format(segDim))
