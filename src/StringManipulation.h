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

#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H

#include <sstream>
#include <string>
#include <vector>

namespace irc_bot
{

extern std::string RemoveStartingWhitespace(std::string const& str);
extern std::vector<std::string> SplitString(std::string const& str, std::string const& delims);

extern bool SubStringMatch(std::string const& haystack, std::string const& needle);
extern std::string RemoveMatchingWord(std::string const& user_input, std::string const& match);
extern std::string ReplaceSymbols(std::string const& message, std::string const& symbol, std::string const& replace);

extern std::string lowercase(std::string str);

// TODO Maybe a different file for this
template<typename I, typename O>
O TypeConverter(I const& i)
{
  O output;

  std::stringstream ss;
  ss << i;
  ss >> output;

  return output;
}

} // namespace irc_bot

#endif // STRING_MANIPULATION_H
