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

#include "LoadBasicCommands.h"
#include "ReadWriteIO.h"
#include "StringManipulation.h"

#include "config.h"

namespace irc_bot
{

namespace
{
  std::string BASIC_COMMAND_PATH = SAVESDIR"/basic_commands";
}

/*
Format of a BasicCommand:
<PermissionsType> <MatchString> <ReturnString>
int             std::string   std::string
*/

static CommandPerm GetPermissionType(unsigned int permisson)
{
  switch (permisson)
  {
    case 0:
      return CommandPerm::OWNER;
      break;
    case 1:
      return CommandPerm::MOD;
      break;
    default:
      return CommandPerm::USER;
      break;
  }
}

static bool is_digit(char const c)
{
  return (c >= '0' && c <= '9');
}

static CommandBreed ConstructCommandBreed(std::string const& str)
{
  CommandBreed cb;

  std::string new_str = RemoveStartingWhitespace(str);
  if (!new_str.empty() && new_str[0] != '\n')
  {
    // Comment, ignore
    if (new_str[0] == '#')
      return cb;

    // Find Perm
    size_t start = 0;
    size_t end   = new_str.find(' ');

    if (end == std::string::npos)
      return cb;

    std::string perm_str = RemoveStartingWhitespace(new_str.substr(start, end));

    if (!perm_str.empty() && !is_digit(perm_str[0]))
      return cb;

    unsigned int perm = 2;
    perm = TypeConverter<std::string, unsigned int>(perm_str);

    CommandPerm p = GetPermissionType(perm);

    // Find Match
    start = end + 1;
    end   = new_str.find(' ', start);

    if (end == std::string::npos)
      return cb;

    std::string match = RemoveStartingWhitespace(new_str.substr(start, end - start));

    // Find Return
    std::string return_str = RemoveStartingWhitespace(new_str.substr(end + 1));

    cb.perm       = p;
    cb.match      = match;
    cb.return_str = return_str;
  }

  return cb;
}

static std::vector<CommandBreed> ConstructBasicCommands(std::string const& raw_basic_commands)
{
  std::vector<CommandBreed> commands;

  std::vector<std::string> basic_commands = SplitString(raw_basic_commands, "\n\0");

  for (auto const& raw_command : basic_commands)
  {
    CommandBreed cb = ConstructCommandBreed(raw_command);

    if (!cb.match.empty() && !cb.return_str.empty())
      commands.push_back(cb);
  }

  return commands;
}

CommandBreed LoadSingleCommandFromString(std::string const& str)
{
  return ConstructCommandBreed(str);
}

std::vector<CommandBreed> LoadBasicCommandsFromString(std::string const& str)
{
  return ConstructBasicCommands(str);
}

std::vector<CommandBreed> LoadBasicCommandsFromPath(std::string const& path)
{
  std::string raw_basic_commands = ReadInFile(path);

  return ConstructBasicCommands(raw_basic_commands);
}

std::vector<CommandBreed> LoadBasicCommands()
{
  std::string raw_basic_commands = ReadInFile(BASIC_COMMAND_PATH);

  return ConstructBasicCommands(raw_basic_commands);
}

} // namespace irc_bot
