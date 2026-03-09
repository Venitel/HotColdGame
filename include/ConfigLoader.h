#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <vector>
#include <string>
#include <functional>

class Section;

//common
bool isChar(const std::string& value);
void doForEachFileLine(const std::string& fileName, const std::function<void(const std::string&)>& callback);

//Sections
template<typename T>
struct ParseResult
{
    bool success;
    T value;
    std::string error;
};
ParseResult<Section> parseSectionConfigLine(const std::string& configLine);
ParseResult<int> parsePercent(const std::string& percentString);
ParseResult<bool> parseAmpDirection(const std::string& ampDirectionString);

const std::vector<Section>& getSectionsConfig();
std::vector<Section> initSections(const std::string& fileName);
std::vector<Section> readSectionsFromConfig(const std::string& fileName);

//Hierarchy
struct HierarchyParameters
{
    //Uses default values if absent in config file
    std::string title = ">> Hierarchy <<";
    std::string prefix = "* ";
    char lineChar = '-';
};
const HierarchyParameters& getHierarchyConfig();
HierarchyParameters readHierarchyConfig(const std::string& fileName);
void displayHierarchy(std::vector<Section>& sections);

#endif