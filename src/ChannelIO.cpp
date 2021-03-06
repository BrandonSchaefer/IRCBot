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

#include <vector>

#include "CommandBreed.h"
#include "ChannelIO.h"
#include "LoadBasicCommands.h"
#include "ReadWriteIO.h"
#include "StringManipulation.h"

#include "config.h"

namespace irc_bot
{

namespace
{
  std::string const DEFAULT_PATH   = SAVESDIR"/";
  std::string const BACK_SAVE      = "_backup";
  std::string const CHANNELS_LISTS = "channels_list";

  std::string const LASTFM_USERNAME = "LASTFM_USERNAME:";
  std::string const MOD_LIST        = "MOD_LIST:";

  std::string const COMMAND         = "COMMAND:";
}

static std::string GetBackupFileName(std::string const& channel)
{
  return DEFAULT_PATH + "." + channel + BACK_SAVE;
}

/*

Main idea, check if SAVESDIR/channel files exists, otherwise
  check if the backup file SAVESDIR/.channel_backup is around.

If a file is found, then split it up on newlines or '\0' into a vector

*/
static std::string LoadRawChannelData(std::string const& channel)
{
  std::string raw_channel_data = ReadInFile(DEFAULT_PATH + channel);
  std::vector<std::string> split_channel_data;

  if (raw_channel_data.empty())
  {
    // If we have to use the back up make sure we re-save a real version!
    raw_channel_data = ReadInFile(GetBackupFileName(channel));
    WriteToFile(raw_channel_data, DEFAULT_PATH + channel);
  }

  return raw_channel_data;
}

/*

# LastFM username :: LASTFM_USERNAME: <username>
# Mod lists       :: MOD_LIST: <username>,<username>,<username>,...etc
# CUSTOM COMMANDS :: <CommandPerm> <MatchString> <ReturnString>

LASTFM_USERNAME: sithrallob
MOD_LIST: sithrallob,thegreatbambibot,other
1 match return str
2 diff stuff

*/

/*
  Make sure to backup the save file! (Since we are overwriting it!):
    Load old save file, save file to .channel_backup
    Save new data to old save file! // If an error happens with in this we can recover
    */
static std::string GenerateModString(LoadedChannelData const& data)
{
  std::stringstream ss;

  ss << MOD_LIST << " ";
  size_t i = 0;
  for (auto const& mod : data.mod_list)
  {
    ss << mod;

    // stupid fence post
    if (i < data.mod_list.size() - 1)
      ss << ",";

    i++;
  }
  ss << '\n';

  return ss.str();
}

static std::string GetPermStr(CommandPerm const& perm)
{
  switch (perm)
  {
    case CommandPerm::OWNER:
      return "owner";
    case CommandPerm::MOD:
      return "mod";
    case CommandPerm::USER:
      return "user";
    case CommandPerm::NONE:
    default:
      return "NONE";
  }
}

void SaveChannelData(LoadedChannelData const& data)
{
  // Backup the current file
  std::string raw_channel_data = LoadRawChannelData(data.channel);
  WriteToFile(raw_channel_data, GetBackupFileName(data.channel));

  // Create the new info to be saved!
  std::stringstream ss;

  // LastFM username
  ss << LASTFM_USERNAME;
  ss << " " << data.lastfm_username << "\n";

  // Mods list
  ss << GenerateModString(data);

  // Custom Commands
  for (auto const& cb : data.custom_commands)
    ss << COMMAND << " " << GetPermStr(cb.perm) << " " << cb.match << " " << cb.return_str << "\n";

  WriteToFile(ss.str(), DEFAULT_PATH + data.channel);
  WriteToFile(ss.str(), GetBackupFileName(data.channel));
}

static std::string GetLastFMUsername(std::string const& line)
{
  return RemoveStartingWhitespace(line.substr(LASTFM_USERNAME.size()));
}

static std::set<std::string> GetModsList(std::string const& line)
{
  std::set<std::string> mod_list;
  std::string raw_username_list = RemoveStartingWhitespace(line.substr(MOD_LIST.size()));
  std::vector<std::string> split_mod_list = SplitString(raw_username_list, ",\n\0");

  // We dont want duplicates
  for (auto const& mod : split_mod_list)
    mod_list.insert(mod);

  return mod_list;
}

static std::vector<CommandBreed> GetBasicCommands(std::vector<std::string> const& split_channel_data)
{
  std::string raw_commands;
  for (auto const& line : split_channel_data)
    if (SubStringMatch(line, COMMAND))
      raw_commands += RemoveMatchingWord(line, COMMAND) + "\n";

  return LoadBasicCommandsFromString(raw_commands);
}

LoadedChannelData LoadChannelData(std::string const& channel)
{
  std::string raw_channel_data = LoadRawChannelData(channel);
  LoadedChannelData data;

  data.channel = channel;

  if (!raw_channel_data.empty())
  {
    std::vector<std::string> split_channel_data = SplitString(raw_channel_data, "\n\0");

    for (auto const& line : split_channel_data)
    {
      if (SubStringMatch(line, LASTFM_USERNAME))
        data.lastfm_username = GetLastFMUsername(line);
      else if (SubStringMatch(line, MOD_LIST))
        data.mod_list = GetModsList(line);
    }

    if (SubStringMatch(raw_channel_data, COMMAND))
      data.custom_commands = GetBasicCommands(split_channel_data);
  }

  return data;
}

} // namespace irc_bot
