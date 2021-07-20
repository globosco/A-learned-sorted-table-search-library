#pragma once

#define DEBUG 0

namespace sts::search{
    
    using namespace std;
    using namespace sts;
    
    
    template<typename T> class StandardLayout{
        
        public:

            /**
             * 
             * Sequential Search of x in data in the index interval [start, end]
             * @param data an array of type T
             * @param x a number of type T
             * @param start start index of search interval
             * @param end end index of search interval
             * 
             * @return If Found return the index of x + 1 else return 0
             * 
            */
            static uint64_t search(T* data, T x, uint64_t start, uint64_t end){

                for (uint64_t i = start; i <= end; i++)
                {
                    if (data[i] == x)
                        return i+1;
                }
                return end;
            }

            /**
             * 
             * Branchy Binary Search of x in data in the interval [l,r]
             * 
             * @param data an array of type T
             * @param x a number of type T
             * @param l start index of search interval
             * @param r end index of search interval
             * 
             * @return The index of predecessor of x
             * 
            */
            static uint64_t branchyBS(T *data, T x , uint64_t l, uint64_t r){ 

                uint64_t lo = l;
                uint64_t hi = r;
                long m = 0;
                #if DEBUG
                    cout << "X:" << x << endl;
                    cout << "lo:" << lo << endl;
                    cout << "hi:" << hi << endl;
                #endif
                while (lo <= hi) {
                    // cout << "LO: " << lo << " HI: " << hi << endl;
                    // cout << "A[lo]:" << data[lo] << " A[hi]" << data[hi] <<endl;
                    // cout << "X: " << x<<endl;
                    m = (lo + hi) / 2;
                    // cout << "m: " << m << endl;
                    // cout << "A[m]: " << data[m];
                    #if DEBUG
                        cout << "lo:" << lo << endl;
                        cout << "hi:" << hi << endl;
                        cout << "m:" << m << endl;
                    #endif

                    if (x < data[m]) {
                        #if DEBUG
                            cout << "x < data[m]" << endl;
                        #endif
                        //cout << "M-1:" << (m-1) << endl;
                        hi = m-1;
                        // cout << "SONO HI " << hi << " "  << (m-1  >= 0)  << endl;
                    } else if (x > data[m]) {
                        #if DEBUG
                            cout << "x > data[m]" << endl;
                        #endif
                        lo = m+1;
                        // cout << "SONO LO " << hi << " "  << (m-1  >= 0)  << endl;
                    } else {
                        #if DEBUG
                            cout << "TROVATO" << endl;
                        #endif
                        // cout << "Trovato in " << m << endl;
                        // cout << data[m] << " = " << x << endl;
                        return m;
                    }
                }
                // cout << "Non Trovato!" << endl;
                //return (x < data[lo]) ?  lo-1 :  lo;
                return hi;
            } 

            /**
             * 
             * Branchfree Binary Search of x in data in the interval [l,r]
             * 
             * @param data an array of type T
             * @param x a number of type T
             * @param l start index of search interval
             * @param r end index of search interval
             * @param prefetch bool 
             * 
             * @return The index of predecessor of x
             * 
            */
            static uint64_t branchfreeBS(T *data, T x,  uint64_t left, uint64_t right, bool prefetch){
                
                const T *base = &data[left];
                uint64_t n = right - left +1;
                //cout << n << endl;
                while (n > 1) {
                    uint64_t half = n / 2;
                    #if DEBUG
                        cout << "half:" << half << endl;
                        cout << "base[half]:" << base[half] << endl;
                        cout << "x:" << x << endl;
                    #endif

                
                    if (prefetch) {
                        __builtin_prefetch(base + half/2, 0, 0);
                        __builtin_prefetch(base + half + half/2, 0, 0);
                    }
                    
                    base = (base[half] <= x) ? base+half : base; 
                    n -= half;
                }
                // cout << "Searching " << x << endl;
                // cout << "Result: " <<  *(base - (*base > x)) << " in " << base - (*base > x) - data << endl;
                return  base - (*base > x) - data ;
            }

            /**
             * 
             * Branchy Interpolation Search of x in data in the interval [l,r]
             * 
             * @param data an array of type T
             * @param x a number of type T
             * @param start start index of search interval
             * @param end end index of search interval
             * 
             * @return The index+1 of x or 0 if not found
             * 
            */
            static uint64_t branchyIS(T *arr, T x, uint64_t start, uint64_t end)
            {
                uint64_t lo, hi;
                lo = start;
                // Find indexes of two corners
                if(end != 0)
                    hi = (end - 1);
                else 
                    hi = 0;
                // Since array is sorted, an element present
                // in array must be in range defined by corner
                while (lo <= hi && x >= arr[lo] && x <= arr[hi])
                {
                    if (lo == hi)
                    {
                        if (arr[lo] == x)
                            return lo+1;
                        return 0;
                    }
                    // Probing the position with keeping
                    // uniform distribution in mind.
                    uint64_t pos = lo + (((double)(hi - lo) /
                                    (arr[hi] - arr[lo])) *
                                    (x - arr[lo]));

                    // Condition of target found
                    if (arr[pos] == x)
                        return pos+1;

                    // If x is larger, x is in upper part
                    if (arr[pos] < x)
                        lo = pos + 1;

                    // If x is smaller, x is in the lower part
                    else
                        hi = pos - 1;
                }
                return 0;
            }

            /**
             * 
             * Three Point Interpolation Search of x in data in the interval [l,r]
             * 
             * @param A an array of type T
             * @param x a number of type T
             * @param start start index of search interval
             * @param end end index of search interval
             * @param guard Size of interval in which perform Sequential Search (default 8)
             * 
             * @return The index of predecessor of x
             * 
            */
            static uint64_t tipFind(T *A, T x, uint64_t start, uint64_t end, uint64_t guard=8)
            {
                uint64_t left = start, right = end;
                uint64_t mid = end/2;
                double y0 = A[left] - x;
                double y1 = A[mid] - x;
                double y2 = A[right] - x;
                //cout << A[left] << "-" << x << "=" << y0 << " " << y1 << " " << y2 << endl;
                double num = (y1*(mid-left)*(1+(y0-y1)/(y1-y2)));
                double den = y0-y2*((y0-y1)/(y1-y2));
                //double num = y1*(mid-right)*(mid-left)*(y2-y0);
                //double den = y2*(mid - right)*(y0-y1) + y0*(mid-left)*(y1-y2);
//  y_1 * (x_1 - x_2) * (x_1 - x_0) * (y_2 - y_0) /
//         (y_2 * (x_1 - x_2) * (y_0 - y_1) +
//             y_0 * (x_1 - x_0) * (y_1 - y_2))
                //cout << num << endl;
                //cout << den << endl;

                uint64_t expected = mid + num / den;
                //cout << "FIRST EXPECTED = " << expected << endl;
                //cout << mid << endl;
                //cout << num/den << endl;
                //while (left < right && left <= expected && right >= expected){
                while (true){
                    
                    //cout << left << " " << right << " " << expected << endl;
                    if(mid <= left || mid >= right || expected <= left || expected >= right || 
                        mid == expected || mid == left || mid == right)
                        return search(A,x,left, right);

                    //cout << "SONO NEL WHILE" << endl;
                    if(abs((double)expected - (double)mid) < guard){
                        //cout << "Sono qui!" << endl;
                        return search(A, x, left, right);
                    }
                    //cout << "NON E' IL PRIMO IF"<<endl;
                    if(A[mid] != A[expected]){
                        if(mid < expected)
                            left = mid;
                        else
                            right = mid;
                    
                        if(expected+guard >= right){
                            //cout << "Sono qui!2" << endl;
                            return search(A, x, left, right);
                        }else if(expected-guard <= left){
                            //cout << expected <<endl;
                            //cout << "Sono qui!3" << endl;
                            return search(A, x, left, right);
                        }
                    }

                    mid = expected;
                    y0 = A[left] - x;
                    y1 = A[mid] - x;
                    y2 = A[right] - x;
                    num = (y1*(mid-right)*(mid-left)*(y2-y0));
                    den = (y2*(mid-right)*(y0-y1)+y0*(mid-left)*(y1-y2));
                    expected = mid + num/den;

                    //cout << num << endl;
                    //cout << den << endl;
                    //cout << "EXPECTED = " << expected << endl;
                    //cout << mid << endl;
                    //cout << num/den << endl;
                
                }
                //return 0;
            }

            /**
             * 
             * K-ary Search of x in data in the interval [l,r]
             * 
             * @param A an array of type T
             * @param x a number of type T
             * @param start start index of search interval
             * @param end end index of search interval
             * @param k Number of interval
             * 
             * @return The index of predecessor of x
             * 
            */
            static uint64_t kBFS(T *keys, T searchKey, uint64_t start, uint64_t end, uint64_t k)
            {
                // left is inclusive, right is exclusive
                uint64_t left = start, right = end;
                while (left < right)
                {
                    //COUNT_ITERATION();
                    // divide [left, right) in k segments
                    uint64_t segmentLeft = left;
                    uint64_t segmentRight = left + (1 * (right - left)) / k;
                    for (uint64_t i = 2; i <= k; ++i)
                    {
                        // advance to the next segment
                        uint64_t nextSeparatorIndex = left + (i * (right - left)) / k;
                        segmentLeft = searchKey > keys[segmentRight] ? segmentRight + 1 : segmentLeft;
                        segmentRight = searchKey > keys[segmentRight] ?  nextSeparatorIndex : segmentRight;
                    }
                    left = segmentLeft;
                    right = segmentRight;
                }
                assert(left == right);
                return left;
            }

            static uint64_t kBBS(T *A, T x, uint64_t start, uint64_t end, uint64_t k){
                 // left is inclusive, right is exclusive
                uint64_t left = start, right = end;
                while (left < right)
                {
                    //COUNT_ITERATION();
                    // divide [left, right) in k segments
                    uint64_t segmentLeft = left;
                    uint64_t segmentRight = left + (right - left) / k;
                    for (uint64_t i = 2; i <= k; ++i)
                    {
                        if (x <= A[segmentRight]) break;
                        // advance to the next segment
                        segmentLeft = segmentRight + 1;
                        segmentRight = left + (i * (right - left)) / k;
                    }
                    left = segmentLeft;
                    right = segmentRight;
                }
                //assert(left == right);
                return left;

            }
            
    };
};