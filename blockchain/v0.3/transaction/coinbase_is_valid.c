#include "transaction.h"
#include <stdio.h>
#include <string.h>


/**
 * validCBInput - checks whether a coinbase transaction input is valid
 * @cb_input: points to the coinbase transaction input to verify
 * @block_index: index of the block the coinbase transaction will belong to
 * Return: 1 if the coinbase input is valid, and 0 otherwise
 */
static int validCBInput(tx_in_t *cb_input, uint32_t block_index)
{
	int i, end, zeroed = 1;
	uint8_t *hash;

	if (!cb_input)
	{
		fprintf(stderr, "validCBInput: NULL parameter\n");
		return (0);
	}

	/* cb_input->block_hash and ->tx_id */
	hash = cb_input->block_hash;
	end = SHA256_DIGEST_LENGTH * 2;
	for (i = 0; i < end; i++)
		zeroed = (hash[i] == 0);

	if (memcmp(&(cb_input->tx_out_hash), &(block_index),
		   sizeof(uint32_t)) != 0)
	{
		fprintf(stderr, "coinbase_is_valid: first %lu bytes %s\n",
			sizeof(uint32_t),
			"of input tx_out_hash do not match block_index");
		return (0);
	}

	/* cb_input->tx_out_hash after matching block_index */
	hash = cb_input->tx_out_hash + sizeof(uint32_t);
	end = SHA256_DIGEST_LENGTH - sizeof(uint32_t);
	for (i = 0; i < end; i++)
		zeroed = (hash[i] == 0);

	if (!zeroed)
	{
		fprintf(stderr, "coinbase_is_valid: input block_hash, %s\n",
			"tx_id, and tx_out_hash not all properly zeroed");
		return (0);
	}
	return (1);
}


/**
 * coinbase_is_valid - checks whether a coinbase transaction is valid
 * @coinbase: points to the coinbase transaction to verify
 * @block_index: index of the block the coinbase transaction
 * Return: 1 if the coinbase transaction is valid, and 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t cb_hash[SHA256_DIGEST_LENGTH];
	tx_in_t *cb_input;
	tx_out_t *cb_output;

	if (!coinbase)
	{
		fprintf(stderr, "coinbase_is_valid: NULL parameter\n");
		return (0);
	}

	transaction_hash(coinbase, cb_hash);
	if (memcmp(coinbase->id, cb_hash, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "coinbase_is_valid: %s\n",
			"hash of transaction does not match id");
		return (0);
	}

	if (llist_size(coinbase->inputs) != 1 ||
	    llist_size(coinbase->outputs) != 1)
	{
		fprintf(stderr, "coinbase_is_valid: %s\n",
			"coinbase must have 1 input and 1 output");
		return (0);
	}
	cb_input = llist_get_head(coinbase->inputs);
	cb_output = llist_get_head(coinbase->outputs);
	if (!cb_input || !cb_output)
	{
		fprintf(stderr, "coinbase_is_valid: %s\n",
			"llist_get_head failure");
		return (0);
	}
	if (cb_output->amount != COINBASE_AMOUNT)
	{
		fprintf(stderr, "coinbase_is_valid: %s\n",
			"output amount does not match expected value");
		return (0);
	}
	return (validCBInput(cb_input, block_index));
}
