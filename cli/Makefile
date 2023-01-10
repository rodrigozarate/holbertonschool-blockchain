CC = gcc
MAKE = make
RM = rm -f

HDR_DIRS = . ../blockchain/v0.3/ ../blockchain/v0.3/transaction/ ../crypto/
SILENCE = -Wno-overlength-strings
CFLAGS = -Wall -Werror -Wextra -pedantic $(SILENCE) $(addprefix -I, $(HDR_DIRS))

LIB_DIRS = ../crypto/ ../blockchain/v0.3/
LDFLAGS = -u OpenSSLGlobalCleanup $(addprefix -I, $(HDR_DIRS)) \
	$(addprefix -L, $(LIB_DIRS))

LNKD_LIBS = hblk_blockchain hblk_crypto ssl crypto llist
LDLIBS = $(addprefix -l, $(LNKD_LIBS)) -pthread

HEADERS = hblk_cli.h help_text.h info_formats.h
CLI_SRC = cleanup.c \
	cli_loop.c \
	cmd_exit.c \
	cmd_help.c \
	cmd_info.c \
	cmd_load.c \
	cmd_mempool_load.c \
	cmd_mempool_save.c \
	cmd_mine.c \
	cmd_new.c \
	cmd_save.c \
	cmd_send.c \
	cmd_wallet_load.c \
	cmd_wallet_save.c \
	hblk_cli.c \
	lexing.c \
	scripts.c
PRINT_SRC = _blockchain_print.c \
	_print_all_unspent.c \
	_print_hex_buffer.c \
	_transaction_print_brief.c \
	_transaction_print.c

OBJ = $(CLI_SRC:.c=.o) $(addprefix printing/, $(PRINT_SRC:.c=.o))

.PHONY: llist all clean oclean fclean re

hblk_cli: /usr/local/lib/libllist.so /usr/local/include/llist.h \
	../crypto/libhblk_crypto.a ../blockchain/v0.3/libhblk_blockchain.a \
	$(HEADERS) $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

../crypto/libhblk_crypto.a:
	$(MAKE) -C ../crypto/ libhblk_crypto.a oclean

../blockchain/v0.3/libhblk_blockchain.a:
	$(MAKE) -C ../blockchain/v0.3/ libhblk_blockchain.a oclean

/usr/local/lib/libllist.so:
/usr/local/include/llist.h:
	cd ../llist/ && sudo ./install.bash && cd -

all: hblk_cli

clean:
	$(RM) *~ hblk_cli

oclean:
	$(RM) $(OBJ)

fclean: clean oclean

re: oclean all
