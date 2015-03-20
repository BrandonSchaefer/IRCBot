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

#include <json/json.h>

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

struct string
{
  char* ptr;
  size_t len;
};

static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string& userdata)
{
  size_t count = size * nmemb;
  userdata += std::string(ptr);

  return count;
}

SongInfo ParseRawLastFMJson(std::string const& raw_json)
{
  std::string artist;
  std::string title;

  Json::Value root;
  Json::Reader reader;

  bool handled = reader.parse(raw_json, root, false);
  if (handled)
  {
    Json::Value track = root.get("track", "array");
    title             = track.get("name", "string").asString();

    Json::Value artist_obj = track.get("artist", "object");
    artist                 = artist_obj.get("name", "string").asString();
  }

  return {artist, title};

}

SongInfo LastFMCurrentSong::GetCurrentPlayingSong(std::string const& username) const
{
  if (curl_)
  {
    std::string url = LAST_FM_URL_BEG + username + LAST_FM_URL_END;
    std::string raw_json;

    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &raw_json);

    CURLcode res;
    res = curl_easy_perform(curl_);

    if (res != CURLE_OK)
      fprintf(stderr, "Failed to parse url\n");

    return ParseRawLastFMJson(raw_json);
  }

  return SongInfo();
}

} // namespace irc_bot
