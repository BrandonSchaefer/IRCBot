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

#include "ReadWriteIO.h"

#include <fstream>
#include <string>
#include <vector>

namespace irc_bot
{

void WriteToFile(std::string const& raw_info, std::string const& path)
{
  std::ofstream os(path.c_str(), std::ofstream::binary);

  if (os.is_open())
  {
    os << raw_info;
    os.close();
  }
  else
  {
    fprintf(stderr, "Failed to load file: %s\n", path.c_str());
  }
}

std::string ReadInFile(std::string const& path)
{
  std::string str;
  std::ifstream is(path.c_str(), std::ifstream::binary);

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
    fprintf(stderr, "Failed to load file: %s\n", path.c_str());
  }

  return str;
}

} // namespace irc_bot
