#pragma once

#define bf_mask(__numbits) ((1ULL << __numbits) - 1)

#define bf_extract(__value, __bitpos, __bitwidth) \
	((__value >> __bitpos) & bf_mask(__bitwidth))

#define bf_insert(__value, __bitpos, __maskbits) \
	(((unsigned long long)__value & bf_mask(__maskbits)) << __bitpos)

#define bf_test(__value, __bit) ((__value & (1<<__bit)) ? 1 : 0)
