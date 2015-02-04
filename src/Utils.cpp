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

#include "Utils.h"

#include <fstream>
#include <string.h>

namespace irc_bot
{

std::string RemoveStartingWhitespace(std::string const& str)
{
  std::string new_str;

  size_t space_count = 0;
  for (auto const& c : str)
  {
    if (c == ' ')
      space_count++;
    else
      break;
  }

  new_str = str.substr(space_count);

  return new_str;
}

std::string ReadInFile(char const* path)
{
  std::string str;
  std::ifstream is(path, std::ifstream::binary);

  if (is)
  {
    is.seekg(0, is.end);
    int length = (int)is.tellg();
    is.seekg(0, is.beg);

    char* buff = new char[length];

    is.read(buff, length);
    is.close();

    buff[length] = '\0';

    str = std::string(buff);
  }
  else
  {
    fprintf(stderr, "Failed to load file: %s\n", path);
  }

  return str;
}

std::vector<std::string> SplitStringOnNewLineOrNull(std::string const& str)
{
  std::vector<std::string> split_str;

  std::string new_str;
  for (auto const& c : str)
  {
    if (c != '\n' && c != '\0')
    {
      new_str += c;
    }
    else
    {
      new_str = RemoveStartingWhitespace(new_str);
      split_str.push_back(new_str);
      new_str.clear();
    }
  }

  if (!new_str.empty())
  {
    new_str = RemoveStartingWhitespace(new_str);
    split_str.push_back(new_str);
  }

  return split_str;
}

bool match_str(const char* buffer, const char* match)
{
  return strstr(buffer, match) != '\0';
}

} // namespace irc_bot
