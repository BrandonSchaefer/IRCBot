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

#include "CommandBreed.h"
#include "LastFMCurrentSong.h"
#include "LoadBasicCommands.h"
#include "SymWeaponInfo.h"
#include "IRCBotController.h"
#include "Utils.h"

#include "ChannelIO.h"

#include <algorithm>
#include <functional>

namespace irc_bot
{

namespace
{
  // FIXME Move this into a save file per channel
  std::string const LAST_FM_USERNAME = "SithralLoB";

  // CONST SERVER MESSAGES
  std::string const PRIVMSG  = "PRIVMSG";
  std::string const MODE     = "MODE";

  // CONST COMMANDS
  std::string const STATS    = "stats";
  std::string const COMPARE  = "compare";
  std::string const SONG     = "song";
  std::string const FIB      = "fib";

  std::string const CHANNEL  = "#colossusofc1out,#thegreatbambibot";
}


static std::string GetMessage(std::string const& data)
{
  // Skip the first one
  size_t start_of_message = data.find(':', 1) + 1;

  return RemoveStartingWhitespace(data.substr(start_of_message));
}

static std::string GetUsername(std::string const& data)
{
  size_t start_of_username = data.find(':') + 1;
  size_t end_of_username   = data.find('!');

  return RemoveStartingWhitespace(data.substr(start_of_username, end_of_username - start_of_username));
}

static std::string GetChannel(std::string const& data)
{
  size_t start_of_channel = data.find('#');
  size_t end_of_channel   = data.find(' ', start_of_channel);

  if (start_of_channel != std::string::npos && end_of_channel != std::string::npos)
    return RemoveStartingWhitespace(data.substr(start_of_channel, end_of_channel - start_of_channel));

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

IRCBotController::IRCBotController(IRCBot::Ptr const& bot)
  : basic_commands_(LoadBasicCommands())
  , bot_(bot)
{
/*
  LoadedChannelData data = LoadChannelData("#test");
  data.custom_commands.push_back({CommandPerm::OWNER, "test", "testing"});
  SaveChannelData(data);
*/

  if (!bot_->ConnectToServer())
  {
    fprintf(stderr, "Failed to connect to irc server.\n");
  }
  else
  {
    bot_->JoinChannel(CHANNEL);
  }

  bot_->data_received = std::bind(&IRCBotController::RecvDataRecived, this, std::placeholders::_1);
}

void IRCBotController::RecvDataRecived(std::string const& data)
{
  std::string new_data = data;
  new_data.erase(new_data.find_last_not_of("\n\r\t")+1);

  if (match_str(new_data.c_str(), PRIVMSG.c_str()))
  {
    HandlePrivMsg(new_data);
  }
  else if (match_str(new_data.c_str(), MODE.c_str()))
  {
    HandleMode(new_data);
  }

  //printf(".%s.\n", new_data.c_str());
}

// :jtv MODE #colossusofc1out +o thegreatbambibot
void IRCBotController::HandleMode(std::string const& server_data)
{
  std::string channel   = GetChannel(server_data);
  size_t username_start = server_data.rfind(' ');
  std::string username  = RemoveStartingWhitespace(server_data.substr(username_start));

  if (!channel.empty())
    channel_mods_[channel].insert(username);
}

bool IRCBotController::UserHasPermissionsForCommand(PrivateMessageData const& msg_data,
                                                    CommandPerm const& perm) const
{
  // User commands are always valid
  if (perm == CommandPerm::USER)
    return true;

  // Check if the user is a mod
  auto const& mods = channel_mods_.find(msg_data.data.channel);
  if (mods != channel_mods_.end())
  {
    for (auto const& mod : mods->second)
    {
      if (mod == msg_data.username)
        return true;
    }
  }

  // Check if its the owner, so skip the '#'
  std::string channel_owner = msg_data.data.channel.substr(1);
  return msg_data.username == channel_owner;
}

std::string lowercase(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

void IRCBotController::HandlePrivMsg(std::string const& server_data)
{
  std::string message = GetMessage(server_data);

  if (message[0] == '!')
  {
    PrivateMessageData msg_data;

    std::string channel  = GetChannel(server_data);
    std::string username = GetUsername(server_data);
    message = RemoveStartingWhitespace(message.substr(1));

    // FIXME this could end up doing something bad...
    channel  = lowercase(channel);
    username = lowercase(username);
    message  = lowercase(message);

    msg_data.username = username;
    msg_data.message  = message;
    msg_data.data     = loaded_controller_.RequestChannelData(channel);

    bool handled = false;

    handled = HandleBasic(msg_data);

    if (!handled)
      handled = HandleStats(msg_data);
    if (!handled)
      handled = HandleCompare(msg_data);
    if (!handled)
      handled = HandleSong(msg_data);
    if (!handled)
      handled = HandleFib(msg_data);

    if (!handled)
      fprintf(stderr, "Failed to find a command for: %s\n", message.c_str());
  }
}

bool IRCBotController::CheckBasicCommands(std::vector<CommandBreed> const& commands,
                                          PrivateMessageData const& msg_data) const
{
  for (auto const& cb : commands)
  {
    if (msg_data.message == cb.match)
    {
      if (UserHasPermissionsForCommand(msg_data, cb.perm))
      {
        bot_->SendMessage(msg_data.data.channel, cb.return_str);
        return true;
      }
    }
  }

  return false;
}

bool IRCBotController::HandleBasic(PrivateMessageData const& msg_data) const
{
  int handled = false;
  handled = CheckBasicCommands(basic_commands_, msg_data);

  if (!handled)
    handled = CheckBasicCommands(msg_data.data.custom_commands, msg_data);
  
  return handled;
}

bool IRCBotController::HandleStats(PrivateMessageData const& msg_data) const
{
  if (match_str(msg_data.message.c_str(), STATS.c_str()))
  {
    std::string message    = RemoveStartingWhitespace(msg_data.message.substr(STATS.size()));
    std::string weapon_url = GetWeaponURL(message);

    if (!weapon_url.empty())
    {
      bot_->SendMessage(msg_data.data.channel, "Weapon info for " + message + ": " + weapon_url);
      return true;
    }
  }

  return false;
}

bool IRCBotController::HandleCompare(PrivateMessageData const& msg_data) const
{
  if (match_str(msg_data.message.c_str(), COMPARE.c_str()))
  {
    // Move down "compare";
    std::string message = RemoveStartingWhitespace(msg_data.message.substr(COMPARE.size()));
    size_t first_weapon_end = message.find(' ');

    if (first_weapon_end != std::string::npos)
    {
      std::string weapon1 = message.substr(0, first_weapon_end);
      std::string weapon2 = RemoveStartingWhitespace(message.substr(first_weapon_end));

      std::string compare_url = GetCompareWeapon(weapon1.c_str(), weapon2.c_str());

      if (!compare_url.empty())
      {
        bot_->SendMessage(msg_data.data.channel, "Weapon Compare info: " + compare_url);
        return true;
      }
    }
  }

  return false;
}

bool IRCBotController::HandleSong(PrivateMessageData const& msg_data) const
{
  if (match_str(msg_data.message.c_str(), SONG.c_str()))
  {
    // Move down "song";
    std::string message = RemoveStartingWhitespace(msg_data.message.substr(SONG.size()));

    SongInfo song_info = last_song_.GetCurrentPlayingSong(LAST_FM_USERNAME);

    if (!song_info.artist.empty() && !song_info.title.empty())
    {
      bot_->SendMessage(msg_data.data.channel, song_info.title + " by " + song_info.artist);
      return true;
    }
  }

  return false;
}

bool IRCBotController::HandleFib(PrivateMessageData const& msg_data) const
{
  if (match_str(msg_data.message.c_str(), FIB.c_str()))
  {
    // Move down "fib";
    std::string message = RemoveStartingWhitespace(msg_data.message.substr(FIB.size()));

    if (!message.empty())
    {
      uint64_t n = TypeConverter<std::string, uint64_t>(message);
      if (n > 0)
      {
        std::string fib_str = TypeConverter<uint64_t, std::string>(fib(n));
        bot_->SendMessage(msg_data.data.channel, "Fib(" + message + ") = " + fib_str);
        return true;
      }
    }
  }

  return false;
}

void IRCBotController::StartBot()
{
  bot_->StartMainServerLoop();
}

} // namespace irc_bot
