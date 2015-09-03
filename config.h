#pragma once

#include "common.h"

#define CFG_FILE_NAME "cfg"
#define LOCAL_PORT    "local_port"
#define REMOTE_HOST   "remote_host"
#define REMOTE_PORT   "remote_port"
#define KEY_SRC       "key_src"

struct config {
    std::string local_port;
    std::string remote_host;
    std::string remote_port;
    std::string key_src;
};

config get_cfg();
