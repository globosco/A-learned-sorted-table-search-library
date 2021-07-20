#pragma once
#include "../util/Utils.hpp"

#define DEBUG 0

namespace sts::search{
    
    using namespace std;
    using namespace sts;
    using namespace sts::util;
    
    template<typename T> class EytzeingerLayout{
        private:

            static T* eytzeingerArray(T *A, T *B, uint64_t i, uint64_t n){
                if(i >= n) return A;
                // cout << "Richiamo..." <<i << endl; 
                A = eytzeingerArray(A, B,  2*i+1, n);
                
                // cout << *(A+1) << " " << i << endl;
                B[i] = *A++;

                A = eytzeingerArray(A, B, 2*i+2, n);

                return A;
            }

        public:

            /**
             * 
             * Branchfree Search of x in Eytzeinger layout
             * @param data an array layout of type T
             * @param x a number of type T
             * @param size number of elements in data
             * @param prefetch bool
             * 
             * @return Predecessor of x
             * 
            */
            static uint64_t branchfreeBS(T* data, T x, uint64_t size, bool prefetch){
                
                uint64_t i = 0;
                uint64_t n = size;
                uint64_t multiplier = 128;
                uint64_t offset = 3*multiplier/2-1;

                while (i < n) {
                    if(prefetch)
                        __builtin_prefetch(data+(multiplier*i + offset));
                        // cout << i << " " << data[i] << " " << x << endl;
                    i = (x <= data[i]) ? (2*i + 1) : (2*i + 2);
                }
                uint64_t j = (i+1) >> __builtin_ffs(~(i+1));
                return (j == 0) ? n : j-1;
                
            }

            /**
             * 
             * Branchfree Search of x in Eytzeinger layout
             * @param data an array layout of type T
             * @param x a number of type T
             * @param size number of elements in data
             * @param prefetch bool
             * 
             * @return Predecessor of x
             * 
            */
            static uint64_t branchfreeKBS(T* data, T x, uint64_t size, bool prefetch, int k){
                
                uint64_t i = 0;
                uint64_t n = size;
                uint64_t multiplier = 128;
                uint64_t offset = 3*multiplier/2-1;

                while (i < n) {
                    if(prefetch)
                        __builtin_prefetch(data+(multiplier*i + offset));
                    i = (x <= data[i]) ? (2*i + 1) : (2*i + 2);
                }
                uint64_t j = (i+1) >> __builtin_ffs(~(i+1));
                return (j == 0) ? n : j-1;
                
            }

            /**
             * 
             * Sequential Search of x in data in the index interval [start, end]
             * @param A a source array of type T 
             * @param B a dest array of type T
             * @param i start index for the costruction of layout
             * @param n number of elements in A and B
             * 
             * 
            */
            static void createLayout(T *A, T *B, uint64_t i, uint64_t n){
                    
                eytzeingerArray(A, B, i, n);
            }

            /**
             * 
             * Sequential Search of x in data in the index interval [start, end]
             * @param A a source array of type T 
             * @param B a dest array of type T
             * @param i start index for the costruction of layout
             * @param n number of elements in A and B
             * 
             * 
            */
            static void createKLayout(T *A, T *B, uint64_t j, uint64_t n, int k, BFEseg<T> *segs){
                uint64_t left = 0;
                uint64_t right = n-1;
                uint64_t segmentLeft = 0;
                uint64_t segmentRight = left + ((right - left)) / k;   
                
                for(int i = 0; i < k; i++){
                    segs[i].left = segmentLeft;
                    segs[i].leftV = A[segmentLeft];
                    segs[i].right = segmentLeft;
                    segs[i].rightV = A[segmentRight];
                    // cout << segmentLeft << " " << segmentRight << endl;
                    eytzeingerArray(A+segmentLeft, B+segmentLeft, j, segmentRight - segmentLeft + 1);
                    // cout << *(A+segmentLeft) << " " << *(A+segmentRight) << endl;
                    // cout << *(B+segmentLeft) << " " << *(B+segmentRight) << endl;
                    segmentLeft = segmentRight + 1;
                    segmentRight = left + ((i+2) * (right - left)) / k;                    
                }
                // for(int i = 375; i<750; i++) cout << B[i] << endl;
                
            }
    };
}