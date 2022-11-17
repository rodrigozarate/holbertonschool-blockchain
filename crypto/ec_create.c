#include "hblk_crypto.h"
#include <openssl/ec.h>
#include <stdio.h>


/**
* ec_create - create eliptic curve key pair
* Return: pointer or NULL
*/
EC_KEY *ec_create(void)
{
	EC_KEY *ec_key;
	EC_GROUP *ec_group;

	ec_key = EC_KEY_new();

	if (!ec_key)
	{
		fprintf(stderr, "ec_create: EC_KEY_new error\n");
		return (NULL);
	}

	ec_group = EC_GROUP_new_by_curve_name(EC_CURVE);
	EC_KEY_set_group(ec_key, ec_group);
	EC_KEY_generate_key(ec_key);
	EC_GROUP_free(ec_group);

	return (ec_key);
}
