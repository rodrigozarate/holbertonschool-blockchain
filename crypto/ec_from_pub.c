#include "hblk_crypto.h"
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <stdio.h>
#include <string.h>


/**
 * bAToEC_POINT - byte array of a public key to EC_POINT
 * @ec_group: pointer
 * @bn_ctx: pointer to structure 
 * @pub: uncompressed representation of the public key/point
 * Return: pointer
 */
EC_POINT *bAToEC_POINT(const EC_GROUP *ec_group, BN_CTX *bn_ctx,
			      const uint8_t pub[EC_PUB_LEN])
{
	EC_POINT *ec_point;
	char *pub_hex_str;
	int i, j, buf_len;

	if (!ec_group || !bn_ctx || !pub)
	{
		fprintf(stderr, "bAToEC_POINT: NULL parameter(s)\n");
		return (NULL);
	}

	ec_point = EC_POINT_new(ec_group);
	if (!ec_point)
	{
		fprintf(stderr, "bAToEC_POINT: EC_POINT_new failure\n");
		return (NULL);
	}

	buf_len = EC_PUB_LEN * 2;
	pub_hex_str = OPENSSL_malloc(sizeof(char) * buf_len);
	if (!pub_hex_str)
	{
		fprintf(stderr, "bAToEC_POINT: OPENSSL_malloc failure\n");
		return (NULL);
	}

	memset(pub_hex_str, 0, buf_len);
	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
		sprintf(pub_hex_str + j, "%02X", (unsigned int)(pub[i]));

	ec_point = EC_POINT_hex2point(ec_group, pub_hex_str, ec_point, bn_ctx);
	if (!ec_point)
	{
		fprintf(stderr, "bAToEC_POINT: EC_KEY_get0_group failure\n");
		return (NULL);
	}

	OPENSSL_free(pub_hex_str);
	return (ec_point);
}


/**
 * ec_from_pub - creates an EC_KEY structure
 * @pub: public key
 * Return: pointer
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *ec_key;
	const EC_GROUP *ec_group;
	BN_CTX *bn_ctx;
	EC_POINT *ec_point;

	if (!pub)
	{
		fprintf(stderr, "ec_from_pub: NULL parameter\n");
		return (NULL);
	}
	ec_key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!ec_key)
	{
		fprintf(stderr,
			"ec_from_pub: EC_KEY_new_by_curve_name failure\n");
		return (NULL);
	}
	ec_group = EC_KEY_get0_group(ec_key);
	if (!ec_group)
	{
		fprintf(stderr, "ec_from_pub: EC_KEY_get0_group failure\n");
		return (NULL);
	}

	bn_ctx = BN_CTX_new();
	if (!bn_ctx)
	{
		fprintf(stderr, "ec_from_pub: BN_CTX_new failure\n");
		return (NULL);
	}
	ec_point = bAToEC_POINT(ec_group, bn_ctx, pub);
	if (EC_KEY_set_public_key(ec_key, ec_point) == 0)
	{
		fprintf(stderr, "ec_from_pub: EC_KEY_set_public_key failure\n");
		return (NULL);
	}
	BN_CTX_free(bn_ctx);
	EC_POINT_free(ec_point);

	return (ec_key);
}
