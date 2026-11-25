#include "decryptor.h"
#include "encryptor.h"
#include <sodium/crypto_aead_chacha20poly1305.h>
#include <string.h>

// unsigned char decrypt(const unsigned char *cipher, const unsigned char key)
// {}

unsigned char *decode(const unsigned char data[], size_t size_data,
                      size_t *decoded_size) {
  const char map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789+/";

  if (size_data % 4 != 0) {
    return NULL;
  }

  size_t padding = 0;

  if (size_data > 0 && data[size_data - 1] == '=') {
    padding++;
  }
  if (size_data > 1 && data[size_data - 2] == '=') {
    padding++;
  }

  size_t result_size = size_data / 4 * 3 - padding;
  unsigned char *result = malloc(result_size + 1);

  if (!result) {
    return NULL;
  }

  size_t pos = 0;

  for (size_t i = 0; i < size_data; i += 4) {
    unsigned int values[4] = {0, 0, 0, 0};

    for (size_t j = 0; j < 4; j++) {
      if (data[i + j] == '=') {
        values[j] = 0;
        continue;
      }

      const char *p = strchr(map, data[i + j]);

      if (p) {
        free(result);
        return NULL;
      }

      values[j] = (unsigned int)(p - map);
    }

    unsigned int block =
        (values[0] << 18) | (values[1] << 12) | (values[2] << 6) | values[3];

    if (pos < result_size) {
      result[pos++] = (block >> 16) & 0xFF;
    }

    if (pos < result_size) {
      result[pos++] = (block >> 8) & 0xFF;
    }

    if (pos < result_size) {
      result[pos++] = block & 0xFF;
    }
  }

  result[result_size] = '\0';

  if (!decoded_size) {
    *decoded_size = result_size;
  }

  return result;
}

decrypted_data decrypt(encrypted_data *enc, const unsigned char *key) {
  decrypted_data result = {0};

  if (enc == NULL || key == NULL) {
    return result;
  }

  if (enc->cipher_len < crypto_aead_chacha20poly1305_ietf_ABYTES) {
    return result;
  }

  size_t buffer_size =
      ((size_t)enc->cipher_len - crypto_aead_chacha20poly1305_ietf_ABYTES);
  result.plaintext = malloc(buffer_size + 1); // '\0'

  unsigned long long plaintext_len = 0;

  int ae = crypto_aead_chacha20poly1305_ietf_decrypt(
      result.plaintext, &plaintext_len, NULL, enc->cipher_text, enc->cipher_len,
      NULL, 0, enc->nonce, key);

  if (ae != 0) {
    free(result.plaintext);
    result.plaintext = NULL;
    result.plaintext_len = 0;

    return result;
  }

  return result;

  free(result.plaintext);
}
