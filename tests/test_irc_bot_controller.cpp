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
#include <gmock/gmock.h>

#include <IRCBotController.h>
#include <IRCBot.h>

#include "mock_irc_bot.h"

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  // Empty ... using an offline bot :)
  ServerInfo const INFO = {};
  std::string CHANNEL   = "#test";
  std::string PRIV_MSG  = ":TheGreatBambiBot!test@192.168.0.1 PRIVMSG #test :";

  std::string FIB_MSG   = "!fib 10";
  std::string STAT_MSG  = "!stats aek";
  std::string COMP_MSG  = "!compare aek m416";
  // Cant really test !song since it depends on the internet... and thats not always going to be around
}

class MockIRCBotController : public testing::Test
{
public:
  MockIRCBotController()
    : bot_(std::make_shared<MockIRCBot>(INFO))
    , controller_(bot_)
  {
  }

  MockIRCBot::Ptr bot_;
  IRCBotController controller_;
};

TEST_F(MockIRCBotController, TestSendPrivateMessage)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->input_received(PRIV_MSG + "!help");
}

TEST_F(MockIRCBotController, TestSendPrivateMessageStats)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->input_received(PRIV_MSG + STAT_MSG);
}

TEST_F(MockIRCBotController, TestSendPrivateMessageCompare)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->input_received(PRIV_MSG + COMP_MSG);
}

TEST_F(MockIRCBotController, TestSendPrivateMessageFib)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->input_received(PRIV_MSG + FIB_MSG);
}

} // namespace irc_bot
} // namespace irc_bot_test
