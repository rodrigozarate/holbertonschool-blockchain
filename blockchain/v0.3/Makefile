CC = gcc
AR = ar -rcs
RM = rm -f

HDR_DIRS = . transaction ../../crypto
CFLAGS = -Wall -Werror -Wextra -pedantic $(addprefix -I, $(HDR_DIRS))
LDFLAGS = -u OpenSSLGlobalCleanup -L../../crypto
LDLIBS = $(addprefix -l, ssl crypto hblk_crypto llist) -pthread

HEADERS = blockchain.h transaction/transaction.h ../../crypto/hblk_crypto.h
BLKCHN_SRC = blockchain_create.c \
	block_create.c \
	block_destroy.c \
	blockchain_destroy.c \
	block_hash.c \
	blockchain_serialize.c \
	blockchain_deserialize.c \
	block_is_valid.c \
	hash_matches_difficulty.c \
	blockchain_difficulty.c \
	block_mine.c
TX_SRC = tx_out_create.c \
	unspent_tx_out_create.c \
	tx_in_create.c \
	transaction_hash.c \
	tx_in_sign.c \
	transaction_create.c \
	transaction_is_valid.c \
	coinbase_create.c \
	coinbase_is_valid.c \
	transaction_destroy.c \
	update_unspent.c
PROV_SRC = _endianness.c

OBJ = $(BLKCHN_SRC:.c=.o) $(addprefix transaction/, $(TX_SRC:.c=.o)) \
	$(addprefix provided/, $(PROV_SRC:.c=.o))

.PHONY: clean oclean fclean re

libhblk_blockchain.a: $(HEADERS) $(OBJ)
	$(AR) $@ $(OBJ)

all: libhblk_blockchain.a

clean:
	$(RM) *~ libhblk_blockchain.a

oclean:
	$(RM) $(OBJ)

fclean: clean oclean

re: oclean all
