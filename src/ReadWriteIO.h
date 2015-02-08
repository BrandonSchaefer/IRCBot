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

#ifndef READ_WRITE_I_O_H
#define READ_WRITE_I_O_H

#include <string>

namespace irc_bot
{

// Simple Read/Write functions to files. No need for a class
extern void WriteToFile      (std::string const& raw_info, std::string const& path);
extern std::string ReadInFile(std::string const& path);

} // namespace irc_bot

#endif // READ_WRITE_I_O_H
