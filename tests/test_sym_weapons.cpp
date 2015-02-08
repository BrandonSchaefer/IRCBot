// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2015 Brandon Schaefer
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

#include <gtest.h>

#include <SymWeaponInfo.cpp>
#include <StringManipulation.cpp>

using namespace testing;

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  std::string const& TEST_WEAPON1 = "aek";
  std::string const& TEST_WEAPON2 = "m416";
}

class TestSymWeapons : public ::Test
{
public:
  TestSymWeapons()
  {}

  SymWeaponInfo info;
};

// I suppose i could assert the actual URL... but if it ever changes it breaks. Best to just
// assert something returned. I should look at getting all the weapons under tests.
TEST_F(TestSymWeapons, TestGetWeaponURL)
{
  std::string weapon = info.GetWeaponURL(TEST_WEAPON1);

  weapon = lowercase(weapon);
  EXPECT_TRUE (SubStringMatch(weapon, TEST_WEAPON1));

  EXPECT_FALSE(weapon.empty());
}

TEST_F(TestSymWeapons, TestGetWeapon)
{
  std::string weapon = info.GetWeapon(TEST_WEAPON1);

  weapon = lowercase(weapon);
  EXPECT_TRUE (SubStringMatch(weapon, TEST_WEAPON1));

  EXPECT_FALSE(weapon.empty());
}

TEST_F(TestSymWeapons, TestCompareWeapons)
{
  std::string weapon = info.GetCompareWeapon(TEST_WEAPON1, TEST_WEAPON2);
  weapon = lowercase(weapon);

  EXPECT_TRUE (SubStringMatch(weapon, TEST_WEAPON1));
  EXPECT_TRUE (SubStringMatch(weapon, TEST_WEAPON2));

  EXPECT_FALSE(weapon.empty());
}

} // namespace irc_bot
} // namespace irc_bot_test
