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

  // BOT SPECIFIC COMMANDS ONLY
  std::string const BOT_JOIN = "bot join";

  std::string const CHANNEL = "#colossusofc1out,#thegreatbambibot";
}

static std::string GetMessage(std::string const& str)
{
  // Skip the first one
  size_t start_of_message = str.find(':', 1) + 1;

  return RemoveStartingWhitespace(str.substr(start_of_message));
}

static std::string GetUsername(std::string const& str)
{
  size_t start_of_username = str.find(':') + 1;
  size_t end_of_username   = str.find('!');

  return RemoveStartingWhitespace(str.substr(start_of_username, end_of_username - start_of_username));
}

IRCBotController::IRCBotController(IRCBot::Ptr const& bot)
  : basic_commands_(LoadBasicCommands())
  , bot_(bot)
{
  if (!bot_->ConnectToServer())
    fprintf(stderr, "Failed to connect to irc server.\n");
  else
    bot_->JoinChannel(CHANNEL);

  bot_->input_received = std::bind(&IRCBotController::RecvServerInputRecived, this, std::placeholders::_1);
}

void IRCBotController::RecvServerInputRecived(std::string const& server_input)
{
  std::string formated_server_input = server_input;
  formated_server_input.erase(formated_server_input.find_last_not_of("\n\r\t")+1);

  printf(".%s.\n", formated_server_input.c_str());

  if (SubStringMatch(formated_server_input, PRIVMSG))
    HandlePrivMsg(formated_server_input);
  else if (SubStringMatch(formated_server_input, MODE))
    HandleMode(formated_server_input);
}

void IRCBotController::HandleMode(std::string const& server_input)
{
  command_handler_.UpdateLoadedChannel(server_input);
  command_handler_.HandleModServerCommand(server_input);
}

std::string lowercase(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

void IRCBotController::HandlePrivMsg(std::string const& server_input)
{
  std::string message = GetMessage(server_input);

  if (!message.empty() && message[0] == '!')
  {
    std::string username = GetUsername(server_input);
    message  = RemoveStartingWhitespace(message.substr(1));
    username = lowercase(username);

    command_handler_.SetUsername(username);
    command_handler_.UpdateLoadedChannel(server_input);

    bool handled = command_handler_.HandleUserInput(bot_, message);

    // FIXME clean me up
    std::string bot_channel = "#" + lowercase(bot_->Name());
    if (SubStringMatch(message, BOT_JOIN) && SubStringMatch(server_input, bot_channel))
    {
      bot_->SendMessage(bot_channel, "Joinning channel: http://www.twitch.tv/" + username);
      bot_->JoinChannel("#" + username);
      bot_->SendMessage("#" + username, "Hello, " + bot_->Name() + " has entered the room.");
      handled = true;
    }

    if (!handled)
      fprintf(stderr, "Failed to find a command for: %s\n", message.c_str());
  }
}

void IRCBotController::StartBot()
{
  bot_->StartMainServerLoop();
}

} // namespace irc_bot
