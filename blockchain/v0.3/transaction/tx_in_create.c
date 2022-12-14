#include "transaction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * tx_in_create - allocates and initializes a transaction input structure
 * @unspent: points to the unspent transaction output
 * Return: pointer
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *new_tx_in;

	if (!unspent)
	{
		fprintf(stderr, "tx_in_create: NULL parameter\n");
		return (NULL);
	}

	new_tx_in = malloc(sizeof(tx_in_t));
	if (!new_tx_in)
	{
		fprintf(stderr, "tx_in_create: malloc failure\n");
		return (NULL);
	}

	memcpy(&(new_tx_in->block_hash), unspent->block_hash,
	       SHA256_DIGEST_LENGTH);
	memcpy(&(new_tx_in->tx_id), unspent->tx_id,
	       SHA256_DIGEST_LENGTH);
	memcpy(&(new_tx_in->tx_out_hash), unspent->out.hash,
	       SHA256_DIGEST_LENGTH);

	bzero(&(new_tx_in->sig), sizeof(sig_t));

	return (new_tx_in);

}
