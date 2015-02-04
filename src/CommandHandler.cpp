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

#include "CommandHandler.h"
#include "LoadBasicCommands.h"
#include "SymWeaponInfo.h"
#include "Utils.h"

namespace irc_bot
{

namespace
{
  // CONST COMMANDS
  std::string const STATS   = "stats";
  std::string const COMPARE = "compare";
  std::string const SONG    = "song";
  std::string const FIB     = "fib";
}

CommandHandler::CommandHandler()
  : basic_commands_(LoadBasicCommands())
{
}

void CommandHandler::SetUsername(std::string const& username)
{
  username_ = username;
}

void CommandHandler::SetLoadedChannel(LoadedChannelData const& loaded_channel)
{
  loaded_channel_ = loaded_channel;
}

std::string CommandHandler::HandleUserInput(std::string const& user_input)
{
  if (SubStringMatch(user_input, STATS))
  {
    return HandleStats(RemoveMatchingWord(user_input, STATS));
  }
  else if (SubStringMatch(user_input, COMPARE))
  {
    return HandleCompare(RemoveMatchingWord(user_input, COMPARE));
  }
  else if (SubStringMatch(user_input, SONG))
  {
    return HandleSong(RemoveMatchingWord(user_input, SONG));
  }
  else if (SubStringMatch(user_input, FIB))
  {
    return HandleFib(RemoveMatchingWord(user_input, FIB));
  }
  else
  {
    return HandleBasic(user_input);
  }
}

bool CommandHandler::UserHasPermissionsForCommand(CommandPerm const& perm) const
{
  // User commands are always valid
  if (perm == CommandPerm::USER)
    return true;

  if (perm == CommandPerm::MOD)
    for (auto const& mod : loaded_channel_.mod_list)
      if (mod == username_)
        return true;

  // Check if its the owner, so skip the '#'
  std::string channel_owner = loaded_channel_.channel.substr(1);
  return username_ == channel_owner;
}

std::string CommandHandler::CheckBasicCommands(std::vector<CommandBreed> const& commands,
                                               std::string const& user_input) const
{
  for (auto const& cb : commands)
    if (user_input == cb.match)
      if (UserHasPermissionsForCommand(cb.perm))
        return cb.return_str;

  return "";
}

std::string CommandHandler::HandleBasic(std::string const& user_input) const
{
  std::string return_str;
  return_str = CheckBasicCommands(basic_commands_, user_input);

  if (return_str.empty())
    return CheckBasicCommands(loaded_channel_.custom_commands, user_input);

  return return_str;
}

std::string CommandHandler::HandleStats(std::string const& user_input) const
{
  std::string weapon_url = GetWeaponURL(user_input);

  if (!weapon_url.empty())
    return "Weapon info for " + user_input + ": " + weapon_url;

  return "";
}

std::string CommandHandler::HandleCompare(std::string const& user_input) const
{
  size_t first_weapon_end = user_input.find(' ');

  if (first_weapon_end != std::string::npos)
  {
    std::string weapon1 = user_input.substr(0, first_weapon_end);
    std::string weapon2 = RemoveStartingWhitespace(user_input.substr(first_weapon_end));

    std::string compare_url = GetCompareWeapon(weapon1.c_str(), weapon2.c_str());

    if (!compare_url.empty())
      return "Weapon Compare info: " + compare_url;
  }
 
  return "";
}

std::string CommandHandler::HandleSong(std::string const& user_input) const
{
  if (!loaded_channel_.lastfm_username.empty())
  {
    SongInfo song_info = last_song_.GetCurrentPlayingSong(loaded_channel_.lastfm_username);

    if (!song_info.artist.empty() && !song_info.title.empty())
      return song_info.title + " by " + song_info.artist;
  }

  return "";
}

// Recursion is to slow ... iterative it is :(
static uint64_t fib(uint64_t n)
{
  uint64_t a   = 0;
  uint64_t b   = 1;
  uint64_t tmp = 0;

  while (n > 0)
  {
    tmp = a;
    a   = b;
    b   = tmp + b;
    --n;
  }

  return a;
}

std::string CommandHandler::HandleFib(std::string const& user_input) const
{
  if (!user_input.empty())
  {
    uint64_t n = TypeConverter<std::string, uint64_t>(user_input);
    if (n > 0)
    {
      std::string fib_str = TypeConverter<uint64_t, std::string>(fib(n));
      return "Fib(" + user_input + ") = " + fib_str;
    }
  }

  return "";
}

} // namespace irc_bot
