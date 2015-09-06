This branch adds to the master branch the capability of en/decrypting the message forwarded between the local client and the remote server. The cryptographic algorithm used is Advanced Encryption Standard (AES) working at the Cipher Feedback (CFB) mode. OpenSSL’s implementation of the algorithm is used. Consequently, this branch has an additional dependency on the OpenSSL library.

The value of the "key_src" entry in the config file specifies the key for AES. If the value is empty or the entire "key_src" entry is not present in the config file, no encryption will be performed.
