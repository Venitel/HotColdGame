#include "ConfigLoader.h"
#include "Section.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace
{
  constexpr char SECTIONS_FILE[] = "sections.cfg";
  constexpr char HIERARCHY_FILE[] = "hierarchy.cfg";
}

const std::vector<Section>& getSectionsConfig()
{
    static const std::vector<Section> sectionsConfig = initSections(std::string(SECTIONS_FILE));
    return sectionsConfig;
}

const HierarchyParameters& getHierarchyConfig()
{
    static const HierarchyParameters hierarchyConfig = readHierarchyConfig(std::string(HIERARCHY_FILE));
    return hierarchyConfig;
}

bool isChar(const std::string& value)
{
  return value.length() == 1;
}

void doForEachFileLine(const std::string& fileName, const std::function<void(const std::string&)>& callback)
{
    std::ifstream file(fileName);
    if(!file.is_open())
    {
      throw std::runtime_error("Failed to open file: " + fileName);    
    }

    std::string fileLine;
    while(std::getline(file, fileLine))
    {
      if(fileLine.empty() || fileLine[0] == '#') continue;

      callback(fileLine);
    }
}

std::vector<Section> initSections(const std::string& fileName)
{
  std::vector<Section> allSections = readSectionsFromConfig(fileName);
  if(allSections.empty())
  {
    throw std::runtime_error("No viable sections loaded from " + fileName);    
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

std::vector<Section> readSectionsFromConfig(const std::string& fileName)
{
  std::vector<Section> sections;
 
  doForEachFileLine(fileName, [&](const std::string& fileLine) {
    //lambda
    ParseResult<Section> configParse = parseSectionConfigLine(fileLine);
    if(!configParse.success)
    {
      std::cerr << "Config '" << fileLine << "' ignored. " << configParse.error << "\n";
      return;
    }

    sections.push_back(configParse.value);
  });

  return sections;
}

ParseResult<Section> parseSectionConfigLine(const std::string& configLine)
{
  std::stringstream stream(configLine);
  std::string textConfig, amplifiedTextConfig, percentConfig, ampDirectionConfig;

  if(!std::getline(stream, textConfig, '|')
    || !std::getline(stream, amplifiedTextConfig, '|')
    || !std::getline(stream, percentConfig, '|')
    || !std::getline(stream, ampDirectionConfig, '|'))
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
  if(!isChar(ampDirectionString) || (ampDirectionString[0] != '-' && ampDirectionString[0] != '+'))
  {
    return {false, false, "Amp direction must be either '+' or '-'"};
  }

  return {true, ampDirectionString[0] == '+', ""};
}

HierarchyParameters readHierarchyConfig(const std::string& fileName)
{
    HierarchyParameters hierarchyConfig;

    doForEachFileLine(fileName, [&](const std::string& fileLine) {
      //lambda
      auto pos = fileLine.find('=');
      if (pos == std::string::npos) 
      {
        std::cerr << "Config line '" << fileLine << "' invalid.\n";
        return;
      }

      std::string key = fileLine.substr(0, pos);
      std::string value = fileLine.substr(pos + 1);

      if (key == "title") 
      {
        hierarchyConfig.title = value;
      }
      else if (key == "prefix") 
      {
        hierarchyConfig.prefix = value;
      }
      else if (key == "line_char")
      {
        if(isChar(value)) 
        {
          hierarchyConfig.lineChar = value[0];
        }
        else
        {
          std::cerr << "line_char value too long '" << value << "'\n";
        }
      }
      else 
      {
        std::cerr << "Config key '" << key << "' unknown.\n";
      }
    });

    return hierarchyConfig;
}

void displayHierarchy(std::vector<Section>& sections)
{
  const HierarchyParameters& hierarchyConfig = getHierarchyConfig();
  std::ostringstream buffer;
  size_t maxLength = hierarchyConfig.title.length();

  for(const Section& section : sections)
  {
    //pass pointer to itself to "fake" 2nd guess in a row (for amplified text)
    std::string textCloser = hierarchyConfig.prefix + section.genDisplay(true, &section);
    std::string textFurther = hierarchyConfig.prefix + section.genDisplay(false, &section);

    buffer << textCloser << "\n" 
          << textFurther << "\n";
    maxLength = std::max({maxLength, textCloser.length(), textFurther.length()});
  }
  
  std::string line(maxLength, hierarchyConfig.lineChar);
  std::cout << line << "\n" 
            << hierarchyConfig.title << "\n" 
            << line << "\n" 
            << buffer.str() //hierarchy
            << line << "\n";
}