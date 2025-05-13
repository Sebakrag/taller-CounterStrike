#ifndef CONNECTION_INFO_H
#define CONNECTION_INFO_H

#include <string>

struct connection_info_t {
private:
    const std::string hostname;
    const std::string port;
    const std::string usr_name;
public:
    connection_info_t(const std::string& hostname, const std::string& port, const std::string& usr_name);

    const std::string get_hostname();
    const std::string get_port();
    const std::string get_usr_name();
};


#endif //CONNECTION_INFO_H
