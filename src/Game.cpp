#include "Game.h"
#include "ConfigLoader.h"
#include "Section.h"
#include "Guess.h"

#include <iostream>
#include <algorithm>
#include <limits>

int getPositiveIntFromUser(const std::string& prompt)
{
  int userInput;
  while(true) 
  {
    std::cout << prompt;
    if(std::cin >> userInput && userInput > 0)
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return userInput;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input!\n";
  }
}

int getDifficulty()
{
  return getPositiveIntFromUser("Enter max number (positive): ");
}

bool checkGuess(Guess currentGuess, Guess lastGuess, int totalNumbers)
{
  int guessedNumber = currentGuess.getGuessedNumber();
  if(guessedNumber < 1 || guessedNumber > totalNumbers)
  {
    std::cout << "Out of range!\n";
    return false;
  }
  else if(currentGuess == lastGuess)
  {
    std::cout << "That's the same number...\n";
    return false;
  }
  return true;
}

void playRound(const int targetNumber, const int maxNumber, const std::vector<Section>& allSections)
{
  Guess lastGuess;
  int totalGuesses = 0;
  const Section* lastSection = nullptr; //pointer cause initialized with no value + repointed in a loop

  while(true)
  {
    Guess currentGuess(getPositiveIntFromUser("Guess a number between 1 and " + std::to_string(maxNumber) + ": "), targetNumber);

    if(checkGuess(currentGuess, lastGuess, maxNumber))
    {
      totalGuesses++;

      if(currentGuess.isCorrect())
      {
        std::cout << "\nYou got it!\nTotal guesses: " << totalGuesses << "\n";
        return;
      }
      else
      {
        //temporary Section so we can use overloaded < operator without needing a lambda
        Section tmpSection("", "", currentGuess.getPercentFromTarget(maxNumber), true); 
        auto sectionIt = lower_bound(allSections.begin(), allSections.end(), tmpSection);
        if(sectionIt != allSections.end()) 
        {
          bool isCloser = lastGuess.getGuessedNumber() > 0 && currentGuess < lastGuess; //< overloaded for guesses
          std::cout << "> " << sectionIt->genDisplay(isCloser, lastSection) << "\n";
          lastSection = &(*sectionIt);
        }

        lastGuess = currentGuess;
      }
    }
  }
}