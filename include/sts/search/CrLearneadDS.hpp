#pragma once

#include "/opt/intel/mkl/include/mkl.h"
#include "/opt/intel/mkl/include/mkl_vml_functions.h"
#include <math.h>

#define DEBUG 0

namespace sts::search{

    using namespace std;
    using namespace sts;

    template<typename T> class CrLearnedDs{

        private:
            uint64_t epsilon = 0;
            T* A = nullptr;
            double* labels = nullptr;
            double w[4] = {0.0}; //= nullptr;
            uint64_t n = 0;

            void predict(T q, double *C){

                *C = std::fma(w[0], q, w[1]);
                *C = std::fma(*C, q, w[2]);
                *C = std::fma(*C, q, w[3]);
                // *C = pow(q,3)*w[0];
                // *C += pow(q,2)*w[1];
                // *C += q*w[2];
                // *C += w[3];
                *C = floor((*C)*n);
            }

            void predict(double *C){
                // std::cout << "Computing Weigth Multiplication..." << std::endl;

                for(int i = 0; i<n; i++){
                    C[i] = pow(A[i],3)*w[0];
                    C[i] += pow(A[i],2)*w[1];
                    C[i] += A[i]*w[2];
                    C[i] += w[3];
                    C[i] = floor(C[i]*n);
                }
            }

            void calculateMaxError(){
                // uint64_t *l = (uint64_t *) malloc(sizeof(uint64_t)*n);


                double  *C;
                int i, j;
                double alpha, beta;

                // std::cout << "Starting ULR Prediction..." << std::endl;
                // cout << n << endl;
                alpha = 1.0; beta = 0.0;
                C = (double *)calloc(n, sizeof( double ));
                if (C == NULL) {
                    C = NULL;
                    std::stringstream errMsg;
                    errMsg << "Allocation Error: Not enough Memory...";
                    throw std::runtime_error(errMsg.str().c_str());
                }

                // for(int i = 0; i<2; i++){
                //     cout << w[i] << " ";
                // }
                // cout << endl;

                // cout << "Predict..." << endl;
                predict(C);

                // for(int i = 0; i<20; i++){
                //     cout << C[i] << " ";
                // }
                // cout << endl;

                for(int i = 0; i<n; i++){
                    int aus = abs(i - floor(C[i]));
                    // cout << aus << " ";
                    if(aus > epsilon) epsilon = aus;
                }
                // cout << "END CALCULATE MAX ERROR" << endl;
            }


        public:

            CrLearnedDs(T* A, uint64_t n, uint64_t epsilon, double *w){
                this->A = A;
                this->n = n;
                this->epsilon = epsilon;
                labels = (double*) malloc(sizeof(double)*n);
                for(uint64_t i = 1; i <= n; i++){
                    labels[i-1] = (double)i/(double)n;
                }
                this->w[0] = w[0];
                this->w[1] = w[1];
                this->w[2] = w[2];
                this->w[3] = w[3];
                free(labels);

            }

            CrLearnedDs(T* A, uint64_t n){
                cout << "Init Ricerca Cubic" << endl;
                this->A = A;
                this->n = n;
                double* data = (double*) malloc(sizeof(double)*(2*n));
                labels = (double*) malloc(sizeof(double)*n);
                double mult1[4] = {0.0}; //(double*) calloc(4, sizeof(double));
                double mult2[2] = {0.0};//= (double*) calloc(2, sizeof(double));
                //w = (double*) calloc(2, sizeof(double));
                for(uint64_t i = 0; i < n; i++){
                    data[i] = (double)A[i];
                }

                for(uint64_t i = n; i < 2*n; i++){
                    data[i] = 1.0;
                }

                // for(uint64_t i = 0; i < 20; i++){
                //     if(i % n == 0)
                //         cout << endl;
                //     cout << data[i] << " ";
                // }
                // cout << endl << endl << "Transpose";

                for(uint64_t i = 1; i <= n; i++){
                    labels[i-1] = (double)i/(double)n;
                }

                // cout << endl << "Labels" << endl;
                // for(uint64_t i = n-20; i < n; i++){
                //     if(i % n == 0)
                //         cout << endl;
                //     cout << labels[i] << " ";
                // }

                cblas_dgemm(CblasColMajor,
                 CblasTrans,
                 CblasNoTrans,
                 2,
                 2,
                 n,
                 1, //alpha
                 data, // A
                 n,  //lda
                 data, //B
                 n, // ldb
                 1, // beta
                 mult1,   //C
                 2 //ldc
                );

                // for(uint64_t i = 0; i < 4; i++){
                //     if(i % 2 == 0)
                //         cout << endl;
                //     cout << mult1[i] << " ";
                // }
                lapack_int * ipiv = (lapack_int *) malloc(sizeof(lapack_int)*2);
                LAPACKE_dgetrf (LAPACK_COL_MAJOR , 2, 2, mult1 , 2,  ipiv );

                // for(uint64_t i = 0; i < 4; i++){
                //     if(i % 2 == 0)
                //         cout << endl;
                //     cout << mult1[i] << " ";
                // }

                LAPACKE_dgetri (LAPACK_COL_MAJOR, 2 , mult1 , 2 ,ipiv );
                // cout << endl << "Mult1";
                // for(uint64_t i = 0; i < 4; i++){
                //     if(i % 2 == 0)
                //         cout << endl;
                //     cout << mult1[i] << " ";
                // }

                cblas_dgemm(CblasColMajor,
                 CblasTrans,
                 CblasNoTrans,
                 2,
                 1,
                 n,
                 1, //alpha
                 data, // A
                 n,  //lda
                 labels, //B
                 n, // ldb
                 1, // beta
                 mult2,   //C
                 2 //ldc
                );
                // cout << endl << "MULT2:" << endl;
                // for(uint64_t i = 0; i < 2; i++){
                //     if(i % 2 == 0)
                //         cout << endl;
                //     cout << mult2[i] << " ";
                // }
                // cout << "END" << endl;

                cblas_dgemm(CblasColMajor,
                 CblasNoTrans,
                 CblasNoTrans,
                 2,
                 1,
                 2,
                 1, //alpha
                 mult1, // A
                 2,  //lda
                 mult2, //B
                 2, // ldb
                 1, // beta
                 w,   //C
                 2 //ldc
                );

                // cout << endl << "W:" << endl;
                // for(uint64_t i = 0; i < 2; i++){
                //     if(i % 2 == 0)
                //         cout << endl;
                //     cout << w[i] << " ";
                // }
                // cout << endl;

                calculateMaxError();
                free(labels);
                // // cout << "EPSILON:" << epsilon << endl;
            }

            uint64_t* search(T* Q, uint64_t q, char* method, uint64_t* intervals){

                double C = 0.0;
                uint64_t *res = (uint64_t*)calloc(q, sizeof(uint64_t));

                for(int i = 0; i<q; i++){
                    C = 0.0;
                    predict(Q[i], &C);

                    uint64_t left = C-epsilon < 0 ? 0 : (uint64_t)C-epsilon;
                    // left = left > n-1 ? n-1 : left;
                    uint64_t right = C+epsilon > n-1 ? n-1 : (uint64_t)C+epsilon;
                    // right = right > n-1 ? n-1 : right;

                    if(!strcmp(method, "bbs"))
                        res[i] = StandardLayout<T>::branchyBS(A, Q[i], left, right);
                    else if(!strcmp(method, "bfs"))
                        res[i] = StandardLayout<T>::branchfreeBS(A, Q[i], left, right, true);
                    else if(!strcmp(method, "ibs"))
                        res[i] = StandardLayout<T>::branchyIS(A, Q[i], left, right);
                    else if(!strcmp(method, "tip"))
                        res[i] = StandardLayout<T>::tipFind(A, Q[i], left, right);
                    else if(!strcmp(method, "kbs"))
                        res[i] = StandardLayout<T>::kBFS(A, Q[i], left, right,3);

                    intervals[i] = right - left;
                }
                return res;
            }

    };
}
