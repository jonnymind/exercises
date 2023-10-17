#ifndef logger_min_hpp
#define logger_min_hpp

#include <memory>
#include <string>
#include <list>

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    typedef enum {
        ERROR,
        INFO,
        DEBUG
    } t_log_level;
    
    class Channel {
    public:
        virtual ~Channel() = default;
        virtual void onMessage(t_log_level level, const std::string& message) = 0;
    };
    
    using ChannelPtr = std::shared_ptr<Channel>;

private:
    t_log_level m_logLevel{DEBUG};

    struct ChannelInfo {
        t_log_level m_logLevel{DEBUG};
        ChannelPtr m_channel;
    };
    std::list<ChannelInfo> m_channels;

public:
    void setLogLevel(t_log_level level) {
        m_logLevel = level;
    }

    void addChannel(t_log_level level, ChannelPtr channel) {
        m_channels.push_back(ChannelInfo{level, channel});
    }

    void log(t_log_level level, const std::string& message) {
        if(level > m_logLevel) {
            return;
        }

        for(auto& cinfo: m_channels) {
            if(level <= cinfo.m_logLevel) {
                cinfo.m_channel->onMessage(level, message);
            }
        }
    }
};

#ifndef LOG
#define LOG (Logger::instance())
#endif

#endif /* logger_min_hpp */
