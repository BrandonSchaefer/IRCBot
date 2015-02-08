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

#ifndef SYM_WEAPON_INFO_H
#define SYM_WEAPON_INFO_H

#include <string>

namespace irc_bot
{

class SymWeaponInfo
{
public:
  SymWeaponInfo();
  std::string GetWeaponURL    (std::string const& search_weapon) const;
  std::string GetWeapon       (std::string const& search_weapon) const;
  std::string GetCompareWeapon(std::string const& search_weapon1,
                               std::string const& search_weapon2) const;
private:
  void LoadInWeaponInfo();

  std::vector<std::string> weapon_info_;

};

} // namespace irc_bot

#endif // SYM_WEAPON_INFO_H
