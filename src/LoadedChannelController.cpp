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

#include "ChannelIO.h"
#include "LoadedChannelController.h"

namespace irc_bot
{

LoadedChannelController::LoadedChannelController()
{
}

void LoadedChannelController::SetLastFMUsername(std::string const& channel,
                                                std::string const& lastfm_username)
{
  LoadedChannelData channel_data = RequestChannelData(channel);
  channel_data.lastfm_username = lastfm_username;

  UpdateChannelData(channel_data);
}

static bool IsDuplicateCommand(std::vector<CommandBreed> const& commands, CommandBreed const& cb)
{
  for (auto const& command : commands)
    if (command.match == cb.match)
      return true;

  return false;
}

void LoadedChannelController::AddCustomCommand(std::string const& channel, CommandBreed const& cb)
{
  LoadedChannelData channel_data = RequestChannelData(channel);

  if (!IsDuplicateCommand(channel_data.custom_commands, cb))
  {
    channel_data.custom_commands.push_back(cb);
    UpdateChannelData(channel_data);
  }
}

bool LoadedChannelController::RemoveCustomCommand(std::string const& channel, std::string const& match_str)
{
  LoadedChannelData channel_data = RequestChannelData(channel);
  int index_to_remove = -1;
  int i = 0;

  for (auto it = channel_data.custom_commands.begin(); it != channel_data.custom_commands.end(); ++it)
  {
    if (it->match == match_str)
    {
      index_to_remove = i;
      break;
    }

    i++;
  }

  if (index_to_remove >= 0)
  {
    channel_data.custom_commands.erase(channel_data.custom_commands.begin() + index_to_remove);
    UpdateChannelData(channel_data);
  }

  return index_to_remove >= 0;
}

void LoadedChannelController::UpdateChannelData(LoadedChannelData const& channel_data)
{
  loaded_channel_data_[channel_data.channel] = channel_data;
  SaveChannelData(channel_data);
}

LoadedChannelData LoadedChannelController::RequestChannelData(std::string const& channel)
{
  auto it = loaded_channel_data_.find(channel);

  if (it == loaded_channel_data_.end())
    loaded_channel_data_[channel] = LoadChannelData(channel);

  return loaded_channel_data_[channel];
}

} // namespace irc_bot
