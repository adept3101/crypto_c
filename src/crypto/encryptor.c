#include "ecnryptor.h"
#include <sodium/crypto_aead_chacha20poly1305.h>
#include <sodium/randombytes.h>

encrypted_data encrypt(const unsigned char *text, const unsigned char *key) {
  encrypted_data obj = {0};

  obj.nonce = malloc(crypto_aead_chacha20poly1305_ietf_NPUBBYTES);

  // size_t size_nnc = sizeof(obj.nonce) / sizeof(unsigned char);
  // size_t size_nnc = crypto_aead_chacha20poly1305_ietf_NPUBBYTES;
  size_t size_txt = strlen((const char *)text);

  randombytes_buf(obj.nonce, crypto_aead_chacha20poly1305_ietf_NPUBBYTES);

  obj.cipher_text = malloc(size_txt + crypto_aead_chacha20poly1305_ietf_ABYTES);
  unsigned long long cipher_len;

  int ae = crypto_aead_chacha20poly1305_ietf_encrypt(
      obj.cipher_text, &cipher_len, text, size_txt, NULL, 0, NULL, obj.nonce,
      key);

  if (ae != 0) {
    free(obj.nonce);
    free(obj.cipher_text);

    obj.nonce = NULL;
    obj.cipher_text = NULL;

    return obj;
  }

  free(obj.nonce);
  free(obj.cipher_text);

  return obj;
}

unsigned char *encode(const unsigned char data[], size_t size_dt) {
  const char map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789=";

  size_t result_size = 4 * ((size_dt + 2) / 3);

  unsigned char *result = malloc(result_size + 1);
  // size_t size_dt = sizeof(data[0]) / sizeof(unsigned char);
  // size_t size_dt = sizeof(data);
  size_t pos = 0;

  for (int i = 0; i < size_dt; i += 3) {
    size_t remaining = size_dt - i;

    unsigned char a = data[i];
    unsigned char b = remaining > 1 ? data[i + 1] : 0;
    unsigned char c = remaining > 2 ? data[i + 2] : 0;

    result[pos++] = map[a >> 2];
    result[pos++] = map[((a & 0x03) << 4) | (b >> 4)];
    result[pos++] = remaining > 1 ? map[((b & 0x0f) << 2) | (c >> 6)] : '=';
    result[pos++] = remaining > 2 ? map[c & 0x3f] : '=';
  }
  result[pos] = '\0';

  return result;
}
