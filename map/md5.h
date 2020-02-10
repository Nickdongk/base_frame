#ifndef _MD5_H_
#define _MD5_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern md5(uint8_t *initial_msg, size_t initial_len, uint32_t *h);

#endif
