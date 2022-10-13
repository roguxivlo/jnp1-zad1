#ifndef TESTING_HPP
#define TESTING_HPP

#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

using std::vector;
using std::unordered_map;

class Test {
    public:
        static vector<uint32_t> randomVector(const size_t size);
        static void printMap(const unordered_map<uint32_t, uint32_t> &map);
        static void printVector(const vector<uint32_t> &v);
};

#endif