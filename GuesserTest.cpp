/**
 * Unit Tests for the class
**/

#include <string>
#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

// constructor truncation

TEST(GuesserTest, ShortSecret_MatchExact_ReturnsTrue) {
    Guesser g("hello");
    ASSERT_TRUE(g.match("hello"));
}

TEST(GuesserTest, LongSecret_TruncatedTo32_AllowsOnlyFirst32Chars) {
    std::string long_s(40, 'Z');
    std::string trunc32(32, 'Z');
    Guesser g(long_s);
    ASSERT_TRUE(g.match(trunc32));
}

// exact match resets remaining

TEST(GuesserTest, ExactMatch_ResetsRemainingToThree) {
    Guesser g("foo");
    g.match("fob");    // close wrong guess remaining→2
    g.match("foo");    // correct
    ASSERT_EQ(g.remaining(), 3u);
}

// close wrong guesses

TEST(GuesserTest, CloseWrongGuess_ReturnsFalse) {
    Guesser g("secret");
    ASSERT_FALSE(g.match("secrex"));  // one char off
}

TEST(GuesserTest, CloseWrongGuess_DecrementsRemaining) {
    Guesser g("secret");
    g.match("secrex");
    ASSERT_EQ(g.remaining(), 2u);
}

// far wrong guesses

TEST(GuesserTest, FarWrongGuess_LocksAndDecrementsRemaining) {
    Guesser g("abc");
    g.match("xxx");    // distance 3 → lock
    ASSERT_EQ(g.remaining(), 2u);
}

TEST(GuesserTest, FarWrongGuess_ThenMatchAlwaysFails) {
    Guesser g("abc");
    g.match("xxx");
    ASSERT_FALSE(g.match("abc"));
}

// exhausting three close failures

TEST(GuesserTest, ThreeCloseFailures_ThenRemainingZero) {
    Guesser g("abc");
    g.match("abd");
    g.match("adc");
    g.match("bac");
    ASSERT_EQ(g.remaining(), 0u);
}

TEST(GuesserTest, ThreeCloseFailures_ThenAnyMatchFails) {
    Guesser g("abc");
    g.match("abd");
    g.match("adc");
    g.match("bac");
    ASSERT_FALSE(g.match("abc"));
}
