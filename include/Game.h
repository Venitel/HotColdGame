#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Section;
class Guess;

int getPositiveIntFromUser(const std::string& prompt);
int getDifficulty();
bool checkGuess(const Guess currentGuess, const Guess lastGuess, const int totalNumbers);
void playRound(const int targetNumber, const int maxNumber, const std::vector<Section>& sections);

#endif