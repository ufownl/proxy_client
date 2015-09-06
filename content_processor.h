#pragma once

#include "common.h"
#include <openssl/aes.h>

class aes_processor: public content_processor {
protected:
    AES_KEY context;
    byte_vector ivec;
    int num;
    int enc;

    aes_processor(const std::string& key_src, int enc);

public:
    byte_vec_ptr operator()(const byte_t* psrc, std::size_t size);
};

struct encoder: aes_processor {
    encoder(const std::string& key_src)
        : aes_processor(key_src, AES_ENCRYPT) {}
};

struct decoder: aes_processor {
    decoder(const std::string& key_src)
        : aes_processor(key_src, AES_DECRYPT) {}
};
