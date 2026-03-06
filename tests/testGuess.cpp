#include <gtest/gtest.h>
#include "Guess.h"
#include "Game.h"

TEST(GuessTest, GuessIsCorrect)
{
    Guess guess(33, 33);
    EXPECT_TRUE(guess.isCorrect());
}

TEST(GuessTest, GuessIsIncorrect)
{
    Guess guess(7, 33);
    EXPECT_FALSE(guess.isCorrect());
}

TEST(GuessTest, GuessPercentCorrectValue)
{
    Guess guess(10, 50);
    int percent = guess.getPercentFromTarget(100); //40
    EXPECT_EQ(percent, 40);
}

TEST(GuessTest, GuessPercentMin)
{
    Guess guess(999, 1000);
    int percent = guess.getPercentFromTarget(1000); //0.01
    EXPECT_GE(percent, 0);
}

TEST(GuessTest, GuessPercentMax)
{
    Guess guess(1, 1000);
    int percent = guess.getPercentFromTarget(1000); //99.9
    EXPECT_LE(percent, 100);
}

TEST(GuessTest, GuessPercentRounding)
{
    Guess guess(1, 11);
    int percent = guess.getPercentFromTarget(16); //62.5
    EXPECT_EQ(percent, 63);
}

TEST(CheckGuessTest, GuessValid)
{
    Guess prevGuess(1, 10);
    Guess guess(4, 10);
    EXPECT_TRUE(checkGuess(guess, prevGuess, 15));
}

TEST(CheckGuessTest, GuessTooBig)
{
    Guess prevGuess(1, 10);
    Guess guess(16, 10);
    EXPECT_FALSE(checkGuess(guess, prevGuess, 15));
}

TEST(CheckGuessTest, GuessTooSmall)
{
    Guess prevGuess(1, 10);
    Guess guess(0, 10);
    EXPECT_FALSE(checkGuess(guess, prevGuess, 15));
}

TEST(CheckGuessTest, GuessSameAsPrevious)
{
    Guess prevGuess(4, 10);
    Guess guess(4, 10);
    EXPECT_FALSE(checkGuess(guess, prevGuess, 15));
}