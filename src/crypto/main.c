#include "ecnryptor.h"
#include <sodium/randombytes.h>

int main() {
  unsigned char key[crypto_aead_chacha20poly1305_ietf_KEYBYTES];
  randombytes_buf(key, sizeof(key));
  char *str = "hello";
  size_t size_str = strlen(str);
  encrypted_data obj = encrypt((const unsigned char *)str, key);
  printf("%s\n", obj.cipher_text);
  // size_t size = sizeof(obj.cipher_text);
  unsigned char *text = encode(obj.cipher_text, size_str);
  printf("%s\n", text);
}
