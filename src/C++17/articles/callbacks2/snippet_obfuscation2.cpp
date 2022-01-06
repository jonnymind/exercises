struct Address {
    // dummy address...
};

class NetworkManager {
public:
    // ... Lots of other stuff...
    bool acceptConnection(const std::string& remote_address, int port);
    bool acceptConnectionFromNotifyThread(Address& remote);
};

class UserManager {
public:
    // ... Lots of other stuff...
    void onKeyPressed(char key);
};

class ConfigurationManager {
public:
    // ... Lots of other stuff...
    bool onConfigurationUpdated(std::istream& input);
    bool onConfigurationUpdatedFromNotifyThread(const std::string& filename);
};

void notifierThreadMain(NetworkManager& nm, UserManager& um, ConfigurationManager& cm)
{
    while(true) {
        // ... call the approriate method in the target object
    }
}

int main()
{
    NetworkManager network_manager;
    UserManager user_manager;
    ConfigurationManager config_manager;

    auto notifierThread = std::thread(notifierThreadMain, network_manager, user_manager, config_manager);
    // ... 
}

