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
	SHA256_CTX ctx;

	if (!digest)
	{
		fprintf(stderr, "sha256: Null digest given\n");
		return (NULL);
	}

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, (const void *)s, (unsigned long)len);
	SHA256_Final((unsigned char *)digest, &ctx);

	return (digest);
}
