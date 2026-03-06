#include <gtest/gtest.h>
#include "ConfigLoader.h"
#include "Section.h"

TEST(ParsePercent, PercentValid)
{
    ParseResult<int> parse;
    std::string percent = "66";
    parse = parsePercent(percent);
    EXPECT_TRUE(parse.success);
    EXPECT_EQ(parse.value, 66);
    EXPECT_TRUE(parse.error.empty());
}

TEST(ParsePercent, PercentTooBig)
{
    ParseResult<int> parse;
    std::string percent = "101";
    parse = parsePercent(percent);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

TEST(ParsePercent, PercentTooSmall)
{
    ParseResult<int> parse;
    std::string percent = "-1";
    parse = parsePercent(percent);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

TEST(ParsePercent, PercentWrongType)
{
    ParseResult<int> parse;
    std::string percent = "a1b2";
    parse = parsePercent(percent);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

TEST(ParsePercent, PercentEmpty)
{
    ParseResult<int> parse;
    std::string percent = "";
    parse = parsePercent(percent);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

TEST(ParseAmpDirection, AmpDirectionPositive)
{
    ParseResult<bool> parse;
    std::string ampDirection = "+";
    parse = parseAmpDirection(ampDirection);
    EXPECT_TRUE(parse.success);
    EXPECT_TRUE(parse.value);
    EXPECT_TRUE(parse.error.empty());
}

TEST(ParseAmpDirection, AmpDirectionNegative)
{
    ParseResult<bool> parse;
    std::string ampDirection = "-";
    parse = parseAmpDirection(ampDirection);
    EXPECT_TRUE(parse.success);
    EXPECT_FALSE(parse.value);
    EXPECT_TRUE(parse.error.empty());
}

TEST(ParseAmpDirection, AmpDirectionInvalid)
{
    ParseResult<bool> parse;
    std::string ampDirection = "-a";
    parse = parseAmpDirection(ampDirection);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

TEST(ParseAmpDirection, AmpDirectionEmpty)
{
    ParseResult<bool> parse;
    std::string ampDirection = "";
    parse = parseAmpDirection(ampDirection);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}

//We test percent and ampdirection intensively, here we only focus on general line cases
TEST(ParseConfigLine, ConfigLineValid)
{
    ParseResult<Section> parse;
    std::string configLine = "COMPLICATED text! 123 SUPER!@#$%/~\\|z|66|+";
    parse = parseConfigLine(configLine);
    EXPECT_TRUE(parse.success);
    EXPECT_EQ(parse.value.getText(), "COMPLICATED text! 123 SUPER!@#$%/~\\");
    EXPECT_EQ(parse.value.getPercent(), 66);
    EXPECT_TRUE(parse.error.empty());
}

TEST(ParseConfigLine, ConfigLineInvalid)
{
    ParseResult<Section> parse;
    std::string configLine = "Text|Another text|22"; //missing ampDirection
    parse = parseConfigLine(configLine);
    EXPECT_FALSE(parse.success);
    EXPECT_FALSE(parse.error.empty());
}