#include "testing.hpp"
#include <iostream>

static const uint32_t MaxSongNumber = 99999999;

vector<uint32_t> Test::randomVector(const size_t size) {
    srand(time(NULL));

    vector<uint32_t> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = rand() % MaxSongNumber + 1;
    }

    return result;
}

void Test::printMap(const unordered_map<uint32_t, uint32_t> &map) {
    for (auto i = map.begin(); i != map.end(); ++i) {
        std::cout<<"("<<i->first<<": "<<i->second<<")\t";
    }
    std::cout<<"\n";
}

void Test::printVector(const vector<uint32_t> &v) {
    for (uint32_t i : v) {
        std::cout<<i<<"\t";
    }
    std::cout<<"\n";
}

// void testVote(void (*fun)(const vector<uint32_t>&, unordered_map<uint32_t, uint32_t>&), int numberOfTests) {
//     srand(time(NULL));
//     vector<uint32_t>tmp;
//     unordered_map<uint32_t, uint32_t> tmpMap;
//     for (int i = 0; i < numberOfTests; ++i) {
//         tmp = randomVector((size_t) (rand() % 20 + 1));
//         fun(tmp, tmpMap);
//         printVector(tmp);
//     }
//     printMap(tmpMap);
// }