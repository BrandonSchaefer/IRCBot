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

#include "LastFMCurrentSong.h"

#include <sstream>

namespace irc_bot
{

namespace
{
  // FIXME Should look at using some sort of parser here...but this works for now
  int const SPLITER_SIZE        = 10; // ","name":"
  std::string const ARTIST_ID   = "artist";
  std::string const TITLE_ID    = "track";
  std::string const LAST_FM_URL_BEG = "http://ajax.last.fm/user/";
  std::string const LAST_FM_URL_END = "/now";
}

LastFMCurrentSong::LastFMCurrentSong()
  : curl_(curl_easy_init())
{
}

LastFMCurrentSong::~LastFMCurrentSong()
{
  curl_easy_cleanup(curl_);
}

static size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  std::ostringstream *stream = (std::ostringstream*)userdata;
  size_t count = size * nmemb;
  stream->write(ptr, count);

  return count;
}

/*
We are going to cheat here, and parse the artist/title our selfs vs using a Json parser
Simply we look for the keyword right before the artist/title

For artist it looks like:
"artist":{"_type":"artist","name":"Monstercat",

Find the second occurence of artist jump over 16 (artist+ ","name":") == Monstercat

For Title:
"track":{"_type":"track","name":"Solace Album Mix"

Find the second occurence of title jump over 15 (track + ","name":") == Solace Album Mix

*/

static std::string FindElementInOutput(std::string const& output, std::string const& keyword)
{
  // Find the second instance of the keyword
  size_t keyword_start = output.find(keyword) + 1;
  keyword_start = output.find(keyword, keyword_start) + keyword.size() + SPLITER_SIZE;
  size_t keyword_end   = output.find('"', keyword_start);

  if (keyword_start != std::string::npos && keyword_end != std::string::npos)
    return output.substr(keyword_start, keyword_end - keyword_start);

  return "";
}

SongInfo LastFMCurrentSong::GetCurrentPlayingSong(std::string const& username) const
{
  if (curl_)
  {
    std::string url = LAST_FM_URL_BEG + username + LAST_FM_URL_END;
    std::ostringstream stream;

    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &stream);

    CURLcode res;
    res = curl_easy_perform(curl_);

    if (res != CURLE_OK)
      fprintf(stderr, "Failed to parse url\n");

    std::string output = stream.str();

    std::string artist = FindElementInOutput(output, ARTIST_ID);
    std::string title  = FindElementInOutput(output, TITLE_ID);

    return {artist, title};
  }

  return SongInfo();
}

} // namespace irc_bot
