/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 2017, Florin Petriuc, <petriuc.florin@gmail.com>
 * Copyright (C) 2018 - 2022, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include <string.h>
#include <stdint.h>
#include "sha256.h"

/* When no other crypto library is available we use this code segment */

/* This is based on SHA256 implementation in LibTomCrypt that was released into
 * public domain by Tom St Denis. */

#define WPA_GET_BE32(a) ((((unsigned long)(a)[0]) << 24) | \
                         (((unsigned long)(a)[1]) << 16) | \
                         (((unsigned long)(a)[2]) <<  8) | \
                          ((unsigned long)(a)[3]))
#define WPA_PUT_BE32(a, val)                                        \
do {                                                                \
  (a)[0] = (unsigned char)((((unsigned long) (val)) >> 24) & 0xff); \
  (a)[1] = (unsigned char)((((unsigned long) (val)) >> 16) & 0xff); \
  (a)[2] = (unsigned char)((((unsigned long) (val)) >> 8) & 0xff);  \
  (a)[3] = (unsigned char)(((unsigned long) (val)) & 0xff);         \
} while(0)

#define WPA_PUT_BE64(a, val)                                    \
do {                                                            \
  (a)[0] = (unsigned char)(((uint64_t)(val)) >> 56);  \
  (a)[1] = (unsigned char)(((uint64_t)(val)) >> 48);  \
  (a)[2] = (unsigned char)(((uint64_t)(val)) >> 40);  \
  (a)[3] = (unsigned char)(((uint64_t)(val)) >> 32);  \
  (a)[4] = (unsigned char)(((uint64_t)(val)) >> 24);  \
  (a)[5] = (unsigned char)(((uint64_t)(val)) >> 16);  \
  (a)[6] = (unsigned char)(((uint64_t)(val)) >> 8);   \
  (a)[7] = (unsigned char)(((uint64_t)(val)) & 0xff); \
} while(0)

/* The K array */
static const unsigned long K[64] = {
  0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
  0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
  0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
  0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
  0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
  0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
  0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
  0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
  0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
  0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
  0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
  0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
  0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Various logical functions */
#define RORc(x, y) \
(((((unsigned long)(x) & 0xFFFFFFFFUL) >> (unsigned long)((y) & 31)) | \
   ((unsigned long)(x) << (unsigned long)(32 - ((y) & 31)))) & 0xFFFFFFFFUL)
#define Ch(x,y,z)   (z ^ (x & (y ^ z)))
#define Maj(x,y,z)  (((x | y) & z) | (x & y))
#define S(x, n)     RORc((x), (n))
#define R(x, n)     (((x)&0xFFFFFFFFUL)>>(n))
#define Sigma0(x)   (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x)   (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x)   (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x)   (S(x, 17) ^ S(x, 19) ^ R(x, 10))

/* Compress 512-bits */
static int sha256_compress(struct sha256_state *md,
                           unsigned char *buf)
{
  unsigned long S[8], W[64];
  int i;

  /* Copy state into S */
  for(i = 0; i < 8; i++) {
    S[i] = md->state[i];
  }
  /* copy the state into 512-bits into W[0..15] */
  for(i = 0; i < 16; i++)
    W[i] = WPA_GET_BE32(buf + (4 * i));
  /* fill W[16..63] */
  for(i = 16; i < 64; i++) {
    W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) +
      W[i - 16];
  }

  /* Compress */
#define RND(a,b,c,d,e,f,g,h,i)                                          \
  do {                                                                  \
    unsigned long t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];       \
    unsigned long t1 = Sigma0(a) + Maj(a, b, c);                        \
    d += t0;                                                            \
    h = t0 + t1;                                                        \
  } while(0)

  for(i = 0; i < 64; ++i) {
    unsigned long t;
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], i);
    t = S[7]; S[7] = S[6]; S[6] = S[5]; S[5] = S[4];
    S[4] = S[3]; S[3] = S[2]; S[2] = S[1]; S[1] = S[0]; S[0] = t;
  }

  /* Feedback */
  for(i = 0; i < 8; i++) {
    md->state[i] = md->state[i] + S[i];
  }

  return 0;
}

/* Initialize the hash state */
void my_sha256_init(struct sha256_state *md)
{
  md->curlen = 0;
  md->length = 0;
  md->state[0] = 0x6A09E667UL;
  md->state[1] = 0xBB67AE85UL;
  md->state[2] = 0x3C6EF372UL;
  md->state[3] = 0xA54FF53AUL;
  md->state[4] = 0x510E527FUL;
  md->state[5] = 0x9B05688CUL;
  md->state[6] = 0x1F83D9ABUL;
  md->state[7] = 0x5BE0CD19UL;
}

/*
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return 0 if successful
*/
int my_sha256_update(struct sha256_state *md,
                            const unsigned char *in,
                            unsigned long inlen)
{
  unsigned long n;

#define block_size 64
  if(md->curlen > sizeof(md->buf))
    return -1;
  while(inlen > 0) {
    if(md->curlen == 0 && inlen >= block_size) {
      if(sha256_compress(md, (unsigned char *)in) < 0)
        return -1;
      md->length += block_size * 8;
      in += block_size;
      inlen -= block_size;
    }
    else {
      n = inlen < (block_size - md->curlen) ? inlen : (block_size - md->curlen);
      memcpy(md->buf + md->curlen, in, n);
      md->curlen += n;
      in += n;
      inlen -= n;
      if(md->curlen == block_size) {
        if(sha256_compress(md, md->buf) < 0)
          return -1;
        md->length += 8 * block_size;
        md->curlen = 0;
      }
    }
  }

  return 0;
}

/*
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (32 bytes)
   @return 0 if successful
*/
int my_sha256_final(unsigned char *out,
                           struct sha256_state *md)
{
  int i;

  if(md->curlen >= sizeof(md->buf))
    return -1;

  /* Increase the length of the message */
  md->length += md->curlen * 8;

  /* Append the '1' bit */
  md->buf[md->curlen++] = (unsigned char)0x80;

  /* If the length is currently above 56 bytes we append zeros
   * then compress.  Then we can fall back to padding zeros and length
   * encoding like normal.
   */
  if(md->curlen > 56) {
    while(md->curlen < 64) {
      md->buf[md->curlen++] = (unsigned char)0;
    }
    sha256_compress(md, md->buf);
    md->curlen = 0;
  }

  /* Pad up to 56 bytes of zeroes */
  while(md->curlen < 56) {
    md->buf[md->curlen++] = (unsigned char)0;
  }

  /* Store length */
  WPA_PUT_BE64(md->buf + 56, md->length);
  sha256_compress(md, md->buf);

  /* Copy output */
  for(i = 0; i < 8; i++)
    WPA_PUT_BE32(out + (4 * i), md->state[i]);

  return 0;
}
