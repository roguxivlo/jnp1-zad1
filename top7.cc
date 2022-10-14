#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

using std::string;
using std::stringstream;
using std::unordered_set;
using std::unordered_map;
using std::vector;
using std::for_each;
using std::pair;


using std::cin;
using std::cout;
using std::cerr;

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

static const song_num_t maxTestSongNumber = 999;

// Global variables
namespace {
    constexpr song_num_t max_song_number = 99999999;
    song_num_t current_max_song = 0;

    charts_t current_chart;
    top_t top_songs;
    rejected_t rejected_songs;

    line_number_t line_number = 0;
    line_t line;

    song_num_t last_chart[7], last_top_call[7];
}


void wrong_line() {
    cerr << "Error in line " << line_number << ": " << line;
}


// Write votes in chart and checks for correctness
void vote(const vote_list_t &votes) {
    for_each(votes.begin(), votes.end(), [](auto song) { ++current_chart[song]; });
}

void writeTop7(const vector<pair<song_num_t, change_t>> &top7);

bool cmp(pair<song_num_t, song_vote_t> a, pair<song_num_t, song_vote_t> b) {
    // sort descending by second attribute, then ascending by
    // first attribute:
    if (a.second > b.second) return true;
    else {
        return a.first < b.first;
    }
}

// Write out top 7 songs from chart, reject some songs, and reset chart.
void summarize() {
    // Sort current_chart elements:
    vector<pair<song_num_t, song_vote_t>> songVotes;
    for (auto& it : current_chart) {
        songVotes.push_back(it);
    }
    sort(songVotes.begin(), songVotes.end(), cmp);

    

}

// 
void top() {

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

/*TESTING*/
vote_list_t random_vote(const size_t size) {
    vote_list_t result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = random() % maxTestSongNumber + 1;
    }

    return result;
}

void print_chart() {
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


int main() {
    while (getline(cin, line)) {
        ++line_number;
        parse();
    }

    //Test funkcji vote:
    int numberOfTests = 10;
    srandom(time(nullptr));

    for (int i = 0; i < numberOfTests; ++i) {
        auto tmp = random_vote(static_cast<size_t>(random() % 20 + 1));
        vote(tmp);
        print_vote(tmp);
    }

    print_chart();
    //koniec testu vote

    return 0;
}
