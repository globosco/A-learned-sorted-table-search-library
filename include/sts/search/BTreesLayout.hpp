#pragma once

#define DEBUG 0

namespace sts::search{
    
    using namespace std;
    using namespace sts;
    
    template<typename T, uint64_t cacheline> class BTreesLayout{
        private:
            
            static uint64_t child(uint64_t c, uint64_t i, uint64_t B) {

                return i*(B + 1) + (c + 1)*B;
            }

            static T* bTreeArray(T* A, T* V, uint64_t i, uint64_t n){


                uint64_t B = cacheline/sizeof(T); //cache = CACHE_LINE_WIDTH/sizeof(T); 
                if (i >= n) return A;

                for (uint64_t c = 0; c <= B; c++) {
                    
                    A = bTreeArray(A, V, child(c, i, B), n);
                    if (c < B && i+c < n) {
                        
                        V[i+c] = *A++;
                    }
                }
                return A;
            }

            static const T* branchfree_inner_search(const T *base, const T x, uint64_t C) {
                if (C <= 1) return base;
                const uint64_t half = C / 2;
                const T *current = &base[half];
                return branchfree_inner_search((*current < x) ? current : base, x, C-half);
            }

        public:

            /**
             * 
             * Sequential Search of x in data in the index interval [start, end]
             * @param A a source array of type T 
             * @param V a dest array of type T
             * @param length number of elements in A and B
             * 
             * 
            */
            static void createBTreeLayout(T* A, T* V, uint64_t length){

                bTreeArray(A, V, 0, length);
            }

            /**
             * 
             * Branchfree Search of x in BTrees layout
             * @param a an array layout of type T
             * @param x a number of type T
             * @param left left index of array
             * @param right right index of array
             * 
             * @return Predecessor of x
             * 
            */
            static uint64_t branchfreeBS(T * a, T x,  uint64_t left, uint64_t right){
                uint64_t j = right;
                uint64_t i = left;
                uint64_t n = right - left +1;
                uint64_t B = cacheline/sizeof(T);
                while (i + B <= n) {
                    __builtin_prefetch(a+child(i, B/2, B), 0, 0);
                    const T *base = &a[i];
                    const T *pred = branchfree_inner_search(base, x, B);
                    uint64_t nth = (*pred < x) + pred - base;
                    {
                        /* nth == B iff x > all values in block. */
                        const T current = base[nth % B];
                        uint64_t next = i + nth;
                        j = (current >= x) ? next : j;
                    }
                    i = child(nth, i, B);
                }
                if (__builtin_expect(i < n, 0)) {
                    // last (partial) block
                    const T *base = &a[i];
                    uint64_t m = n - i;
                    while (m > 1) {
                        uint64_t half = m / 2;
                        const T *current = &base[half];

                        base = (*current < x) ? current : base;
                        m -= half;
                    }

                    T ret = (*base < x) + base - a;
                    return (ret == n) ? j : ret;
                }
                return j;
            }


    };
}