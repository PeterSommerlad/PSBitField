# PSBitField ![tests](https://github.com/PeterSommerlad/PSBitField/actions/workflows/psbitfieldtests.yml/badge.svg)

A safe and guaranteed order bitfield helper to put in a union for accessing hardware device registers.
(If you just keep "bitfield" entries of the same size in a union).


This is a bitfield implementation to be used within unions for device registers
where the bits are positioned absolutely with the least-significant-bit has from-index 0
using the aliases with the _v makes the data members volatile to prevent optimiazations
but note, that setting inidividual fields mean both reading and writing the word
accessing individual union members is sanctioned, because they have the same `struct`-ure.

Note, that `volatile` of the union variable is propagated, so no need to instantiate with `uint16_t volatile`.

Possible misuse: mixing `psbf::allbitsN` with `psbf::bitsM` in a union where N and M differ. This will be undefined behavior!


Note: this library is inspired by https://stackoverflow.com/questions/31726191/is-there-a-portable-alternative-to-c-bitfields and an observation at a client who had actual undefined behavior in its application.

## Usage

```C++
#include "psbitfield.h"

union MyReg16 {
  template<uint8_t from, uint8_t width>
    using bf =  psbf::bits16<from,width>; // shorthand below
  psbf::allbits16 word; // should be the first to ensure init: MyReg16 reg{};
  bf<0,4> firstnibble;
  bf<4,1> fourthbit;
  bf<5,3> threebits;
  bf<8,8> secondbyte;
};

void testDemonstrateUsage(){
  MyReg16 volatile var{}; // at mmio address
  var.firstnibble = 42;
  std::cout << std::hex << var.word;
  var.fourthbit = 1;
  var.threebits = 5;
  std::cout << var.word;
  unsigned x = var.secondbyte  ;
  std::cout << x;
  //var.threebits = 8; // asserts
}

```


## reference

  - read a bitfield member as unsigned (implicit or explicit conversion)
  - assign to a bitfield member from an unsigned value the size of the allbits part, the actual value assigned must not exceed the representable value of the bitfield!
  

```C++
namespace psbf {
...
// for use as first union member
using allbits64 = ...;
using allbits32 = ...;
using allbits16 = ...;
using allbits8  = ...;

// use the matching number of bits for individual fields:

template<uint8_t from, uint8_t width>
using bits8 = bitfield<from,width,uint8_t>;
template<uint8_t from, uint8_t width>
using bits16 = bitfield<from,width,uint16_t>;
template<uint8_t from, uint8_t width>
using bits32 = bitfield<from,width,uint32_t>;
template<uint8_t from, uint8_t width>
using bits64 = bitfield<from,width,uint64_t>;
}
```

