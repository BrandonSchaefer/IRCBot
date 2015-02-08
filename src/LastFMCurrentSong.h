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

#ifndef LAST_F_M_CURRENT_SONG_H
#define LAST_F_M_CURRENT_SONG_H

#include <curl/curl.h>
#include <string>

namespace irc_bot
{

struct SongInfo
{
  std::string const artist;
  std::string const title;
};

class LastFMCurrentSong
{
public:
  LastFMCurrentSong();
  ~LastFMCurrentSong();

  SongInfo GetCurrentPlayingSong(std::string const& username) const;

private:
  CURL* curl_;

};

} // namespace irc_bot

#endif // LAST_F_M_CURRENT_SONG_H
