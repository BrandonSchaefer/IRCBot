#include <gtest.h>
#include <gmock/gmock.h>

#include <IRCBotController.h>
#include <IRCBot.h>

namespace irc_bot
{
namespace irc_bot_test
{

namespace
{
  // Empty ... using an offline bot :)
  ServerInfo const INFO = {};
  std::string CHANNEL   = "#test";
  std::string PRIV_MSG  = ":TheGreatBambiBot!test@192.168.0.1 PRIVMSG #test :";

  std::string FIB_MSG   = "!fib 10";
  std::string STAT_MSG  = "!stats aek";
  std::string COMP_MSG  = "!compare aek m416";
  // Cant really test !song since it depends on the internet... and thats not always going to be around
}

class MockIRCBot : public IRCBot
{
public:
  typedef std::shared_ptr<MockIRCBot> Ptr;

  MockIRCBot(ServerInfo const& info)
  : IRCBot(info)
  {}

  virtual bool ConnectToServer() { return true; }
  virtual void StartMainServerLoop() {}

  virtual void JoinChannel(std::string const& channel) const {}

  MOCK_CONST_METHOD2(SendMessage, void(std::string const&, std::string const&));
};

class MockIRCBotController : public testing::Test
{
public:
  MockIRCBotController()
    : bot_(std::make_shared<MockIRCBot>(INFO))
    , controller_(bot_)
  {
  }

  MockIRCBot::Ptr bot_;
  IRCBotController controller_;
};

TEST_F(MockIRCBotController, TestSendPrivateMessage)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->data_received(PRIV_MSG + "!***");
}

TEST_F(MockIRCBotController, TestSendPrivateMessageStats)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->data_received(PRIV_MSG + STAT_MSG);
}

TEST_F(MockIRCBotController, TestSendPrivateMessageCompare)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->data_received(PRIV_MSG + COMP_MSG);
}

TEST_F(MockIRCBotController, TestSendPrivateMessageFib)
{
  EXPECT_CALL(*bot_, SendMessage(CHANNEL, testing::_));
  bot_->data_received(PRIV_MSG + FIB_MSG);
}

} // namespace irc_bot
} // namespace irc_bot_test
