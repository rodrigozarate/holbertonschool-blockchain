#include "blockchain.h"
#include <llist.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * blockchain_destroy - deletes an existing blockchain
 * @blockchain: pointer
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
	{
		fprintf(stderr, "blockchain_destroy: NULL parameter\n");
		return;
	}
	if (llist_destroy(blockchain->chain, 1, NULL) != 0)
	{
		fprintf(stderr, "blockchain_destroy: llist_destroy: %s\n",
			strE_LLIST(llist_errno));
		return;
	}

	free(blockchain);
}
