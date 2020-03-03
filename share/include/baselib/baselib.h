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

#endif
