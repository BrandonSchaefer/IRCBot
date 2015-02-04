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
#include "LoadBasicCommands.h"
#include "IRCBotController.h"
#include "Utils.h"

#include <algorithm>
#include <functional>

namespace irc_bot
{

namespace
{
  // CONST SERVER MESSAGES
  std::string const PRIVMSG = "PRIVMSG";
  std::string const MODE    = "MODE";

  // CONST COMMANDS
  std::string const STATS   = "stats";
  std::string const COMPARE = "compare";
  std::string const SONG    = "song";
  std::string const CUSTOM  = "custom";
  std::string const REMOVE  = "remove";
  std::string const FIB     = "fib";

  std::string const CHANNEL = "#colossusofc1out,#thegreatbambibot";
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

IRCBotController::IRCBotController(IRCBot::Ptr const& bot)
  : basic_commands_(LoadBasicCommands())
  , bot_(bot)
{
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

  if (SubStringMatch(new_data, PRIVMSG))
  {
    HandlePrivMsg(new_data);
  }
  else if (SubStringMatch(new_data, MODE))
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

  LoadedChannelData channel_data = loaded_controller_.RequestChannelData(channel);

  if (!channel.empty())
  {
    channel_data.mod_list.insert(username);
    loaded_controller_.UpdateChannelData(channel_data);
  }
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

    command_handler_.SetUsername(username);
    command_handler_.SetLoadedChannel(loaded_controller_.RequestChannelData(channel));

    msg_data.username = username;
    msg_data.message  = message;
    msg_data.data     = loaded_controller_.RequestChannelData(channel);

    std::string message_for_bot = command_handler_.HandleUserInput(message);

    bool handled = false;

    if (!message_for_bot.empty())
    {
      bot_->SendMessage(channel, message_for_bot);
      handled = true;
    }

    if (SubStringMatch(message, CUSTOM))
    {
      handled = HandleCustom(channel, RemoveMatchingWord(message, CUSTOM));
    }
    else if (SubStringMatch(message, REMOVE))
    {
      handled = HandleRemove(channel, RemoveMatchingWord(message, REMOVE));
    }

    if (!handled)
      fprintf(stderr, "Failed to find a command for: %s\n", message.c_str());
  }
}


bool IRCBotController::HandleCustom(std::string const& channel, std::string const& user_input)
{
  CommandBreed cb = LoadSingleCommandFromString(user_input);

  if (!cb.match.empty())
  {
    loaded_controller_.AddCustomCommand(channel, cb);
    return true;
  }

  return false;
}

bool IRCBotController::HandleRemove(std::string const& channel, std::string const& user_input)
{
  return loaded_controller_.RemoveCustomCommand(channel, user_input);
}

void IRCBotController::StartBot()
{
  bot_->StartMainServerLoop();
}

} // namespace irc_bot
