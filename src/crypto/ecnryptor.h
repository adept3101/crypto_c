#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  unsigned char *cipher_text;
  unsigned char *nonce;
} encrypted_data;

encrypted_data encrypt(const unsigned char *text, const unsigned char *key);
unsigned char *encode(const unsigned char data[], size_t size_dt);

#endif // !ENCRYPTOR_H
