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

#ifndef I_R_C_BOT_H
#define I_R_C_BOT_H

#include <functional>
#include <memory>
#include <string>

namespace irc_bot
{

struct ServerInfo
{
  const std::string server;
  const std::string port;
  const std::string password;
  const std::string nick;
  const std::string user;
};

class IRCBot
{
public:
  typedef std::shared_ptr<IRCBot> Ptr;

  IRCBot(ServerInfo const& info);

  std::string Name() const;

  virtual bool ConnectToServer();
  virtual void StartMainServerLoop();

  virtual void JoinChannel (std::string const& channel) const;
  virtual void LeaveChannel(std::string const& channel) const;
  virtual void SendMessage(std::string const& channel, std::string const& message) const;

  // FIXME Make this private
  // when emited, means we have received a raw_buffer from the server
  std::function<void(std::string const&)> input_received;

private:
  bool ConnectToIRCServer();

  virtual bool SendData(std::string const& data) const;

  ServerInfo const info_;
  int socket_;
};

} // namespace irc_bot

#endif // I_R_C_BOT_H
