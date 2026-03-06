#ifndef GUESS_H
#define GUESS_H

class Guess
{
 public:     
    Guess() = default;
    Guess(int guessedNumber, int targetNumber);

    bool operator<(const Guess &other) const;
    bool operator==(const Guess &other) const;

    int getGuessedNumber() const;

    bool isCorrect() const;
    int getDiffFromTarget() const;
    int getPercentFromTarget(int maxNumber) const;
 private:
    int guessedNumber_ = 0; //init value (non-valid guess)
    int targetNumber_;
};

#endif