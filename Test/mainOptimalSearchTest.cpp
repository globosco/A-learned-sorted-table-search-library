#include <stdio.h>
#include <iostream>
#include "../include/sts/util/Utils.hpp"
#include "../include/sts/search/StandardLayout.hpp"
#include "../include/sts/search/EytzeingerLayout.hpp"
#include "../include/sts/search/VanEmdeBoaLayout.hpp"
#include "../include/sts/search/BTreesLayout.hpp"
#include "../include/sts/search/LrLearneadDs.hpp"
#include "../include/sts/search/QrLearneadDS.hpp"
#include "../include/sts/search/CrLearneadDS.hpp"

#define RES pow(10, 9)

using namespace sts;
using namespace sts::util;
using namespace sts::search;

int main(int argc, char* argv[]){
    char *dataName, *outputPath, *path, *method, *type, *params, *exponent;
    int best, align, shuffle, sort;

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
        if(strcmp(method, "bbs") && strcmp(method, "bfs") && strcmp(method, "ibs")){
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
        if(strcmp(type, "uint64") && strcmp(type, "uint32")){
            std::cerr << "Integer Type Invalid...Aborting..." << std::endl;
            return 1;
        }
    }

    //Regression Params File parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-w"))
    {
        std::cerr << "Regression Params File  Missed...Aborting..." << std::endl;
        return 1;
    }else{
        params = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-w");
        if(!params || !strncmp ( params, "-", 1 )){
            std::cerr << "Regression Params File  Missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Exponent parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-e"))
    {
        std::cerr << "Exponentt  Missed...Aborting..." << std::endl;
        return 1;
    }else{
        exponent = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-e");
        if(!exponent || !strncmp ( exponent, "-", 1 )){
            std::cerr << "Exponent Missed...Aborting..." << std::endl;
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

    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-b"))
    {
        best=0;
    }else{
        best=1;
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
    std::string wFn, oFn, QFn, AFn;
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

    if(path == NULL){
        ss << params << "_kary_" << string(exponent) <<".json";
        wFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << "json/" << params << "_kary_" << string(exponent) <<".json";
        wFn = ss.str();
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

    uint64_t m,q, *eps, exp = atoi(exponent);
    uint64_t *Al, *Ql;
    uint32_t *Ai, *Qi;
    double **w;
    uint64_t *esp;
    FILE * out;
    std::clock_t c_start;
    std::clock_t c_end;
    string obfs;
    uint64_t *intervalArray;
    uint64_t intervalsSum=0;

    double timerSearch = 0.0, timerSort = 0.0, timerCon = 0.0, timerRead = 0.0;

    cout << "Creating output file" << endl;
    out = fopen(oFn.c_str(), "r");
    if(out == NULL){
        out = fopen(oFn.c_str(), "w+");
        fprintf(out, "Name, Type, #Elem, Method, RF, Sort Time, Mean Sort Time, Construction Time, Mean Construction Time, Query Time, Mean Query Time\n");
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

    if(best){
        c_start = std::clock();
        Utils<uint64_t>::readOBFSParamsFromJson(wFn, params, exp, &w, &esp, &eps);
        c_end = std::clock();
        obfs = string(exponent)  + "-O-BFS BEST";
        cout <<  obfs << endl;
    }else{
        c_start = std::clock();
        Utils<uint64_t>::readOBFSLinParamsFromJson(wFn, params, &w, &esp, &eps);
        c_end = std::clock();
        obfs = "O.BFS Linear";
    }
    // for(int i = 0; i < 3; i++){
    //     cout << eps[i] << endl;
    // }

    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j <= esp[i]; j++){
    //         cout << w[i][j] << endl;
    //     }
    //     cout << endl;
    // }


    timerRead += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    cout << "Read in " << timerRead << " seconds" << endl;
    OBFSseg<uint64_t> segl[exp];
    OBFSseg<uint32_t> segi[exp];
    
    if(!strcmp(type, "uint64")){
        uint64_t left = 0;
        uint64_t right = m-1;
        uint64_t segmentLeft = 0;
        uint64_t segmentRight = left + ((right - left)) / exp + 1;

        for(int i = 0; i < exp; i++){

            // cout << "Segment: " << segmentLeft << " " << segmentRight << endl;

            switch (esp[i])
            {
            case 1:
                segl[i].linear = new LrLearnedDs<uint64_t>(Al,m, eps[i], w[i]);
                segl[i].quadratic = nullptr;
                segl[i].cubic = nullptr;
                break;
            case 2:
                segl[i].linear = nullptr;
                segl[i].quadratic = new QrLearnedDs<uint64_t>(Al,m, eps[i], w[i]);
                segl[i].cubic = nullptr;
                break;
            case 3:
                segl[i].linear = nullptr;
                segl[i].quadratic = nullptr;
                segl[i].cubic = new CrLearnedDs<uint64_t>(Al,m, eps[i], w[i]);
                break;
            }
            segl[i].left = segmentLeft;
            segl[i].right = segmentRight;
            segmentLeft = segmentRight + 1;
            segmentRight = left + ((i+2) * (right - left)) / exp + 1;

        }
    }else{

        uint64_t left = 0;
        uint64_t right = m-1;
        uint64_t segmentLeft = 0;
        uint64_t segmentRight = left + ((right - left)) / exp;

        for(int i = 0; i < exp; i++){
            // cout << "Segment" << i+1 <<": " << segmentLeft << " " << segmentRight << endl;
            switch (esp[i])
            {
            case 1:
                segi[i].linear = new LrLearnedDs<uint32_t>(Ai,m, eps[i], w[i]);
                segi[i].quadratic = nullptr;
                segi[i].cubic = nullptr;
                break;
            case 2:
                segi[i].linear = nullptr;
                segi[i].quadratic = new QrLearnedDs<uint32_t>(Ai,m, eps[i], w[i]);
                segi[i].cubic = nullptr;
                break;
            case 3:
                segi[i].linear = nullptr;
                segi[i].quadratic = nullptr;
                segi[i].cubic = new CrLearnedDs<uint32_t>(Ai,m, eps[i], w[i]);
                break;
            }
            segl[i].left = segmentLeft;
            segl[i].right = segmentRight;
            segmentLeft = segmentRight + 1;
            segmentRight = left + ((i+2) * (right - left)) / exp;

        }
    }
    
    intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
    uint64_t *res = (uint64_t *)calloc(q, sizeof(uint64_t));
    // Start Query
    c_start = std::clock();
    if(!strcmp(type, "uint64")){
        for(int j = 0; j < q; j++){
            uint64_t left = 0;
            uint64_t right = m-1;
            uint64_t segmentLeft = 0;
            uint64_t segmentRight = left + ((right - left)) / exp;

            int s = 0;
    
            for (uint64_t i = 0; i < exp; ++i)
            {
                // cout << "Query:" << Ql[j] << endl;
                // cout << "Segment Interval: " << Al[segmentLeft] << " " << Al[segmentRight] << endl;
                // cout << "Segment " << s << ": " << segmentLeft << " " << segmentRight << endl;
                if (Ql[j] <= Al[segmentRight]){
                    switch (esp[i])
                    {
                    case 1:
                        res[j] = *(segl[i].linear->search(&Ql[j], 1, method, &intervalArray[j]));
                        break;
                    case 2:
                        res[j] = *(segl[i].quadratic->search(&Ql[j], 1, method, &intervalArray[j]));
                        break;
                    case 3:
                        res[j] = *(segl[i].cubic->search(&Ql[j], 1, method, &intervalArray[j]));
                        break;
                    }
                    break;
                }
                segmentLeft = segmentRight + 1;
                segmentRight = left + ((i+2) * (right - left)) / exp;
                s++;
            }
        }
    }else{
        for(int j = 0; j < q; j++){
            uint64_t left = 0;
            uint64_t right = m-1;
            uint64_t segmentLeft = 0;
            uint64_t segmentRight = left + ((right - left)) / exp;
            int s = 0;
    
            for (uint64_t i = 0; i < exp; ++i)
            {
                // cout << "Query:" << Qi[j] << endl;
                // cout << "Segment Interval: " << Ai[segmentLeft] << " " << Ai[segmentRight] << endl;
                // cout << "Segment " << i << ": " << segmentLeft << " " << segmentRight << endl;
                if (Qi[j] <= Ai[segmentRight]){
                    switch (esp[i])
                    {
                    case 1:
                        // cout << Ai[*(segi[i].linear->search(&Qi[j], 1, method, &intervalArray[j]))] << endl;
                        res[j] = *(segi[i].linear->search(&Qi[j], 1, method, &intervalArray[j]));
                        break;
                    case 2:
                        // cout << Ai[*(segi[i].quadratic->search(&Qi[j], 1, method, &intervalArray[j]))] << endl;
                        res[j] = *(segi[i].quadratic->search(&Qi[j], 1, method, &intervalArray[j]));
                        break;
                    case 3:
                        // cout << Ai[*(segi[i].cubic->search(&Qi[j], 1, method, &intervalArray[j]))] << endl;
                        res[j] = *(segi[i].cubic->search(&Qi[j], 1, method, &intervalArray[j]));
                        break;
                    }
                    break;
                }
                segmentLeft = segmentRight + 1;
                segmentRight = left + ((i+2) * (right - left)) / exp;
                s++;
            }
        }
    }
    c_end = std::clock();
    
    for(int i = 0; i<q;i++){
      //cout << intervalArray[i] << endl;
      intervalsSum += intervalArray[i];
    }

    bool wrong = false;
    // if(!strcmp(type, "uint64")){
    //     for(int j = 0; j < q; j++){
    //         if(Ql[j] != Al[res[j]]){
    //             wrong = true;
    //             std::cout << "Errore in " << j << endl;
    //             std::cout << Ql[j] << " " << Al[res[j]] << endl;
    //         }
    //     }
    // }else{
    //     for(int j = 0; j < q; j++){
    //         if(Qi[j] != Ai[res[j]]){
    //             wrong = true;
    //             std::cout << "Errore in " << j << endl;
    //             std::cout << Qi[j] << " " << Ai[res[j]] << endl;
    //         }
    //     }
    // }
    
    cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';
    
    timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    if(wrong){
        fprintf(out, "%s, %s, %lu, %s, %.2lf, NAN, NAN, NAN, NAN, NAN, NAN\n", dataName, type, m, obfs.c_str(), (1-((double)intervalsSum/q)/m)*100);
    }else{
        fprintf(out, "%s, %s, %lu, %s, %.2lf, %.10e, %.10e, %.10e, %.10e, %.10e, %.10e\n", dataName, type, m, obfs.c_str(), (1-((double)intervalsSum/q)/m)*100, timerSort, timerSort/m, timerCon, timerCon/m, timerSearch, timerSearch/q);
    }

    return 0;
}
