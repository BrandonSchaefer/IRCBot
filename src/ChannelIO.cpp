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

#include <vector>

#include "ChannelIO.h"
#include "Utils.h"

#include "config.h"

namespace irc_bot
{

namespace
{
  std::string const DEFAULT_PATH   = SAVESDIR"/";
  std::string const CHANNELS_LISTS = "channels_list";
}

std::vector<std::string> LoadChannelsListTextFile()
{
  std::string raw_channels_list = ReadInFile((DEFAULT_PATH + CHANNELS_LISTS).c_str());
  std::vector<std::string> channel_files_to_load = SplitStringOnNewLineOrNull(raw_channels_list);

  return channel_files_to_load;
}

// FIXME figure out if i need a class here or not
ChannelIO::ChannelIO()
{
}

// FIXME Finish loading all these commands.
void ChannelIO::LoadFiles()
{
  std::vector<std::string> channel_files_to_load = LoadChannelsListTextFile();
  for (auto const& chan : channel_files_to_load)
  {
    std::string chan_loaded = ReadInFile((DEFAULT_PATH + chan).c_str());
    if (!chan_loaded.empty())
    {
      std::vector<std::string> file_contents = SplitStringOnNewLineOrNull(chan_loaded);
      for (auto const& s : file_contents)
        printf("%s\n", s.c_str());
    }
  }
}

} // namespace irc_bot
