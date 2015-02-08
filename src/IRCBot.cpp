//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/* * Copyright (C) 2015 Brandon Schaefer
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

#include "IRCBot.h"
#include "StringManipulation.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <string>

#include <unistd.h>

namespace irc_bot
{

namespace
{
  const int MAX_BUFF = 256;
}

IRCBot::IRCBot(ServerInfo const& info)
  : info_(info)
  , socket_(0)
{
}

bool IRCBot::SendData(std::string const& data) const
{
  int len = data.size();
  int bytes_sent = send(socket_, data.c_str(), len, 0);

  return bytes_sent > 0;
}

bool IRCBot::ConnectToServer()
{
  struct addrinfo hints, *servinfo;

  int res;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((res = getaddrinfo(info_.server.c_str(), info_.port.c_str(), &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
    return false;
  }

  if ((socket_ = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)))
  {
    fprintf(stderr, "client: socket %i\n", socket_);
  }

  if (connect(socket_, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
  {
    fprintf(stderr, "Error in connecting");
    close(socket_);
    return false;
  }

  freeaddrinfo(servinfo);

  return ConnectToIRCServer();
}

bool IRCBot::ConnectToIRCServer()
{
  // Send info to connect the the correct irc server
  // Order matters PASS, NICK, USER
  std::string pass = "PASS " + info_.password;
  std::string nick = "NICK " + info_.nick;
  std::string user = "USER " + info_.user;

  bool ret;

  ret = SendData(pass.c_str());
  ret = SendData(nick.c_str());
  ret = SendData(user.c_str());

  return ret;
}

void IRCBot::JoinChannel(std::string const& channel) const
{
  std::string message = "JOIN " + channel + "\r\n";
  SendData(message);
}

void IRCBot::LeaveChannel(std::string const& channel) const
{
  std::string message = "PART " + channel + "\r\n";
  SendData(message);
}

void IRCBot::SendMessage(std::string const& channel, std::string const& message) const
{
  std::string priv_message = "PRIVMSG " + channel + " :" + message + "\r\n";
  SendData(priv_message);
}

std::string IRCBot::Name() const
{
  std::string formated_name = info_.nick;
  formated_name.erase(formated_name.find_last_not_of("\n\r\t")+1);

  return formated_name;
}

void IRCBot::StartMainServerLoop()
{
  int numbytes;
  char buf[MAX_BUFF];

  bool done = false;
  while(!done)
  {
    numbytes = recv(socket_, buf, MAX_BUFF - 1, 0);
    buf[numbytes] = '\0';

    // Handle the PING/PONG internally
    if (SubStringMatch(std::string(buf), "PING"))
    {
      std::string pong(buf);
      size_t i_pos = pong.find('I');
      if (i_pos != std::string::npos)
      {
        pong[i_pos] = 'O';
        SendData(pong.c_str());
      }
    }
    else
    {
      input_received(std::string(buf));
    }

    if (numbytes <= 0)
    {
      fprintf(stderr, "-----------------Connection Closed---------------\n");
      done = true;
    }
  }
}

} // namespace irc_bot
