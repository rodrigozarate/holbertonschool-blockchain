#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

/* int8_t */
#include <stdint.h>
/* size_t */
#include <stddef.h>
/* SHA256_DIGEST_LENGTH */
#include <openssl/sha.h>
/* EC_KEY */
#include <openssl/ec.h>
/* NID_secp256k1 */
#include <openssl/obj_mac.h>

#define EC_CURVE NID_secp256k1
#define EC_PUB_LEN 65
#define PUB_FILENAME "key_pub.pem"
#define PRI_FILENAME "key.pem"


uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

EC_KEY *ec_create(void);
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
uint8_t *bAxEC_POINT(const EC_POINT *ec_point, const EC_GROUP *ec_group,
			       BN_CTX *bn_ctx, uint8_t pub[EC_PUB_LEN]);

EC_POINT *bAToEC_POINT(const EC_GROUP *ec_group, BN_CTX *bn_ctx,
			      const uint8_t pub[EC_PUB_LEN]);
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);

int ec_save(EC_KEY *key, char const *folder);
FILE *ptnfile_w(char const *folder, const char *filename);

FILE *fileLD(char const *folder, const char *filename);
EC_KEY *ec_load(char const *folder);


#endif /* HBLK_CRYPTO_H */
