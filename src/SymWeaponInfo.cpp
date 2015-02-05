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

#include <algorithm>

#include "SymWeaponInfo.h"
#include "Utils.h"

namespace irc_bot
{

namespace
{

const std::string BASE_URL    = "http://symthic.com/bf4-weapon-info?w=";
const std::string BASE_VS_URL = "http://symthic.com/bf4-compare?";
const std::string VS_SPLIT    = "_vs_";

// FIXME Move to a text file and read in this file, so its easy to update
const std::string WEAPON_INFO[] = {
  // Carbines/DMR/Shotguns
  "A-91",
  "ACW-R",
  "AK_5C",
  "AKU-12",
  "G36C",
  "ACE_21_CQB",
  "ACE_52_CQB",
  "M4",
  "MTAR-21",
  "SG553",
  "Type-95B-1",
  "ACE_53_SV",
  "M39_EMR",
  "MK11_MOD_0",
  "QBU-88",
  "RFP",
  "SCAR-H_SV",
  "SKS",
  "SVD-12",

  // Pistols
  "CZ-75",
  "Deagle_44",
  "FN57",
  "G18",
  "Compact_45",
  "M1911",
  "M9",
  "93R",
  "MP412_Rex",
  "MP443",
  "P226",
  "QSZ-92",
  "SW40",
  "44_Magnum",
  "Unica_6",

  // PDW
  "AS_VAL",
  "CBJ-MS",
  "JS2",
  "PDW-R",
  "MP7",
  "MPX",
  "MX4",
  "P90",
  "PP-2000",
  "CZ-3A1",
  "SR-1",
  "UMP-45",
  "UMP-9",

  // Assult Rifles
  "AEK-971",
  "AK-12",
  "AR160",
  "Bulldog",
  "CZ-805",
  "F2000",
  "FAMAS",
  "ACE_23",
  "L85A2",
  "M16A4",
  "M416",
  "QBZ-95-1",
  "SAR-21",
  "SCAR-H",
  "AUG_A3",

  // LMG
  "AWS",
  "LSAT",
  "M240B",
  "M249",
  "M60E4",
  "MG4",
  "PKP_Pecheneg",
  "QBB-95-1",
  "RPK-74M",
  "U-100_MK5",
  "RPK-12",
  "Type-88_LMG",

  // Recon
  "ARM-2",
  "CS-LR4",
  "CS5",
  "FY-JS",
  "GOL_Magnum",
  "JNG-90",
  "L96A1",
  "SRR-61",
  "M40A5",
  "M82A3",
  "M98B",
  "Railgun",
  "Scout_Elite",
  "SR338",
  "338-Recon",
  "SV98",
};

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

/*
Loop through all BASE_WEAPONS
case 1: Keep name the same, check for a sub str match
case 2: Remove all _/-, and check for a sub str match
*/
std::string GetWeapon(std::string const& search_weapon)
{
  if (!search_weapon.empty())
  {
    std::string l_search_weapon = search_weapon;
    std::transform(l_search_weapon.begin(), l_search_weapon.end(), l_search_weapon.begin(), ::tolower);

    for (auto const& weapon : WEAPON_INFO)
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

std::string GetWeaponURL(std::string const& search_weapon)
{
  std::string weapon = GetWeapon(search_weapon);

  if (!weapon.empty())
    return BASE_URL + weapon;

  return "";
}

std::string GetCompareWeapon(std::string const& search_weapon1, std::string const& search_weapon2)
{
  std::string weapon1 = GetWeapon(search_weapon1);
  std::string weapon2 = GetWeapon(search_weapon2);

  if (!weapon1.empty() && !weapon2.empty())
    return BASE_VS_URL + weapon1 + VS_SPLIT + weapon2;

  return "";
}

} // namespace irc_bot
