#pragma once

#include <bits/stdc++.h>
#include <type_traits>
#include <stdio.h>
#include <stdlib.h>
#include "../search/StandardLayout.hpp"
#include "../search/LrLearnedDS.hpp"
#include "../search/QrLearnedDS.hpp"
#include "../search/CrLearnedDS.hpp"
#include "../../dep/json.hpp"
#include "../../dep/util.h"

#define DEBUG 0

#define ROW_WIDTH 1

namespace sts::util{

    using namespace std;
    using namespace sts;
    using namespace sts::search;
    using json = nlohmann::json;

    template<class KeyType = uint64_t>
    struct OBFSseg {
        LrLearnedDs<KeyType> *linear;
        QrLearnedDs<KeyType> *quadratic;
        CrLearnedDs<KeyType> *cubic;
        uint64_t left;
        uint64_t right;
    };

    template<class KeyType = uint64_t>
    struct BFEseg {
        uint64_t left;
        uint64_t right;
        KeyType leftV;
        KeyType rightV;
    };

    // template<class KeyType>
    // struct Row {
    // KeyType key;
    // uint64_t data[ROW_WIDTH];
    // };

    // template<class KeyType = uint64_t>
    // struct EqualityLookup {
    // KeyType key;
    // uint64_t result;
    // };
    template<typename T> class Utils{

        public:

            static bool checkResult (T* A, uint64_t* R, T* Q, uint64_t q){

              bool wrong = false;

              for(int j = 0; j < q; j++){
                  if(Q[j] != A[R[j]]){
                      //cout << Q[j] << " " << A[R[j]] << endl;
                      wrong = true;
                  }
              }
              return wrong;
            }

            static int cmpfunc (const void * a, const void * b) {
                if ( *(T*)a <  *(T*)b ) return -1;
	        else if ( *(T*)a == *(T*)b ) return 0;
	        else return 1;
            }

            /* Arrange the N elements of ARRAY in random order.
            Only effective if N is much smaller than RAND_MAX;
            if this may not be the case, use a better random
            number generator. */
            static void shuffle_array(T *array, size_t n)
            {
                if (n > 1)
                {
                    size_t i;
                    for (i = 0; i < n - 1; i++)
                    {
                    size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
                    T t = array[j];
                    array[j] = array[i];
                    array[i] = t;
                    }
                }
            }

            static uint64_t getDataFromCsv(string fn, T **data, bool align){

                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                FILE* fp;
                uint64_t i = 0;
                std::string line;

                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                std::ifstream myFile(fn);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << " Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                //i++;

                while(std::getline(myFile, line))
                {
                    i++;
                }

                //cout << "N. of Elements read: " << i << endl;
                myFile.clear();
                myFile.seekg(0, ios::beg);

                if(align){

                    int err = posix_memalign((void**)data, 64, i*sizeof(T));
                    if(err) {
                        std::stringstream errMsg;
                        errMsg << "Allocation Error " << err << ": Not enough Memory...";
                        throw std::runtime_error(errMsg.str().c_str());
                    }
                }else{
                    // getchar();
                    *data = (T*)malloc(i*sizeof(T));
                    if(*data == NULL) {
                        std::stringstream errMsg;
                        errMsg << "Allocation Error: Not enough Memory...";
                        throw std::runtime_error(errMsg.str().c_str());
                    }
                }
                //fseek(fp, 0L, SEEK_SET);
                // for(i = 0; !feof(fp); i++){
                //     fscanf(fp, "%d\n", &data[0][i]);
                // }
                i = 0;
                while(std::getline(myFile, line))
                {

                    *(*data + i) = (T)std::stoul(line);
                    // cout << data[0][i] << endl;
                    // getchar();
                    i++;
                }
                return i;
            }

            static uint64_t getDataFromBin(string fn, T **data, bool align){

                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                uint64_t i = 0;
                std::string line;
                uint64_t m;
                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                ifstream myFile (fn, ios::in | ios::binary);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                myFile.read(reinterpret_cast<char*> (&m), sizeof(uint64_t));
                //cout << "N. Data: " << m << endl;
                //i++;
                if(align){

                    int err = posix_memalign((void**)data, 64, m*sizeof(T));
                    if(err) {
                        std::stringstream errMsg;
                        errMsg << "Allocation Error " << err << ": Not enough Memory...";
                        throw std::runtime_error(errMsg.str().c_str());
                    }
                }else{
                    // getchar();
                    *data = (T*)malloc(m*sizeof(T));
                    if(*data == NULL) {
                        std::stringstream errMsg;
                        errMsg << "Allocation Error: Not enough Memory...";
                        throw std::runtime_error(errMsg.str().c_str());
                    }
                }
                //fseek(fp, 0L, SEEK_SET);
                // for(i = 0; !feof(fp); i++){
                //     fscanf(fp, "%d\n", &data[0][i]);
                // }
                i = 0;
                while(i<m)
                {
                    myFile.read(reinterpret_cast<char*> (*data+i), sizeof(T));
                    //*(*data + i) = (T)std::stoul(line);
                    // if(i< 10)
                    //     cout << data[0][i] << endl;
                    // getchar();
                    i++;
                }
                return i;
            }

            static uint64_t readRegrParams(std::string fn, double **wdata){
                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                FILE* fp;
                uint64_t i = 0;
                std::string line;

                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                std::ifstream myFile(fn);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << " Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                while(std::getline(myFile, line))
                {
                    i++;
                }

                //cout << "N. of Params read: " << i << endl;
                myFile.clear();
                myFile.seekg(0, ios::beg);


                *wdata = (double*)malloc(sizeof(double)*i);

                if (wdata == NULL) {

                    std::stringstream errMsg;
                    errMsg << "Allocation Error: Not enough Memory...";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                for(int j = 0; j < i; j++)
                {
                    std::getline(myFile, line);
                    *(*wdata + j) = (double)std::atof(line.c_str());
                    // cout << "W[" << j << "]:" << wdata[0][j] << endl;
                    // getchar();
                }

                myFile.close();
                return i;
            }

            static void readOBFSParamsFromJson(
                                                std::string fn, char * entry, uint32_t k,
                                                double ***w, uint64_t **esp, uint64_t **eps
            ){

                //cout << fn.c_str() << endl;

                json j;

                std::ifstream i(fn);

                if(!i.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File " << fn << " Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                i >> j;

                i.close();

                // cout << std::setw(4) << j << endl;
                // cout << entry << endl;
                // cout << std::setw(4) << j[entry]["1"] << endl;

                *w = (double**)malloc(k*sizeof(double*));
                *esp = (uint64_t *) malloc(k*sizeof(uint64_t));
                *eps = (uint64_t *) malloc(k*sizeof(uint64_t));

                for(int r = 1; r < k+1; r++){
                    std::stringstream ss;
                    std::stringstream ss2;
                    ss << r;
                    // cout << entry << endl;
                    // cout << j << endl;
                    // cout << j[entry] << endl;
                    // cout << std::setw(4) << j[entry][ss.str()] << endl;
                    ss2 << j[entry][ss.str()]["best_model"];
                    ss2 >> *(*esp+(r-1));
                    uint64_t curr_e = *(*esp+(r-1));

                    *((*w)+(r-1)) = (double*)malloc((curr_e+1)*sizeof(double));
                    *(*eps+(r-1)) = j[entry][ss.str()][ss2.str()]["epsilon"];
                    cout << "Epsilon " << r << ":" << j[entry][ss.str()][ss2.str()]["epsilon"] << endl;
                    for(int e = 0; e <= curr_e; e++){
                        std::stringstream ss3;
                        ss3 << e;

                        *(*((*w)+(r-1))+(curr_e - e)) = j[entry][ss.str()][ss2.str()]["w"][ss3.str()];
                        cout << *(*((*w)+(r-1))+e) << endl;
                    }

                    cout << "Epsilon " << r << ":" << eps[0][r-1] << endl;

                }
            }

            static void readOBFSLinParamsFromJson(
                                                std::string fn, char * entry,
                                                double ***w, uint64_t **esp, uint64_t **eps
            ){

                //cout << fn.c_str() << endl;

                json j;

                std::ifstream i(fn);

                if(!i.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File " << fn << " Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                i >> j;

                i.close();

                // cout << std::setw(4) << j << endl;
                // cout << entry << endl;
                // cout << std::setw(4) << j[entry]["1"] << endl;

                *w = (double**)malloc(3*sizeof(double*));
                *esp = (uint64_t *) malloc(3*sizeof(uint64_t));
                *eps = (uint64_t *) malloc(3*sizeof(uint64_t));

                for(int r = 1; r < 4; r++){
                    std::stringstream ss;
                    std::stringstream ss2;
                    ss << r;
                    // cout << entry << endl;
                    // cout << j << endl;
                    // cout << j[entry] << endl;
                    // cout << std::setw(4) << j[entry][ss.str()] << endl;
                    // ss2 << j[entry][ss.str()]["best_model"];
                    // ss2 >> *(*esp+(r-1));
                    ss2 << "1";
                    ss2 >> *(*esp+(r-1));

                    uint64_t curr_e = *(*esp+(r-1));

                    *((*w)+(r-1)) = (double*)malloc((curr_e+1)*sizeof(double));
                    *(*eps+(r-1)) = j[entry][ss.str()]["1"]["epsilon"];
                    cout << "Epsilon " << r << ":" << j[entry][ss.str()]["1"]["epsilon"] << endl;
                    for(int e = 0; e <= curr_e; e++){
                        std::stringstream ss3;
                        ss3 << e;

                        *(*((*w)+(r-1))+(curr_e - e)) = j[entry][ss.str()]["1"]["w"][ss3.str()];
                        cout << *(*((*w)+(r-1))+(curr_e - e)) << endl;
                    }

                    //cout << "Epsilon " << r << ":" << eps[0][r-1] << endl;

                }
            }

            static uint64_t readEpsilon(std::string fn, uint64_t esp){

                stringstream ss;

                json j;
                if(esp == 1){
                    ss << fn << "_linear.json";
                    //cout << ss.str() << endl;
                    std::ifstream i(ss.str());
                    i >> j;
                     i.close();
                }else if(esp == 2){
                    ss << fn << "_quadratic.json";
                    //cout << ss.str() << endl;
                    std::ifstream i(ss.str());
                    i >> j;
                    i.close();
                }else if(esp == 3){
                    ss << fn << "_cubic.json";
                    //cout << ss.str() << endl;
                    std::ifstream i(ss.str());
                    i >> j;
                    i.close();
                }

                ss.str("");
                ss.clear();
                ss << esp;

                cout << ss.str() << endl << endl;
                cout << fn.substr(fn.find_last_of("/")+1) << endl;
                cout << j[fn.substr(fn.find_last_of("/")+1)][ss.str()]["epsilon"] << endl;

                return j[fn.substr(fn.find_last_of("/")+1)][ss.str()]["epsilon"];
            }

            static void convertQuery(T *dataset, uint64_t N, string fn, string ofn){

                vector<EqualityLookup<T>> lookups;


                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                uint64_t i = 0;
                std::string line;
                uint64_t m;
                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                ifstream myFile (fn, ios::in | ios::binary);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                ofstream outFile (ofn, ios::out | ios::binary);

                if(!outFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << ofn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                myFile.read(reinterpret_cast<char*> (&m), sizeof(uint64_t));

                cout << m << " Query To Read" << endl;

                lookups.reserve(m);

                i = 0;
                T aus = 0;
                uint64_t res;

                while(i<m)
                {
                    //cout << "Reading " << i+1 << "to" << m << "..." << endl;

                    myFile.read(reinterpret_cast<char*> (&aus), sizeof(T));

                    // cout << "Value: " << aus << endl;

                    //cout << "Searching index..." << endl;
                    res = StandardLayout<T>::branchyBS(dataset, aus, 0, N-1);

                    //cout << "Inserting Result into struct" << endl;
                    lookups.push_back({aus, res});

                    i++;
                }

                // Write size.
                outFile.write(reinterpret_cast<const char*>(&m), sizeof(uint64_t));
                // Write values.
                outFile.write(reinterpret_cast<const char*>(lookups.data()), m*sizeof(EqualityLookup<T>));

            }

            static void extract100kQuery(T *dataset, uint64_t N, string fn, string ofn){

                vector<T> lookups;


                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                uint64_t i = 0;
                std::string line;
                uint64_t m;
                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                ifstream myFile (fn, ios::in | ios::binary);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                ofstream outFile (ofn, ios::out | ios::binary);

                if(!outFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << ofn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                myFile.read(reinterpret_cast<char*> (&m), sizeof(uint64_t));

                cout << m << " Query To Read" << endl;

                lookups.reserve(100000);

                i = 0;
                T aus = 0;
                uint64_t res;

                while(i<100000)
                {
                    //cout << "Reading " << i+1 << "to" << m << "..." << endl;

                    myFile.read(reinterpret_cast<char*> (&aus), sizeof(T));

                    //cout << "Value: " << aus << endl;

                    //cout << "Searching index..." << endl;
                    //res = StandardLayout<T>::branchyBS(dataset, aus, 0, N-1);

                    //cout << "Inserting Result into struct" << endl;
                    lookups.push_back(aus);

                    i++;
                }
                m = 100000;
                // Write size.
                outFile.write(reinterpret_cast<const char*>(&m), sizeof(uint64_t));
                // Write values.
                outFile.write(reinterpret_cast<const char*>(lookups.data()), 100000*sizeof(T));

            }

            static void extractKQuery(T *dataset, uint64_t N, string fn, string ofn, int k){

                vector<T> lookups;


                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                uint64_t i = 0;
                std::string line;
                uint64_t m;
                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                ifstream myFile (fn, ios::in | ios::binary);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                ofstream outFile (ofn, ios::out | ios::binary);

                if(!outFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << ofn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                myFile.read(reinterpret_cast<char*> (&m), sizeof(uint64_t));

                cout << m << " Query To Read" << endl;

                lookups.reserve(k);

                i = 0;
                T aus = 0;
                uint64_t res;

                while(i<k)
                {
                    //cout << "Reading " << i+1 << "to" << m << "..." << endl;

                    myFile.read(reinterpret_cast<char*> (&aus), sizeof(T));

                    //cout << "Value: " << aus << endl;

                    //cout << "Searching index..." << endl;
                    //res = StandardLayout<T>::branchyBS(dataset, aus, 0, N-1);

                    //cout << "Inserting Result into struct" << endl;
                    lookups.push_back(aus);

                    i++;
                }
                m = k;
                // Write size.
                outFile.write(reinterpret_cast<const char*>(&m), sizeof(uint64_t));
                // Write values.
                outFile.write(reinterpret_cast<const char*>(lookups.data()), k*sizeof(T));

            }


            static void convertDataset32(T *dataset, uint64_t N, string fn, string ofn){

                vector<T> lookups;
                if(!std::is_same<T, uint64_t>::value && !std::is_same<T, uint32_t>::value){
                    std::stringstream errMsg;
                    errMsg << "Unsupported Type: Use Int or Long";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                uint64_t i = 0;
                std::string line;
                uint64_t m;
                //fp = fopen( fn.c_str(), "r" );

                // Create an input filestream
                ifstream myFile (fn, ios::in | ios::binary);

                if(!myFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << fn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                ofstream outFile (ofn, ios::out | ios::binary);

                if(!outFile.is_open()){
                    std::stringstream errMsg;
                    errMsg << "File Error: Opening File" << ofn << "Failed.";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                myFile.read(reinterpret_cast<char*> (&m), sizeof(uint64_t));

                cout << m << " Query To Read" << endl;

                lookups.reserve(m);

                i = 0;
                T aus = 0;
                uint64_t res;

                while(i<m)
                {
                    //cout << "Reading " << i+1 << "to" << m << "..." << endl;

                    myFile.read(reinterpret_cast<char*> (&aus), sizeof(T));

                    //cout << "Value: " << aus << endl;

                    //cout << "Searching index..." << endl;
                    //res = StandardLayout<T>::branchyBS(dataset, aus, 0, N-1);

                    //cout << "Inserting Result into struct" << endl;
                    lookups.push_back(aus);

                    i++;
                }

                // Write size.
                outFile.write(reinterpret_cast<const char*>(&m), sizeof(T));
                // Write values.
                outFile.write(reinterpret_cast<const char*>(lookups.data()), m*sizeof(T));

            }


            static void print_matrix(T *A, uint64_t nr_rows_A, uint64_t nr_cols_A) {

                for(uint64_t i = 0; i < nr_rows_A; ++i){
                    for(uint64_t j = 0; j < nr_cols_A; ++j){
                    std::cout << A[j * nr_rows_A + i] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }


            static char* getCmdOption(char ** begin, char ** end, const std::string & option)
            {
                char ** itr = std::find(begin, end, option);
                if (itr != end && ++itr != end)
                {
                    return *itr;
                }
                return 0;
            }

            static bool cmdOptionExists(char** begin, char** end, const std::string& option)
            {
                return std::find(begin, end, option) != end;
            }
    };
}
