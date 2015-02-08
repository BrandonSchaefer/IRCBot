// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2015 Brandon Schaefer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Brandon Schaefer <brandontschaefer@gmail.com>
 */

#include <gtest.h>
#include <StringManipulation.h>

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  std::string const WHITE_SPACE_TEST    = "       this is a test";
  std::string const NO_WHITE_SPACE_TEST = "this is a test";

  std::string const SPLIT_TEST_DELIM        = "-_=+)(!";
  std::string const SPLIT_STR_TEST          = "a-b_c=d+e)f(g!";
  std::vector<std::string> const SPLIT_TEST = {"a", "b", "c", "d", "e", "f", "g"};

  std::string const HAYSTACK = "this is a haystack, not a needle!";
  std::string const NEEDLE   = "needle";

  std::string const USER_INPUT    = "custom 2 test this!";
  std::string const MATCH_REMOVED = "2 test this!";
  std::string const MATCH         = "custom";

  std::string const UPPERCASE = "HELLO";
  std::string const LOWERCASE  = "hello";

  std::string const STR_NUMBER = "17";
  unsigned int NUMBER          = 17;
}

TEST(StringManipulation, TestRemoveStartingWhitespace)
{
  std::string removed = RemoveStartingWhitespace(WHITE_SPACE_TEST);
  EXPECT_EQ(removed, NO_WHITE_SPACE_TEST);
}

TEST(StringManipulation, TestSplitString)
{
  std::vector<std::string> split_string = SplitString(SPLIT_STR_TEST, SPLIT_TEST_DELIM);

  for (size_t i = 0; i < SPLIT_TEST.size(); i++)
    EXPECT_EQ(split_string[i], SPLIT_TEST[i]);
}

TEST(StringManipulation, TestSubStringMatch)
{
  EXPECT_TRUE(SubStringMatch(HAYSTACK, NEEDLE));
}

TEST(StringManipulation, TestSubStringMatchFail)
{
  EXPECT_FALSE(SubStringMatch(NEEDLE, HAYSTACK));
}

TEST(StringManipulation, TestRemoveMatchingWord)
{
  std::string remove_match(RemoveMatchingWord(USER_INPUT, MATCH));
  EXPECT_EQ(remove_match, MATCH_REMOVED);
}

TEST(StringManipulation, TestLowercase)
{
  std::string lower = lowercase(UPPERCASE);
  EXPECT_EQ(lower, LOWERCASE);
}

TEST(StringManipulation, TestTypeConvertStrToInt)
{
  unsigned int number = TypeConverter<std::string, unsigned int>(STR_NUMBER);
  EXPECT_EQ(number, NUMBER);
}

// TODO Do i really expect a 0 to be returned? What if i want a 0?
TEST(StringManipulation, TestTypeConvertFailure)
{
  int number = TypeConverter<std::string, int>("laksdjfadf");
  EXPECT_EQ(number, 0);
}

} // namespace irc_bot_test
} // namespace irc_bot
