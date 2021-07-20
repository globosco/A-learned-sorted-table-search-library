#pragma once
#include "search.h"

template <typename KeyType>
class BFS : public Search<KeyType> {
public:
  uint64_t search(const std::vector<Row<KeyType>>& data,
                  const KeyType lookup_key,
                  size_t* num_qualifying, size_t start, size_t end) const {
    // Returns the sum over all values with the given lookup key.
    // Caution: data has to be sorted.
    
    *num_qualifying = 0;

    auto base = data.begin() + start;
    uint64_t n = end - start + 1;
    //cout << n << endl;
    while (n > 1) {
        uint64_t half = n / 2;
        #if DEBUG
            cout << "half:" << half << endl;
            cout << "base[half]:" << base[half] << endl;
            cout << "x:" << x << endl;
        #endif

    
        
        // __builtin_prefetch(base + half/2, 0, 0);
        // __builtin_prefetch(base + half + half/2, 0, 0);
        
        
        base = (base[half].key <= lookup_key) ? base+half : base; 
        n -= half;
    }
    //cout << "Searching " << x << endl;
    //cout << "Result: " <<  *(base - (*base > x)) << " in " << base - (*base > x) - data << endl;
    
  
    // Sum over all values with that key.
    
    ++(*num_qualifying);
    uint64_t result = base->data[0];

    while (++base!=data.end() && base->key==lookup_key) {
      result += base->data[0];
      ++(*num_qualifying);
    }
    return result;
  }

  std::string name() const {
    return "BinarySearch";
  }

};

