#include "logger_med.hpp"

#include <gtest/gtest.h>
#include <tuple>

struct TestChannel: public Logger::Channel {
    int m_messageCount{0};

    virtual ~TestChannel() = default;
    virtual void onMessage(Logger::t_log_level level, const std::string& message) {
        m_messageCount++;
    }
};

using BaseTestData = std::tuple<Logger::t_log_level, Logger::t_log_level, int>;
struct BaseTest: testing::TestWithParam<BaseTestData> {};

TEST_P(BaseTest, properFilter) {
    // Should receive a message if logLevel <= msgLevel (lower level = higher priority)
    auto [logLevel, msgLevel, expect] = GetParam();
    auto channel = std::make_shared<TestChannel>();
    LOG.addChannel(Logger::DEBUG, channel);
    LOG.setLogLevel(logLevel);
    LOG.log(msgLevel, "A Message");
    ASSERT_EQ(channel->m_messageCount, expect);
}

TEST_P(BaseTest, channelLevelFilter) {
    // Should receive a message if logLevel <= msgLevel (lower level = higher priority)
    auto [logLevel, msgLevel, expect] = GetParam();
    auto channelBase = std::make_shared<TestChannel>();
    auto channelFiltered = std::make_shared<TestChannel>();
    LOG.addChannel(Logger::DEBUG, channelBase);
    LOG.addChannel(logLevel, channelFiltered);
    LOG.setLogLevel(Logger::DEBUG);
    LOG.log(msgLevel, "A Message");
    ASSERT_EQ(channelBase->m_messageCount, 1);
    ASSERT_EQ(channelFiltered->m_messageCount, expect);
}

INSTANTIATE_TEST_SUITE_P(SimpleLogs, BaseTest, 
    testing::Values( 
        BaseTestData{Logger::DEBUG, Logger::DEBUG, 1},
        BaseTestData{Logger::DEBUG, Logger::INFO, 1},
        BaseTestData{Logger::DEBUG, Logger::ERROR, 1},
        BaseTestData{Logger::INFO, Logger::INFO, 1},
        BaseTestData{Logger::INFO, Logger::ERROR, 1},
        BaseTestData{Logger::ERROR, Logger::ERROR, 1},
        BaseTestData{Logger::INFO, Logger::DEBUG, 0},
        BaseTestData{Logger::ERROR, Logger::INFO, 0},
        BaseTestData{Logger::ERROR, Logger::DEBUG, 0}
));

