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
 * fileLD - FILE pointer
 * @folder: path
 * @filename: name of file
 * Return: FILE pointer
 */
FILE *fileLD(char const *folder, const char *filename)
{
	struct stat st;
	char file_path[PATH_MAX];
	FILE *dest_file;
	int end_slash;

	if (!folder || !filename)
	{
		fprintf(stderr, "fileLD: NULL parameter(s)\n");
		return (NULL);
	}
	if (lstat(folder, &st) == -1)
	{
		if (errno == ENOENT)
			fprintf(stderr, "fileLD: fopen: '%s': %s\n",
				folder, strerror(ENOENT));
		else
			perror("fileLD: fopen");
		return (NULL);
	}
	if (!S_ISDIR(st.st_mode))
	{
		fprintf(stderr,
			"fileLD: `folder` is not a directory\n");
		return (NULL);
	}

	end_slash = (folder[strlen(folder) - 1] == '/');
	sprintf(file_path, end_slash ? "%s%s" : "%s/%s", folder, filename);
	dest_file = fopen(file_path, "r");
	if (!dest_file)
	{
		if (errno == ENOENT)
			fprintf(stderr, "fileLD: fopen: '%s': %s\n",
				file_path, strerror(ENOENT));
		else
			perror("fileLD: fopen");
		return (NULL);
	}
	return (dest_file);
}

/**
 * ec_load - loads EC key pair
 * @folder: path
 * Return: pointer
 */
EC_KEY *ec_load(char const *folder)
{
	EC_KEY *ec_key;
	FILE *pub_key_file, *pri_key_file;

	if (!folder)
	{
		fprintf(stderr, "ec_load: NULL parameter\n");
		return (NULL);
	}

	pri_key_file = fileLD(folder, PRI_FILENAME);
	pub_key_file = fileLD(folder, PUB_FILENAME);
	if (!pub_key_file || !pri_key_file)
		return (NULL);

	ec_key = PEM_read_EC_PUBKEY(pub_key_file, NULL, NULL, NULL);
	if (!ec_key)
	{
		fprintf(stderr, "ec_load: PEM_read_EC_PUBKEY failure\n");
		return (NULL);
	}

	ec_key = PEM_read_ECPrivateKey(pri_key_file, &ec_key, NULL, NULL);
	if (!ec_key)
	{
		fprintf(stderr, "ec_load: PEM_read_ECPrivateKey failure\n");
		return (NULL);
	}

	if (fclose(pri_key_file) != 0 || fclose(pub_key_file) != 0)
	{
		perror("ec_load: fclose");
		return (NULL);
	}

	return (ec_key);
}
