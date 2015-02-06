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
  std::string const GOOGLE_URL = "http://google.com/#q=";

  // CONST COMMANDS
  std::string const STATS      = "stats";
  std::string const COMPARE    = "compare";
  std::string const SONG       = "song";
  std::string const CUSTOM     = "custom";
  std::string const REMOVE     = "remove";
  std::string const GOOGLE     = "google";
  std::string const FIB        = "fib";
  std::string const HELP       = "help";
  std::string const SET_LASTFM = "set lastfm";
  std::string const BOT_LEAVE  = "bot leave";

  // TO REPLACE KEYWORDS
  std::string const SYMBOL_USER = "#user";

  std::string const POS_MOD_OP = "+o";
  std::string const NEG_MOD_OP = "-o";
}

static std::string GetChannel(std::string const& str)
{
  std::string channel;
  size_t start_of_channel = str.find('#');

  if (start_of_channel != std::string::npos)
  {
    std::string chunk = str.substr(start_of_channel);

    for (auto const& c : chunk)
    {
      if (c != '\n' && c != '\0' && c != '\r' && c != ' ')
        channel += c;
      else
        break;
    }
  }

  return channel;
}

CommandHandler::CommandHandler()
  : basic_commands_(LoadBasicCommands())
{
}

void CommandHandler::SetUsername(std::string const& username)
{
  username_ = username;
}

void CommandHandler::UpdateLoadedChannel(std::string const& server_input)
{
  std::string const channel = GetChannel(server_input);
  if (!channel.empty())
    loaded_channel_ = loaded_controller_.RequestChannelData(channel);
}

void CommandHandler::HandleModServerCommand(std::string const& server_input)
{
  std::vector<std::string> raw_server_mode_list = SplitString(server_input, " \r\n");

  for (size_t i = 0; i < raw_server_mode_list.size(); i++)
  {
    if (raw_server_mode_list[i] == POS_MOD_OP)
    {
      loaded_channel_.mod_list.insert(raw_server_mode_list[i + 1]);
    }
    else if(raw_server_mode_list[i] == NEG_MOD_OP)
    {
      loaded_channel_.mod_list.erase(raw_server_mode_list[i + 1]);
    }
  }

  loaded_controller_.UpdateChannelData(loaded_channel_);
}

std::string CommandHandler::ReplaceSymbols(std::string const& message) const
{
  if (SubStringMatch(message, SYMBOL_USER))
  {
    size_t start = message.find(SYMBOL_USER);

    if (start != std::string::npos)
    {
      std::string front = message.substr(0, start);
      std::string end   = message.substr(start + SYMBOL_USER.size()); 
      return front + username_ + end;
    }
  }

  return message;
}

bool CommandHandler::HandleUserInput(IRCBot::Ptr const& bot, std::string const& user_input)
{
  std::string message_for_bot;

  if (SubStringMatch(user_input, STATS))
  {
    message_for_bot = HandleStats(RemoveMatchingWord(user_input, STATS));
  }
  else if (SubStringMatch(user_input, COMPARE))
  {
    message_for_bot = HandleCompare(RemoveMatchingWord(user_input, COMPARE));
  }
  else if (SubStringMatch(user_input, SONG))
  {
    message_for_bot = HandleSong(RemoveMatchingWord(user_input, SONG));
  }
  else if (SubStringMatch(user_input, FIB))
  {
    message_for_bot = HandleFib(RemoveMatchingWord(user_input, FIB));
  }
  else if (SubStringMatch(user_input, GOOGLE))
  {
    message_for_bot = HandleGoogle(RemoveMatchingWord(user_input, GOOGLE));
  }
  else if (SubStringMatch(user_input, HELP))
  {
    message_for_bot = HandleHelp(RemoveMatchingWord(user_input, HELP));
  }
  else if (SubStringMatch(user_input, CUSTOM))
  {
    return HandleCustom(RemoveMatchingWord(user_input, CUSTOM));
  }
  else if (SubStringMatch(user_input, REMOVE))
  {
    return HandleRemove(RemoveMatchingWord(user_input, REMOVE));
  }
  else if (SubStringMatch(user_input, SET_LASTFM))
  {
    HandleSetLastFM(RemoveMatchingWord(user_input, SET_LASTFM));
    return true;
  }
  else if (SubStringMatch(user_input, BOT_LEAVE))
  {
    std::string usr_channel = "#" + username_;
    bot->SendMessage (usr_channel, "Alright, I'll leave... BibleThump");
    bot->LeaveChannel(usr_channel);
    return true;
  }
  else
  {
    message_for_bot = HandleBasic(user_input);
  }

  if (!message_for_bot.empty())
  {
    message_for_bot = ReplaceSymbols(message_for_bot);
    bot->SendMessage(loaded_channel_.channel, message_for_bot);
  }

  return !message_for_bot.empty();
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

  if (!loaded_channel_.channel.empty())
  {
    // Check if its the owner, so skip the '#'
    std::string channel_owner = loaded_channel_.channel.substr(1);
    return username_ == channel_owner;
  }

  return false;
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

std::string CommandHandler::HandleGoogle(std::string const& user_input) const
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
    return GOOGLE_URL + user_input;

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

bool CommandHandler::HandleCustom(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    CommandBreed cb = LoadSingleCommandFromString(user_input);

    if (!cb.match.empty())
    {
      loaded_controller_.AddCustomCommand(loaded_channel_.channel, cb);
      return true;
    }
  }

  return false;
}

bool CommandHandler::HandleRemove(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
    return loaded_controller_.RemoveCustomCommand(loaded_channel_.channel, user_input);

  return false;
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

void CommandHandler::HandleSetLastFM(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    loaded_channel_.lastfm_username = user_input;
    loaded_controller_.UpdateChannelData(loaded_channel_);
  }
}

std::string CommandHandler::HandleHelp(std::string const& user_input) const
{
  std::stringstream ss;

  for (auto const& command : basic_commands_)
    if (UserHasPermissionsForCommand(command.perm))
      ss << command.match << ", ";

  for (auto const& command : loaded_channel_.custom_commands)
    if (UserHasPermissionsForCommand(command.perm))
      ss << command.match << ", ";

  std::string help_str = ss.str();

  // Remove the last ", " (stupid fence post)
  return help_str.substr(0, help_str.size() - 2);
}

} // namespace irc_bot
