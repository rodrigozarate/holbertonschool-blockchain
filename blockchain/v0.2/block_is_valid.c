#include "blockchain.h"
#include <stdio.h>
#include <string.h>


/**
 * verifyHashes - verifies hashes
 * @block: pointer
 * @prev_block: pointer
 * Return: 0 on success, or 1 on failure
 */
int verifyHashes(block_t const *block, block_t const *prev_block)
{
	uint8_t prev_hash_buf[SHA256_DIGEST_LENGTH],
		hash_buf[SHA256_DIGEST_LENGTH];

	if (!block_hash(prev_block, prev_hash_buf) ||
	    !block_hash(block, hash_buf))
	{
		fprintf(stderr, "block_is_valid: block_hash failure\n");
		return (1);
	}

	if (memcmp(prev_block->hash, prev_hash_buf,
		   SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "block_is_valid: invalid hash of prev_block\n");
		return (1);
	}

	if (memcmp(block->hash, hash_buf,
		   SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "block_is_valid: invalid hash of block\n");
		return (1);
	}

	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		fprintf(stderr, "block_is_valid: %s\n",
			"block difficulty not met by block hash");
		return (1);
	}

	if (memcmp(block->info.prev_hash, prev_hash_buf,
		   SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "block_is_valid: %s\n",
			"invalid hash of prev_block in block prev_hash");
		return (1);
	}

	return (0);
}


/**
 * block_is_valid - verifies a block in a blockchain
 * @block: pointer
 * @prev_block: pointer
 * Return: 0 on success, or 1 on failure
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	block_t genesis_model = GEN_BLK;

	if (!block)
	{
		fprintf(stderr, "block_is_valid: NULL block\n");
		return (1);
	}

	if (block->info.index == 0)
	{
		if (memcmp(block, &genesis_model, sizeof(block_t)) == 0)
			return (0);

		fprintf(stderr, "block_is_valid: %s\n",
			"block index 0, but not valid Genesis Block");
		return (1);
	}

	if (block->info.index != prev_block->info.index + 1)
	{
		fprintf(stderr, "block_is_valid: %s\n",
			"block index should be prev_block index + 1");
		return (1);
	}

	if (!prev_block)
	{
		fprintf(stderr, "block_is_valid: %s\n",
			"NULL prev_block, but not first block");
		return (1);
	}

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
	{
		fprintf(stderr, "block_is_valid: %s\n",
			"block->data.len over BLOCKCHAIN_DATA_MAX");
		return (1);
	}

	return (verifyHashes(block, prev_block));
}
