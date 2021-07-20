#include <stdio.h>
#include <iostream>
#include "../include/sts/util/Utils.hpp"
#include "../include/sts/search/StandardLayout.hpp"
#include "../include/sts/search/EytzeingerLayout.hpp"
#include "../include/sts/search/VanEmdeBoaLayout.hpp"
#include "../include/sts/search/BTreesLayout.hpp"
#include "../include/dep/branching_binary_search.h"
#include "../include/dep/util.h"


#define RES pow(10, 9)

using namespace sts;
using namespace sts::util;
using namespace sts::search;

int main(int argc, char* argv[]){
    char *dataName, *outputPath, *path, *method, *type, *nKary;
    int align, shuffle, sort;

    std::cout << "Check Parameters..." << std::endl;

    //Print help
    if(Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-h"))
    {
        std::cout << "Help"<< std::endl;
        return 0;
    }

    //Check Path parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-p"))
    {
        path = NULL;
    }else{
        path = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-p");
        if(!path || !strncmp ( path, "-", 1 )){
            path = NULL;
        }
    }


    //Check Dataset name parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-d"))
    {
        std::cerr << "Dataset name missed...Aborting..." << std::endl;
        return 1;
    }else{
        dataName = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-d");
        if(!dataName || !strncmp ( dataName, "-", 1 )){
            std::cerr << "Dataset name missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Check Output filename parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-o"))
    {
        std::cerr << "Output filename missed...Aborting..." << std::endl;
        return 1;
    }else{
        outputPath = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-o");
        if(!outputPath || !strncmp ( outputPath, "-", 1 )){
            std::cerr << "Output file path missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Method Name parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-m"))
    {
        std::cerr << "Method Name Missed...Aborting..." << std::endl;
        return 1;
    }else{
        method = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-m");
        if(!method || !strncmp ( method, "-", 1 )){
            std::cerr << "Method name Missed...Aborting..." << std::endl;
            return 1;
        }
        if(strcmp(method, "kbfs") ){
            std::cerr << "Method name Invalid...Aborting..." << std::endl;
            return 1;   
        }
    }

    //Integer Type parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-t"))
    {
        std::cerr << "Integer Type Missed...Aborting..." << std::endl;
        return 1;
    }else{
        type = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-t");
        if(!type || !strncmp ( type, "-", 1 )){
            std::cerr << "Integer Type Missed...Aborting..." << std::endl;
            return 1;
        }
        cout << type << endl;
        cout <<  strcmp(type, "uint64");
        cout << strcmp(type, "uint32");
        if(strcmp(type, "uint64") && strcmp(type, "uint32")){
            std::cerr << "Integer Type Invalid...Aborting..." << std::endl;
            return 1;   
        }
    }

    //Exponent parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-k"))
    {
        std::cerr << "K-ary value  Missed...Aborting..." << std::endl;
        return 1;
    }else{
        nKary = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-k");
        if(!nKary || !strncmp ( nKary, "-", 1 )){
            std::cerr << "Kary-value Missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Check align memory params
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-a"))
    {
        align=0;
    }else{
        align=1;
    }

    //Check shuffle data params
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-sh"))
    {
        shuffle=0;
    }else{
        shuffle=1;
    }

    //Check sorting data params
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-so"))
    {
        sort=0;
    }else{
        sort=1;
    }


    /*
    *
    * Creating Filename from params   
    * 
    */
    std::stringstream ss;
    std::string wFn, bFn, oFn, QFn, AFn, BFn;
    ss.str("");
    ss.clear();

    if(path == NULL){
        ss << dataName << "_" << type <<"_equality_lookups_1M_single";
        QFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << "Query/" << dataName << "_" << type <<"_equality_lookups_1M_single";
        QFn = ss.str();
        ss.str("");
        ss.clear();
    }
    if(path == NULL){
        ss << dataName << "_" << type;
        AFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << dataName << "_" << type;
        AFn = ss.str();
        ss.str("");
        ss.clear();
    }

    ss << outputPath << dataName << ".std.out" <<".csv";
    oFn = ss.str();
    ss.str("");
    ss.clear();

    cout << oFn << endl;

    /*
    
    Reading Files
    
    */

    uint64_t m,q, nK = atoi(nKary);;
    uint64_t *Al = NULL, *Ql  = NULL, *Vl  = NULL;
    uint32_t *Ai = NULL, *Qi = NULL, *Vi = NULL;
    FILE * out;
    dumdum s[MAX_H+1];
    std::clock_t c_start;
    std::clock_t c_end;

    double timerSearch = 0.0, timerSort = 0.0, timerCon = 0.0, timerRead = 0.0;

    cout << "Creating output file" << endl;
    out = fopen(oFn.c_str(), "r");
    if(out == NULL){
        out = fopen(oFn.c_str(), "w+");
        fprintf(out, "Name, Type, #Elem, Method, K, Sort Time, Mean Sort Time, Construction Time, Mean Construction Time, Query Time, Mean Query Time\n");
    }else{
        fclose(out);
        out = fopen(oFn.c_str(), "a+");
    }

    if(!strcmp(type, "uint64")){
        c_start = std::clock();
        cout << "Reading Input data: AFn" << endl;
        m = Utils<uint64_t>::getDataFromBin(AFn, &Al, align);
        cout << "Reading Query data: QFn" << endl;
        q = Utils<uint64_t>::getDataFromBin(QFn, &Ql, align);
        c_end = std::clock();
    }else{
        c_start = std::clock();
        cout << "Reading Input data: AFn" << endl;
        m = Utils<uint32_t>::getDataFromBin(AFn, &Ai, align);
        cout << "Reading Query data: QFn" << endl;
        q = Utils<uint32_t>::getDataFromBin(QFn, &Qi, align);
        c_end = std::clock();
    }

    timerRead += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    cout << "Read in " << timerRead << " seconds" << endl;

    cout << "Sorting Dataset..." << endl;
    if(!strcmp(type, "uint64")){
        if(shuffle){
            Utils<uint64_t>::shuffle_array(Al, m);
            c_start = std::clock();
            qsort(Al,m, sizeof(unsigned int),Utils<uint64_t>::cmpfunc);
            c_end = std::clock();
        }else if(sort){
            c_start = std::clock();
            qsort(Al,m, sizeof(unsigned int),Utils<uint64_t>::cmpfunc);
            c_end = std::clock(); 
        }else{
            c_start = std::clock();
            c_end = std::clock(); 
        }
    }else{
        if(shuffle){
            Utils<uint32_t>::shuffle_array(Ai, m);
            c_start = std::clock();
            qsort(Ai,m, sizeof(unsigned int),Utils<uint32_t>::cmpfunc);
            c_end = std::clock();
        }else if(sort){
            c_start = std::clock();
            qsort(Ai,m, sizeof(unsigned int),Utils<uint32_t>::cmpfunc);
            c_end = std::clock(); 
        }else{
            c_start = std::clock();
            c_end = std::clock(); 
        }
    }

    timerSort += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    
    uint64_t *res = (uint64_t*)malloc(sizeof(uint64_t)*q);

    if(!strcmp(method, "kbfs")){

        if(!strcmp(type, "uint64")){

            cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i] = StandardLayout<uint64_t>::kBFS(Al,Ql[i], 0, m, nK);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i] = StandardLayout<uint32_t>::kBFS(Ai,Qi[i], 0, m, nK);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }

    // timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

    bool wrong = false;
    if(!strcmp(type, "uint64")){
        for(int j = 0; j < q; j++){
            if(Ql[j] != Al[res[j]]){
                wrong = true;
                // cout << "Errore in " << j << endl;
                // cout << Ql[j] << " " << Al[res[j]] << endl;
            }
        }
    }else{
        for(int j = 0; j < q; j++){
            if(Qi[j] != Ai[res[j]]){
                wrong = true;
                // cout << "Errore in " << j << endl;
                // cout << Qi[j] << " " << Ai[res[j]] << endl;
            }
        }
    }

    if(!wrong){
        
    fprintf(out, "%s, %s, %lu, %s, %lu, %.10e, %.10e, %.10e, %.10e, %.10e, %.10e\n", dataName, type, m, method, nK, timerSort, timerSort/m, timerCon, timerCon/m, timerSearch, timerSearch/q);    
    }else{

    fprintf(out, "%s, %s, %lu, %s, %lu, NAN, NAN, NAN, NAN, NAN, NAN\n", dataName, type, m, method, nK);    
    }
    return 0;
}
