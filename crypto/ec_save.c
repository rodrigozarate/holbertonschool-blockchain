#include "hblk_crypto.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <linux/limits.h>
#include <string.h>


/**
 * ptnfile_w - FILE pointer to a new file
 * @folder: path
 * @filename: file
 * Return: FILE pointer
 */
FILE *ptnfile_w(char const *folder, const char *filename)
{
	struct stat st;
	char file_path[PATH_MAX];
	FILE *dest_file;
	int end_slash;

	if (!folder || !filename)
	{
		fprintf(stderr, "ptnfile_w: NULL parameter(s)\n");
		return (NULL);
	}

	if (lstat(folder, &st) == -1)
	{
		if (errno == ENOENT)
			fprintf(stderr, "ptnfile_w: fopen: '%s': %s\n",
				folder, strerror(ENOENT));
		else
			perror("ptnfile_w: fopen");
		return (NULL);
	}

	if (!S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "ptnfile_w: `folder` is not a directory\n");
		return (NULL);
	}

	end_slash = (folder[strlen(folder) - 1] == '/');
	sprintf(file_path, end_slash ? "%s%s" : "%s/%s", folder, filename);
	dest_file = fopen(file_path, "w");
	if (!dest_file)
	{
		perror("ptnfile_w: fopen");
		return (NULL);
	}

	return (dest_file);
}

/**
 * ec_save - saves EC key pair
 * @key: pointer EC key pair
 * @folder:  path
 * Return: 1 on success or 0
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *pub_key_file, *pri_key_file;

	if (!key || !folder)
	{
		fprintf(stderr, "ec_save: NULL parameter(s)\n");
		return (0);
	}
	if (!EC_KEY_check_key(key))
	{
		fprintf(stderr, "ec_save: key verification failed\n");
		return (0);
	}
	if (mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1 &&
	    errno != EEXIST)
	{
		perror("ec_save: mkdir");
		return (0);
	}
	pri_key_file = ptnfile_w(folder, PRI_FILENAME);
	pub_key_file = ptnfile_w(folder, PUB_FILENAME);
	if (!pub_key_file || !pri_key_file)
		return (0);
	if (PEM_write_ECPrivateKey(pri_key_file, key,
				   NULL, NULL, 0, NULL, NULL) == 0)
	{
		fprintf(stderr, "ec_save: PEM_write_ECPrivateKey failure\n");
		return (0);
	}
	if (PEM_write_EC_PUBKEY(pub_key_file, key) == 0)
	{
		fprintf(stderr, "ec_save: PEM_write_EC_PUBKEY failure\n");
		return (0);
	}
	if (fclose(pri_key_file) != 0 || fclose(pub_key_file) != 0)
	{
		perror("ec_save: fclose");
		return (0);
	}
	return (1);
}
