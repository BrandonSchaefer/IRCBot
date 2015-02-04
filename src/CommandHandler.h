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

#include <string>
#include <vector>

#include "LastFMCurrentSong.h"
#include "LoadedChannelData.h"

namespace irc_bot
{

class CommandHandler
{
public:
  CommandHandler();

  void SetUsername(std::string const& username);
  void SetLoadedChannel(LoadedChannelData const& loaded_channel);

  std::string HandleUserInput(std::string const& user_input);

private:
  std::string HandleBasic  (std::string const& user_input) const;
  std::string HandleStats  (std::string const& user_input) const;
  std::string HandleCompare(std::string const& user_input) const;
  std::string HandleSong   (std::string const& user_input) const;
  std::string HandleFib    (std::string const& user_input) const;

  bool UserHasPermissionsForCommand(CommandPerm const& perm) const;

  std::string CheckBasicCommands(std::vector<CommandBreed> const& commands,
                                 std::string const& user_input) const;

  std::string username_;
  LoadedChannelData loaded_channel_;

  std::vector<CommandBreed> basic_commands_;
  LastFMCurrentSong last_song_;
};

} // namespace irc_bot

#endif // COMMAND_HANDLER_H
