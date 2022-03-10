#ifndef PSBITFIELD_H_
#define PSBITFIELD_H_

#include <cstdint>
#include <climits>
#include <type_traits>
#include <limits>
#include <cassert>
#include <ostream>


// this is a bitfield implementation to be used within unions for device registers
// where the bits are positioned absolutely with the least-significant-bit has from-index 0
// using the aliases with the _v makes the data members volatile to prevent optimiazations
// but note, that setting inidividual fields mean both reading and writing the word
// accessing individual union members is sanctioned, because they have the same struct-ure.
//
// Usage:
//
//	union MyReg16 {
//		template<uint8_t from, uint8_t width>
//	    using bf =  psbf::bits16<from,width>; // shorthand below
//		psbf::allbits16 word; // should be the first to ensure init: MyReg16 reg{};
//		bf<0,4> firstnibble;
//		bf<4,1> fourthbit;
//		bf<5,3> threebits;
//		bf<8,8> secondbyte;
//	};
//	MyReg16 volatile var{}; // at mmio address
//	var.firstnibble = 42;
//	std::cout << std::hex << var.word;
//	var.fourthbit = 1;
//	var.threebits = 5;
//	std::cout << var.word;
//	unsigned const x = var.secondbyte; // would not deduce integer type
//	std::cout << x;
//  // x.threebits = 8; // asserts!
//
//  volatile from the outer variable is propagated!
//  all bitfield accesses are unsigned


namespace psbf {

template<uint8_t from, uint8_t width, typename UINT=uint32_t>
struct bitfield{
	using result_type = std::remove_volatile_t<UINT>;
	using as_volatile = std::conditional_t<std::is_volatile_v<UINT>,UINT,UINT volatile>;
	using expr_type = std::conditional_t<sizeof(result_type)<=sizeof(unsigned),unsigned,result_type >;
	static_assert(std::numeric_limits<UINT>::is_integer && ! std::numeric_limits<UINT>::is_signed, "must use unsigned bitfield base type");
	static_assert(std::numeric_limits<UINT>::digits == sizeof(UINT)*CHAR_BIT);
	static constexpr inline uint8_t wordsize = sizeof(UINT)*CHAR_BIT;
	static constexpr inline expr_type widthmask = (width == wordsize)?result_type(-1):(result_type(1)<<width)-1;
	static constexpr inline expr_type mask = (result_type(-1) >> (wordsize-width)) << from; // we have two's complement!
	static_assert(widthmask ==  (mask>>from) );
	static_assert((width==wordsize?result_type(-1):(result_type(1u)<<width)-result_type(1u))==(from > 0? mask>>from: mask));
	static_assert(from < wordsize, "starting position too big");
	static_assert(from+width <= wordsize, "bitfield too wide");
	static_assert(width>0, "zero-size bitfields not supported");
	as_volatile& allbitsvolatileforwrite() volatile & {
		return const_cast<as_volatile&>(allbits);
	}
	as_volatile const & allbitsvolatileforread() const volatile  {
		return const_cast<as_volatile const&>(allbits);
	}

	UINT  allbits; // UINT could/should be volatile
	operator result_type() const volatile { return (expr_type(allbitsvolatileforread()) & mask) >> from;}
	constexpr
	operator result_type() const  { return (expr_type(allbits) & mask) >> from;}

	void operator=(result_type newval) volatile & { // don't support chaining!
		assert(0==(newval& ~widthmask));
		allbitsvolatileforwrite() = UINT((expr_type(allbitsvolatileforread()) & ~mask ) | ((expr_type(newval)&widthmask)<<from));
	}
	constexpr void operator=(result_type newval)  & { // don't support chaining!
		assert(0==(newval& ~widthmask));
		allbits = UINT((expr_type(allbits) & ~mask) | ((expr_type(newval)&widthmask)<<from));
	}
};

namespace detail{
template<typename UINT=uint32_t>
using allbits=bitfield<0,std::numeric_limits<UINT>::digits,UINT>;
}

// for use as first union member
using allbits64 = detail::allbits<uint64_t >;
using allbits32 = detail::allbits<uint32_t >;
using allbits16 = detail::allbits<uint16_t >;
using allbits8  = detail::allbits<uint8_t >;

template<uint8_t from, uint8_t width>
using bits8 = bitfield<from,width,uint8_t>;
template<uint8_t from, uint8_t width>
using bits16 = bitfield<from,width,uint16_t>;
template<uint8_t from, uint8_t width>
using bits32 = bitfield<from,width,uint32_t>;
template<uint8_t from, uint8_t width>
using bits64 = bitfield<from,width,uint64_t>;

}



#endif /* PSBITFIELD_H_ */
