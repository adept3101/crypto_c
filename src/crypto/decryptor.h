#ifndef DECRYPTOR_H
#define DECRYPTOR_H

// #include <stdlib.h>
#include <stddef.h>
#include "encryptor.h"
// encrypted_data encrypt(const unsigned char *text, const unsigned char *key);
// unsigned char *encode(const unsigned char data[], size_t size_dt);

typedef struct {
    unsigned char *plaintext;
    size_t plaintext_len;
} decrypted_data;

// unsigned char decrypt(const unsigned char *cipher, const unsigned char *key);
decrypted_data decrypt(encrypted_data *enc, const unsigned char *key);
unsigned char *decode(const unsigned char encode[], size_t size_data, size_t *decoded_size);

#endif // !DECRYPTOR_H
