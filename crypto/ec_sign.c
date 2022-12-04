#include "hblk_crypto.h"
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <stdio.h>
#include <string.h>


/**
 * ec_sign - signs with EC_KEY private key
 * @key: pointer to structure
 * @msg: pointer
 * @msglen: message lenght
 * @sig: address
 * Return: pointer
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		 sig_t *sig)
{
	unsigned int sig_len;

	if (!key || !msg || !sig)
	{
		fprintf(stderr, "ec_sign: NULL parameter(s)\n");
		return (NULL);
	}

	if (!EC_KEY_check_key(key))
	{
		fprintf(stderr, "ec_sign: key verification failed\n");
		return (NULL);
	}

	if (ECDSA_size(key) != SIG_MAX_LEN)
	{
		fprintf(stderr,
			"ec_sign: invalid ECDSA size for key, expected %i\n",
			SIG_MAX_LEN);
		return (NULL);
	}

	memset(sig->sig, 0, SIG_MAX_LEN);

	if (ECDSA_sign(0, msg, msglen, sig->sig,
		       &sig_len, (EC_KEY *)key) == 0)
	{
		sig->len = 0;
		fprintf(stderr, "ec_sign: ECDSA_sign failure\n");
		return (NULL);
	}

	sig->len = sig_len;
	return (sig->sig);
}
