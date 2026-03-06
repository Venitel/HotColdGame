#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <vector>
#include <string>

class Section;

template<typename T>
struct ParseResult
{
    bool success;
    T value;
    std::string error;
};

ParseResult<Section> parseConfigLine(const std::string& configLine);
ParseResult<int> parsePercent(const std::string& percentString);
ParseResult<bool> parseAmpDirection(const std::string& ampDirectionString);

std::vector<Section> initSections();
std::vector<Section> readSectionsFromConfig();
void displayHierarchy(std::vector<Section>& sections);

#endif