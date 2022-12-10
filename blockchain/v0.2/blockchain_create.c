#include "blockchain.h"
#include <llist.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * strE_LLIST - returns error description
 * @code: llist error code
 * Return: pointer to string
 */
char *strE_LLIST(E_LLIST code)
{
	switch (code)
	{
	case LLIST_SUCCESS:
		return ("success");
	case LLIST_NODE_NOT_FOUND:
		return ("node not found");
	case LLIST_MULTITHREAD_ISSUE:
		return ("multithreading issue");
	case LLIST_NULL_ARGUMENT:
		return ("NULL parameter(s)");
	case LLIST_OUT_OF_RANGE:
		return ("out of range");
	case LLIST_MALLOC_ERROR:
		return ("malloc error");
	default:
		return ("(unknown error code)");
	}
}


/**
 * blockchain_create - creates and initializes a blockchain structure
 * Return: pointer
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = malloc(sizeof(blockchain_t));
	genesis = calloc(1, sizeof(block_t));
	if (!blockchain || !genesis)
	{
		fprintf(stderr, "blockchain_create: malloc/calloc failure\n");
		return (NULL);
	}

	genesis->info.timestamp = GEN_BLK_TS;
	genesis->data.len = GEN_BLK_DT_LEN;
	memcpy(genesis->data.buffer, GEN_BLK_DT_BUF, GEN_BLK_DT_LEN);
	memcpy(genesis->hash, GEN_BLK_HSH, SHA256_DIGEST_LENGTH);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!(blockchain->chain))
	{
		free(blockchain);
		free(genesis);
		fprintf(stderr, "blockchain_create: llist_create: %s\n",
			strE_LLIST(llist_errno));
		return (NULL);
	}

	if (llist_add_node(blockchain->chain, (llist_node_t)genesis,
		ADD_NODE_FRONT) != 0)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		fprintf(stderr, "blockchain_create: llist_add_node: %s\n",
				strE_LLIST(llist_errno));
		return (NULL);
	}

	return (blockchain);
}
