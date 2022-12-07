#include "blockchain.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include "provided/endianness.h"
#include <llist.h>


/**
 * pathToWriteFD - validates a path
 * @path: path
 * Return: file descriptor upon success, or -1 upon failure
 */
int pathToWriteFD(char const *path)
{
	struct stat st;
	int fd;

	if (lstat(path, &st) == -1)
	{
		if (errno != ENOENT)
		{
			perror("pathToWriteFD: lstat");
			return (-1);
		}
	}
	else if (S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "pathToWriteFD: path is a directory\n");
		return (-1);
	}

	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, /* 0664 */
		  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
	{
		if (errno == ENOENT)
		{
			fprintf(stderr, "pathToWriteFD: %s\n",
				"some directory in path does not exist");
		}
		else
			perror("pathToWriteFD: open");
		return (-1);
	}

	return (fd);
}


/**
 * fheaderBlk - writes a serialized file header
 * @fd: file descriptor
 * @blockchain: pointer
 * Return: 0 on success, or 1 upon failure
 */
int fheaderBlk(int fd, const blockchain_t *blockchain)
{
	bc_file_hdr_t header = { HBLK_MAG, HBLK_VER, 0, 0 };
	int block_ct;

	if (!blockchain)
	{
		fprintf(stderr, "fheaderBlk: NULL parameter\n");
		return (1);
	}

	header.hblk_endian = _get_endianness();
	if (header.hblk_endian == 0)
	{
		fprintf(stderr,
			"fheaderBlk: _get_endianness failure\n");
		return (1);
	}

	block_ct = llist_size(blockchain->chain);
	if (block_ct == -1)
	{
		fprintf(stderr, "fheaderBlk: llist_size: %s\n",
			strE_LLIST(llist_errno));
		return (1);
	}
	header.hblk_blocks = (uint32_t)block_ct;

	if (write(fd, &header, sizeof(bc_file_hdr_t)) == -1)
	{
		perror("fheaderBlk: write");
		return (1);
	}

	return (0);
}


/**
 * writeBlocks - writes serialized blocks to a file
 * @fd: file descriptor
 * @blockchain: pointer
 * Return: 0 on success, or 1 upon failure
 */
int writeBlocks(int fd, const blockchain_t *blockchain)
{
	int i, block_ct;
	block_t *block = NULL;

	if (!blockchain)
	{
		fprintf(stderr, "writeBlocks: NULL parameter\n");
		return (1);
	}

	block_ct = llist_size(blockchain->chain);
	if (block_ct == -1)
	{
		fprintf(stderr, "writeBlocks: llist_size: %s\n",
			strE_LLIST(llist_errno));
		return (1);
	}

	for (i = 0; i < block_ct; i++)
	{
		block = (block_t *)llist_get_node_at(blockchain->chain, i);
		if (!block)
		{
			fprintf(stderr, "writeBlocks: llist_get_node_at: %s\n",
				strE_LLIST(llist_errno));
			return (1);
		}

		if (write(fd, &(block->info), sizeof(block_info_t)) == -1 ||
		    write(fd, &(block->data.len), sizeof(uint32_t)) == -1 ||
		    write(fd, &(block->data.buffer), block->data.len) == -1 ||
		    write(fd, &(block->hash), SHA256_DIGEST_LENGTH) == -1)
		{
			perror("writeBlocks: write");
			return (1);
		}
	}

	return (0);
}


/**
 * blockchain_serialize - serializes a blockchain into a file
 * @blockchain: pointer
 * @path: path to  file
 * Return: 0 upon success, or -1 upon failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd;

	if (!blockchain || !path)
	{
		fprintf(stderr, "blockchain_serialize: NULL parameter(s)\n");
		return (-1);
	}

	fd = pathToWriteFD(path);
	if (fd == -1)
		return (-1);

	if (fheaderBlk(fd, blockchain) != 0)
	{
		close(fd);
		return (-1);
	}

	if (writeBlocks(fd, blockchain) != 0)
	{
		close(fd);
		return (-1);
	}

	close(fd);
	return (0);
}
