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

#include <CommandHandler.h>
#include <IRCBotOffline.h>

#include "mock_irc_bot.h"

using namespace testing;


namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  std::string const USERNAME  = "thetest";
  std::string const CHANNEL   = "#thetest";

  std::string const RET_STR   = "ret_str";
  std::string const MATCH     = "test";
  std::string const CUSTOM_C  = "custom 2 " + MATCH + " " + RET_STR;
  std::string const REMOVRE_C = "remove " + MATCH;

  std::string const HELP      =  "help";
  std::string const STATS     = "stats aek";
  std::string const COMPARE   = "compare aek m416";
  std::string const GOOGLE    = "google test";
  std::string const FIB       = "fib 10";
  std::string const BOT_LEAVE = "bot leave";

  ServerInfo const INFO = {};
}

class TestCommandHandler : public ::Test
{
public:
  TestCommandHandler()
    : bot_(std::make_shared<MockIRCBot>(INFO))
  {
    handler_.SetUsername(USERNAME);
    handler_.UpdateLoadedChannel(CHANNEL);
  }

  CommandHandler handler_;
  MockIRCBot::Ptr bot_;
};

TEST_F(TestCommandHandler, TestHelp)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, _));
  handler_.HandleUserInput(bot_, HELP);
}

TEST_F(TestCommandHandler, TestStats)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, _));
  handler_.HandleUserInput(bot_, STATS);
}

TEST_F(TestCommandHandler, TestCompare)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, _));
  handler_.HandleUserInput(bot_, COMPARE);
}

TEST_F(TestCommandHandler, TestGoogle)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, _));
  handler_.HandleUserInput(bot_, GOOGLE);
}

TEST_F(TestCommandHandler, TestFib)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, _));
  handler_.HandleUserInput(bot_, FIB);
}

TEST_F(TestCommandHandler, TestLeaveChannel)
{
  EXPECT_CALL(*bot_, LeaveChannel("#" + USERNAME));
  EXPECT_CALL(*bot_, SendMessage ("#" + USERNAME, _));
  handler_.HandleUserInput(bot_, BOT_LEAVE);
}

TEST_F(TestCommandHandler, TestCustomCommand)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, RET_STR));

  int ret;
  ret = handler_.HandleUserInput(bot_, CUSTOM_C);
  EXPECT_TRUE(ret);

  ret = handler_.HandleUserInput(bot_, MATCH);
  EXPECT_TRUE(ret);
}

TEST_F(TestCommandHandler, TestRemoveCommand)
{
  int ret;
  ret = handler_.HandleUserInput(bot_, CUSTOM_C);
  EXPECT_TRUE(ret);

  ret = handler_.HandleUserInput(bot_, REMOVRE_C);
  EXPECT_TRUE(ret);

  ret = handler_.HandleUserInput(bot_, MATCH);
  EXPECT_FALSE(ret);
}

} // namespace irc_bot
} // namespace irc_bot_test
