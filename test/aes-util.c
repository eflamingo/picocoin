/*
 * Copyright 2016 Duncan Tebbs
 * Distributed under the MIT/X11 software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include <ccoin/crypto/aes_util.h>      // for read_aes_file, etc
#include <ccoin/cstr.h>
#include <ccoin/util.h>

#include <assert.h>                     // for assert
#include <stddef.h>                     // for size_t
#include <stdint.h>                     // for uint8_t
#include <stdio.h>                      // for printf, NULL
#include <stdlib.h>                     // for free
#include <string.h>                     // for strlen, memcmp
#include <stdbool.h>                    // for bool, true
#include <unistd.h>                     // for unlink

#include "libtest.h"

static const char s_password[] = "test_picocoin_password";
static const char filename[] = "aes_util.dat";

static const char s_plaintext_0[417+1] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus cursus, tellus at commodo tristique, nisi mi euismod erat, at fringilla ligula quam semper nibh. Cras blandit venenatis venenatis. Curabitur sollicitudin fermentum cursus. Aliquam gravida augue eu tortor commodo, a consectetur nulla convallis. Nullam id mattis purus. Sed nec velit quis magna finibus suscipit. Aenean ornare accumsan ipsum at tempor.";
static const uint8_t s_ciphertext_0[432] = {
0xab,0xf6,0xb0,0xfa,0x06,0x36,0x98,0xe0,0x98,0xb1,0xd8,0xb2,0xd7,0x68,0xf7,0x9b,
0x16,0xf6,0xf9,0xb2,0xff,0x02,0xfd,0x67,0x63,0x47,0xe0,0x58,0x53,0x66,0xc0,0xa9,
0x52,0x38,0xc1,0xdd,0x04,0xdf,0x53,0xc4,0x5e,0x10,0x85,0x9c,0x4d,0x7e,0xbc,0xfd,
0xc1,0x5e,0xaf,0xb2,0x60,0xac,0xe0,0x04,0xe6,0x59,0xf4,0xf0,0x65,0xeb,0xd9,0x7c,
0xec,0x19,0xa1,0xc2,0x5f,0x79,0x91,0x62,0x07,0xc3,0x55,0x7b,0x7d,0xa1,0xfe,0x77,
0x6d,0xac,0x3e,0xbc,0x51,0x71,0xce,0xe7,0xa4,0x5b,0xd4,0x58,0x54,0x76,0x44,0x5a,
0x4e,0x65,0xfe,0x85,0xbb,0x52,0x00,0x69,0xac,0xf8,0x1f,0x47,0xc5,0x79,0xf1,0x22,
0x07,0xde,0x3c,0x04,0x79,0xef,0x3e,0xea,0x36,0x32,0xe2,0x58,0x02,0x08,0xd7,0x9c,
0xf0,0x2a,0x91,0x77,0x33,0x9f,0x97,0x17,0x5f,0xc8,0xd2,0x8d,0x1d,0xce,0x49,0x12,
0xe0,0x57,0xea,0xb3,0x2d,0xc6,0xc9,0x6c,0xb2,0x22,0x9d,0x06,0xfb,0xad,0x63,0x75,
0x0a,0x45,0xbc,0xc2,0x7f,0x8f,0xf7,0xae,0x8d,0x09,0x2b,0x3d,0xa6,0x40,0xaf,0x0f,
0x13,0x70,0x1c,0x85,0xdb,0x66,0xf8,0x03,0xb4,0x76,0xf8,0xf0,0x26,0x67,0xee,0xd7,
0x90,0x24,0x6e,0xa5,0x91,0x77,0x5b,0x76,0xe0,0xc2,0x6d,0x10,0x38,0xac,0xd4,0xa9,
0xb3,0xdc,0x8e,0xce,0xfb,0x77,0x80,0x3c,0xfd,0x29,0xf3,0x09,0x87,0x5c,0xa1,0xf5,
0x77,0xf6,0x97,0x64,0xe8,0x02,0x64,0xd2,0x95,0x9d,0xf2,0x50,0xaa,0x8f,0x33,0x5f,
0xb2,0x09,0x49,0xa5,0x14,0x00,0x3c,0x87,0x0d,0x49,0x99,0xdb,0x7b,0x76,0xec,0x74,
0xc8,0x36,0x07,0xcf,0x45,0x1e,0x23,0xae,0xda,0x9c,0xed,0x79,0x9b,0xdf,0xda,0x9e,
0xcd,0x3c,0xb4,0xd7,0x47,0x08,0x78,0xd0,0x1e,0xa9,0x7d,0x77,0x10,0x31,0xfc,0x65,
0x60,0x7f,0x77,0xdd,0xca,0x93,0x71,0xf9,0x44,0xe0,0x93,0xd1,0x43,0x3b,0xfe,0x8d,
0xe3,0x8d,0x06,0xbb,0x97,0xb6,0xa4,0x45,0xf6,0x74,0x92,0x70,0x1a,0x6c,0xb5,0x47,
0x8c,0xe8,0x6f,0x92,0xde,0x15,0x1f,0x25,0x98,0x22,0xa4,0x05,0x46,0x29,0x3d,0x20,
0xd9,0x0e,0xf1,0x35,0xb4,0xf0,0xc8,0x80,0x45,0xd1,0x57,0xee,0xf6,0x7f,0xf7,0xfa,
0xd3,0xf7,0xdf,0xc6,0xa1,0x20,0xb5,0x88,0x0f,0xc5,0xb6,0x57,0xb2,0x75,0x41,0x66,
0x5c,0xd0,0xe0,0x01,0x31,0x67,0xde,0xef,0x69,0xbf,0x96,0xff,0x6b,0xc6,0x1d,0x02,
0x5a,0x5b,0x7e,0xf9,0x03,0x27,0xc4,0xf3,0x23,0x17,0xcd,0xd7,0xf5,0xeb,0x80,0xd6,
0xef,0xfd,0x79,0xaa,0x02,0x24,0x4f,0x75,0xe8,0x85,0xa7,0x07,0x44,0xcf,0x91,0xb7,
0x02,0x62,0x06,0xd6,0x46,0x72,0x72,0x38,0xcf,0xe0,0xc5,0x84,0x0d,0xba,0x85,0x43,
};

static const char s_plaintext_1[400+1] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus cursus, tellus at commodo tristique, nisi mi euismod erat, at fringilla ligula quam semper nibh. Cras blandit venenatis venenatis. Curabitur sollicitudin fermentum cursus. Aliquam gravida augue eu tortor commodo, a consectetur nulla convallis. Nullam id mattis purus. Sed nec velit quis magna finibus suscipit. Aenean ornare accumsan";
static const uint8_t s_ciphertext_1[416] = {
0xab,0xf6,0xb0,0xfa,0x06,0x36,0x98,0xe0,0x98,0xb1,0xd8,0xb2,0xd7,0x68,0xf7,0x9b,
0x16,0xf6,0xf9,0xb2,0xff,0x02,0xfd,0x67,0x63,0x47,0xe0,0x58,0x53,0x66,0xc0,0xa9,
0x52,0x38,0xc1,0xdd,0x04,0xdf,0x53,0xc4,0x5e,0x10,0x85,0x9c,0x4d,0x7e,0xbc,0xfd,
0xc1,0x5e,0xaf,0xb2,0x60,0xac,0xe0,0x04,0xe6,0x59,0xf4,0xf0,0x65,0xeb,0xd9,0x7c,
0xec,0x19,0xa1,0xc2,0x5f,0x79,0x91,0x62,0x07,0xc3,0x55,0x7b,0x7d,0xa1,0xfe,0x77,
0x6d,0xac,0x3e,0xbc,0x51,0x71,0xce,0xe7,0xa4,0x5b,0xd4,0x58,0x54,0x76,0x44,0x5a,
0x4e,0x65,0xfe,0x85,0xbb,0x52,0x00,0x69,0xac,0xf8,0x1f,0x47,0xc5,0x79,0xf1,0x22,
0x07,0xde,0x3c,0x04,0x79,0xef,0x3e,0xea,0x36,0x32,0xe2,0x58,0x02,0x08,0xd7,0x9c,
0xf0,0x2a,0x91,0x77,0x33,0x9f,0x97,0x17,0x5f,0xc8,0xd2,0x8d,0x1d,0xce,0x49,0x12,
0xe0,0x57,0xea,0xb3,0x2d,0xc6,0xc9,0x6c,0xb2,0x22,0x9d,0x06,0xfb,0xad,0x63,0x75,
0x0a,0x45,0xbc,0xc2,0x7f,0x8f,0xf7,0xae,0x8d,0x09,0x2b,0x3d,0xa6,0x40,0xaf,0x0f,
0x13,0x70,0x1c,0x85,0xdb,0x66,0xf8,0x03,0xb4,0x76,0xf8,0xf0,0x26,0x67,0xee,0xd7,
0x90,0x24,0x6e,0xa5,0x91,0x77,0x5b,0x76,0xe0,0xc2,0x6d,0x10,0x38,0xac,0xd4,0xa9,
0xb3,0xdc,0x8e,0xce,0xfb,0x77,0x80,0x3c,0xfd,0x29,0xf3,0x09,0x87,0x5c,0xa1,0xf5,
0x77,0xf6,0x97,0x64,0xe8,0x02,0x64,0xd2,0x95,0x9d,0xf2,0x50,0xaa,0x8f,0x33,0x5f,
0xb2,0x09,0x49,0xa5,0x14,0x00,0x3c,0x87,0x0d,0x49,0x99,0xdb,0x7b,0x76,0xec,0x74,
0xc8,0x36,0x07,0xcf,0x45,0x1e,0x23,0xae,0xda,0x9c,0xed,0x79,0x9b,0xdf,0xda,0x9e,
0xcd,0x3c,0xb4,0xd7,0x47,0x08,0x78,0xd0,0x1e,0xa9,0x7d,0x77,0x10,0x31,0xfc,0x65,
0x60,0x7f,0x77,0xdd,0xca,0x93,0x71,0xf9,0x44,0xe0,0x93,0xd1,0x43,0x3b,0xfe,0x8d,
0xe3,0x8d,0x06,0xbb,0x97,0xb6,0xa4,0x45,0xf6,0x74,0x92,0x70,0x1a,0x6c,0xb5,0x47,
0x8c,0xe8,0x6f,0x92,0xde,0x15,0x1f,0x25,0x98,0x22,0xa4,0x05,0x46,0x29,0x3d,0x20,
0xd9,0x0e,0xf1,0x35,0xb4,0xf0,0xc8,0x80,0x45,0xd1,0x57,0xee,0xf6,0x7f,0xf7,0xfa,
0xd3,0xf7,0xdf,0xc6,0xa1,0x20,0xb5,0x88,0x0f,0xc5,0xb6,0x57,0xb2,0x75,0x41,0x66,
0x5c,0xd0,0xe0,0x01,0x31,0x67,0xde,0xef,0x69,0xbf,0x96,0xff,0x6b,0xc6,0x1d,0x02,
0x5a,0x5b,0x7e,0xf9,0x03,0x27,0xc4,0xf3,0x23,0x17,0xcd,0xd7,0xf5,0xeb,0x80,0xd6,
0xca,0x00,0x3a,0x9a,0xd7,0xb5,0x8d,0x07,0xff,0x0c,0x15,0x1a,0xf5,0x85,0x3e,0xce,
};

static const char s_plaintext_2[399+1] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus cursus, tellus at commodo tristique, nisi mi euismod erat, at fringilla ligula quam semper nibh. Cras blandit venenatis venenatis. Curabitur sollicitudin fermentum cursus. Aliquam gravida augue eu tortor commodo, a consectetur nulla convallis. Nullam id mattis purus. Sed nec velit quis magna finibus suscipit. Aenean ornare accumsa";
static const uint8_t s_ciphertext_2[400] = {
0xab,0xf6,0xb0,0xfa,0x06,0x36,0x98,0xe0,0x98,0xb1,0xd8,0xb2,0xd7,0x68,0xf7,0x9b,
0x16,0xf6,0xf9,0xb2,0xff,0x02,0xfd,0x67,0x63,0x47,0xe0,0x58,0x53,0x66,0xc0,0xa9,
0x52,0x38,0xc1,0xdd,0x04,0xdf,0x53,0xc4,0x5e,0x10,0x85,0x9c,0x4d,0x7e,0xbc,0xfd,
0xc1,0x5e,0xaf,0xb2,0x60,0xac,0xe0,0x04,0xe6,0x59,0xf4,0xf0,0x65,0xeb,0xd9,0x7c,
0xec,0x19,0xa1,0xc2,0x5f,0x79,0x91,0x62,0x07,0xc3,0x55,0x7b,0x7d,0xa1,0xfe,0x77,
0x6d,0xac,0x3e,0xbc,0x51,0x71,0xce,0xe7,0xa4,0x5b,0xd4,0x58,0x54,0x76,0x44,0x5a,
0x4e,0x65,0xfe,0x85,0xbb,0x52,0x00,0x69,0xac,0xf8,0x1f,0x47,0xc5,0x79,0xf1,0x22,
0x07,0xde,0x3c,0x04,0x79,0xef,0x3e,0xea,0x36,0x32,0xe2,0x58,0x02,0x08,0xd7,0x9c,
0xf0,0x2a,0x91,0x77,0x33,0x9f,0x97,0x17,0x5f,0xc8,0xd2,0x8d,0x1d,0xce,0x49,0x12,
0xe0,0x57,0xea,0xb3,0x2d,0xc6,0xc9,0x6c,0xb2,0x22,0x9d,0x06,0xfb,0xad,0x63,0x75,
0x0a,0x45,0xbc,0xc2,0x7f,0x8f,0xf7,0xae,0x8d,0x09,0x2b,0x3d,0xa6,0x40,0xaf,0x0f,
0x13,0x70,0x1c,0x85,0xdb,0x66,0xf8,0x03,0xb4,0x76,0xf8,0xf0,0x26,0x67,0xee,0xd7,
0x90,0x24,0x6e,0xa5,0x91,0x77,0x5b,0x76,0xe0,0xc2,0x6d,0x10,0x38,0xac,0xd4,0xa9,
0xb3,0xdc,0x8e,0xce,0xfb,0x77,0x80,0x3c,0xfd,0x29,0xf3,0x09,0x87,0x5c,0xa1,0xf5,
0x77,0xf6,0x97,0x64,0xe8,0x02,0x64,0xd2,0x95,0x9d,0xf2,0x50,0xaa,0x8f,0x33,0x5f,
0xb2,0x09,0x49,0xa5,0x14,0x00,0x3c,0x87,0x0d,0x49,0x99,0xdb,0x7b,0x76,0xec,0x74,
0xc8,0x36,0x07,0xcf,0x45,0x1e,0x23,0xae,0xda,0x9c,0xed,0x79,0x9b,0xdf,0xda,0x9e,
0xcd,0x3c,0xb4,0xd7,0x47,0x08,0x78,0xd0,0x1e,0xa9,0x7d,0x77,0x10,0x31,0xfc,0x65,
0x60,0x7f,0x77,0xdd,0xca,0x93,0x71,0xf9,0x44,0xe0,0x93,0xd1,0x43,0x3b,0xfe,0x8d,
0xe3,0x8d,0x06,0xbb,0x97,0xb6,0xa4,0x45,0xf6,0x74,0x92,0x70,0x1a,0x6c,0xb5,0x47,
0x8c,0xe8,0x6f,0x92,0xde,0x15,0x1f,0x25,0x98,0x22,0xa4,0x05,0x46,0x29,0x3d,0x20,
0xd9,0x0e,0xf1,0x35,0xb4,0xf0,0xc8,0x80,0x45,0xd1,0x57,0xee,0xf6,0x7f,0xf7,0xfa,
0xd3,0xf7,0xdf,0xc6,0xa1,0x20,0xb5,0x88,0x0f,0xc5,0xb6,0x57,0xb2,0x75,0x41,0x66,
0x5c,0xd0,0xe0,0x01,0x31,0x67,0xde,0xef,0x69,0xbf,0x96,0xff,0x6b,0xc6,0x1d,0x02,
0x1b,0xf5,0xa7,0x35,0xa8,0xee,0x23,0xb4,0xe0,0x80,0x0b,0xcf,0xad,0xcb,0x80,0xeb,
};

static void test_write_aes_file(const uint8_t *pt, size_t pt_len,
			       const uint8_t *ct, size_t ct_len)
{
	void *ciphertext;
	size_t ciphertext_len;
	char prefix[32];

	bool rc = write_aes_file(filename, (char *)s_password, strlen(s_password),
		    pt, pt_len);
	assert(rc);

	assert(bu_read_file(filename, &ciphertext, &ciphertext_len, 1024));
	assert(ciphertext);
	assert(ciphertext_len);

	fprintf(stderr, "====================\n");
	fprintf(stderr, "ENCRYPT TEST:\n");
	fprintf(stderr, " plaintext (%d bytes): %s\n", (int )pt_len, pt);
	sprintf(prefix, " ciphertext(%d bytes)", (int )ciphertext_len);
	dumphex(prefix, ciphertext, ciphertext_len);
	fprintf(stderr, "====================\n\n");

	assert(ct_len == ciphertext_len);
	assert(!memcmp(ct, ciphertext, sizeof(ct_len)));

	int rcv = unlink(filename);
	assert(rcv == 0);

	free(ciphertext);
}

static void test_read_aes_file(const uint8_t *ct, size_t ct_len,
			       const uint8_t *pt, size_t pt_len)
{
	char prefix[32];

	bool rc = bu_write_file(filename, ct, ct_len);
	assert(rc);

	cstring *plaintext = read_aes_file(filename, (char *)s_password,
			   			   strlen(s_password), ct_len);

	fprintf(stderr, "====================\n");
	fprintf(stderr, "DECRYPT TEST:\n");
	sprintf(prefix, " ciphertext(%d bytes)", (int )ct_len);
	dumphex(prefix, ct, ct_len);
	fprintf(stderr, " plaintext (%d bytes): %s\n", (int )pt_len, pt);
	fprintf(stderr, "====================\n\n");

	assert(NULL != plaintext);
	assert(pt_len == plaintext->len);
	assert(!memcmp(pt, plaintext->str, pt_len));

	int rcv = unlink(filename);
	assert(rcv == 0);

	cstr_free(plaintext, true);
}

static void test_encryption()
{
	test_write_aes_file((const uint8_t *)s_plaintext_0, strlen(s_plaintext_0),
			   s_ciphertext_0, sizeof(s_ciphertext_0));

	test_write_aes_file((const uint8_t *)s_plaintext_1, strlen(s_plaintext_1),
			   s_ciphertext_1, sizeof(s_ciphertext_1));

	test_write_aes_file((const uint8_t *)s_plaintext_2, strlen(s_plaintext_2),
			   s_ciphertext_2, sizeof(s_ciphertext_2));
}

static void test_decryption()
{
	test_read_aes_file(s_ciphertext_0, sizeof(s_ciphertext_0),
			   (uint8_t *)s_plaintext_0, strlen(s_plaintext_0));

	test_read_aes_file(s_ciphertext_1, sizeof(s_ciphertext_1),
			   (uint8_t *)s_plaintext_1, strlen(s_plaintext_1));

	test_read_aes_file(s_ciphertext_2, sizeof(s_ciphertext_2),
			   (uint8_t *)s_plaintext_2, strlen(s_plaintext_2));
}

int main(int argc, char **argv)
{
	//test_encryption();
	//test_decryption();

	return 0;
}
