#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * block_destroy - deletes an existing block
 * @block: pointer
 */
void block_destroy(block_t *block)
{
	if (!block)
	{
		fprintf(stderr, "block_destroy: NULL parameter\n");
		return;
	}

	free(block);
}
