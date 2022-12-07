#include "blockchain.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include "provided/endianness.h"
#include <llist.h>
#include <string.h>


/**
 * pathToReadFD - validates a path
 * @path: path to file
 * Return: file descriptor upon success, or -1 upon failure
 */
int pathToReadFD(char const *path)
{
	struct stat st;
	int fd;

	if (lstat(path, &st) == -1)
	{
		fprintf(stderr, "pathToReadFD: lstat '%s': %s\n", path,
			strerror(errno));
		return (-1);
	}

	if ((size_t)(st.st_size) < sizeof(bc_file_hdr_t) + GEN_BLK_SERIAL_SZ)
	{
		fprintf(stderr, "pathToReadFD: %s\n",
			"file too small to contain header and Genesis Block");
		return (-1);
	}

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "pathToReadFD: open '%s': %s\n", path,
			strerror(errno));
	}

	return (fd);
}


/**
 * readBlkchnFileHdr - reads file header
 * @fd: file descriptor already open for reading
 * @local_endianness: 1 for little endian, 2 for big endian
 * @file_endianness: 1 for little endian, 2 for big endian
 * @block_ct: stores count of blocks
 * Return: 0 on success, or 1 upon failure
 */
int readBlkchnFileHdr(int fd, uint8_t local_endianness,
		      uint8_t *file_endianness, uint32_t *block_ct)
{
	bc_file_hdr_t header;

	if (!file_endianness || !block_ct)
	{
		fprintf(stderr, "readBlkchnFileHdr: NULL parameter(s)\n");
		return (1);
	}
	if (local_endianness != 1 && local_endianness != 2)
	{
		fprintf(stderr,
			"readBlkchnFileHdr: invalid local_endianness\n");
		return (1);
	}
	if (read(fd, &header, sizeof(bc_file_hdr_t)) == -1)
	{
		perror("readBlkchnFileHdr: read");
		return (1);
	}
	if (strncmp((char *)header.hblk_magic, HBLK_MAG, HBLK_MAG_LEN) != 0)
	{
		fprintf(stderr,
			"readBlkchnFileHdr: invalid magic number\n");
		return (1);
	}
	if (strncmp((char *)header.hblk_version, HBLK_VER, HBLK_VER_LEN) != 0)
	{
		fprintf(stderr, "readBlkchnFileHdr: %s\n",
			"serialized with incompatible version number");
		return (1);
	}

	*file_endianness = header.hblk_endian;
	if (*file_endianness != local_endianness)
#ifdef __GNUC__
		header.hblk_blocks = __builtin_bswap32(header.hblk_blocks);
#else
		_swap_endian(&(header.hblk_blocks), 32);
#endif
	*block_ct = header.hblk_blocks;
	return (0);
}


/**
 * bswapBlock - reverses endianness
 * @block: pointer
 */
void bswapBlock(block_t *block)
{
	if (!block)
	{
		fprintf(stderr, "bswapBlock: NULL parameter\n");
		return;
	}

#ifdef __GNUC__ /* compiled with gcc, can use gcc builtins for fast assembly */
	block->info.index      = __builtin_bswap32(block->info.index);
	block->info.difficulty = __builtin_bswap32(block->info.difficulty);
	block->info.timestamp  = __builtin_bswap64(block->info.timestamp);
	block->info.nonce      = __builtin_bswap64(block->info.nonce);
	block->data.len        = __builtin_bswap32(block->data.len);
#else /* use function to manually byte swap */
	_swap_endian(&(block->info.index), 32);
	_swap_endian(&(block->info.difficulty), 32);
	_swap_endian(&(block->info.timestamp), 64);
	_swap_endian(&(block->info.nonce), 64);
	_swap_endian(&(block->data.len), 32);
#endif
}


/**
 * readBlocks - reads serialized blocks
 * @fd: file descriptor
 * @blockchain: pointer
 * @local_endianness: 1 for little endian, 2 for big endian
 * @file_endianness: 1 for little endian, 2 for big endian
 * @block_ct: count of blocks in blockchain
 * Return: 0 on success, or 1 upon failure
 */
int readBlocks(int fd, const blockchain_t *blockchain,
		uint8_t local_endianness, uint8_t file_endianness,
		uint32_t block_ct)
{
	uint32_t i;
	block_t *block;

	if (!blockchain)
	{
		fprintf(stderr, "readBlocks: NULL parameter\n");
		return (1);
	}
	if (!llist_is_empty(blockchain->chain))
	{
		fprintf(stderr, "readBlocks: target blockchain not empty\n");
		return (1);
	}
	for (i = 0; i < block_ct; i++)
	{ /* assumes header has already been read from fd */
		block = calloc(1, sizeof(block_t));
		if (!block)
		{
			fprintf(stderr, "readBlocks: calloc failure\n");
			return (1);
		}
		if (read(fd, &(block->info), sizeof(block_info_t)) == -1 ||
		    read(fd, &(block->data.len), sizeof(uint32_t)) == -1 ||
		    read(fd, &(block->data.buffer), block->data.len) == -1 ||
		    read(fd, &(block->hash), SHA256_DIGEST_LENGTH) == -1)
		{
			perror("readBlocks: read");
			return (1);
		}
		if (local_endianness != file_endianness)
			bswapBlock(block);
		if (llist_add_node(blockchain->chain, (llist_node_t)block,
				   ADD_NODE_REAR) != 0)
		{
			fprintf(stderr, "readBlocks: llist_add_node: %s\n",
				strE_LLIST(llist_errno));
			return (1);
		}
	}
	return (0);
}


/**
 * blockchain_deserialize - deserializes a blockchain
 * @path: full path
 * Return: pointer
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd;
	uint8_t local_endianness, file_endianness;
	uint32_t block_ct;
	blockchain_t *blockchain;
	block_t *genesis;

	if (!path)
	{
		fprintf(stderr, "blockchain_deserialize: NULL parameter\n");
		return (NULL);
	}

	fd = pathToReadFD(path);
	if (fd == -1)
		return (NULL);
	blockchain = blockchain_create();
	if (!blockchain)
	{
		close(fd);
		return (NULL);
	}

	genesis = (block_t *)llist_pop(blockchain->chain);
	if (genesis)
		free(genesis);
	local_endianness = _get_endianness();

	if (readBlkchnFileHdr(fd, local_endianness, &file_endianness,
			      &block_ct) != 0 ||
	    readBlocks(fd, blockchain, local_endianness, file_endianness,
		       block_ct) != 0)
	{
		close(fd);
		blockchain_destroy(blockchain);
		return (NULL);
	}

	close(fd);
	return (blockchain);
}
