#include "transaction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * unspent_tx_out_create - allocates and initializes an unspent transaction
 *   output structure
 *
 * @block_hash: the hash of the block
 * @tx_id: the hash of a transaction in the block block_hash
 * @out: transaction output
 *
 * Return: pointer
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *new_unspent_tx_out;

	if (!block_hash || !tx_id || !out)
	{
		fprintf(stderr, "unspent_tx_out_create: NULL parameter(s)\n");
		return (NULL);
	}

	new_unspent_tx_out = malloc(sizeof(unspent_tx_out_t));
	if (!new_unspent_tx_out)
	{
		fprintf(stderr, "unspent_tx_out_create: malloc failure\n");
		return (NULL);
	}

	memcpy(&(new_unspent_tx_out->block_hash), block_hash,
	       SHA256_DIGEST_LENGTH);
	memcpy(&(new_unspent_tx_out->tx_id), tx_id,
	       SHA256_DIGEST_LENGTH);
	memcpy(&(new_unspent_tx_out->out), out, sizeof(tx_out_t));

	return (new_unspent_tx_out);
}
