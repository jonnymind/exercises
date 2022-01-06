std::queue<Connection> expiredConnectionAttempts;

void purgeExpiredConnection(Connection& connection) {
    // Do some necessary cleanup on connection data, and then...
    connection.recycle();   // Don't even attpemt memory release.
}

void onNetworkData(const std::vector<byte>& data) {
    // Do whatever is needed with data...

    // Since we're here, let's do a bit of house keeping.
    for(connection: expiredConnectionAttempts) {
        purgeExpiredConnection(connection);
    }
}
