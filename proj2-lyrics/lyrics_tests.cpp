#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>
#include <set>
#include <string>

#include "include/lyrics.h"

using namespace std;
using namespace testing;

// CleanToken Test Cases//
TEST(CleanToken, StartEndPuncCleaning) {
  ASSERT_THAT(cleanToken(".helloworld"), StrEq("helloworld"));
  ASSERT_THAT(cleanToken("helloworld."), StrEq("helloworld"));
  ASSERT_THAT(cleanToken(".helloworld."), StrEq("helloworld"));
  ASSERT_THAT(cleanToken(".!,helloworld"), StrEq("helloworld"));
  ASSERT_THAT(cleanToken(".!,helloworld.!?"), StrEq("helloworld"));
  ASSERT_THAT(cleanToken("helloworld./"), StrEq("helloworld"));
  ASSERT_THAT(cleanToken(".!,helloworld"), StrEq("helloworld"));
  ASSERT_THAT(cleanToken("12314124"), StrEq(""));
}

TEST(CleanToken, MiddlePuncCleaning) {
  ASSERT_THAT(cleanToken("hello.world"), StrEq("hello.world"));
  ASSERT_THAT(cleanToken(".hello.world"), StrEq("hello.world"));
  ASSERT_THAT(cleanToken("hello.world."), StrEq("hello.world"));
  ASSERT_THAT(cleanToken(".hello.world."), StrEq("hello.world"));
  ASSERT_THAT(cleanToken(".hello.world.."), StrEq("hello.world"));
  ASSERT_THAT(cleanToken("..hello.world."), StrEq("hello.world"));
  ASSERT_THAT(cleanToken("..hello.world.."), StrEq("hello.world"));
}

// GatherTokens Test Cases//
//  Text with leading spaces
//  Text with trailing spaces
//  Text with multiple spaces between words
TEST(GatherTokens, LeadingSpaces) {
  string text = " real eyes realize real lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, LeadingSpacesX) {
  string text = "  real eyes realize real lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, LeadingSpacesXX) {
  string text = "   real eyes realize real lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingSpaces) {
  string text = "real eyes realize real lies ";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingSpacesX) {
  string text = "real eyes realize real lies  ";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingSpacesXX) {
  string text = "real eyes realize real lies   ";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, MultiSpacesBetWords) {
  string text = "real  eyes realize  real lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, MultiSpacesBetWordsX) {
  string text = "real   eyes realize   real lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, MultiSpacesBetWordsXX) {
  string text = "real eyes    realize real    lies";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingAndLeadingSpaces) {
  string text = "  real eyes realize real lies  ";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingLeadingMultiSpaceCombo) {
  string text = " real  eyes realize  real lies ";
  set<string> expected = {"real", "eyes", "realize", "lies"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// BuildIndices Test Cases//
TEST(BuildIndices, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedInvertedIndex = {
      {"bells", {"Brother John", "Carol of the bells", "Jingle bells"}},
      {"how", {"Carol of the bells"}},
      {"bags", {"Baa baa Black Sheep"}},
      {"the", {"Carol of the bells", "Jingle bells"}},
      {"ringing", {"Brother John"}},
      {"wool", {"Baa baa Black Sheep"}},
      {"silver", {"Carol of the bells"}},
      {"morning", {"Brother John"}},
      {"have", {"Baa baa Black Sheep"}},
      {"jingle", {"Jingle bells"}},
      {"sweet", {"Carol of the bells"}},
      {"sir", {"Baa baa Black Sheep"}},
      {"yes", {"Baa baa Black Sheep"}},
      {"three", {"Baa baa Black Sheep"}},
      {"all", {"Brother John", "Jingle bells"}},
      {"sleeping", {"Brother John"}},
      {"you", {"Baa baa Black Sheep", "Brother John"}},
      {"any", {"Baa baa Black Sheep"}},
      {"full", {"Baa baa Black Sheep"}},
      {"way", {"Jingle bells"}},
      {"are", {"Brother John"}},
      {"hark", {"Carol of the bells"}},
  };

  map<string, string> expectedArtistIndex = {
      {"Brother John", "Unknown"},
      {"Jingle bells", "James Pierpont"},
      {"Carol of the bells", "Mykola Leontovych"},
      {"Baa baa Black Sheep", "Unknown"}};

  map<string, set<string>> studentInvertedIndex;
  map<string, string> studentArtistIndex;
  int studentNumProcesed =
      buildIndices(filename, studentInvertedIndex, studentArtistIndex);

  string invertedIndexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) inverted index incorrect\n";
  EXPECT_THAT(studentInvertedIndex, ContainerEq(expectedInvertedIndex))
      << invertedIndexTestFeedback;

  string artistIndexTestFeedback =
      "buildIndices(\"" + filename + "\", ...) artist index incorrect\n";
  EXPECT_THAT(studentArtistIndex, ContainerEq(expectedArtistIndex))
      << artistIndexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

TEST(BuildIndices, FileNotFound) {
  string filename = "data/NA.txt";
  map<string, set<string>> studentInvertedIndex;
  map<string, string> studentArtistIndex;
  int studentNumProcessed =
      buildIndices(filename, studentInvertedIndex, studentArtistIndex);

  string retTestFeedback = "buildIndex(\"" + filename +
                           "\", ...) return value incorrect for missing file\n";
  EXPECT_THAT(studentNumProcessed, Eq(0)) << retTestFeedback;

  EXPECT_TRUE(studentInvertedIndex.empty())
      << "Inverted index should be empty when file not found\n";
  EXPECT_TRUE(studentArtistIndex.empty())
      << "Artist index should be empty when file not found\n";
}

// FindQueryMatches Test Cases//
map<string, set<string>> INDEX = {
    {"bells", {"Brother John", "Carol of the bells", "Jingle bells"}},
    {"how", {"Carol of the bells"}},
    {"bags", {"Baa baa Black Sheep"}},
    {"the", {"Carol of the bells", "Jingle bells"}},
    {"ringing", {"Brother John"}},
    {"wool", {"Baa baa Black Sheep"}},
    {"silver", {"Carol of the bells"}},
    {"morning", {"Brother John"}},
    {"have", {"Baa baa Black Sheep"}},
    {"jingle", {"Jingle bells"}},
    {"sweet", {"Carol of the bells"}},
    {"sir", {"Baa baa Black Sheep"}},
    {"yes", {"Baa baa Black Sheep"}},
    {"three", {"Baa baa Black Sheep"}},
    {"all", {"Brother John", "Jingle bells"}},
    {"sleeping", {"Brother John"}},
    {"you", {"Baa baa Black Sheep", "Brother John"}},
    {"any", {"Baa baa Black Sheep"}},
    {"full", {"Baa baa Black Sheep"}},
    {"way", {"Jingle bells"}},
    {"are", {"Brother John"}},
    {"hark", {"Carol of the bells"}},
};

TEST(FindQueryMatches, FirstQueryTermNotInIndex) {
  set<string> expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "hello"), ContainerEq(expected));
}

TEST(FindQueryMatches, LaterQueryTermNotInIndexWithModifier) {
  set<string> expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "bells +NA"), ContainerEq(expected));

  expected = {"Brother John", "Carol of the bells", "Jingle bells"};
  EXPECT_THAT(findQueryMatches(INDEX, "bells -NA"), ContainerEq(expected));
}

TEST(FindQueryMatches, PuncAndCaseSensitivity) {
  set<string> expected = {"Carol of the bells"};
  EXPECT_THAT(findQueryMatches(INDEX, "SWEET!!!"), ContainerEq(expected));
}

TEST(FindQueryMatches, LaterUnmodifiedTermNotInIndex) {
  set<string> expected = {"Brother John", "Carol of the bells", "Jingle bells"};
  EXPECT_THAT(findQueryMatches(INDEX, "bells what"), ContainerEq(expected));
}

TEST(FindQueryMatches, Combination) {
  set<string> expected = {"Brother John"};
  EXPECT_THAT(findQueryMatches(INDEX, "bells +ringing -jingle"),
              ContainerEq(expected));
}