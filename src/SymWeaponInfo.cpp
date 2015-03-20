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

#include <algorithm>

#include "ReadWriteIO.h"
#include "SymWeaponInfo.h"
#include "StringManipulation.h"

#include "config.h"

namespace irc_bot
{

namespace
{

std::string const BASE_URL     = "http://symthic.com/bf4-weapon-info?w=";
std::string const BASE_VS_URL  = "http://symthic.com/bf4-compare?";
std::string const VS_SPLIT     = "_vs_";
std::string const DEFAULT_PATH = SAVESDIR"/raw_sym_weapons";

}

static std::string RemoveUnderscoresAndDashes(std::string const& str)
{
  std::string new_str;
  for (auto const& c : str)
  {
    if (c != '_' && c != '-')
      new_str += c;
  }

  return new_str;
}

SymWeaponInfo::SymWeaponInfo()
{
  LoadInWeaponInfo();
}

void SymWeaponInfo::LoadInWeaponInfo()
{
  std::string raw_weapon_info = ReadInFile(DEFAULT_PATH);
  if (!raw_weapon_info.empty())
  {
    std::vector<std::string> weapons = SplitString(raw_weapon_info, "\r\n");

    // Filter out comments
    for (auto const& str : weapons)
      if (!str.empty() && str[0] != '#')
        weapon_info_.push_back(str);
  }
}

/*
Loop through all BASE_WEAPONS
ckSDLWindow.TestSetWindowData
case 1: Keep name the same, check for a sub str match
case 2: Remove all _/-, and check for a sub str match
*/
std::string SymWeaponInfo::GetWeapon(std::string const& raw_search_weapon) const
{
  if (!raw_search_weapon.empty())
  {
    std::string search_weapon = RemoveCharacters(raw_search_weapon, " _-");
    std::string l_search_weapon = search_weapon;
    std::transform(l_search_weapon.begin(), l_search_weapon.end(), l_search_weapon.begin(), ::tolower);

    for (auto const& weapon : weapon_info_)
    {
      // Case1
      std::string l_weapon = weapon;
      std::transform(l_weapon.begin(), l_weapon.end(), l_weapon.begin(), ::tolower);

      if (SubStringMatch(l_weapon, l_search_weapon))
        return weapon;

      // Case2
      std::string no_dash_underscore_weapon = RemoveUnderscoresAndDashes(l_weapon);

      if (SubStringMatch(no_dash_underscore_weapon, l_search_weapon))
        return weapon;
    }
  }

  return "";
}

std::string SymWeaponInfo::GetWeaponURL(std::string const& search_weapon) const
{
  std::string weapon = GetWeapon(search_weapon);

  if (!weapon.empty())
    return BASE_URL + weapon;

  return "";
}

std::string SymWeaponInfo::GetCompareWeapon(std::string const& search_weapon1,
                                            std::string const& search_weapon2) const
{
  std::string weapon1 = GetWeapon(search_weapon1);
  std::string weapon2 = GetWeapon(search_weapon2);

  if (!weapon1.empty() && !weapon2.empty())
    return BASE_VS_URL + weapon1 + VS_SPLIT + weapon2;

  return "";
}

} // namespace irc_bot
