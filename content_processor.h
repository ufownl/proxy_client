#pragma once

#include "common.h"

struct encoder: content_processor {
    encoder(const std::string& key_src) {}
};

struct decoder: content_processor {
    decoder(const std::string& key_src) {}
};
