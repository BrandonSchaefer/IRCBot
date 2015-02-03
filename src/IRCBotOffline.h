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

#ifndef I_R_C_BOT_OFFLINE_H
#define I_R_C_BOT_OFFLINE_H

#include "IRCBot.h"

namespace irc_bot
{

// This is just for testing, so i can send command without connecting to anything!
class IRCBotOffline : public IRCBot
{
public:
  IRCBotOffline(ServerInfo const& info);
  virtual ~IRCBotOffline() {}

  virtual bool ConnectToServer() override;
  virtual void StartMainServerLoop() override;

  virtual void JoinChannel(std::string const& channel) const override;
  virtual void SendMessage(std::string const& channel, std::string const& message) const override;

private:
  virtual bool SendData(std::string const& data) const override;

  ServerInfo const info_;
};

} // namespace irc_bot

#endif // I_R_C_BOT_OFFLINE_H
