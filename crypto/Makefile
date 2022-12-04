CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

SRC = sha256.c \
	ec_create.c \
	ec_to_pub.c \
	ec_from_pub.c \
	ec_save.c \
	ec_load.c \
	ec_sign.c \
	ec_verify.c \

OBJ = $(SRC:.c=.o)

.PHONY: libhblk_crypto.a clean oclean flcean

libhblk_crypto.a: $(OBJ)
	ar -rcs libhblk_crypto.a $(OBJ)

all: libhblk_crypto.a

clean:
	$(RM) *~ libhblk_crypto.a

oclean:
	$(RM) $(OBJ)

fclean: clean oclean