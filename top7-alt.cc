#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

using std::pair;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using std::cin;
using std::cout;
using std::cerr;
using std::for_each;
using std::make_pair;
using std::sort;
using std::partial_sort;
using std::to_string;


/* Type definitions */
using line_t = string;
using line_number_t = uint64_t;

using song_num_t = uint32_t;
using points_t = uint64_t;
using change_t = int8_t;
using song_votes_t = uint64_t;

using vote_list_t = vector<song_num_t>;
using chart_elem_t = pair<song_num_t, song_votes_t>;
using charts_t = unordered_map<song_num_t, song_votes_t>;
using chart_list_t = vector<pair<song_num_t, change_t>>;

using top_elem_t = pair<song_num_t, points_t>;
using top_t = vector<top_elem_t>;
using song_set_t = unordered_set<song_num_t>;


// Test constant
static const song_num_t maxTestSongNumber = 9;


/* Global variables */
namespace {
    constexpr change_t ranking_length = 7;
    constexpr song_num_t max_song_number = 99999999;
    constexpr change_t new_song = 10;
    song_num_t current_max_song = 0;

    charts_t current_chart;
    top_t top_songs;
    song_set_t rejected_songs;

    line_number_t line_number = 0;
    line_t line;

    vector<song_num_t> last_chart(ranking_length), last_top_call(ranking_length);
    bool open_voting = false, after_first_chart = false;

    bool cmp(const chart_elem_t &a, const chart_elem_t &b) {
        return a.second > b.second || (a.second == b.second && a.first < b.first);
    }
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


void write_top7(const chart_list_t &top7) {
    // top7 zawiera tylko i wyłącznie 7 piosenek ze zmianami pozycji
    for (auto &[song, change] : top7) {
        if (song != 0) {
            cout << song << " " << (change == new_song ? "-" : to_string(change))
                 << "\n";
        }
    }
}


/* Algorithms */
// Write votes in chart
void vote(const song_set_t &votes) {
    for_each(votes.begin(), votes.end(), [](auto song) { ++current_chart[song]; });
}


/* Moja implementacja summarize
 */
// Write out top 7 songs from chart, reject some songs, and reset chart.
void summarize() {
    // Sort current_chart elements:
    vector<chart_elem_t> song_votes(current_chart.begin(), current_chart.end());
    sort(song_votes.begin(), song_votes.end(), cmp);

    // Compute changes for top 7 songs
    chart_list_t changes(ranking_length);
    change_t i = 0;
    for (auto &[song, votes] : song_votes) {
        if (i >= ranking_length) {
            break;
        } else {
            change_t diff = new_song;
            for (change_t j = 0; j < ranking_length; ++j) {
                if (last_chart[j] == song) {
                    diff = j - i; // diff = pozycja ostatnio - pozycja obecna
                    // jeśli np. spadek z 1 na 3 to zmiana -2 i analogicznie
                    // jeśli nie było ostatnio to zostaje new_song == 10
                    break;
                }
            }
            changes[i] = make_pair(song, diff);
            ++i;
        }
    }

    write_top7(changes);

    // clear current chart
    current_chart.clear();
    // reject some songs
    if (after_first_chart) {
        for (auto song: last_chart) {
            bool seen = false;
            for (const auto &[song2, y]: changes) {
                if (song == song2) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                rejected_songs.emplace(song);
            }
        }
    } else after_first_chart = true;
    // add points to top7 and remember this chart and add points to top 7
    for (auto k = 0; k < ranking_length; ++k) {
        last_chart[k] = changes[k].first;
    }

    for (auto k = 0; k < ranking_length; ++k) {
        bool added = false;
        for (auto &[song, pts]: top_songs) {
            if (song == last_chart[k]) {
                pts += ranking_length - k;
                added = true;
                break;
            }
        }
        if (!added) {
            top_songs.emplace_back(last_chart[k], ranking_length - k);
        }
    }

    // chyba więcej nie trzeba tutaj, tylko moze czyścić top z utworów daleko
    if (top_songs.size() <= ranking_length) {
        sort(top_songs.begin(), top_songs.end()); // żeby było posortowane zawsze
    } else {
        partial_sort(top_songs.begin(), top_songs.begin() + ranking_length,
                     top_songs.end(), cmp); // top 7 jest posortowane
        for (size_t k = ranking_length; k != top_songs.size(); ++k) {
            if (rejected_songs.contains(top_songs[k].first)) {
                top_songs.erase(top_songs.begin() + static_cast<ssize_t>(k));
                --k;
            }
        }
    }
}


//
void top() {

}


/* Input parsing */
void wrong_line() {
    cerr << "Error in line " << line_number << ": " << line << "\n";
}

void cast_votes(stringstream &ss) {
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
    vote(votes);
}

void new_charts(stringstream &ss) {
    string temp;
    song_num_t maximum;
    ss >> temp; // reads NEW
    ss >> maximum; // reads max song number
    if (ss >> temp || temp != "NEW" || maximum == 0 ||
        maximum < current_max_song || maximum > max_song_number) {
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
    ss >> temp; // reads TOP
    if (ss >> temp || temp != "TOP") { // something more...
        wrong_line();
    } else {
        top();
    }
}

void parse() {
    ++line_number;
    stringstream ss(line);
    if (line.starts_with("NEW")) {
        new_charts(ss);
    } else if (line.starts_with("TOP")) {
        write_top_songs(ss);
    } else if (!open_voting) { // no NEW before votes were cast
        wrong_line();
    } else {
        cast_votes(ss);
    }
}


int main() {
    while (getline(cin, line)) { // odczyta i przemieli jeśli jest EOF w linii
        parse();
    }

    /*
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
     */

    return 0;
}
