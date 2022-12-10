#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


/**
 * block_create - creates and initializes a block structure
 *
 * @prev: pointer
 * @data: pointer
 * @data_len: size
 * Return: new block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len)
{
	block_t *new_blk;

	if (!prev || !data)
	{
		fprintf(stderr, "block_create: NULL parameter(s)\n");
		return (NULL);
	}

	new_blk = calloc(1, sizeof(block_t));
	if (!new_blk)
	{
		fprintf(stderr, "block_create: calloc failure\n");
		return (NULL);
	}

	new_blk->info.index = prev->info.index + 1;
	new_blk->info.timestamp = (uint64_t)time(NULL);
	memcpy(new_blk->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	new_blk->data.len = data_len < BLOCKCHAIN_DATA_MAX ?
		data_len : BLOCKCHAIN_DATA_MAX;
	memcpy(new_blk->data.buffer, data, new_blk->data.len);

	return (new_blk);
}
