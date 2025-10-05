#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext);

/**
 * Print instructions for using the program.
 */
void printMenu();

int main() {
  Random::seed(time(NULL));
  string command;

  vector<string> dictionary;
  ifstream dictFile("dictionary.txt");
  string word;
  while (dictFile >> word) {
    dictionary.push_back(word);
  }
  dictFile.close();

  vector<string> quadgrams;
  vector<int> counts;
  ifstream quadFile("english_quadgrams.txt");
  string line;
  while (getline(quadFile, line)) {
    size_t commaPos = line.find(',');
    if (commaPos != string::npos) {
      string quadgram = line.substr(0, commaPos);
      int count = stoi(line.substr(commaPos + 1));
      quadgrams.push_back(quadgram);
      counts.push_back(count);
    }
  }
  quadFile.close();
  QuadgramScorer scorer(quadgrams, counts);

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dictionary);
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      string inputFile, outputFile;
      cout << "Enter input filename: ";
      getline(cin, inputFile);
      cout << "Enter output filename: ";
      getline(cin, outputFile);

      ifstream inFile(inputFile);
      string cipherText;
      string fileLine;
      while (getline(inFile, fileLine)) {
        cipherText += fileLine;
        if (!inFile.eof()) cipherText += "\n";
      }
      inFile.close();

      string cleanedText;
      for (char c : cipherText) {
        if (isalpha(c)) {
          cleanedText += toupper(c);
        } else if (isspace(c)) {
          cleanedText += c;
        }
      }

      vector<char> bestKey = decryptSubstCipher(scorer, cleanedText);

      string decrypted;
      for (char c : cipherText) {
        if (isalpha(c)) {
          char upperCase = toupper(c);
          int pos = ALPHABET.find(upperCase);
          decrypted += bestKey[pos];
        } else {
          decrypted += c;
        }
      }

      ofstream outFile(outputFile);
      outFile << decrypted;
      outFile.close();
      cout << "Decrypted text written to " << outputFile << endl;
    } else if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  int assignedChar = ALPHABET.find(c);
  int rotAmount = (assignedChar + amount) % 26;

  return ALPHABET[rotAmount];
}

string rot(const string& line, int amount) {
  std::string result;
  for (int i = 0; i < line.length(); i++) {
    if (isalpha(line[i])) {
      char upperCase = toupper(line[i]);
      int assignedChar = ALPHABET.find(upperCase);
      int rotAmount = (assignedChar + amount) % 26;
      int index = ALPHABET[rotAmount];
      result += index;
    } else if (isspace(line[i])) {
      result += line[i];
    }
  }
  return result;
}

void caesarEncryptCommand() {
  int rotationAmount;
  string word;

  cout << "Enter the text to encrypt: " << endl;
  getline(cin, word);

  cout << "Enter the number of characters to rotate by: " << endl;
  cin >> rotationAmount;

  string encrypted = rot(word, rotationAmount);
  cout << encrypted << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  for (string& s : strings) {
    s = rot(s, amount);
  }
}

string clean(const string& s) {
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      result += toupper(c);
    }
  }
  return result;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student
  vector<string> result;
  stringstream ss(s);
  string word;
  while (ss >> word) {
    string cleanedWord = clean(word);
    if (!cleanedWord.empty()) {
      result.push_back(cleanedWord);
    }
  }
  return result;
}

string joinWithSpaces(const vector<string>& words) {
  string result;
  for (size_t i = 0; i < words.size(); i++) {
    result += words[i];
    if (i < words.size() - 1) {
      result += " ";
    }
  }
  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  int count = 0;
  for (const string& word : words) {
    for (const string& dictWord : dict) {
      if (word == dictWord) {
        count++;
        break;
      }
    }
  }
  return count;
}

void caesarDecryptCommand(const vector<string>& dict) {
  string ciphertext;
  cout << "Enter the text to decrypt: ";
  getline(cin, ciphertext);

  vector<string> words = splitBySpaces(ciphertext);
  bool foundGoodDecryption = false;

  for (int shift = 0; shift < 26; shift++) {
    vector<string> decryptedWords = words;
    rot(decryptedWords, shift);

    int validWords = numWordsIn(decryptedWords, dict);
    if (validWords > decryptedWords.size() / 2) {
      cout << joinWithSpaces(decryptedWords) << endl;
      foundGoodDecryption = true;
    }
  }

  if (!foundGoodDecryption) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      char upperCase = toupper(c);
      int pos = ALPHABET.find(upperCase);
      result += cipher[pos];
    } else {
      result += c;
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  string text;
  cout << "Enter the text to encrypt: ";
  getline(cin, text);

  string upperText;
  for (char c : text) {
    if (isalpha(c)) {
      upperText += toupper(c);
    } else {
      upperText += c;
    }
  }

  vector<char> randomCipher = genRandomSubstCipher();
  string encrypted = applySubstCipher(randomCipher, upperText);
  cout << encrypted << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  double totalScore = 0.0;
  string cleaned;
  for (char c : s) {
    if (isalpha(c)) {
      cleaned += toupper(c);
    }
  }

  for (size_t i = 0; i <= cleaned.length() - 4; i++) {
    string quadgram = cleaned.substr(i, 4);
    totalScore += scorer.getScore(quadgram);
  }

  return totalScore;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  string text;
  cout << "Enter a string for scoring: ";
  getline(cin, text);

  double score = scoreString(scorer, text);
  cout << score << endl;
}

vector<char> hillClimb(const QuadgramScorer& scorer, const string& ciphertext) {
  vector<char> currentKey = genRandomSubstCipher();
  double currentScore =
      scoreString(scorer, applySubstCipher(currentKey, ciphertext));

  int trialsWithoutImprovement = 0;
  while (trialsWithoutImprovement < 1000) {
    vector<char> newKey = currentKey;
    int pos1 = Random::randInt(25);
    int pos2 = Random::randInt(25);
    while (pos1 == pos2) {
      pos2 = Random::randInt(25);
    }

    char temp = newKey[pos1];
    newKey[pos1] = newKey[pos2];
    newKey[pos2] = temp;

    double newScore = scoreString(scorer, applySubstCipher(newKey, ciphertext));

    if (newScore > currentScore) {
      currentKey = newKey;
      currentScore = newScore;
      trialsWithoutImprovement = 0;
    } else {
      trialsWithoutImprovement++;
    }
  }

  return currentKey;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  vector<char> bestKey;
  double bestScore = -1e9;

  for (int run = 0; run < 25; run++) {
    vector<char> key = hillClimb(scorer, ciphertext);
    double score = scoreString(scorer, applySubstCipher(key, ciphertext));

    if (score > bestScore) {
      bestScore = score;
      bestKey = key;
    }
  }

  return bestKey;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  string ciphertext;
  string cleanedText;

  cout << "Enter the text to decrypt: ";
  getline(cin, ciphertext);

  for (char c : ciphertext) {
    if (isalpha(c)) {
      cleanedText += toupper(c);
    } else {
      cleanedText += c;
    }
  }

  vector<char> bestKey = decryptSubstCipher(scorer, cleanedText);
  string decrypted = applySubstCipher(bestKey, cleanedText);
  cout << decrypted << endl;
}

#pragma endregion SubstDec
