#include "hblk_crypto.h"
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


/**
 * bAxEC_POINT - EC_POINT public key array of bytes
 *
 * @ec_point: pointer to the EC_POINT
 * @ec_group: pointer to the EC_GROUP
 * @bn_ctx: pointer to the BN_CTX
 * @pub: address to store key
 * Return: pointer
 */
uint8_t *bAxEC_POINT(const EC_POINT *ec_point, const EC_GROUP *ec_group,
			       BN_CTX *bn_ctx, uint8_t pub[EC_PUB_LEN])
{
	char *pub_temp;
	int i, j;

	if (!ec_point || !ec_group || !bn_ctx || !pub)
	{
		fprintf(stderr, "bAxEC_POINT: NULL parameter(s)\n");
		return (NULL);
	}

	pub_temp = EC_POINT_point2hex(ec_group, ec_point,
				      POINT_CONVERSION_UNCOMPRESSED, bn_ctx);
	if (!pub_temp)
	{
		fprintf(stderr, "bAxEC_POINT: EC_POINT_point2hex failure\n");
		return (NULL);
	}

	memset(pub, 0, EC_PUB_LEN);

	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
	{
		if (sscanf(pub_temp + j, "%02X", (unsigned int *)(pub + i)) != 1)
		{
			perror("bAxEC_POINT: sscanf");
			errno = 0;
			return (NULL);
		}
	}

	OPENSSL_free(pub_temp);

	return (pub);
}


/**
 * ec_to_pub - extracts the public key from an EC_KEY
 * @key: pointer
 * @pub: address to store key
 * Return: pointer
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *ec_point;
	const EC_GROUP *ec_group;
	BN_CTX *bn_ctx;

	if (!key || !pub)
	{
		fprintf(stderr, "ec_to_pub: NULL parameter(s)\n");
		return (NULL);
	}

	ec_point = EC_KEY_get0_public_key(key);
	if (!ec_point)
	{
		fprintf(stderr, "ec_to_pub: EC_KEY_get0_public_key failure\n");
		return (NULL);
	}

	ec_group = EC_KEY_get0_group(key);
	if (!ec_group)
	{
		fprintf(stderr, "ec_to_pub: EC_KEY_get0_group failure\n");
		return (NULL);
	}

	bn_ctx = BN_CTX_new();
	if (!bn_ctx)
	{
		fprintf(stderr, "ec_to_pub: BN_CTX_new failure\n");
		return (NULL);
	}

	pub = bAxEC_POINT(ec_point, ec_group, bn_ctx, pub);

	BN_CTX_free(bn_ctx);
	return (pub);
}
