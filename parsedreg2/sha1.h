#ifndef MHASH_SHA1_H
#define MHASH_SHA1_H

#include <sys/types.h>

typedef u_int8_t word8;
typedef u_int32_t word32;

/* The SHA block size and message digest sizes, in bytes */
#define SHA_DATASIZE    64
#define SHA_DATALEN     16
#define SHA_DIGESTSIZE  20
#define SHA_DIGESTLEN    5
/* The structure for storing SHA info */

typedef struct sha_ctx {
  word32 digest[SHA_DIGESTLEN];  /* Message digest */
  word32 count_l, count_h;       /* 64-bit block count */
  word8 block[SHA_DATASIZE];     /* SHA data buffer */
  int index;                             /* index into buffer */
} SHA_CTX;

void sha_init(struct sha_ctx *ctx);
void sha_update(struct sha_ctx *ctx, word8 *buffer, word32 len);
void sha_final(struct sha_ctx *ctx);
void sha_digest(struct sha_ctx *ctx, word8 *s);
void sha_copy(struct sha_ctx *dest, struct sha_ctx *src);

#endif
