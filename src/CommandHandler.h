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

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <functional>
#include <string>
#include <vector>

#include "LastFMCurrentSong.h"
#include "LoadedChannelController.h"
#include "LoadedChannelData.h"

namespace irc_bot
{

class CommandHandler
{
public:
  CommandHandler();

  void SetUsername(std::string const& username);
  void UpdateLoadedChannel(std::string const& server_input);

  void HandleModServerCommand(std::string const& server_input);
  bool HandleUserInput(std::string const& user_input);

  std::function<void(std::string const&, std::string const&)> command_return_message;

private:
  std::string HandleBasic  (std::string const& user_input) const;
  std::string HandleStats  (std::string const& user_input) const;
  std::string HandleCompare(std::string const& user_input) const;
  std::string HandleSong   (std::string const& user_input) const;
  std::string HandleGoogle (std::string const& user_input) const;
  std::string HandleFib    (std::string const& user_input) const;

  bool HandleCustom(std::string const& user_input);
  bool HandleRemove(std::string const& user_input);

  bool UserHasPermissionsForCommand(CommandPerm const& perm) const;

  std::string CheckBasicCommands(std::vector<CommandBreed> const& commands,
                                 std::string const& user_input) const;

  std::vector<CommandBreed> basic_commands_;
  LastFMCurrentSong last_song_;

  LoadedChannelController loaded_controller_;
  LoadedChannelData loaded_channel_;
  std::string username_;
};

} // namespace irc_bot

#endif // COMMAND_HANDLER_H
