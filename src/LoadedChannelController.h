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

#ifndef LOADED_CHANNEL_CONTROLLER_H
#define LOADED_CHANNEL_CONTROLLER_H

#include <string>
#include <unordered_map>

#include "LoadedChannelData.h"

namespace irc_bot
{

class LoadedChannelController
{
public:
  LoadedChannelController();

  void SetLastFMUsername(std::string const& channel, std::string const& lastfm_username);

  void AddCustomCommand   (std::string const& channel, CommandBreed const& cb);
  bool RemoveCustomCommand(std::string const& channel, std::string const& match_str);

  void UpdateChannelData(LoadedChannelData const& channel_data);
  LoadedChannelData RequestChannelData(std::string const& channel);

private:

  // mapping between "channel" --> loaded data
  std::unordered_map<std::string, LoadedChannelData> loaded_channel_data_;
};

} // namespace irc_bot

#endif // LOADED_CHANNEL_CONTROLLER_H
