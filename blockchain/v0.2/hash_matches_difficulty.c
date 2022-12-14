#include "blockchain.h"
#include <stdio.h>


/**
 * hash_matches_difficulty - checks whether a given hash match
 * @hash: hash to check
 * @difficulty: minimum difficulty the hash should match
 * Return: 1 if ok 0 on failure
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty)
{
	int8_t i, j;
	uint32_t diff = difficulty;

	if (!hash)
	{
		fprintf(stderr, "hash_matches_difficulty: NULL parameter\n");
		return (0);
	}

	if (difficulty > 256)
	{
		fprintf(stderr, "hash_matches_difficulty: %s\n",
			"difficulty out of range for 256 bit hash");
		return (0);
	}

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		for (j = 7; diff && j >= 0; diff--, j--)
		{
			if (hash[i] & 1 << j)
				return (0);
		}
	}

	return (1);
}
