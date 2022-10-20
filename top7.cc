#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>

using std::pair;
using std::string;
using std::stringstream;
using std::set;
using std::unordered_set;
using std::unordered_map;
using std::vector;

using std::for_each;
using std::numeric_limits;

using std::cin;
using std::cout;
using std::cerr;

// TODO w pliku alt jest wersja z interfejsem do testowania

/* Type definitions */
using line_t = string;
using line_number_t = uint64_t;

using song_num_t = uint32_t;
using points_t = uint64_t;
using song_votes_t = uint64_t;

using chart_elem_t = pair<song_num_t, song_votes_t>;
using top_elem_t = pair<song_num_t, points_t>;

using song_set_t = unordered_set<song_num_t>;
using charts_t = unordered_map<song_num_t, song_votes_t>;
using top_t = unordered_map<song_num_t, points_t>;
using song_list_t = vector<song_num_t>;


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

    song_list_t last_chart, last_top_call;

    bool open_voting = false;

    struct cmp {
        bool operator()(const chart_elem_t &a, const chart_elem_t &b) const {
            return a.second > b.second || (a.second == b.second &&
                                           a.first < b.first);
        }
    };
}


/* Writing output */
void writeTop7(const set<chart_elem_t, cmp> &bestSongs, const song_list_t &last) {
    int currentRank = 1;

    for (const auto &[song, pts]: bestSongs) {
        auto it = find(last.begin(), last.end(), song);
        if (it != last.end()) {
            auto previousRank = it - last.begin() + 1;
            cout << song << " " << previousRank - currentRank << "\n";
        } else {
            cout << song << " -\n";
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
        // Get top7 songs from current_chart using set.
        set<chart_elem_t, cmp> bestSongs;

        for (auto const &[song, votes]: current_chart) {
            bestSongs.emplace(song, votes);
            if (bestSongs.size() > ranking_length) {
                bestSongs.erase((--bestSongs.end()));
            }
        }

        // Write out top 7 songs from chart.
        writeTop7(bestSongs, last_chart);

        // Update top_songs.
        points_t points = ranking_length;
        for (const auto &[song, votes]: bestSongs) {
            top_songs[song] += points;
            points--;
        }

        // Reject some songs.
        song_set_t bestSongsUS;
        for (const auto &[song, votes]: bestSongs) {
            bestSongsUS.emplace(song);
        }
        for (auto song: last_chart) {
            if (!bestSongsUS.contains(song)) {
                rejected_songs.emplace(song);
            }
        }

        // Reset the chart.
        current_chart.clear();

        // Update last_chart.
        last_chart.clear();
        for (const auto &[song, votes]: bestSongs) {
            last_chart.emplace_back(song);
        }
    }
}

// 
void top() {
    // Get top7 songs from top_songs using set.
    set<top_elem_t, cmp> bestSongs;
    for (const auto &[song, points]: top_songs) {
        bestSongs.emplace(song, points);
        if (bestSongs.size() > ranking_length) {
            bestSongs.erase((--bestSongs.end()));
        }
    }

    // Write top songs:
    writeTop7(bestSongs, last_top_call);

    // Update last_top_call:
    last_top_call.clear();
    for (const auto &[song, pts]: bestSongs) {
        last_top_call.emplace_back(song);
    }

    // reject from top_songs rejected songs which have not made it
    // to the top7 ranking since they will never be in top7 again.
    // TODO do przemyślenia: czy powyższe oczyszczanie poprawi złożoność?
    // Jak będzie dużo notowań z różnymi utworami to w top_songs będzie
    // bardzo dużo elementów, więc z uwagi na tworzenie bestSongs za każdym
    // razem przy wywołaniu TOP - tak

    vector<song_num_t> bestSongsVec;
    for (auto it = bestSongs.begin(); it != bestSongs.end(); ++it) {
        bestSongsVec.push_back(it->first);
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
        // something more in text, error in conversion or song number is too large
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
    if (ss >> temp) { // something more than TOP...
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
