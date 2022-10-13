#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "testing.h"

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

static const song_num_t MaxSongNumber = 99999999;

// Write votes in chart, does not check if vote vector is valid!
static void vote(const vote_t &vote, charts_t &chart) {
    for (votes_t i : vote) {

    }
}

// Write out top7 songs from chart, reject some songs, and
// reset chart.
static void summarise(charts_t &chart, rejected_t &rejectedSongs);

static void top(const top_t &topSongsOfAllTime);



int main() {
    /*
     * coś tam
     */

    return 0;
}
