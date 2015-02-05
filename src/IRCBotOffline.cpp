//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/* * Copyright (C) CURRENT_YEAR Brandon Schaefer
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

#include "IRCBotOffline.h"

#include <iostream>

namespace irc_bot
{

namespace
{
  const int MAX_BUFF = 256;
}

IRCBotOffline::IRCBotOffline(ServerInfo const& info)
  : IRCBot(info)
  , info_(info)
{
}

bool IRCBotOffline::SendData(std::string const& data) const
{
  printf("Sending Data :: %s\n", data.c_str());

  return true;
}

bool IRCBotOffline::ConnectToServer()
{
  return true;
}

void IRCBotOffline::JoinChannel(std::string const& channel) const
{
  std::string message = "JOIN " + channel + "\r\n";
  SendData(message);
}

void IRCBotOffline::SendMessage(std::string const& channel, std::string const& message) const
{
  std::string priv_message = "PRIVMSG " + channel + " :" + message + "\r\n";
  SendData(priv_message);
}

// Simple fgets, then attempt to see if the message gets matched
void IRCBotOffline::StartMainServerLoop()
{
  char buf[MAX_BUFF];

  input_received(":jtv MODE #test +o thegreatbambibot\r\n");

  while (fgets(buf, MAX_BUFF, stdin) != (char*)EOF)
  {
    std::string data = ":TheGreatBambiBot!test@192.168.0.1 PRIVMSG #test :" + std::string(buf);
    input_received(data);
  }
}

} // namespace irc_bot
