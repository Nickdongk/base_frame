#ifndef _BASELIB_H_
#define _BASELIB_H_

#ifndef B_MAX
#define B_MAX(a, b)     (((a) >= (b))? (a):(b))
#endif

#ifndef B_MIN
#define B_MIN(a, b)     (((a) <= (b))? (a):(b))
#endif

#ifndef B_ROUND
#define B_ROUND(a, b)   (((a) + (b) - 1) & (~((b) - 1)))
#endif

#ifndef B_L_BYTE
#define B_L_BYTE(a)     ((a) & 0xff)
#endif

#ifndef B_H_BYTE
#define B_H_BYTE(a)     (((a) >> 8) & 0xff)
#endif

#ifndef B_ROUND_UP
#define B_ROUND_UP(a, b)    (((a) + (b) - 1) / (b))
#endif

#endif
