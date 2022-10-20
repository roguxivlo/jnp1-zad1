#include <iostream>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <limits>


using std::pair;
using std::string;
using std::stringstream;
using std::set;
using std::unordered_set;
using std::unordered_map;
using std::vector;

using std::all_of;
using std::for_each;
using std::make_pair;
using std::numeric_limits;

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

using song_set_t = unordered_set<song_num_t>;
using charts_t = unordered_map<song_num_t, song_vote_t>;
using top_t = unordered_map<song_num_t, points_t>;
using vote_list_t = vector<song_num_t>;


// Test constant
static const song_num_t maxTestSongNumber = 9999;


/* Global variables */
namespace {
    constexpr size_t ranking_length = 7;
    constexpr song_num_t max_song_number = 99999999;
    song_num_t current_max_song = 0;

    charts_t current_chart;
    top_t top_songs;
    song_set_t rejected_songs;

    line_number_t line_number = 0;
    line_t line;

    vector<song_num_t> last_chart, last_top_call;

    bool open_voting = false;

    struct cmp {
        bool operator() (const pair<song_vote_t, song_num_t> &a,
            const pair<song_vote_t, song_num_t> &b) 
            const {
                if (a.first > b.first) return true;
                if (a.first < b.first) return false;
                if (a.second < b.second) return true;
                return false;
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

void print_chart_last() {
    cout << "Printing lasst chart:\n";
    for (const auto &i: last_chart) {
        cout << "(" << i << ")\t";
    }
    cout << "\n";
}

void print_vote(const vote_list_t &votes) {
    cout<<"\nPrinting vote:\n";
    for (auto v: votes) {
        cout << v << "\t";
    }
    cout << "\n";
}


/* Writing output */
void writeTop7(const set<pair<song_vote_t, song_num_t>, cmp> &bestSongs, const vector<song_num_t> &last) {
    int currentRank = 1, previousRank;

    for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
        auto iter = std::find(last.begin(), last.end(), it->second);
        if (iter != last.end()) {
            previousRank = iter - last.begin() + 1;
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
void vote(const song_set_t &votes) {
    for_each(votes.begin(), votes.end(), [](auto song) { ++current_chart[song]; });
}


// Write out top 7 songs from chart, reject some songs, and reset chart.
void summarize() {
    if (current_max_song > 0) {
        // Get top7 songs from current_chart using std::set.
        set<pair<song_vote_t, song_num_t>, cmp> bestSongs;

        for (auto const& [song, votes] : current_chart) {
            bestSongs.insert(std::make_pair(votes, song));
            if (bestSongs.size() > ranking_length) {
                bestSongs.erase((--bestSongs.end()));
            }
        }
        
        // Write out top 7 songs from chart.
        writeTop7(bestSongs, last_chart);

        
        // Update top_songs.
        points_t points = ranking_length;
        for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
            top_songs[it->second] += points;
            points--;
        }

        // Reject some songs.
        unordered_set<song_num_t> bestSongsUS;
        for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
            bestSongsUS.emplace(it->second);
        }
        for (auto it = last_chart.begin(); it != last_chart.end(); ++it) {
            if (!bestSongsUS.contains(*it)) {
                rejected_songs.insert(*it);
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

void top() {
    // Get top7 songs from top_songs usinng std::set.
    set<pair<song_num_t, points_t>, cmp> bestSongs;
     for (auto const& [song, points] : top_songs) {
        bestSongs.insert(std::make_pair(points, song));
        if (bestSongs.size() > ranking_length) {
            bestSongs.erase((--bestSongs.end()));
        }
    }

    // Write top songs:
    writeTop7(bestSongs, last_top_call);

    // Update last_top_call:
    last_top_call.clear();
    for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
        last_top_call.push_back(it->second);
    }


    // reject from top_songs rejected songs which have not made it
    // to the top7 rankingm since they will never be in top7 again.

    vector<song_num_t> bestSongsVec;
    for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
        bestSongsVec.push_back(it->second);
    }

    for (auto it = top_songs.begin(); it != top_songs.end();) {
        if (rejected_songs.contains(it->first)) {
            auto iter = find(bestSongsVec.begin(), bestSongsVec.end(), it->first);
            if (iter == bestSongsVec.end()) {
                it = top_songs.erase(it);
            }
            else {
                it++;
            }
        }
        else {
            it++;
        }
    }
}


/* Input parsing */
void wrong_line() {
    cerr << "Error in line " << line_number << ": " << line << "\n";
}

void cast_votes() {
    if (!open_voting) {
        wrong_line();
    } else {
        stringstream ss(line);
        song_set_t votes;
        song_num_t song;
        while (ss >> song) {
            if (song == 0 || song > current_max_song ||
                rejected_songs.contains(song) || votes.contains(song)) {
                wrong_line();
                return;
            }
            votes.emplace(song);
        }
        if (song == 0 || song == numeric_limits<song_num_t>::max()) {
            wrong_line();
            return;
        }
        vote(votes);
    }
}

void new_charts(stringstream &ss) {
    string temp;
    song_num_t maximum;
    ss >> maximum; // reads max song number
    if (ss >> temp || maximum == 0 || maximum < current_max_song ||
        maximum > max_song_number) {
        // something more or song number is too large
        wrong_line();
    } else {
        if (open_voting) {
            summarize();
        }
        current_max_song = maximum;
        open_voting = true;
    }
}

void write_top_songs(stringstream &ss) {
    string temp;
    if (ss >> temp) { // something more...
        wrong_line();
    } else {
        top();
    }
}

void parse() {
    stringstream ss(line);
    string word;
    ss >> word;
    if (word == "NEW") {
        new_charts(ss);
    } else if (word == "TOP") {
        write_top_songs(ss);
    } else if (!all_of(line.begin(), line.end(), isspace)) {
        cast_votes();
    }
}



int main() {
    while (getline(cin, line)) {
        ++line_number;
        parse();
    }
    return 0;
}