#include <stdio.h>
#include <iostream>
#include "../include/sts/util/Utils.hpp"
#include "../include/sts/search/StandardLayout.hpp"
#include "../include/sts/search/EytzeingerLayout.hpp"
#include "../include/sts/search/VanEmdeBoaLayout.hpp"
#include "../include/sts/search/BTreesLayout.hpp"

#define RES pow(10, 9)

using namespace sts;
using namespace sts::util;
using namespace sts::search;

int main(int argc, char* argv[]){
    char *dataName, *outputPath, *path, *method, *type;
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
        if(strcmp(type, "uint64") && strcmp(type, "uint32")){
            std::cerr << "Integer Type Invalid...Aborting..." << std::endl;
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
    std::string wFn, bFn, oFn, QFn, AFn, BFn, OFn;
    ss.str("");
    ss.clear();

    if(path == NULL){
        ss << dataName << "_" << type <<"_equality_lookups_1M_single";
        QFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << dataName << "_" << type <<"_equality_lookups_1M_single";
        QFn = ss.str();
        ss.str("");
        ss.clear();
    }
    if(path == NULL){
        ss << dataName << "_" << type <<"_equality_lookups_100k_single";
        OFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << dataName << "_" << type <<"_equality_lookups_100k_single";
        OFn = ss.str();
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

    /*

    Reading Files

    */

    uint64_t m,q;
    uint64_t *Al, *Ql, *Vl;
    uint32_t *Ai, *Qi, *Vi;
    FILE * out;
    dumdum s[MAX_H+1];
    std::clock_t c_start;
    std::clock_t c_end;

    double timerSearch = 0.0, timerSort = 0.0, timerCon = 0.0, timerRead = 0.0;

    if(!strcmp(type, "uint64")){
        c_start = std::clock();
        cout << "Reading Input data: AFn" << endl;
        m = Utils<uint64_t>::getDataFromBin(AFn, &Al, align);
        Utils<uint64_t>::extract100kQuery(Al,m, QFn,OFn);
        c_end = std::clock();
    }else{
        c_start = std::clock();
        cout << "Reading Input data: AFn" << endl;
        m = Utils<uint32_t>::getDataFromBin(AFn, &Ai, align);
        Utils<uint32_t>::extract100kQuery(Ai, m, QFn, OFn);
        c_end = std::clock();
    }



    timerRead += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    cout << "Read in " << timerRead << " seconds" << endl;

    return 0;
}
