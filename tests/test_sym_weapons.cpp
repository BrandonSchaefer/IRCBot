#include <gtest.h>
#include <SymWeaponInfo.cpp>

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  std::string const& TEST_WEAPON1 = "aek";
  std::string const& TEST_WEAPON2 = "m416";
}

// I suppose i could assert the actual URL... but if it ever changes it breaks. Best to just
// assert something returned. I should look at getting all the weapons under tests.
TEST(SymWeapons, TestGetWeaponURL)
{
  std::string weapon = GetWeaponURL(TEST_WEAPON1);

  EXPECT_FALSE(weapon.empty());
}

TEST(SymWeapons, TestGetWeapon)
{
  std::string weapon = GetWeapon(TEST_WEAPON1);

  EXPECT_FALSE(weapon.empty());
}

TEST(SymWeapons, TestCompareWeapons)
{
  std::string weapon = GetCompareWeapon(TEST_WEAPON1, TEST_WEAPON2);

  EXPECT_FALSE(weapon.empty());
}

} // namespace irc_bot
} // namespace irc_bot_test
