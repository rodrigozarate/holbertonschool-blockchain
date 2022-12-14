#include "blockchain.h"
#include <stdio.h>
#include <llist.h>


/**
 * adjustDifficulty - sets difficulty rating
 * @blockchain: blockchain
 * @latest_blk: block
 * @difficulty: difficutly setting
 * Return: 0 on success, 1 on failure
 */
uint8_t adjustDifficulty(const blockchain_t *blockchain,
			 const block_t *latest_blk, uint32_t *difficulty)
{
	block_t *last_adj_blk;
	int block_ct;
	uint64_t expected_dt, actual_dt;

	if (!blockchain || !latest_blk || !difficulty)
	{
		fprintf(stderr, "adjustDifficulty: NULL parameter(s)\n");
		return (1);
	}

	block_ct = llist_size(blockchain->chain);
	if (block_ct == -1)
	{
		fprintf(stderr, "adjustDifficulty: llist_size: %s\n",
			strE_LLIST(llist_errno));
		return (1);
	}

	last_adj_blk = llist_get_node_at(blockchain->chain,
					 block_ct -
					 DIFFICULTY_ADJUSTMENT_INTERVAL);
	if (!last_adj_blk)
	{
		fprintf(stderr, "adjustDifficulty: llist_get_node_at: %s\n",
			strE_LLIST(llist_errno));
		return (1);
	}

	expected_dt = (latest_blk->info.index -
		       last_adj_blk->info.index) * BLOCK_GENERATION_INTERVAL;
	actual_dt = latest_blk->info.timestamp - last_adj_blk->info.timestamp;

	if (actual_dt < (expected_dt / 2))
		(*difficulty)++;

	if (*difficulty && actual_dt > (expected_dt * 2))
		(*difficulty)--;

	return (0);
}


/**
 * blockchain_difficulty - computes difficulty
 * @blockchain: blockchain
 * Return: difficulty
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *latest_blk;
	uint32_t difficulty;

	if (!blockchain)
	{
		fprintf(stderr, "blockchain_difficulty: NULL parameter\n");
		return (0);
	}

	latest_blk = llist_get_tail(blockchain->chain);
	if (!latest_blk)
	{
		fprintf(stderr, "blockchain_difficulty: llist_get_tail: %s\n",
			strE_LLIST(llist_errno));
		return (0);
	}

	difficulty = latest_blk->info.difficulty;

	if (latest_blk->info.index != 0 &&
	    latest_blk->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL == 0)
	{
		if (adjustDifficulty(blockchain, latest_blk, &difficulty) != 0)
			return (0);
	}

	return (difficulty);
}
