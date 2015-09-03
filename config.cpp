#include "config.h"

config get_cfg() {
    config cfg;
    std::ifstream file(CFG_FILE_NAME);
    if (!file.is_open()) throw std::runtime_error("Cannot open config file.");
    std::string line;
    while (std::getline(file, line)) {
        std::string::size_type pos = line.find('=');
        if (pos == line.npos) continue;
        std::string name = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        std::string& item = name == LOCAL_PORT  ? cfg.local_port  :
                            name == REMOTE_HOST ? cfg.remote_host :
                            name == REMOTE_PORT ? cfg.remote_port :
                            name == KEY_SRC     ? cfg.key_src     :
                                                  value;
        item = value;
    }
    return cfg;
}
