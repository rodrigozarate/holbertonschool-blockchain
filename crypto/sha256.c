#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <stdio.h>

/**
* sha256 - hash function
* @s: secuence of bytes to be hashed
* @len: number of bytes to hash in s
* @digest: hash
* Return: hash or NULL
*/
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH])
{
}
