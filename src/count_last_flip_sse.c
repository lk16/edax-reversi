<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 3e1ed4f (fix cr/lf in repository to lf)
/**
 * @file count_last_flip_sse.c
 *
 *
 * A function is provided to count the number of fipped disc of the last move.
 *
 * The basic principle is to read into an array a precomputed result. Doing
 * this is easy for a single line ; as we can use arrays of the form:
 *  - COUNT_FLIP[square where we play][8-bits disc pattern].
 * The problem is thus to convert any line of a 64-bits disc pattern into an
 * 8-bits disc pattern. A fast way to do this is to select the right line,
 * with a bit-mask, to gather the masked-bits into a continuous set by the 
 * SSE PMOVMSKB or PSADBW instruction.
 * Once we get our 8-bits disc patterns, we directly get the number of
 * flipped discs from the precomputed array, and add them from each flipping
 * lines.
 * For optimization purpose, the value returned is twice the number of flipped
 * disc, to facilitate the computation of disc difference.
 *
<<<<<<< HEAD
 * @date 1998 - 2023
 * @author Richard Delorme
 * @author Toshihiko Okuhara
 * @version 4.5
=======
 * @date 1998 - 2020
 * @author Richard Delorme
 * @author Toshihiko Okuhara
 * @version 4.4
>>>>>>> 3e1ed4f (fix cr/lf in repository to lf)
 * 
 */

#include "bit.h"
<<<<<<< HEAD
#include <stdint.h>

/** precomputed count flip array */
const uint8_t COUNT_FLIP[8][256] = {
=======

/** precomputed count flip array */
const unsigned char COUNT_FLIP[8][256] = {
>>>>>>> 3e1ed4f (fix cr/lf in repository to lf)
	{
		 0,  0,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		12, 12,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	},
	{
		 0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		10, 10, 10, 10,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	},
	{
		 0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 8, 10,  8,  8,  8, 10,  8,  8,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	},
	{
		 0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 6, 10,  8,  8,  6,  6,  6,  6,  6, 10,  8,  8,  6,  6,  6,  6,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	},
	{
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
};

/* bit masks for diagonal lines */
const V4DI mask_dvhd[64] = {
	{{ 0x0000000000000001, 0x00000000000000ff, 0x0101010101010101, 0x8040201008040201 }},
	{{ 0x0000000000000102, 0x00000000000000ff, 0x0202020202020202, 0x0080402010080402 }},
	{{ 0x0000000000010204, 0x00000000000000ff, 0x0404040404040404, 0x0000804020100804 }},
	{{ 0x0000000001020408, 0x00000000000000ff, 0x0808080808080808, 0x0000008040201008 }},
	{{ 0x0000000102040810, 0x00000000000000ff, 0x1010101010101010, 0x0000000080402010 }},
	{{ 0x0000010204081020, 0x00000000000000ff, 0x2020202020202020, 0x0000000000804020 }},
	{{ 0x0001020408102040, 0x00000000000000ff, 0x4040404040404040, 0x0000000000008040 }},
	{{ 0x0102040810204080, 0x00000000000000ff, 0x8080808080808080, 0x0000000000000080 }},
	{{ 0x0000000000000102, 0x000000000000ff00, 0x0101010101010101, 0x4020100804020100 }},
	{{ 0x0000000000010204, 0x000000000000ff00, 0x0202020202020202, 0x8040201008040201 }},
	{{ 0x0000000001020408, 0x000000000000ff00, 0x0404040404040404, 0x0080402010080402 }},
	{{ 0x0000000102040810, 0x000000000000ff00, 0x0808080808080808, 0x0000804020100804 }},
	{{ 0x0000010204081020, 0x000000000000ff00, 0x1010101010101010, 0x0000008040201008 }},
	{{ 0x0001020408102040, 0x000000000000ff00, 0x2020202020202020, 0x0000000080402010 }},
	{{ 0x0102040810204080, 0x000000000000ff00, 0x4040404040404040, 0x0000000000804020 }},
	{{ 0x0204081020408000, 0x000000000000ff00, 0x8080808080808080, 0x0000000000008040 }},
	{{ 0x0000000000010204, 0x0000000000ff0000, 0x0101010101010101, 0x2010080402010000 }},
	{{ 0x0000000001020408, 0x0000000000ff0000, 0x0202020202020202, 0x4020100804020100 }},
	{{ 0x0000000102040810, 0x0000000000ff0000, 0x0404040404040404, 0x8040201008040201 }},
	{{ 0x0000010204081020, 0x0000000000ff0000, 0x0808080808080808, 0x0080402010080402 }},
	{{ 0x0001020408102040, 0x0000000000ff0000, 0x1010101010101010, 0x0000804020100804 }},
	{{ 0x0102040810204080, 0x0000000000ff0000, 0x2020202020202020, 0x0000008040201008 }},
	{{ 0x0204081020408000, 0x0000000000ff0000, 0x4040404040404040, 0x0000000080402010 }},
	{{ 0x0408102040800000, 0x0000000000ff0000, 0x8080808080808080, 0x0000000000804020 }},
	{{ 0x0000000001020408, 0x00000000ff000000, 0x0101010101010101, 0x1008040201000000 }},
	{{ 0x0000000102040810, 0x00000000ff000000, 0x0202020202020202, 0x2010080402010000 }},
	{{ 0x0000010204081020, 0x00000000ff000000, 0x0404040404040404, 0x4020100804020100 }},
	{{ 0x0001020408102040, 0x00000000ff000000, 0x0808080808080808, 0x8040201008040201 }},
	{{ 0x0102040810204080, 0x00000000ff000000, 0x1010101010101010, 0x0080402010080402 }},
	{{ 0x0204081020408000, 0x00000000ff000000, 0x2020202020202020, 0x0000804020100804 }},
	{{ 0x0408102040800000, 0x00000000ff000000, 0x4040404040404040, 0x0000008040201008 }},
	{{ 0x0810204080000000, 0x00000000ff000000, 0x8080808080808080, 0x0000000080402010 }},
	{{ 0x0000000102040810, 0x000000ff00000000, 0x0101010101010101, 0x0804020100000000 }},
	{{ 0x0000010204081020, 0x000000ff00000000, 0x0202020202020202, 0x1008040201000000 }},
	{{ 0x0001020408102040, 0x000000ff00000000, 0x0404040404040404, 0x2010080402010000 }},
	{{ 0x0102040810204080, 0x000000ff00000000, 0x0808080808080808, 0x4020100804020100 }},
	{{ 0x0204081020408000, 0x000000ff00000000, 0x1010101010101010, 0x8040201008040201 }},
	{{ 0x0408102040800000, 0x000000ff00000000, 0x2020202020202020, 0x0080402010080402 }},
	{{ 0x0810204080000000, 0x000000ff00000000, 0x4040404040404040, 0x0000804020100804 }},
	{{ 0x1020408000000000, 0x000000ff00000000, 0x8080808080808080, 0x0000008040201008 }},
	{{ 0x0000010204081020, 0x0000ff0000000000, 0x0101010101010101, 0x0402010000000000 }},
	{{ 0x0001020408102040, 0x0000ff0000000000, 0x0202020202020202, 0x0804020100000000 }},
	{{ 0x0102040810204080, 0x0000ff0000000000, 0x0404040404040404, 0x1008040201000000 }},
	{{ 0x0204081020408000, 0x0000ff0000000000, 0x0808080808080808, 0x2010080402010000 }},
	{{ 0x0408102040800000, 0x0000ff0000000000, 0x1010101010101010, 0x4020100804020100 }},
	{{ 0x0810204080000000, 0x0000ff0000000000, 0x2020202020202020, 0x8040201008040201 }},
	{{ 0x1020408000000000, 0x0000ff0000000000, 0x4040404040404040, 0x0080402010080402 }},
	{{ 0x2040800000000000, 0x0000ff0000000000, 0x8080808080808080, 0x0000804020100804 }},
	{{ 0x0001020408102040, 0x00ff000000000000, 0x0101010101010101, 0x0201000000000000 }},
	{{ 0x0102040810204080, 0x00ff000000000000, 0x0202020202020202, 0x0402010000000000 }},
	{{ 0x0204081020408000, 0x00ff000000000000, 0x0404040404040404, 0x0804020100000000 }},
	{{ 0x0408102040800000, 0x00ff000000000000, 0x0808080808080808, 0x1008040201000000 }},
	{{ 0x0810204080000000, 0x00ff000000000000, 0x1010101010101010, 0x2010080402010000 }},
	{{ 0x1020408000000000, 0x00ff000000000000, 0x2020202020202020, 0x4020100804020100 }},
	{{ 0x2040800000000000, 0x00ff000000000000, 0x4040404040404040, 0x8040201008040201 }},
	{{ 0x4080000000000000, 0x00ff000000000000, 0x8080808080808080, 0x0080402010080402 }},
	{{ 0x0102040810204080, 0xff00000000000000, 0x0101010101010101, 0x0100000000000000 }},
	{{ 0x0204081020408000, 0xff00000000000000, 0x0202020202020202, 0x0201000000000000 }},
	{{ 0x0408102040800000, 0xff00000000000000, 0x0404040404040404, 0x0402010000000000 }},
	{{ 0x0810204080000000, 0xff00000000000000, 0x0808080808080808, 0x0804020100000000 }},
	{{ 0x1020408000000000, 0xff00000000000000, 0x1010101010101010, 0x1008040201000000 }},
	{{ 0x2040800000000000, 0xff00000000000000, 0x2020202020202020, 0x2010080402010000 }},
	{{ 0x4080000000000000, 0xff00000000000000, 0x4040404040404040, 0x4020100804020100 }},
	{{ 0x8000000000000000, 0xff00000000000000, 0x8080808080808080, 0x8040201008040201 }}
};

/**
 * Count last flipped discs when playing on the last empty.
 *
 * @param pos the last empty square.
 * @param P player's disc pattern.
 * @return flipped disc count.
 */

int last_flip(int pos, unsigned long long P)
{
<<<<<<< HEAD
	uint_fast8_t	n_flips;
	unsigned int	t;
	const uint8_t *COUNT_FLIP_X = COUNT_FLIP[pos & 7];
	const uint8_t *COUNT_FLIP_Y = COUNT_FLIP[pos >> 3];
  #ifdef AVXLASTFLIP	// no gain
	__m256i PP = _mm256_set1_epi64x(P);

	n_flips  = COUNT_FLIP_X[(P >> (pos & 0x38)) & 0xFF];
    #ifdef __AVX512VL__
    	t = _cvtmask32_u32(_mm256_test_epi8_mask(PP, mask_dvhd[pos].v4));
    #else
	t = _mm256_movemask_epi8(_mm256_sub_epi8(_mm256_setzero_si256(), _mm256_and_si256(PP, mask_dvhd[pos].v4)));
    #endif
	n_flips += COUNT_FLIP_Y[t & 0xFF];
	t >>= 16;

  #else
	__m128i PP = _mm_set1_epi64x(P);
	__m128i II = _mm_sad_epu8(_mm_and_si128(PP, mask_dvhd[pos].v2[0]), _mm_setzero_si128());

	n_flips  = COUNT_FLIP_X[_mm_extract_epi16(II, 4)];
	n_flips += COUNT_FLIP_X[_mm_cvtsi128_si32(II)];
    #ifdef __AVX512VL__
    	t = _cvtmask16_u32(_mm_test_epi8_mask(PP, mask_dvhd[pos].v2[1]));
    #else
	t = _mm_movemask_epi8(_mm_sub_epi8(_mm_setzero_si128(), _mm_and_si128(PP, mask_dvhd[pos].v2[1])));
    #endif
  #endif
	n_flips += COUNT_FLIP_Y[t >> 8];
	n_flips += COUNT_FLIP_Y[t & 0xFF];
=======
	unsigned char	n_flips;
	unsigned int	t;
	const unsigned char *COUNT_FLIP_X = COUNT_FLIP[pos & 7];
	const unsigned char *COUNT_FLIP_Y = COUNT_FLIP[(unsigned int) pos >> 3];	// cast to reduce movslq
#ifdef AVXLASTFLIP
	__m256i	MP = _mm256_and_si256(_mm256_broadcastq_epi64(_mm_cvtsi64_si128(P)), mask_dvhd[pos].v4);

	n_flips  = COUNT_FLIP_X[(P >> (pos & 0x38)) & 0xff];
	t = _mm256_movemask_epi8(_mm256_sub_epi8(_mm256_setzero_si256(), MP));
	n_flips += COUNT_FLIP_Y[t & 0xff];
	t >>= 16;
#else
	__m128i	PP, II;

	PP = _mm_cvtsi64_si128(P);
	PP = _mm_unpacklo_epi64(PP, PP);
	II = _mm_sad_epu8(_mm_and_si128(PP, mask_dvhd[pos].v2[0]), _mm_setzero_si128());
	n_flips  = COUNT_FLIP_X[_mm_extract_epi16(II, 4)];
	n_flips += COUNT_FLIP_X[(unsigned int) _mm_cvtsi128_si32(II)];
	t = _mm_movemask_epi8(_mm_sub_epi8(_mm_setzero_si128(), _mm_and_si128(PP, mask_dvhd[pos].v2[1])));
#endif
	n_flips += COUNT_FLIP_Y[t >> 8];
<<<<<<< HEAD
	n_flips += COUNT_FLIP_Y[(unsigned char) t];
>>>>>>> 3e1ed4f (fix cr/lf in repository to lf)
=======
	n_flips += COUNT_FLIP_Y[t & 0xff];
>>>>>>> 26dad03 (Use player bits only in board_score_1)

	return n_flips;
}

<<<<<<< HEAD
=======
/**
 * @file count_last_flip_sse.c
 *
 *
 * A function is provided to count the number of fipped disc of the last move.
 *
 * The basic principle is to read into an array a precomputed result. Doing
 * this is easy for a single line ; as we can use arrays of the form:
 *  - COUNT_FLIP[square where we play][8-bits disc pattern].
 * The problem is thus to convert any line of a 64-bits disc pattern into an
 * 8-bits disc pattern. A fast way to do this is to select the right line,
 * with a bit-mask, to gather the masked-bits into a continuous set by the 
 * SSE PMOVMSKB or PSADBW instruction.
 * Once we get our 8-bits disc patterns, we directly get the number of
 * flipped discs from the precomputed array, and add them from each flipping
 * lines.
 * For optimization purpose, the value returned is twice the number of flipped
 * disc, to facilitate the computation of disc difference.
 *
 * @date 1998 - 2020
 * @author Richard Delorme
 * @author Toshihiko Okuhara
 * @version 4.4
 * 
 */

#include "bit.h"

/** precomputed count flip array */
const unsigned char COUNT_FLIP[8][256] = {
	{
		 0,  0,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		12, 12,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
		 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	},
	{
		 0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		10, 10, 10, 10,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
		 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	},
	{
		 0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 8, 10,  8,  8,  8, 10,  8,  8,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
		 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	},
	{
		 0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 6, 10,  8,  8,  6,  6,  6,  6,  6, 10,  8,  8,  6,  6,  6,  6,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
		 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	},
	{
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
		 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	{
		 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
};

/* bit masks for diagonal lines */
const V4DI mask_dvhd[64] = {
	{{ 0x0000000000000001, 0x00000000000000ff, 0x0101010101010101, 0x8040201008040201 }},
	{{ 0x0000000000000102, 0x00000000000000ff, 0x0202020202020202, 0x0080402010080402 }},
	{{ 0x0000000000010204, 0x00000000000000ff, 0x0404040404040404, 0x0000804020100804 }},
	{{ 0x0000000001020408, 0x00000000000000ff, 0x0808080808080808, 0x0000008040201008 }},
	{{ 0x0000000102040810, 0x00000000000000ff, 0x1010101010101010, 0x0000000080402010 }},
	{{ 0x0000010204081020, 0x00000000000000ff, 0x2020202020202020, 0x0000000000804020 }},
	{{ 0x0001020408102040, 0x00000000000000ff, 0x4040404040404040, 0x0000000000008040 }},
	{{ 0x0102040810204080, 0x00000000000000ff, 0x8080808080808080, 0x0000000000000080 }},
	{{ 0x0000000000000102, 0x000000000000ff00, 0x0101010101010101, 0x4020100804020100 }},
	{{ 0x0000000000010204, 0x000000000000ff00, 0x0202020202020202, 0x8040201008040201 }},
	{{ 0x0000000001020408, 0x000000000000ff00, 0x0404040404040404, 0x0080402010080402 }},
	{{ 0x0000000102040810, 0x000000000000ff00, 0x0808080808080808, 0x0000804020100804 }},
	{{ 0x0000010204081020, 0x000000000000ff00, 0x1010101010101010, 0x0000008040201008 }},
	{{ 0x0001020408102040, 0x000000000000ff00, 0x2020202020202020, 0x0000000080402010 }},
	{{ 0x0102040810204080, 0x000000000000ff00, 0x4040404040404040, 0x0000000000804020 }},
	{{ 0x0204081020408000, 0x000000000000ff00, 0x8080808080808080, 0x0000000000008040 }},
	{{ 0x0000000000010204, 0x0000000000ff0000, 0x0101010101010101, 0x2010080402010000 }},
	{{ 0x0000000001020408, 0x0000000000ff0000, 0x0202020202020202, 0x4020100804020100 }},
	{{ 0x0000000102040810, 0x0000000000ff0000, 0x0404040404040404, 0x8040201008040201 }},
	{{ 0x0000010204081020, 0x0000000000ff0000, 0x0808080808080808, 0x0080402010080402 }},
	{{ 0x0001020408102040, 0x0000000000ff0000, 0x1010101010101010, 0x0000804020100804 }},
	{{ 0x0102040810204080, 0x0000000000ff0000, 0x2020202020202020, 0x0000008040201008 }},
	{{ 0x0204081020408000, 0x0000000000ff0000, 0x4040404040404040, 0x0000000080402010 }},
	{{ 0x0408102040800000, 0x0000000000ff0000, 0x8080808080808080, 0x0000000000804020 }},
	{{ 0x0000000001020408, 0x00000000ff000000, 0x0101010101010101, 0x1008040201000000 }},
	{{ 0x0000000102040810, 0x00000000ff000000, 0x0202020202020202, 0x2010080402010000 }},
	{{ 0x0000010204081020, 0x00000000ff000000, 0x0404040404040404, 0x4020100804020100 }},
	{{ 0x0001020408102040, 0x00000000ff000000, 0x0808080808080808, 0x8040201008040201 }},
	{{ 0x0102040810204080, 0x00000000ff000000, 0x1010101010101010, 0x0080402010080402 }},
	{{ 0x0204081020408000, 0x00000000ff000000, 0x2020202020202020, 0x0000804020100804 }},
	{{ 0x0408102040800000, 0x00000000ff000000, 0x4040404040404040, 0x0000008040201008 }},
	{{ 0x0810204080000000, 0x00000000ff000000, 0x8080808080808080, 0x0000000080402010 }},
	{{ 0x0000000102040810, 0x000000ff00000000, 0x0101010101010101, 0x0804020100000000 }},
	{{ 0x0000010204081020, 0x000000ff00000000, 0x0202020202020202, 0x1008040201000000 }},
	{{ 0x0001020408102040, 0x000000ff00000000, 0x0404040404040404, 0x2010080402010000 }},
	{{ 0x0102040810204080, 0x000000ff00000000, 0x0808080808080808, 0x4020100804020100 }},
	{{ 0x0204081020408000, 0x000000ff00000000, 0x1010101010101010, 0x8040201008040201 }},
	{{ 0x0408102040800000, 0x000000ff00000000, 0x2020202020202020, 0x0080402010080402 }},
	{{ 0x0810204080000000, 0x000000ff00000000, 0x4040404040404040, 0x0000804020100804 }},
	{{ 0x1020408000000000, 0x000000ff00000000, 0x8080808080808080, 0x0000008040201008 }},
	{{ 0x0000010204081020, 0x0000ff0000000000, 0x0101010101010101, 0x0402010000000000 }},
	{{ 0x0001020408102040, 0x0000ff0000000000, 0x0202020202020202, 0x0804020100000000 }},
	{{ 0x0102040810204080, 0x0000ff0000000000, 0x0404040404040404, 0x1008040201000000 }},
	{{ 0x0204081020408000, 0x0000ff0000000000, 0x0808080808080808, 0x2010080402010000 }},
	{{ 0x0408102040800000, 0x0000ff0000000000, 0x1010101010101010, 0x4020100804020100 }},
	{{ 0x0810204080000000, 0x0000ff0000000000, 0x2020202020202020, 0x8040201008040201 }},
	{{ 0x1020408000000000, 0x0000ff0000000000, 0x4040404040404040, 0x0080402010080402 }},
	{{ 0x2040800000000000, 0x0000ff0000000000, 0x8080808080808080, 0x0000804020100804 }},
	{{ 0x0001020408102040, 0x00ff000000000000, 0x0101010101010101, 0x0201000000000000 }},
	{{ 0x0102040810204080, 0x00ff000000000000, 0x0202020202020202, 0x0402010000000000 }},
	{{ 0x0204081020408000, 0x00ff000000000000, 0x0404040404040404, 0x0804020100000000 }},
	{{ 0x0408102040800000, 0x00ff000000000000, 0x0808080808080808, 0x1008040201000000 }},
	{{ 0x0810204080000000, 0x00ff000000000000, 0x1010101010101010, 0x2010080402010000 }},
	{{ 0x1020408000000000, 0x00ff000000000000, 0x2020202020202020, 0x4020100804020100 }},
	{{ 0x2040800000000000, 0x00ff000000000000, 0x4040404040404040, 0x8040201008040201 }},
	{{ 0x4080000000000000, 0x00ff000000000000, 0x8080808080808080, 0x0080402010080402 }},
	{{ 0x0102040810204080, 0xff00000000000000, 0x0101010101010101, 0x0100000000000000 }},
	{{ 0x0204081020408000, 0xff00000000000000, 0x0202020202020202, 0x0201000000000000 }},
	{{ 0x0408102040800000, 0xff00000000000000, 0x0404040404040404, 0x0402010000000000 }},
	{{ 0x0810204080000000, 0xff00000000000000, 0x0808080808080808, 0x0804020100000000 }},
	{{ 0x1020408000000000, 0xff00000000000000, 0x1010101010101010, 0x1008040201000000 }},
	{{ 0x2040800000000000, 0xff00000000000000, 0x2020202020202020, 0x2010080402010000 }},
	{{ 0x4080000000000000, 0xff00000000000000, 0x4040404040404040, 0x4020100804020100 }},
	{{ 0x8000000000000000, 0xff00000000000000, 0x8080808080808080, 0x8040201008040201 }}
};

/**
 * Count last flipped discs when playing on the last empty.
 *
 * @param pos the last empty square.
 * @param P player's disc pattern.
 * @return flipped disc count.
 */

int last_flip(int pos, unsigned long long P)
{
	unsigned char	n_flips;
	unsigned int	t;
	const unsigned char *COUNT_FLIP_X = COUNT_FLIP[pos & 7];
	const unsigned char *COUNT_FLIP_Y = COUNT_FLIP[pos >> 3];
#ifdef AVXLASTFLIP
	__m256i	MP = _mm256_and_si256(_mm256_broadcastq_epi64(_mm_cvtsi64_si128(P)), mask_dvhd[pos].v4);

	n_flips  = COUNT_FLIP_X[(unsigned char) (P >> (pos & 0x38))];
	t = _mm256_movemask_epi8(_mm256_sub_epi8(_mm256_setzero_si256(), MP));
	n_flips += COUNT_FLIP_Y[(unsigned char) t];
	t >>= 16;
#else
	__m128i	PP, II;

	PP = _mm_cvtsi64_si128(P);
	PP = _mm_unpacklo_epi64(PP, PP);
	II = _mm_sad_epu8(_mm_and_si128(PP, mask_dvhd[pos].v2[0]), _mm_setzero_si128());
	n_flips  = COUNT_FLIP_X[_mm_cvtsi128_si32(II)];
	n_flips += COUNT_FLIP_X[_mm_extract_epi16(II, 4)];
	t = _mm_movemask_epi8(_mm_sub_epi8(_mm_setzero_si128(), _mm_and_si128(PP, mask_dvhd[pos].v2[1])));
#endif
	n_flips += COUNT_FLIP_Y[t >> 8];
	n_flips += COUNT_FLIP_Y[(unsigned char) t];

	return n_flips;
}
<<<<<<< HEAD
>>>>>>> 1dc032e (Improve visual c compatibility)
=======

>>>>>>> 6506166 (More SSE optimizations)
=======
>>>>>>> 3e1ed4f (fix cr/lf in repository to lf)
