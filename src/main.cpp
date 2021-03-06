// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2015 Brandon Schaefer
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

#include "IRCBot.h"
#include "IRCBotOffline.h"
#include "IRCBotController.h"

#include <string.h>

#include "ReadWriteIO.h"

namespace
{
  // Server Info
  std::string const SERVER   = "irc.twitch.tv";
  //std::string const SERVER   = "irc.freenode.net";
  std::string const PORT     = "6667";
  // FIXME Figure out how to correctly store this password...plain test == BAD BAD FUCKING BAD (hello!)
  std::string const PASSWORD = "oauth:z11bz3a6fzf1d8a2jbqd9demm8di2j"; // FIXME TODO REMOVE ME BEFORE COMMITS!
  std::string const NICK     = "TheGreatBambiBot";
  std::string const USER     = "TheGreateBambiBot nothing here :Real Name";

  irc_bot::ServerInfo const INFO = { SERVER,
                                     PORT,
                                     PASSWORD,
                                     NICK,
                                     USER
                                   };
}

int main(int argc, char* argv[])
{
  irc_bot::IRCBot::Ptr bot;
  if (argc > 1 && argc <= 2)
  {
    // Simple way to turn the bot into offline/testing mode
    if (strcmp("--offline", argv[1]) == 0)
      bot = std::make_shared<irc_bot::IRCBotOffline>(INFO);
  }
  else
  {
    bot = std::make_shared<irc_bot::IRCBot>(INFO);
  }

  irc_bot::IRCBotController bot_controller(bot);

  bot_controller.StartBot();

  return 0;
}
