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

#ifndef LOAD_BASIC_COMMANDS_H
#define LOAD_BASIC_COMMANDS_H

#include <string>
#include <vector>

#include "CommandBreed.h"

namespace irc_bot
{

extern CommandBreed LoadSingleCommandFromString(std::string const& str);
extern std::vector<CommandBreed> LoadBasicCommandsFromString(std::string const& str);
extern std::vector<CommandBreed> LoadBasicCommandsFromPath(std::string const& path);
extern std::vector<CommandBreed> LoadBasicCommands();

} // namespace irc_bot

#endif // LOAD_BASIC_COMMANDS_H
