#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

/* int8_t */
#include <stdint.h>
/* size_t */
#include <stddef.h>
/* SHA256_DIGEST_LENGTH */
#include <openssl/sha.h>

uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

#endif /* HBLK_CRYPTO_H */
