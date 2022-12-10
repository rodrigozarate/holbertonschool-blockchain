#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <llist.h>
#include <stdint.h>
#include "../../crypto/hblk_crypto.h"


#define BLOCKCHAIN_DATA_MAX 1024

#define HBLK_MAG            "HBLK"
#define HBLK_MAG_LEN        4
#define HBLK_VER            "0.1"
#define HBLK_VER_LEN        3

#define GEN_BLK_TS          1537578000
#define GEN_BLK_DT_BUF      "Holberton School"
#define GEN_BLK_DT_LEN      16
#define GEN_BLK_HSH \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define GEN_BLK { \
	{ \
		0, \
		0, \
		1537578000, \
		0, \
		{0} \
	}, \
	{ \
		"Holberton School", \
		16 \
	}, \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" \
}

#define GEN_BLK_SERIAL_SZ 108


/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t  prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data
 * @buffer: Data buffer
 * @len: Data size (in bytes)
 */
typedef struct block_data_s
{
	int8_t   buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

/**
 * struct block_s - Block structure
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	uint8_t      hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct bc_file_hdr_s - blockchain file header structure
 *
 * @hblk_magic: "HBLK" (ASCII 48 42 4c 4b);
 * @hblk_version: Identifies the version
 * @hblk_endian: 1 or 2 lil or big
 * @hblk_blocks: Number of blocks in the blockchain
 */
typedef struct bc_file_hdr_s
{
	int8_t   hblk_magic[4];
	int8_t   hblk_version[3];
	uint8_t  hblk_endian;
	uint32_t hblk_blocks;
} bc_file_hdr_t;


char *strE_LLIST(E_LLIST code);
blockchain_t *blockchain_create(void);

block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len);

void block_destroy(block_t *block);

void blockchain_destroy(blockchain_t *blockchain);

uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

int pathToWriteFD(char const *path);
int fheaderBlk(int fd, const blockchain_t *blockchain);
int writeBlocks(int fd, const blockchain_t *blockchain);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);

int pathToReadFD(char const *path);
int readBlkchnFileHdr(int fd, uint8_t local_endianness,
		      uint8_t *file_endianness, uint32_t *block_ct);
void bswapBlock(block_t *block);
int readBlocks(int fd, const blockchain_t *blockchain,
	       uint8_t local_endianness, uint8_t file_endianness,
	       uint32_t block_ct);
blockchain_t *blockchain_deserialize(char const *path);

int block_is_valid(block_t const *block, block_t const *prev_block);

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
                            uint32_t difficulty);

int block_is_valid(block_t const *block, block_t const *prev_block);
int verifyHashes(block_t const *block, block_t const *prev_block);

void block_mine(block_t *block);

#endif /* BLOCKCHAIN_H */
