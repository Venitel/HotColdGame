#include "Section.h"

Section::Section(std::string text, std::string ampamplifiedText, int percent, bool ampDirection) 
    :   text_{text}, 
        amplifiedText_{ampamplifiedText}, 
        percent_{percent}, 
        ampDirection_{ampDirection} 
{}

bool Section::operator<(const Section &other) const
{
    return percent_ < other.percent_;
}

int Section::getPercent() const
{
    return percent_;
}

const std::string& Section::getText() const
{
    return text_;
}

std::string Section::genDisplay(const bool closerGuess, const Section* lastSection) const
{
    if(lastSection && this == lastSection && (closerGuess == ampDirection_))
    {
        return amplifiedText_;
    }
    return text_;
}

