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
#include <ReadWriteIO.h>

#include "config.h"

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  // FIXME This needs to be a bit more dynamic... could be wrong on a different system
  std::string const TEST_PATH = TESTDIR"/.tmp_test_file";
  std::string const TEST_TEXT = "THIS IS A TEST!";
}

TEST(ReadWriteIO, TestWriteThenRead)
{
  WriteToFile(TEST_TEXT, TEST_PATH);

  std::string raw_text = ReadInFile(TEST_PATH);

  EXPECT_EQ(raw_text, TEST_TEXT);
}

} // irc_bot_test
} // irc_bot
