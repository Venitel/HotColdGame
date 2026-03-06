#ifndef SECTION_H
#define SECTION_H

#include <string>

class Section
{
 public:     
  Section() = default;
  Section(std::string text, std::string ampamplifiedText, int percent, bool ampDirection);

  bool operator<(const Section &other) const;

  int getPercent() const;
  const std::string& getText() const;

  std::string genDisplay(const bool closerGuess, const Section* lastSection) const;

 private:
  std::string text_; 
  std::string amplifiedText_;
  int percent_;
  bool ampDirection_;
};

#endif