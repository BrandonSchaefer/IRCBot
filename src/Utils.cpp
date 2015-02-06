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

void WriteToFile(std::string const& raw_info, char const* path)
{
  std::ofstream os(path, std::ofstream::binary);

  if (os.is_open())
  {
    os << raw_info;
    os.close();
  }
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

    char* buff = new char[length + 1];

    is.read(buff, length);
    is.close();

    buff[length] = '\0';

    str = std::string(buff);
    delete buff;
  }
  else
  {
    fprintf(stderr, "Failed to load file: %s\n", path);
  }

  return str;
}

std::vector<std::string> SplitString(std::string const& str, std::string const& delims)
{
  std::vector<std::string> split_str;

  std::string new_str;
  for (auto const& c : str)
  {
    if (delims.find(c) == std::string::npos)
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

bool SubStringMatch(std::string const& haystack, std::string const& needle)
{
  return strstr(haystack.c_str(), needle.c_str()) != '\0';
}

std::string RemoveMatchingWord(std::string const& user_input, std::string const& match)
{
  return RemoveStartingWhitespace(user_input.substr(match.size()));
}

} // namespace irc_bot
