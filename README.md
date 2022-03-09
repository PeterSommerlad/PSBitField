# PSBitField ![tests](https://github.com/PeterSommerlad/PSBitField/actions/workflows/psbitfieldtests.yml/badge.svg)

A safe and guaranteed order bitfield helper to put in a union for accessing hardware device registers.
(If you just keep "bitfield" entries of the same size in a union).


this is a bitfield implementation to be used within unions for device registers
where the bits are positioned absolutely with the least-significant-bit has from-index 0
using the aliases with the _v makes the data members volatile to prevent optimiazations
but note, that setting inidividual fields mean both reading and writing the word
accessing individual union members is sanctioned, because they have the same `struct`-ure.


Note: this library is inspired by https://stackoverflow.com/questions/31726191/is-there-a-portable-alternative-to-c-bitfields and an observation at a client who had actual undefined behavior in its application.

## Usage

```C++
#include "psbitfield.h"

union MyReg16 {
  template<uint8_t from, uint8_t width>
    using bf =  psbf::bits16_v<from,width>; // shorthand below
  psbf::allbits16v word; // should be the first to ensure init: MyReg16 reg{};
  bf<0,4> firstnibble;
  bf<4,1> fourthbit;
  bf<5,3> threebits;
  bf<8,8> secondbyte;
};

void testDemonstrateUsage(){
  MyReg16 var{}; // at mmio address
  var.firstnibble = 42;
  std::cout << std::hex << var.word;
  var.fourthbit = 1;
  var.threebits = 5;
  std::cout << var.word;
  auto x { var.secondbyte } ;
  std::cout << x;
}

```


## reference
```C++
namespace psbf {
...
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
```

