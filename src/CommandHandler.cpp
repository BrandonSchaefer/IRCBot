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

#include "CommandHandler.h"
#include "LoadBasicCommands.h"
#include "StringManipulation.h"

namespace irc_bot
{

namespace
{
  //std::string const GOOGLE_URL = "http://google.com/#q=";
  std::string const GOOGLE_URL    = "http://lmgtfy.com/?q=";
  std::string const BF4_STATS_URL = "http://bf4stats.com/pc/";

  // CONST COMMANDS
  std::string const STATS      = "stats";
  std::string const PLAY_STATS = "playerstats";
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
  std::string const SYMBOL_ARG  = "#";

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
  Command stats      = {STATS,      std::bind(&CommandHandler::HandleStats,       this, std::placeholders::_1)};
  Command play_stats = {PLAY_STATS, std::bind(&CommandHandler::HandlePlayerStats, this, std::placeholders::_1)};
  Command compare    = {COMPARE,    std::bind(&CommandHandler::HandleCompare,     this, std::placeholders::_1)};
  Command song       = {SONG,       std::bind(&CommandHandler::HandleSong,        this, std::placeholders::_1)};
  Command custom     = {CUSTOM,     std::bind(&CommandHandler::HandleCustom,      this, std::placeholders::_1)};
  Command remove     = {REMOVE,     std::bind(&CommandHandler::HandleRemove,      this, std::placeholders::_1)};
  Command google     = {GOOGLE,     std::bind(&CommandHandler::HandleGoogle,      this, std::placeholders::_1)};
  Command fib        = {FIB,        std::bind(&CommandHandler::HandleFib,         this, std::placeholders::_1)};
  Command help       = {HELP,       std::bind(&CommandHandler::HandleHelp,        this, std::placeholders::_1)};
  Command set_lastfm = {SET_LASTFM, std::bind(&CommandHandler::HandleSetLastFM,   this, std::placeholders::_1)};

  commands_ = {
               stats,   play_stats,
               compare, song,
               custom,  remove,
               google,  fib,
               help,    set_lastfm,
              };
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

void CommandHandler::HandleModeServerCommand(std::string const& server_input)
{
  std::vector<std::string> raw_server_mode_list = SplitString(server_input, " \r\n");

  for (size_t i = 0; i < raw_server_mode_list.size() - 1; i++)
  {
    if (raw_server_mode_list[i] == POS_MOD_OP)
      loaded_channel_.mod_list.insert(raw_server_mode_list[i + 1]);
    else if(raw_server_mode_list[i] == NEG_MOD_OP)
      loaded_channel_.mod_list.erase(raw_server_mode_list[i + 1]);
  }

  loaded_controller_.UpdateChannelData(loaded_channel_);
}

bool CommandHandler::HandleUserInput(IRCBot::Ptr const& bot, std::string const& user_input)
{
  std::string message_for_bot;

  for (auto& command : commands_)
    if (SubStringMatch(user_input, command.match_str))
      message_for_bot = command.function(RemoveMatchingWord(user_input, command.match_str));

  // FIXME I should look at cleaning this part of a bit
  if (message_for_bot.empty())
  {
    if (SubStringMatch(user_input, BOT_LEAVE))
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
  }

  if (!message_for_bot.empty())
  {
    message_for_bot = ReplaceSymbols(message_for_bot, SYMBOL_USER, username_);
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

static std::string ReplaceArguments(std::string const& raw_arugments, std::string const& str)
{
  std::vector<std::string> args = SplitString(raw_arugments, " \r\n");
  std::string replace_str = str;

  for (size_t i = 0; i < args.size(); i++)
  {
    std::string str_num = TypeConverter<size_t, std::string>(i+1);
    std::string find    = SYMBOL_ARG + str_num + SYMBOL_ARG;

    replace_str = ReplaceSymbols(replace_str, find, args[i]);
  }

  return replace_str;
}

std::string CommandHandler::CheckBasicCommands(std::vector<CommandBreed> const& commands,
                                               std::string const& user_input) const
{
  for (auto const& cb : commands)
  {
    if (UserHasPermissionsForCommand(cb.perm) && !user_input.empty())
    {
      std::vector<std::string> split_user_input = SplitString(user_input, " \n\r");
      std::string command = lowercase(split_user_input[0]);
      if (command == cb.match)
      {
        std::string arguemnts = RemoveMatchingWord(user_input, cb.match);
        return ReplaceArguments(arguemnts, cb.return_str);
      }
    }
  }

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
  std::string weapon_url = sym_weapons_.GetWeaponURL(user_input);

  if (!weapon_url.empty())
    return "Weapon info for " + user_input + ": " + weapon_url;

  return "";
}

std::string CommandHandler::HandlePlayerStats(std::string const& user_input) const
{
  if (!user_input.empty())
    return BF4_STATS_URL + user_input;

  return "";
}

std::string CommandHandler::HandleCompare(std::string const& user_input) const
{
  size_t first_weapon_end = user_input.find(' ');

  if (first_weapon_end != std::string::npos)
  {
    std::string weapon1 = user_input.substr(0, first_weapon_end);
    std::string weapon2 = RemoveStartingWhitespace(user_input.substr(first_weapon_end));

    std::string compare_url = sym_weapons_.GetCompareWeapon(weapon1.c_str(), weapon2.c_str());

    if (!compare_url.empty())
      return "Weapon Compare info: " + compare_url;
  }

  return "";
}

std::string CommandHandler::HandleGoogle(std::string const& user_input) const
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    std::string google_search;
    for (auto const& c : user_input)
    {
      if (c == ' ')
        google_search += "+";
      else
        google_search += c;
    }

    return GOOGLE_URL + google_search;
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

std::string CommandHandler::HandleCustom(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    CommandBreed cb = LoadSingleCommandFromString(user_input);

    if (!cb.match.empty())
    {
      bool handled = loaded_controller_.AddCustomCommand(loaded_channel_.channel, cb);
      loaded_channel_ = loaded_controller_.RequestChannelData(loaded_channel_.channel);

      if (handled)
        return "Command " + cb.match + " was successfully added!";
      else
        return "Failed to add command " + cb.match + " already exists!";
    }
  }

  return "Invalid arguments, must be: !custom <PERMISSIOM> <MATCH> <RETURN_STR>";
}

std::string CommandHandler::HandleRemove(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    loaded_controller_.RemoveCustomCommand(loaded_channel_.channel, user_input);
    loaded_channel_ = loaded_controller_.RequestChannelData(loaded_channel_.channel);

    return "Command " + user_input + " successfully removed!";
  }

  return "Unable to find command " + user_input + ", failed to remove.";
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

std::string CommandHandler::HandleSetLastFM(std::string const& user_input)
{
  if (UserHasPermissionsForCommand(CommandPerm::MOD))
  {
    loaded_channel_.lastfm_username = user_input;
    loaded_controller_.UpdateChannelData(loaded_channel_);

    return "Successfully set the lastfm user name to: " + user_input;
  }

  return "Failed to set the lastfm user name, you must be a mod or owner!";
}

std::string CommandHandler::HandleHelp(std::string const& user_input) const
{
  std::stringstream ss;

  for (auto const& command : basic_commands_)
    if (UserHasPermissionsForCommand(command.perm))
      ss << "!" << command.match << ", ";

  for (auto const& command : loaded_channel_.custom_commands)
    if (UserHasPermissionsForCommand(command.perm))
      ss << "!" << command.match << ", ";

  std::string help_str = ss.str();

  // Remove the last ", " (stupid fence post)
  return help_str.substr(0, help_str.size() - 2);
}

} // namespace irc_bot
