#include <stdio.h>
#include <iostream>
#include "../include/sts/util/Utils.hpp"
#include "../include/sts/search/StandardLayout.hpp"
#include "../include/sts/search/EytzeingerLayout.hpp"
#include "../include/sts/search/VanEmdeBoaLayout.hpp"
#include "../include/sts/search/BTreesLayout.hpp"
#include "../include/sts/search/LrLearnedDS.hpp"
#include "../include/sts/search/QrLearnedDS.hpp"
#include "../include/sts/search/CrLearnedDS.hpp"

#define RES pow(10, 9)

using namespace sts;
using namespace sts::util;
using namespace sts::search;

int main(int argc, char* argv[]){
    char *dataName, *outputPath, *path, *method, *type, *params, *exponent, *jsonPath;
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

    //Check JSON filename parameter
    if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-j"))
    {
        std::cerr << "JSON filename missed...Aborting..." << std::endl;
        return 1;
    }else{
        jsonPath = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-j");
        if(!jsonPath || !strncmp ( jsonPath, "-", 1 )){
            std::cerr << "JSIN file path missed...Aborting..." << std::endl;
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
        if(strcmp(method, "bbs") && strcmp(method, "bfs") && strcmp(method, "ibs") && strcmp(method, "tip")
        && strcmp(method, "kbs")){
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

    // //Regression Exponent parameter
    // if(!Utils<uint32_t>::cmdOptionExists(argv, argv+argc, "-e"))
    // {
    //     std::cerr << "Exponent  Missed...Aborting..." << std::endl;
    //     return 1;
    // }else{
    //     exponent = Utils<uint32_t>::getCmdOption(argv, argv + argc, "-e");
    //     if(!exponent || !strncmp ( exponent, "-", 1 )){
    //         std::cerr << "Exponent  Missed...Aborting..." << std::endl;
    //         return 1;
    //     }
    // }

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
    std::string wFn, oFn, QFn, AFn, jFn;
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
        ss << params;
        wFn = ss.str();
        ss.str("");
        ss.clear();
    }else{
        ss << path << params;
        wFn = ss.str();
        ss.str("");
        ss.clear();
    }

    ss << jsonPath << dataName << "_" << type;
    jFn = ss.str();
    ss.str("");
    ss.clear();

    ss << outputPath << dataName << ".rgr.out" <<".csv";
    oFn = ss.str();
    ss.str("");
    ss.clear();

    cout << oFn << endl;

    /*

    Reading Files

    */

    uint64_t m,q, eps, exp;
    uint64_t *Al, *Ql;
    uint32_t *Ai, *Qi;
    double *w;
    FILE * out;
    std::clock_t c_start;
    std::clock_t c_end;
    uint64_t *intervalArray;
    uint64_t intervalsSum=0;

    double timerSearch = 0.0, timerSort = 0.0, timerCon = 0.0, timerRead = 0.0;

    cout << "Creating output file" << endl;
    out = fopen(oFn.c_str(), "r");
    if(out == NULL){
        out = fopen(oFn.c_str(), "w+");
        fprintf(out, "Name, Type, #Elem, Regr Exponent, Method, Epsilon, RF, Sort Time, Mean Sort Time, Query Time, Mean Query Time\n");
    }else{
        fclose(out);
        out = fopen(oFn.c_str(), "a+");
    }

    if(!strcmp(type, "uint64")){
        c_start = std::clock();
        cout << "Reading Input data: " << AFn << endl;
        m = Utils<uint64_t>::getDataFromBin(AFn, &Al, align);
        cout << "Reading Query data: " << QFn << endl;
        q = Utils<uint64_t>::getDataFromBin(QFn, &Ql, align);
        cout << "Reading Regression Params: " << wFn << endl;
        exp = Utils<uint64_t>::readRegrParams(wFn, &w);
        cout << "Reading Epsilon Params: " << jFn << endl;
        eps = Utils<uint64_t>::readEpsilon(jFn, exp-1);
        c_end = std::clock();

    }else{
        c_start = std::clock();
        cout << "Reading Input data: " << AFn << endl;
        m = Utils<uint32_t>::getDataFromBin(AFn, &Ai, align);
        cout << "Reading Query data: " << QFn << endl;
        q = Utils<uint32_t>::getDataFromBin(QFn, &Qi, align);
        cout << "Reading Regression Params: " << wFn << endl;
        exp = Utils<uint32_t>::readRegrParams(wFn, &w);
        cout << "Reading Epsilon Params: " << jFn << endl;
        eps = Utils<uint32_t>::readEpsilon(jFn, exp-1);
        c_end = std::clock();
    }

    timerRead += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    cout << "Read in " << timerRead << " seconds" << endl;
    cout << "Exponent Regression: " << exp-1 << endl;
    cout << "Parameters:" << endl;

    for(int i = 0; i < exp; i ++){
        cout << "W[" << i << "]: " << w[i] << endl;
    }
    cout << "b: " << w[exp-1] << endl;

    cout << "With Epsilon: " << eps << endl;

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

    LrLearnedDs<uint64_t> *linRgr;
    QrLearnedDs<uint64_t> *qdrRgr;
    CrLearnedDs<uint64_t> *cbcRgr;
    LrLearnedDs<uint32_t> *linRgrInt;
    QrLearnedDs<uint32_t> *qdrRgrInt;
    CrLearnedDs<uint32_t> *cbcRgrInt;

    cout << "Quering..." << endl;
    uint64_t *res;
    if(!strcmp(type, "uint64")){
        switch (exp)
        {
        case 2:
            cout << "Linear" << endl;
            linRgr = new LrLearnedDs<uint64_t>(Al, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = linRgr->search(Ql,q, method, intervalArray);

            c_end = std::clock();
            for(int i = 0; i<q;i++){
              //cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';

            break;
        case 3:
            qdrRgr = new QrLearnedDs<uint64_t>(Al, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = qdrRgr->search(Ql,q, method, intervalArray);

            c_end = std::clock();

            for(int i = 0; i<q;i++){
              //cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';
            break;
        case 4:
            cbcRgr = new CrLearnedDs<uint64_t>(Al, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = cbcRgr->search(Ql,q, method, intervalArray);

            c_end = std::clock();

            for(int i = 0; i<q;i++){
              //cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';
            break;
        default:
            std::stringstream errMsg;
            errMsg << "Exponent " << exp <<" not allowed...";
            throw std::runtime_error(errMsg.str().c_str());
            break;
        }
    }else{
        switch (exp)
        {
        case 2:
            linRgrInt = new LrLearnedDs<uint32_t>(Ai, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = linRgrInt->search(Qi,q, method, intervalArray);

            c_end = std::clock();


            for(int i = 0; i<q;i++){
              //cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';

            break;
        case 3:
            cout << "Eseguo Ricerca Quadratic..." << endl;
            qdrRgrInt = new QrLearnedDs<uint32_t>(Ai, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = qdrRgrInt->search(Qi,q, method,intervalArray);

            c_end = std::clock();

            for(int i = 0; i<q;i++){
            //   cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';
            break;
        case 4:
            cbcRgrInt = new CrLearnedDs<uint32_t>(Ai, m, eps, w);
            intervalArray = (uint64_t*)calloc(q, sizeof(uint64_t));
            c_start = std::clock();

            res = cbcRgrInt->search(Qi,q, method,intervalArray);

            c_end = std::clock();

            for(int i = 0; i<q;i++){
              //cout << intervalArray[i] << endl;
              intervalsSum += intervalArray[i];
            }

            cout << intervalsSum << " " << intervalsSum/q << " " << m << '\n';
            break;
        default:
            std::stringstream errMsg;
            errMsg << "Exponent " << exp <<" not allowed...";
            throw std::runtime_error(errMsg.str().c_str());
            break;
        }
    }

    bool wrong = false;
    if(!strcmp(type, "uint64")){
        for(int j = 0; j < q; j++){
            if(Ql[j] != Al[res[j]]){
                wrong = true;
                // std::cout << "Errore in " << j << endl;
                // std::cout << Ql[j] << " " << Al[res[j]] << endl;
            }
        }
    }else{
        for(int j = 0; j < q; j++){
            if(Qi[j] != Ai[res[j]]){
                wrong = true;
                // std::cout << "Errore in " << j << endl;
                // std::cout << Qi[j] << " " << Ai[res[j]] << endl;
            }
        }
    }

    timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    // fprintf(out, "%s", dataName);
    if(wrong){
        fprintf(out, "%s, %s, %lu, %lu, %s, %lu, %.2lf, NAN, NAN, NAN, NAN\n", dataName, type, m, exp-1, method, eps, (1-((double)intervalsSum/q)/m)*100);
    }else{

        fprintf(out, "%s, %s, %lu, %lu, %s, %lu, %.2lf, %.10e, %.10e, %.10e, %.10e\n", dataName, type, m, exp-1, method, eps, (1-((double)intervalsSum/q)/m)*100, timerSort, timerSort/m, timerSearch, timerSearch/q);
    }
    return 0;
}
