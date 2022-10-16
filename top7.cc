#include <iostream>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>


using std::string;
using std::stringstream;
using std::set;
using std::unordered_set;
using std::unordered_map;
using std::vector;
using std::for_each;
using std::pair;
using std::make_pair;


using std::cin;
using std::cout;
using std::cerr;


/* Type definitions */
using line_t = string;
using line_number_t = uint64_t;

using song_num_t = uint32_t;
using points_t = uint32_t;
using change_t = int8_t;
using song_vote_t = uint32_t;

using rejected_t = unordered_set<song_num_t>;
using charts_t = unordered_map<song_num_t, song_vote_t>;
using top_t = unordered_map<song_num_t, points_t>;
using vote_list_t = vector<song_num_t>;
// using top_songs_t = set<>


// Test constant
static const song_num_t maxTestSongNumber = 9;


/* Global variables */
namespace {
    constexpr size_t ranking_length = 7;
    constexpr song_num_t max_song_number = 99999999;
    song_num_t current_max_song = 0;

    charts_t current_chart;
    top_t top_songs;
    rejected_t rejected_songs;

    line_number_t line_number = 0;
    line_t line;

    vector<song_num_t> last_chart, last_top_call;

    struct cmp {
        bool operator() (const pair<song_vote_t, song_num_t> &a,
            const pair<song_vote_t, song_num_t> &b) 
            const {
                return (a.first > b.first ||
                        (!(b.first > a.first) && (a.second < b.second)));
        }
    };
}


/* TESTING */
vote_list_t random_vote(const size_t size) {
    vote_list_t result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = random() % maxTestSongNumber + 1;
    }

    return result;
}

void print_chart() {
    cout << "Printing chart:\n";
    for (const auto &i: current_chart) {
        cout << "(" << i.first << ": " << i.second << ")\t";
    }
    cout << "\n";
}

void print_vote(const vote_list_t &votes) {
    for (auto v: votes) {
        cout << v << "\t";
    }
    cout << "\n";
}


/* Writing output */
void writeTop7(const set<pair<song_vote_t, song_num_t>, cmp> &bestSongs) {
    int currentRank = 1, previousRank;

    for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
        auto iter = std::find(last_chart.begin(), last_chart.end(), it->second);
        if (iter != last_chart.end()) {
            previousRank = iter - last_chart.begin();
            cout << it->second << " " << previousRank - currentRank << "\n";
        }
        else {
            cout << it->second << " -\n";
        }
        currentRank++;
    }
}



/* Algorithms */
// Write votes in chart and checks for correctness
void vote(const vote_list_t &votes) {
    for_each(votes.begin(), votes.end(), [](auto song) { ++current_chart[song]; });
}

// debug
void print_set(const set<pair<song_vote_t, song_num_t>, cmp> &S) {
    cout << "Printing set:\n";
    for (auto it = S.begin(); it != S.end(); ++it) {
        cout << "(" << it->second << ": " << it->first << ") ";
    }
    cout << "\n";
}
// ~debug

// Write out top 7 songs from chart, reject some songs, and reset chart.
void summarize() {
    if (current_max_song > 0) {
        // Get top7 songs from current_chart using std::set.
        set<pair<song_vote_t, song_num_t>, cmp> bestSongs;

        for (auto const& [song, votes] : current_chart) {
            bestSongs.insert(std::make_pair(votes, song));
            if (bestSongs.size() > ranking_length) {
                bestSongs.erase(bestSongs.begin());
            }
        }
        
        // testing:
        print_set(bestSongs);
        // ~testing

        // Write out top 7 songs from chart.
        writeTop7(bestSongs);
        
        // Update top_songs.
        points_t points = ranking_length;
        for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
            top_songs[it->second] += points;
            points--;
        }

        // Reject some songs.
        for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
            if (std::find(last_chart.begin(), last_chart.end(), it->second) == last_chart.end()) {
                rejected_songs.insert(it->second);
            }
        }

        // Reset the chart.
        current_chart.clear();

        // Update last_chart.
        last_chart.clear();
        for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
            last_chart.push_back(it->second);
        }
    }
}

// 
void top() {

}


/* Input parsing */
void wrong_line() {
    cerr << "Error in line " << line_number << ": " << line;
}

void parse() {
    stringstream ss;
    if (line.starts_with("NEW")) {
        string temp;
        song_num_t maximum;
        ss >> temp; // reads NEW
        ss >> maximum; // reads max song number
        if (ss >> temp || maximum > max_song_number) { // something more...
            wrong_line();
            return;
        }
        current_max_song = maximum;
        summarize();

    } else if (line.starts_with("TOP")) {
        string temp;
        ss >> temp; // reads TOP
        if (ss >> temp) { // something more...
            wrong_line();
            return;
        }
        top();

    } else {
        vote_list_t votes;
        song_num_t song;
        while (ss >> song) {
            if (song == 0 || song > current_max_song ||
                rejected_songs.contains(song)) {
                wrong_line();
                return;
            }
            votes.emplace_back(song);
        }
        vote(votes);
    }
}



int main() {
    while (getline(cin, line)) {
        ++line_number;
        parse();
    }

    //Test funkcji vote:
    int numberOfTests = 5;
    srandom(time(nullptr));

    for (int i = 0; i < numberOfTests; ++i) {
        auto tmp = random_vote(static_cast<size_t>(random() % 3 + 1));
        vote(tmp);
        print_vote(tmp);
    }

    print_chart();
    //koniec testu vote

    // test funkcji summarize:
    summarize();

    // koniec testu summarize.

    return 0;
}
