#ifndef logger_hpp
#define logger_hpp

#include <map>
#include <memory>
#include <string>

class Logger {
public:
    static Logger& instance();

    typedef enum {
        ERROR,
        INFO,
        DEBUG
    } t_log_level;
    
    void log(t_log_level level, const std::string& message);
    
    void setMinLevel(t_log_level level);
    void error(const std::string& message) { log(ERROR, message); }
    void info(const std::string& message) { log(INFO, message); }
    void debug(const std::string& message) { log(DEBUG, message); }
    
    class Channel {
    public:
        virtual ~Channel() = default;
        virtual void onMessage(t_log_level level, const std::string& message) = 0;
    };
    
    using ChannelToken = int;
    using ChannelPtr = std::shared_ptr<Channel>;
    ChannelToken addChannel(t_log_level minLevel, ChannelPtr channel);
    void setChannelLevel(ChannelToken token, t_log_level minLevel);
    void removeChannel(ChannelToken token);

private:
    struct ChannelSettings {
        t_log_level minLevel;
        ChannelPtr channel;
    };
    
    using ChannelMap = std::map<ChannelToken, ChannelSettings>;
    ChannelToken m_lastToken{0};
    t_log_level minLevel;
};

#ifndef LOG
#define LOG (Logger::instance())
#endif

#endif /* logger_hpp */
