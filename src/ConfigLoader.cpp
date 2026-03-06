#include "ConfigLoader.h"
#include "Section.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

const std::string configFileName = "sections.cfg";
const std::string hierarchyTitle = ">> Hierarchy <<";
const std::string hierarchyPrefix = "* ";
const char lineSymbol = '-';

std::vector<Section> initSections()
{
  std::vector<Section> allSections = readSectionsFromConfig();
  if(allSections.empty())
  {
    throw std::runtime_error("No viable sections loaded from " + configFileName);    
  }
  //sort sections ascending (by %)
  sort(allSections.begin(), allSections.end());
  if(allSections.back().getPercent() < 100) //since its all sorted by %, the last value is the largest
  {
    std::cerr << "No section configured for 100% - possible user input with no feedback.\n";
  }

  displayHierarchy(allSections);

  return allSections;
}

std::vector<Section> readSectionsFromConfig()
{
  std::vector<Section> sections;
  std::ifstream file(configFileName);
  if(!file.is_open())
  {
    throw std::runtime_error("Failed to open config file: " + configFileName);    
  }

  std::string fileLine;
  while(getline(file, fileLine))
  {
    if(fileLine.empty() || fileLine[0] == '#') continue;

    ParseResult<Section> configParse = parseConfigLine(fileLine);
    if(!configParse.success)
    {
      std::cerr << "Config '" << fileLine << "' ignored. " << configParse.error << "\n";
      continue;
    }

    sections.push_back(configParse.value);
  }

  return sections;
}

ParseResult<Section> parseConfigLine(const std::string& configLine)
{
  std::stringstream stream(configLine);
  std::string textConfig, amplifiedTextConfig, percentConfig, ampDirectionConfig;

  if(!getline(stream, textConfig, '|')
    || !getline(stream, amplifiedTextConfig, '|')
    || !getline(stream, percentConfig, '|')
    || !getline(stream, ampDirectionConfig, '|'))
  {
    return {false, Section(), "Invalid config line."};
  }

  ParseResult<int> percentParse = parsePercent(percentConfig);
  if(!percentParse.success)
  {
    return {false, Section(), percentParse.error};
  }

  ParseResult<bool> ampDirectionParse = parseAmpDirection(ampDirectionConfig);
  if(!ampDirectionParse.success)
  {
    return {false, Section(), ampDirectionParse.error};
  }

  return {true, {textConfig, amplifiedTextConfig, percentParse.value, ampDirectionParse.value}, ""};
}

ParseResult<int> parsePercent(const std::string& percentString)
{
  int percent;
  try
  {
      percent = std::stoi(percentString);
  }
  catch(...)
  {
      return {false, 0, "The % parameter '" + percentString + "' has invalid value."};
  }

  if(percent < 0 || percent > 100)
  {
      return {false, 0, "Percent must be between 0 and 100."};
  }

  return {true, percent, ""};
}

ParseResult<bool> parseAmpDirection(const std::string& ampDirectionString)
{
  if(ampDirectionString.length() != 1 || (ampDirectionString[0] != '-' && ampDirectionString[0] != '+'))
  {
    return {false, false, "Amp direction must be either '+' or '-'"};
  }

  return {true, ampDirectionString[0] == '+', ""};
}

void displayHierarchy(std::vector<Section>& sections)
{
  std::ostringstream buffer;
  size_t maxLength = hierarchyTitle.length();

  for(const Section& section : sections)
  {
    std::string text = hierarchyPrefix + section.genDisplay(true, &section);
    //pass pointer to itself to "fake" 2nd guess in a row (for amplified text)
    std::string amplifiedText = hierarchyPrefix + section.genDisplay(false, &section);

    buffer << text << "\n" << amplifiedText << "\n";
    maxLength = std::max({maxLength, text.length(), amplifiedText.length()});
  }
  
  std::string line(maxLength, lineSymbol);
  std::cout << line << "\n" << hierarchyTitle << "\n" << line << "\n" << buffer.str() << line << "\n";
}