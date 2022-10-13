#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
// #include "testing.hpp"

using std::unordered_set;
using std::unordered_map;
using std::vector;

// Język zmiennych itd. można zawsze zmienić na polski jeśli tak wolisz
using song_num_t = uint32_t;
using votes_t = uint32_t;
using points_t = uint32_t;

using rejected_t = unordered_set<song_num_t>;
using charts_t = unordered_map<song_num_t, votes_t>;
using top_t = unordered_map<song_num_t, points_t>;
using vote_t = vector<votes_t>;

static const song_num_t MaxSongNumber = 999;

// Write votes in chart, does not check if vote vector is valid!
void vote(const vote_t &vote, charts_t &chart) {
    for (votes_t i : vote) {
        chart[i]++;
    }
}

// Write out top7 songs from chart, reject some songs, and
// reset chart.
static void summarise(charts_t &chart, rejected_t &rejectedSongs);

static void top(const top_t &topSongsOfAllTime);


/*TESTING*/
vote_t randomVote(const size_t size) {
    srand(time(NULL));

    vote_t result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = rand() % MaxSongNumber + 1;
    }

    return result;
}

void printChart(const charts_t &map) {
    for (auto i = map.begin(); i != map.end(); ++i) {
        std::cout<<"("<<i->first<<": "<<i->second<<")\t";
    }
    std::cout<<"\n";
}

void printVote(const vote_t &v) {
    for (uint32_t i : v) {
        std::cout<<i<<"\t";
    }
    std::cout<<"\n";
}



int main() {
    /*
     * coś tam
     */
    srand(time(NULL));
    
    //Test funkcji vote:
    int numberOfTests = 10;
    vote_t tmp;
    charts_t tmpMap;
    for (int i = 0; i < numberOfTests; ++i) {
        tmp = randomVote((size_t) (rand() % 20 + 1));
        vote(tmp, tmpMap);
        printVote(tmp);
    }
    printChart(tmpMap);
    //koniec testu vote


    return 0;
}
