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

#include <gmock/gmock.h>
#include <string>

#include <IRCBot.h>

namespace irc_bot
{
namespace irc_bot_test
{
class MockIRCBot : public IRCBot
{
public:
  typedef std::shared_ptr<MockIRCBot> Ptr;

  MockIRCBot(ServerInfo const& info)
  : IRCBot(info)
  {}

  virtual bool ConnectToServer() { return true; }
  virtual void StartMainServerLoop() {}

  MOCK_CONST_METHOD2(SendMessage,  void(std::string const&, std::string const&));
  MOCK_CONST_METHOD1(LeaveChannel, void(std::string const&));
  MOCK_CONST_METHOD1(JoinChannel,  void(std::string const&));
};

} // namespace irc_bot
} // namespace irc_bot_test
