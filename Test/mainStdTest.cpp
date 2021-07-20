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
        if(strcmp(method, "BS") && strcmp(method, "bbs") && strcmp(method, "bfs") 
            && strcmp(method, "ibs") && strcmp(method, "bfe") && strcmp(method, "bfv") &&
           strcmp(method, "bft512") && strcmp(method, "bft32k") 
           && strcmp(method, "tip") && strcmp(method, "kbs") ){
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
        std::cout << type << endl;
        std::cout <<  strcmp(type, "uint64");
        std::cout << strcmp(type, "uint32");
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

    std::cout << oFn << endl;

    /*
    
    Reading Files
    
    */

    uint64_t m,q;
    uint64_t *Al = NULL, *Ql  = NULL, *Vl  = NULL;
    uint32_t *Ai = NULL, *Qi = NULL, *Vi = NULL;
    FILE * out;
    dumdum s[MAX_H+1];
    std::clock_t c_start;
    std::clock_t c_end;

    double timerSearch = 0.0, timerSort = 0.0, timerCon = 0.0, timerRead = 0.0;

    std::cout << "Creating output file" << endl;
    out = fopen(oFn.c_str(), "r");
    if(out == NULL){
        out = fopen(oFn.c_str(), "w+");
        fprintf(out, "Name, Type, #Elem, Method, Sort Time, Mean Sort Time, Construction Time, Mean Construction Time, Query Time, Mean Query Time\n");
    }else{
        fclose(out);
        out = fopen(oFn.c_str(), "a+");
    }

    if(!strcmp(type, "uint64")){
        c_start = std::clock();
        std::cout << "Reading Input data: AFn" << endl;
        m = Utils<uint64_t>::getDataFromBin(AFn, &Al, align);
        std::cout << "Reading Query data: QFn" << endl;
        q = Utils<uint64_t>::getDataFromBin(QFn, &Ql, align);
        c_end = std::clock();
    }else{
        c_start = std::clock();
        std::cout << "Reading Input data: AFn" << endl;
        m = Utils<uint32_t>::getDataFromBin(AFn, &Ai, align);
        std::cout << "Reading Query data: QFn" << endl;
        q = Utils<uint32_t>::getDataFromBin(QFn, &Qi, align);
        c_end = std::clock();
    }

    timerRead += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    std::cout << "Read in " << timerRead << " seconds" << endl;

    std::cout << "Sorting Dataset..." << endl;
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
    
    

    if(strcmp(method, "BS") && strcmp(method, "bfs") && strcmp(method, "ibs") 
       && strcmp(method, "tip") && strcmp(method, "kbs") && strcmp(method, "bbs")){
        if(!strcmp(type, "uint64")){
            Vl = (uint64_t *)calloc(m,sizeof(uint64_t));

            if(Vl == NULL)  return 1;
        }else{
            Vi = (uint32_t *)calloc(m,sizeof(uint32_t));
            if(Vi == NULL)  return 1;
        }
    }

    uint64_t *res = (uint64_t *) malloc(sizeof(uint64_t)*q);

//    if(!strcmp(method, "BS")){

//         if(!strcmp(type, "uint64")){
//             std::vector<uint64_t> keys;
//             vector<Row<uint64_t>> AVec;
//             std::vector<Row<uint64_t>>::iterator low;
//             for(uint64_t i=0; i<m; i++){
//                 keys.push_back(Al[i]);
//             }
//             AVec = SOSDDutil::add_values(keys);
//             //vector<uint64_t>AVec(Al, Al + m);
//             std::cout << "Query " << method << "..." << endl;
//             timerSearch = 0;
//             BranchingBinarySearch<uint64_t> *searcherPTR = new BranchingBinarySearch<uint64_t>();
//             BranchingBinarySearch<uint64_t> searcher = *searcherPTR;
//             size_t num_q  = 0;
//             auto c_start = std::chrono::high_resolution_clock::now();
//             for(uint64_t i = 0; i<q; i++){
                
                
//                 //std::cout <<  "Stamppaaaa" <<endl;
//                 searcher.search(AVec, Ql[i], &num_q, 0, m);
                
//                 //std::cout << (low - AVec.begin()) << endl;
//                 //std::cout << "SSTART:: " << c_start.time_since_epoch().count() << " End: " << c_end.time_since_epoch().count() << endl;
                
//                 //std::cout << timerSearch << endl;
//             }
//             auto c_end = std::chrono::high_resolution_clock::now();
//             timerSearch = c_end.time_since_epoch().count() - c_start.time_since_epoch().count();
//             std::cout << num_q <<  endl;
//             std::cout << timerSearch/q << endl;
            
//         }else{
//             vector<Row<uint32_t>> AVec;
//             for(uint64_t i=0; i<m; i++){
//                 AVec.push_back({Ai[i], i});
//             }
//             std::cout << "Query " << method << "..." << endl;
//             c_start = std::clock();
//             for(uint64_t i = 0; i<q; i++){
//                 std::lower_bound(AVec.begin(),AVec.end(), Qi[i], [](const Row<uint32_t>& lhs,
//                                   const uint32_t lookup_key) {
//                                  return lhs.key < lookup_key;
//                                });
//             }
//             c_end = std::clock();
//             timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
//         }
//     }else if(!strcmp(method, "bbs")){
    if(!strcmp(method, "bbs")){
        if(!strcmp(type, "uint64")){

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint64_t>::branchyBS(Al,Ql[i], 0, m);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint32_t>::branchyBS(Ai,Qi[i], 0, m);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "bfs")){

        if(!strcmp(type, "uint64")){

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint64_t>::branchfreeBS(Al,Ql[i], 0, m, false);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint32_t>::branchfreeBS(Ai,Qi[i], 0, m, false);
                // std::cout << res[i] << endl;
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "kbs")){

        if(!strcmp(type, "uint64")){

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint64_t>::kBFS(Al,Ql[i], 0, m, 20);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = StandardLayout<uint32_t>::kBFS(Ai,Qi[i], 0, m, 20);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "ibs")){
        if(!strcmp(type, "uint64")){

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                //if(!(i%10000)) std::cout << i << " of " << q << " -> " << (double)i/(double)q*100 << "%..." << endl;
                res[i]  = StandardLayout<uint64_t>::branchyIS(Al,Ql[i], 0, m);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                //if(!(i%10000)) std::cout << i << " of " << q << " -> " << (double)i/(double)q*100 << "%..." << endl;
                res[i]  = StandardLayout<uint32_t>::branchyIS(Ai,Qi[i], 0, m);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "tip")){
        if(!strcmp(type, "uint64")){
            
            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                if(!(i%10000)) std::cout << i << " of " << q << " -> " << (double)i/(double)q*100 << "%..." << endl;
                //std::cout << "ELEMENTO:" << Ql[i] << endl;
                res[i]  = StandardLayout<uint64_t>::tipFind(Al,Ql[i], 0, m-1);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                if(!(i%10000)) std::cout << i << " of " << q << " -> " << (double)i/(double)q*100 << "%..." << endl;
                //std::cout << "ELEMENTO:" << Ql[i] << endl;
                res[i]  = StandardLayout<uint32_t>::tipFind(Ai,Qi[i], 0, m-1);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "bfe")){
        if(!strcmp(type, "uint64")){

            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            EytzeingerLayout<uint64_t>::createLayout(Al, Vl,0,m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = EytzeingerLayout<uint64_t>::branchfreeBS(Vl,Ql[i], m, true);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{
            
            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            EytzeingerLayout<uint32_t>::createLayout(Ai, Vi,0,m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = EytzeingerLayout<uint32_t>::branchfreeBS(Vi,Qi[i], m, true);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "bfv")){
        if(!strcmp(type, "uint64")){

            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            VanEmdeBoaLayout<uint64_t>::create_vanemdeboa(Al, Vl, m, s);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = VanEmdeBoaLayout<uint64_t>::search(Vl,Ql[i], m, s);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{
            
            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            VanEmdeBoaLayout<uint32_t>::create_vanemdeboa(Ai, Vi, m, s);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
                res[i]  = VanEmdeBoaLayout<uint32_t>::search(Vi,Qi[i], m, s);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "bft512")){
        if(!strcmp(type, "uint64")){

            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            BTreesLayout<uint64_t,512>::createBTreeLayout(Al, Vl, m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
               res[i]  =  BTreesLayout<uint64_t,512>::branchfreeBS(Vl, Ql[i], 0, m-1);
            }
            c_end = std::clock();
        }else{
            
            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            BTreesLayout<uint32_t, 512>::createBTreeLayout(Ai, Vi, m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
               res[i]  =  BTreesLayout<uint32_t,512>::branchfreeBS(Vi,Qi[i], 0, m-1);
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }else if(!strcmp(method, "bft32k")){
        if(!strcmp(type, "uint64")){

            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            BTreesLayout<uint64_t,32000>::createBTreeLayout(Al, Vl, m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
               res[i]  =  BTreesLayout<uint64_t,32000>::branchfreeBS(Vl, Ql[i], 0, m-1);
            //    cout << Vl[i] << endl;
            // //    cout << Ql[i] << " " << Vl[res[i]] << endl;
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }else{
            
            std::cout << "Layout Construction..." << endl;
            c_start = std::clock();
            BTreesLayout<uint32_t, 32000>::createBTreeLayout(Ai, Vi, m);
            c_end = std::clock();
            timerCon = ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;

            std::cout << "Query " << method << "..." << endl;
            c_start = std::clock();
            for(uint64_t i = 0; i<q; i++){
               res[i]  =  BTreesLayout<uint32_t,32000>::branchfreeBS(Vi,Qi[i], 0, m-1);
            //    cout << Qi[i] << " " << Vi[res[i]] << endl;
            }
            c_end = std::clock();
            timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
        }
    }

    // timerSearch += ((double)c_end-(double)c_start) / (double)CLOCKS_PER_SEC;
    bool wrong = false;
    if(strcmp(method, "BS") && strcmp(method, "bfs") && strcmp(method, "ibs") 
       && strcmp(method, "tip") && strcmp(method, "kbs") && strcmp(method, "bbs")){
        if(!strcmp(type, "uint64")){
            for(int j = 0; j < q; j++){
                if(Ql[j] != Vl[res[j]]){
                    wrong = true;
                    // std::cout << "Errore in " << j << endl;
                    // std::cout << Ql[j] << " " << Vl[res[j]] << endl;
                }
            }
        }else{
            for(int j = 0; j < q; j++){
                if(Qi[j] != Vi[res[j]]){
                    wrong = true;
                    // std::cout << "Errore in s" << Ai[res[j]] << endl;
                }
            }
        }
    }else{
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
                    // std::cout << "Errore in s" << Ai[res[j]] << endl;
                }
            }
        }
    }

    if(wrong){
        
        fprintf(out, "%s, %s, %lu, %s, NAN, NAN, NAN, NAN, NAN, NAN\n", dataName, type, m, method);
    }else{
        fprintf(out, "%s, %s, %lu, %s, %.10e, %.10e, %.10e, %.10e, %.10e, %.10e\n", dataName, type, m, method, timerSort, timerSort/m, timerCon, timerCon/m, timerSearch, timerSearch/q);
    }
    
    return 0;
}
