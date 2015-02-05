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

#ifndef I_R_C_BOT_CONTROLLER_H
#define I_R_C_BOT_CONTROLLER_H

#include "CommandBreed.h"
#include "CommandHandler.h"
#include "IRCBot.h"
#include "IRCBotOffline.h"

#include <set>
#include <vector>
#include <unordered_map>

namespace irc_bot
{

class IRCBotController
{
public:
  IRCBotController(IRCBot::Ptr const& irc_bot);

  void StartBot();

private:
  void RecvServerInputRecived(std::string const& server_input);
  void RecvCommandReturnMessage(std::string const& channel, std::string const& message_for_bot);

  void HandlePrivMsg(std::string const& server_input);
  void HandleMode   (std::string const& server_input);

  std::vector<CommandBreed> basic_commands_;
  IRCBot::Ptr bot_;

  CommandHandler command_handler_;
};

} // namespace irc_bot

#endif // I_R_C_BOT_CONTROLLER_H
