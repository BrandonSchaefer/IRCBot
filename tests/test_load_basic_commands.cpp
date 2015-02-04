#include <gtest.h>
#include <LoadBasicCommands.h>

using namespace testing;

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  std::string const COMMENT       = "#this is a command, and LoadBasic should return an empty vector";

  // The basics of a valid command
  CommandPerm const PERM          = CommandPerm::MOD;
  std::string const PERM_STR      = "1";
  std::string const MATCH         = "test_match";
  std::string const RETURN_STR    = "testing the return str!";

  // 3 valid commands here
  std::string const MULTIPLE_COMMANDS = "#we are testing\n1 a eeks ajdf\n2 b tmps\n2 c test";
  size_t const VALID_COMMANDS         = 3;
}

TEST(LoadBasicCommand, TestCommentsReturnNothing)
{
  std::vector<CommandBreed> commands = LoadBasicCommandsFromString(COMMENT);
  EXPECT_TRUE(commands.empty());
}

TEST(LoadBasicCommand, TestValidCommand)
{
  std::vector<CommandBreed> commands = LoadBasicCommandsFromString(MULTIPLE_COMMANDS);
  EXPECT_EQ(commands.size(), VALID_COMMANDS);
}

TEST(LoadBasicCommand, TestEmptyStringReturnNothing)
{
  std::vector<CommandBreed> commands = LoadBasicCommandsFromString("");
  EXPECT_TRUE(commands.empty());
}

TEST(LoadBasicCommand, TestReturnsNothingWithPermOnly)
{
  std::vector<CommandBreed> commands = LoadBasicCommandsFromString(PERM_STR);
  EXPECT_TRUE(commands.empty());
}

TEST(LoadBasicCommand, TestReturnsNothingWithPermAndMatch)
{
  std::vector<CommandBreed> commands = LoadBasicCommandsFromString(PERM_STR + " " + MATCH);
  EXPECT_TRUE(commands.empty());
}

TEST(LoadBasicCommand, TestLoadSingleCommand)
{
  CommandBreed cb = LoadSingleCommandFromString(PERM_STR + " " + MATCH + " " + RETURN_STR);
  EXPECT_EQ(cb.perm,       PERM);
  EXPECT_EQ(cb.match,      MATCH);
  EXPECT_EQ(cb.return_str, RETURN_STR);
}

} // namespace irc_bot
} // namespace irc_bot_test
