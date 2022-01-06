// Unchanged code omitted for brevity...

struct NotificationAdapter {
    NotificationAdapter(NetworkManager& nm, UserManager& um, ConfigurationManager& cm):
        network_manager_{nm}, user_manager_{um}, config_manager_{cm}
    {}

    void onNetworkHandler(Address address) {
        auto [host, port] = convertAddress(address);
        if(!network_manager_.acceptConnection(host, port)) {
            // manage some error
        }
    }

    void onKeyboardHandler(char c) {user_manager_.onKeyPressed(c);}

    void onConfigurationUpdated(std::string filename) {
        auto input = std::ifstream(filename);
        // ...eventually manage the error...
        if(! config_manager.onConfigurationUpdated(input)) {
            // Handle problems.
        }
    }

private:
    // Notice the manager objects held by reference.
    NetworkManager& network_manager_;
    UserManager& user_manager_;
    ConfigurationManager& config_manager_;
}

int main()
{
    NetworkManager network_manager;
    UserManager user_manager;
    ConfigurationManager config_manager;

    NotificationAdapter adapter(network_manager, user_manager, config_manager);

    auto notifierThread = std::thread( notifierThreadMain,
        [&adapter](Address address) { adapter.onNetworkHandler(address); },
        [&adapter](char key) { adapter.onKeyPressed(key); },
        [&adapter](std::string filename) { adapter.onConfigurationUpdated(filename); }
    );
    
    // ... 
}

