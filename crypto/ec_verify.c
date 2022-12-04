#include "hblk_crypto.h"
#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>


/**
 * ec_verify - verifies signature
 *
 * @key: pointer to EC_KEY structure
 * @msg: pointer
 * @msglen: size
 * @sig: pointer to signature
 * Return: 1 if ok
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
	      sig_t const *sig)
{
	const EC_GROUP *ec_group;

	if (!key || !msg || !sig)
	{
		fprintf(stderr, "ec_verify: NULL parameter(s)\n");
		return (0);
	}

	if (!EC_KEY_check_key(key))
	{
		fprintf(stderr, "ec_verify: EC_KEY_check_key failure\n");
		return (0);
	}

	ec_group = EC_KEY_get0_group(key);
	if (!ec_group || EC_GROUP_get_curve_name(ec_group) != EC_CURVE ||
	    ECDSA_size(key) != SIG_MAX_LEN)
	{
		fprintf(stderr,
			"ec_verify: expecting key with secp256k1 curve\n");
		return (0);
	}

	if (sig->len > SIG_MAX_LEN || sig->len == 0)
	{
		fprintf(stderr, sig->len ?
			"ec_verify: signature over max length\n" :
			"ec_verify: signature length of 0\n");
		return (0);
	}

	if (ECDSA_verify(0, msg, (int)msglen, sig->sig,
			 (int)sig->len, (EC_KEY *)key) == 0)
	{
		fprintf(stderr, "ec_verify: ECDSA_verify failure\n");
		return (0);
	}

	return (1);
}
