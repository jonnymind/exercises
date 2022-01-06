struct Address {
    // dummy address...
};

using OnNetworkHandler = std::function<void(Address)>;
using OnKeyboardHandler = std::function<void(char))>;
using OnFileChangeHandler = std::function<void(std::string))>;

void notifierThreadMain(OnNetworkHandler nh, OnKeyboardHandler kh, OnFileChangeHandler fh)
{
    while(true) {
        // ... call nh, kh, fh appropriately
    }
}

class NetworkManager {
public:
    // ... Lots of other stuff...
    bool acceptConnection(const std::string& remote_address, int port);
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
};

int main()
{
    NetworkManager network_manager;
    UserManager user_manager;
    ConfigurationManager config_manager;

    auto notifierThread = std::thread(notifierThreadMain,
        [&network_manager](Address address) {
            auto [host, port] = convertAddress(address);
            if(!network_manager.acceptConnection(host, port)) {
                // manage some error
            }
        },
        [&user_manager](char key) {user_manager.onKeyPressed(key);},
        [&config_manager](std::string filename) {
            auto input = std::ifstream(filename);
            // ...eventually manage the error...
            if(! config_manager.onConfigurationUpdated(input)) {
                // Handle problems.
            }
        }
    );
    
    // ... 
}

