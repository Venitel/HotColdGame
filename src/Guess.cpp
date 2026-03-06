#include "Guess.h"
#include <cmath>

Guess::Guess(int guessedNumber, int targetNumber)
    :   guessedNumber_{guessedNumber}, 
        targetNumber_{targetNumber}
{}

bool Guess::operator<(const Guess &other) const
{
    return getDiffFromTarget() < other.getDiffFromTarget();
}

bool Guess::operator==(const Guess &other) const
{
    return guessedNumber_ == other.guessedNumber_;
}

int Guess::getGuessedNumber() const
{
    return guessedNumber_;
}

bool Guess::isCorrect() const
{
    return guessedNumber_ == targetNumber_;
}

int Guess::getDiffFromTarget() const
{
    return abs(targetNumber_ - guessedNumber_);
}

int Guess::getPercentFromTarget(int maxNumber) const
{
    return round((getDiffFromTarget() * 100.0) / maxNumber);
}
