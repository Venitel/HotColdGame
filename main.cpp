#include "Game.h"
#include "Section.h"
#include "ConfigLoader.h"

#include <iostream>
#include <random>

int main() 
{
  bool restart = false;
  std::vector<Section> allSections;
  try
  {
    allSections = initSections();
  }
  catch(const std::exception& exc)
  {
      std::cerr << "Error: " << exc.what();
      return 1;
  }

  //random number generator
  std::random_device rd;   
  std::mt19937 rng(rd());
  
  do
  {
    int maxNumber = getDifficulty();
    std::uniform_int_distribution<> dist(1, maxNumber);
    playRound(dist(rng), maxNumber, allSections);

    std::cout << "Play again? (Y/N): ";
  } while (toupper(std::cin.get()) == 'Y');

  return 0;
}