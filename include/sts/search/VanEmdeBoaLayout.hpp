#pragma once

#define DEBUG 0

#define MAX_H 32

namespace sts::search{
    
    using namespace std;
    using namespace sts;

    typedef unsigned char h_type;
    typedef unsigned long int I;

    /**
     * Structure for Van emde Boas Layout
    */
    struct dumdum {
        h_type h0, h1, dummy[2];
        I m0, m1;
    };
    
    template<typename T> class VanEmdeBoaLayout{

        private:

            static void sequencer(I h, dumdum *s, unsigned d) {
                if (h == 0) return;
                I h0 = h/2;
                I h1 = h-h0-1;
                sequencer(h0, s, d);
                s[d+h0].h0 = h0;
                s[d+h0].m0 = (((I)2)<<h0)-1;
                s[d+h0].h1 = h1;
                s[d+h0].m1 = (((I)2)<<h1)-1;
                sequencer(h1, s, d+h0+1);
            }

            static T* copy_data(T *a0, T *a, I n, I h, I *rtl, dumdum *s, I path, unsigned d) {

                if (d > h || rtl[d] >= n) return a0;

                // visit left child
                path <<= 1;
                rtl[d+1] = rtl[d-s[d].h0] + s[d].m0 + (path&s[d].m0)*(s[d].m1);
                a0 = copy_data(a0, a, n, h, rtl, s, path, d+1);

                a[rtl[d]] = *a0++;

                // visit right child
                path += 1;
                rtl[d+1] = rtl[d-s[d].h0] + s[d].m0 + (path&s[d].m0)*(s[d].m1);
                a0 = copy_data(a0, a, n, h, rtl, s, path, d+1);

                return a0;
            }

            static void veb_array(T *a0, T* a, I n0, dumdum *s) {

                I n = n0;
                I h;
                // find smallest h such that sum_i=0^h 2^h >= n
                unsigned m = 1;
                for (h = 0; m < n; h++, m += 1<<h);
                // cout << "QUI1" << endl;
                // build our magic sequence
                dumdum q = {(h_type)h, 0, {0, 0}, ((I)2<<h)-1, 1};
                std::fill_n(s, MAX_H+1, q);
                sequencer(h, s, 0);

                // allocate new array and copy data into it
                //T *a = (T *)calloc(sizeof(T),n);
                I rtl[MAX_H+1];
                rtl[0] = 0;
                // cout << "Qui2" << endl;
                copy_data(a0, a, n, h, rtl, s, 0, 0);
                // cout << "Qui3" << endl;
            }

        public:

            /**
             * 
             * Branchfree Search of x in Van Emde boas layout
             * @param A a source array of type T 
             * @param B a dest array of type T
             * @param n number of elements in A and B
             * @param s Addres of auxiliary structure
             * 
             * 
            */
            static void create_vanemdeboa(T *A, T *B, uint64_t n, dumdum *s){
                
                veb_array(A, B, n, s);
            }


            /**
             * 
             * Sequential Search of x in data in the index interval [start, end]
             * @param a an array layout of type T
             * @param x a number of type T
             * @param n number of elements in data
             * @param s Addres of auxiliary structure
             * 
             * @return Predecessor of x
             * 
            */
            static I search(T *a, T x, I n, dumdum *s) {
                I rtl[MAX_H+1];
                I j = n;
                I i = 0;
                I p = 0;
                for (int d = 0; i < n; d++) {
                    rtl[d] = i;
                    if (x < a[i]) {
                        p <<= 1;
                        j = i;
                    } else if (x > a[i]) {
                        p = (p << 1) + 1;
                    } else {
                        return i;
                    }
                    i = rtl[d-s[d].h0] + s[d].m0 + (p&s[d].m0)*(s[d].m1);
                }
                return j;
            }

    };
}