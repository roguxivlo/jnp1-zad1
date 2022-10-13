#ifndef TESTING_H
#define TESTING_H

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

namespace test
{
    class Test {
        public:
            vector<uint32_t> randomVector(const size_t size);
            void testVote(void (*fun)(const vector<uint32_t>, unordered_map<uint32_t, uint32_t>));
    };
}
#endif