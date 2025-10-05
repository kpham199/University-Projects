#include "include/lyrics.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  int firstLetter = 0;
  int lastLetter = token.length() - 1;
  string result;
  bool alphaExist = false;

  for (char c : token) {
    if (isalpha(c)) {
      alphaExist = true;
      break;
    }
  }

  if (!alphaExist) return "";

  while (firstLetter < token.length() && ispunct(token[firstLetter]))
    firstLetter++;
  while (lastLetter >= 0 && ispunct(token[lastLetter]))
    lastLetter--;

  for (int i = firstLetter; i <= lastLetter; i++) {
    result += tolower(token[i]);
  }

  return result;
}

set<string> gatherTokens(const string& text) {
  set<string> result;
  string word;
  for (char c : text) {
    if (isspace(c)) {
      if (!word.empty()) {
        string cleanedToken = cleanToken(word);
        if (!cleanedToken.empty()) {
          result.insert(cleanedToken);
        }
        word = "";
      }
    } else {
      word += c;
    }
  }

  if (!word.empty()) {
    string cleanedToken = cleanToken(word);
    if (!cleanedToken.empty()) {
      result.insert(cleanedToken);
    }
  }
  return result;
}

int buildIndices(const string& filename,
                 map<string, set<string>>& inverted_index,
                 map<string, string>& artist_index) {
  ifstream file(filename);
  string songName;
  string songArtist;
  string lyric;
  set<string> lyrics;
  int song_num = 0;
  while (getline(file, songName) && getline(file, songArtist) &&
         getline(file, lyric)) {
    artist_index[songName] = songArtist;
    lyrics = gatherTokens(lyric);
    for (auto uniqueLyrics : lyrics) {
      inverted_index[uniqueLyrics].insert(songName);
    }
    song_num++;
  }
  return song_num;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& query) {
  istringstream iss(query);
  string token;
  set<string> result;
  bool isFirstTerm = true;

  set<string> allSongs;
  for (auto& [word, titles] : index) {
    allSongs.insert(titles.begin(), titles.end());
  }

  while (iss >> token) {
    char modifier = (token[0] == '+' || token[0] == '-') ? token[0] : '\0';
    string cleaned = modifier ? cleanToken(token.substr(1)) : cleanToken(token);
    if (cleaned.empty()) continue;

    set<string> matches;
    if (index.count(cleaned)) matches = index.at(cleaned);

    if (isFirstTerm) {
      if (modifier == '-') {
        result = allSongs;
        for (auto& song : matches) result.erase(song);
      } else {
        result = matches;
      }
      isFirstTerm = false;
      continue;
    }

    if (modifier == '+') {
      set<string> newResult;
      for (auto& song : result) {
        if (matches.count(song)) newResult.insert(song);
      }
      result = std::move(newResult);
    } else if (modifier == '-') {
      for (auto& song : matches) {
        result.erase(song);
      }
    } else {
      for (auto& song : matches) {
        result.insert(song);
      }
    }
  }

  return result;
}

void searchEngine(const string& filename) {
    map<string, set<string>> invertedIndex;
    map<string, string> songToArtist;
    set<string> uniqueArtists;

    ifstream inputFile(filename);

    int songCount = 0;
    int wordCount = 0;
    int artistCount = 0;

    if (!inputFile) {
        cout << "Invalid filename." << endl;
    }

    cout << "Stand by while building indices..." << endl;

    if (inputFile) {
        songCount = buildIndices(filename, invertedIndex, songToArtist);
        wordCount = invertedIndex.size();

        for (const auto& [song, artist] : songToArtist) {
            uniqueArtists.insert(artist);
        }
        artistCount = uniqueArtists.size();
    }

    cout << "Indexed " << songCount << " songs containing "
         << wordCount << " unique terms and " << artistCount << " artists." << endl;

    string queryLine;
    cout << "Enter query sentence (press enter to quit): ";
    while (getline(cin, queryLine)) {
        if (queryLine.empty()) break;

        set<string> results = findQueryMatches(invertedIndex, queryLine);
        cout << "Found " << results.size() << " matching songs" << endl;

        for (const string& title : results) {
            string artist = (songToArtist.count(title) ? songToArtist.at(title) : "Unknown");
            cout << title << " by " << artist << endl;
        }

        cout << "Enter query sentence (press enter to quit): ";
    }

    cout << "Thank you for searching our Lyrics DB!" << endl;
  }