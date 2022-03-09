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
//		using bf =  psbf::bits16_v<from,width>; // shorthand below
//		psbf::allbits16v word; // should be the first to ensure init: MyReg16 reg{};
//		bf<0,4> firstnibble;
//		bf<4,1> fourthbit;
//		bf<5,3> threebits;
//		bf<8,8> secondbyte;
//	};
//
//	MyReg16 var{}; // at mmio address
//	var.firstnibble = 42;
//	std::cout << std::hex << var.word;
//	var.fourthbit = 1;
//	var.threebits = 5;
//	std::cout << var.word;
//	auto x { var.secondbyte } ;

namespace psbf {

template<uint8_t from, uint8_t width, typename UINT=uint32_t>
struct bitfield{
	using result_type = std::remove_volatile_t<UINT>;
	static_assert(std::numeric_limits<UINT>::is_integer && ! std::numeric_limits<UINT>::is_signed, "must use unsigned bitfield base type");
	static_assert(std::numeric_limits<UINT>::digits == sizeof(UINT)*CHAR_BIT);
	static constexpr inline uint8_t wordsize = sizeof(UINT)*CHAR_BIT;
	static constexpr inline result_type widthmask = (width == wordsize)?result_type(-1):(result_type(1)<<width)-1;
	static constexpr inline result_type mask = (result_type(-1) >> (wordsize-width)) << from; // we have two's complement!
	static_assert(widthmask ==  (mask>>from) );
	static_assert((width==wordsize?result_type(-1):(result_type(1u)<<width)-result_type(1u))==(from > 0? mask>>from: mask));
	static_assert(from < wordsize, "starting position too big");
	static_assert(from+width <= wordsize, "bitfield too wide");
	static_assert(width>0, "zero-size bitfields not supported");

	UINT  allbits; // UINT could/should be volatile
	constexpr
	operator result_type() const volatile { return (allbits & mask) >> from;}
	constexpr
	operator result_type() const  { return (allbits & mask) >> from;}
	constexpr auto& operator=(result_type newval) volatile & {
		assert(0==(newval& ~widthmask));
		allbits = (allbits & ~mask) | ((newval&widthmask)<<from);
		return *this;
	}
	friend std::ostream & operator<<(std::ostream &out,bitfield const volatile &me){
		return out << static_cast<UINT>(me);
	}
	friend std::ostream & operator<<(std::ostream &out,bitfield const  &me){
		return out << static_cast<UINT>(me);
	}
};

template<typename UINT=uint32_t>
using allbits=bitfield<0,std::numeric_limits<UINT>::digits,UINT>;

// for use as first union member
using allbits64=allbits<uint64_t >;
using allbits32=allbits<uint32_t >;
using allbits16=allbits<uint16_t >;
using allbits8 =allbits<uint8_t >;
using allbits64v=allbits<uint64_t volatile>;
using allbits32v=allbits<uint32_t volatile>;
using allbits16v=allbits<uint16_t volatile>;
using allbits8v =allbits<uint8_t volatile>;

template<uint8_t from, uint8_t width>
using bits8 = bitfield<from,width,uint8_t>;
template<uint8_t from, uint8_t width>
using bits8_v = bitfield<from,width,uint8_t volatile>;
template<uint8_t from, uint8_t width>
using bits16 = bitfield<from,width,uint16_t>;
template<uint8_t from, uint8_t width>
using bits16_v = bitfield<from,width,uint16_t volatile>;
template<uint8_t from, uint8_t width>
using bits32 = bitfield<from,width,uint32_t>;
template<uint8_t from, uint8_t width>
using bits32_v = bitfield<from,width,uint32_t volatile>;
template<uint8_t from, uint8_t width>
using bits64 = bitfield<from,width,uint64_t>;
template<uint8_t from, uint8_t width>
using bits64_v = bitfield<from,width,uint64_t volatile>;

}



#endif /* PSBITFIELD_H_ */
