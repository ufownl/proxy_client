#include "content_processor.h"

aes_processor::aes_processor(const std::string& key_src, int enc)
  : context()
  , ivec(16)
  , num(0)
  , enc(enc) {
    AES_set_encrypt_key(
        reinterpret_cast<const unsigned char*>(&key_src[0]),
          (key_src.size() <= 16 ? 16 :
           key_src.size() <= 24 ? 24 :
                                  32 ) * 8,
        &context);
}

byte_vec_ptr aes_processor::operator()(
  const byte_t* psrc, std::size_t size) {
    byte_vec_ptr pdata(new byte_vector(size));
    AES_cfb128_encrypt(
        psrc, &(*pdata)[0], size, &context, &ivec[0], &num, enc);
    return pdata;
}
