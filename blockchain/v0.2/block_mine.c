#include "blockchain.h"
#include <limits.h>
#include <string.h>


/**
 * block_mine - mines a block
 * @block: block
 */
void block_mine(block_t *block)
{
	uint64_t i, orig_nonce;
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];

	if (!block)
	{
		fprintf(stderr, "block_mine: NULL parameter\n");
		return;
	}

	orig_nonce = block->info.nonce;

	for (i = 0; i < ULONG_MAX; i++)
	{
		block->info.nonce = i;

		block_hash(block, hash_buf);
		if (hash_matches_difficulty(hash_buf, block->info.difficulty))
		{
			memcpy(block->hash, hash_buf, SHA256_DIGEST_LENGTH);
			return;
		}
	}

	fprintf(stderr, "block_mine: %s %u\n", "no required nonce found ",
		block->info.difficulty);
	block->info.nonce = orig_nonce;
}

