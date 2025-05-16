#include "connection_info.h"

connection_info_t::connection_info_t(const std::string& hostname, const std::string& port,
                                     const std::string& usr_name):
        hostname(hostname), port(port), usr_name(usr_name) {}

const std::string connection_info_t::get_hostname() { return hostname; }

const std::string connection_info_t::get_port() { return port; }

const std::string connection_info_t::get_usr_name() { return usr_name; }
