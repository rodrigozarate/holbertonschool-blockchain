#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <llist.h>
#include <stdint.h>
#include "../../crypto/hblk_crypto.h"


# define COINBASE_AMOUNT 50


/**
 * struct transaction_s - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
 * @hash:   Hash of @amount and @pub. Serves as output ID
 */
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct tx_in_s - Transaction input
 * @block_hash:  Hash of the Block containing the transaction @tx_id
 * @tx_id:       ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
 * @sig:         Signature.
 */
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 * @block_hash: Hash of the Block containing the transaction @tx_id
 * @tx_id:      ID of the transaction containing @out
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t;

/**
 * struct su_info_s - spender unspent info
 * @sender_unspent: list of unspent outputs matching the sender's public key
 * @sender_pub: sender's public key
 * @send_amt: proposed amount to send
 * @total_unspent_amt: total of all unspent outputs combined to be used as
 *   inputs
 */
typedef struct su_info_s
{
	llist_t *sender_unspent;
	uint8_t  sender_pub[EC_PUB_LEN];
	uint32_t send_amt;
	uint32_t total_unspent_amt;
} su_info_t;

/**
 * struct sign_info_s - input signing info
 * @tx_id: hash of new transaction
 * @sender: sender's key pair
 * @all_unspent: list of all unspent outputs
 */
typedef struct sign_info_s
{
	uint8_t       tx_id[SHA256_DIGEST_LENGTH];
	const EC_KEY *sender;
	llist_t      *all_unspent;
} sign_info_t;

/**
 * struct iv_info_s - input validation info
 * @tx_id: id of transaction to be validated
 * @tl_input_amt: total of every unspent output amount referenced by an input
 * @all_unspent: list of all unspent outputs in the blockchain
 */
typedef struct iv_info_s
{
	uint8_t  tx_id[SHA256_DIGEST_LENGTH];
	uint32_t tl_input_amt;
	llist_t *all_unspent;
} iv_info_t;

/**
 * struct uo_info_s - unspent output info
 * @block_hash: hash of block containing transactions to update unpsent list
 * @tx_id: id of current transaction to scan when updating
 * @all_unspent: list of all unspent outputs in the blockchain
 */
typedef struct uo_info_s
{
	uint8_t  block_hash[SHA256_DIGEST_LENGTH];
	uint8_t  tx_id[SHA256_DIGEST_LENGTH];
	llist_t *all_unspent;
} uo_info_t;

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);

tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);

uint8_t *createTxDataBuff(const transaction_t *transaction,
			  int *input_ct, int *output_ct,
			  size_t *tx_IO_buf_sz);
int populateTxDataBuff(const transaction_t *transaction,
		       int input_ct, int output_ct,
		       uint8_t *tx_IO_buf);
uint8_t *transaction_hash(transaction_t const *transaction,
			  uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		  EC_KEY const *sender, llist_t *all_unspent);

llist_t *setTxInputs(llist_t *all_unspent, su_info_t *su_info);
llist_t *setTxOutputs(const uint8_t sender_pub[EC_PUB_LEN],
		      const uint8_t recv_pub[EC_PUB_LEN], su_info_t *su_info);
transaction_t *newTransaction(llist_t *tx_inputs, llist_t *tx_outputs,
			      const EC_KEY *sender, llist_t *all_unspent);
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
				  uint32_t amount, llist_t *all_unspent);

int transaction_is_valid(transaction_t const *transaction,
			 llist_t *all_unspent);

transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);

int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);

void transaction_destroy(transaction_t *transaction);

int delRfrncdOutput(tx_in_t *tx_in, unsigned int idx,
		    llist_t *unspent);

llist_t *update_unspent(llist_t *transactions,
			uint8_t block_hash[SHA256_DIGEST_LENGTH],
			llist_t *all_unspent);

#include "../blockchain.h"

#endif /* TRANSACTION_H */
