#include <iostream>
#include <unordered_set>
#include <unordered_map>

using std::unordered_set;
using std::unordered_map;

// Język zmiennych itd. można zawsze zmienić na polski jeśli tak wolisz
using song_num_t = uint32_t;
using votes_t = uint32_t;
using points_t = uint32_t;

using rejected_t = unordered_set<song_num_t>;
using charts_t = unordered_map<song_num_t, votes_t>;
using top_t = unordered_map<song_num_t, points_t>;

int main() {
    /*
     * coś tam
     */

    return 0;
}
