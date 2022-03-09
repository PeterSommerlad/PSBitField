#include "psbitfield.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"


union TestField32 {
	template<uint8_t from, uint8_t width>
	using bf=psbf::bitfield<from,width>;
	psbf::allbits<> word;
	bf<0,4> firstnibble;
	bf<4,1> fourthbit;
	bf<5,3> threebits;
	bf<8,8> secondbyte;
	bf<16,16> ashort;
};

void testDefaultInitIsActuallyZero() {
	TestField32 volatile field{};
	ASSERT_EQUAL(01u,field.word);
}
void testNonZeroInitRemains(){
	TestField32 field{{42u}};
	ASSERT_EQUAL(42u,field.word);
}
void testAccessingAllSetBitsIsCorrect(){
	TestField32 field{{0xffff'ffffu}};
	ASSERT_EQUAL(0xffff'ffffu, field.word);
	ASSERT_EQUAL(0xfu, field.firstnibble);
	ASSERT_EQUAL(0b1u, field.fourthbit);
	ASSERT_EQUAL(0b111u, field.threebits);
	ASSERT_EQUAL(255u, field.secondbyte);
	ASSERT_EQUAL(0xffffu, field.ashort);
}
void testWritingBitsInAllSetBitsClearsBits(){
	TestField32 field{{0xffff'ffffu}};
	field.ashort = 10;
	ASSERT_EQUAL(0xA'ffffu, field.word);
}
void testWritingBitInAllSetBitsClearsBits(){
	TestField32 field{{0xffff'ffffu}};
	field.fourthbit = 0;
	ASSERT_EQUAL(0xffff'ffefu, field.word);
}

void testWritingBitsInAllClearBitsSetsBits(){
	TestField32 volatile field{};
	field.threebits=5;
	ASSERT_EQUAL(5u<<5,field.word);
	ASSERT_EQUAL(5u,field.threebits);
}

void testWritingMultipleFieldsInAllClearBitsSetsBits(){
	TestField32  field{};
	field.firstnibble = 0b1010u;
	field.fourthbit = 1;
	field.threebits = 0b10;
	field.secondbyte = 0xA5u;
	field.ashort = 0xAFFEu;
	ASSERT_EQUAL(0xAFFE'A55Au,field.word);
}
namespace b64 {
union TestField {
	template<uint8_t from, uint8_t width>
	using bf=psbf::bits64<from,width>;
	psbf::allbits64 word;
	bf<0,4> firstnibble;
	bf<4,1> fourthbit;
	bf<5,3> threebits;
	bf<8,8> secondbyte;
	bf<16,16> ashort;
	bf<32,32> dword;
};

void testDefaultInitIsActuallyZero() {
	TestField volatile field{};
	ASSERT_EQUAL(0u,field.word);
}
void testNonZeroInitRemains(){
	TestField field{{42u}};
	ASSERT_EQUAL(42u,field.word);
}
void testAccessingAllSetBitsIsCorrect(){
	TestField field{{0xffff'ffff'ffff'ffffu}};
	ASSERT_EQUAL(0xffff'ffff'ffff'ffffu, field.word);
	ASSERT_EQUAL(0xfu, field.firstnibble);
	ASSERT_EQUAL(0b1u, field.fourthbit);
	ASSERT_EQUAL(0b111u, field.threebits);
	ASSERT_EQUAL(255u, field.secondbyte);
	ASSERT_EQUAL(0xffffu, field.ashort);
	ASSERT_EQUAL(0xffff'ffffu, field.dword);
}
void testWritingBitsInAllSetBitsClearsBits(){
	TestField field{{0xffff'ffff'ffff'ffffu}};
	field.ashort = 10;
	ASSERT_EQUAL(0xffff'ffff'000A'ffffu, field.word);
}
void testWritingBitInAllSetBitsClearsBits(){
	TestField field{{0xffff'ffff'ffff'ffffu}};
	field.fourthbit = 0;
	ASSERT_EQUAL(0xffff'ffff'ffff'ffefu, field.word);
}

void testWritingBitsInAllClearBitsSetsBits(){
	TestField volatile field{};
	field.threebits=5;
	ASSERT_EQUAL(5u<<5,field.word);
	ASSERT_EQUAL(5u,field.threebits);
}

void testWritingMultipleFieldsInAllClearBitsSetsBits(){
	TestField  field{};
	field.firstnibble = 0b1010u;
	field.fourthbit = 1;
	field.threebits = 0b10;
	field.secondbyte = 0xA5u;
	field.ashort = 0xAFFEu;
	field.dword = 0xDEAD'BEEFu;
	ASSERT_EQUAL(0xDEAD'BEEF'AFFE'A55Au,field.word);
}

}

namespace b16 {
union TestField {
	template<uint8_t from, uint8_t width>
	using bf=psbf::bits16<from,width>;
	psbf::allbits16 word;
	bf<0,4> firstnibble;
	bf<4,1> fourthbit;
	bf<5,3> threebits;
	bf<8,8> secondbyte;
};

void testDefaultInitIsActuallyZero() {
	TestField volatile field{};
	ASSERT_EQUAL(0u,field.word);
}
void testNonZeroInitRemains(){
	TestField field{{42u}};
	ASSERT_EQUAL(42u,field.word);
}
void testAccessingAllSetBitsIsCorrect(){
	TestField field{{0xffffu}};
	ASSERT_EQUAL(0xffffu, field.word);
	ASSERT_EQUAL(0xfu, field.firstnibble);
	ASSERT_EQUAL(0b1u, field.fourthbit);
	ASSERT_EQUAL(0b111u, field.threebits);
	ASSERT_EQUAL(255u, field.secondbyte);
}
void testWritingBitsInAllSetBitsClearsBits(){
	TestField field{{0xffffu}};
	field.secondbyte = 10;
	ASSERT_EQUAL(0x0A'ffu, field.word);
}
void testWritingBitInAllSetBitsClearsBits(){
	TestField field{{0xffffu}};
	field.fourthbit = 0;
	ASSERT_EQUAL(0xffefu, field.word);
}

void testWritingBitsInAllClearBitsSetsBits(){
	TestField volatile field{};
	field.threebits=5;
	ASSERT_EQUAL(5u<<5,field.word);
	ASSERT_EQUAL(5u,field.threebits);
}

void testWritingMultipleFieldsInAllClearBitsSetsBits(){
	TestField  field{};
	field.firstnibble = 0b1010u;
	field.fourthbit = 1;
	field.threebits = 0b10;
	field.secondbyte = 0xA5u;
	ASSERT_EQUAL(0xA55Au,field.word);
}

}
namespace b8 {
union TestField {
	template<uint8_t from, uint8_t width>
	using bf=psbf::bits8<from,width>;
	psbf::allbits8 word;
	bf<0,4> firstnibble;
	bf<4,1> fourthbit;
	bf<5,3> threebits;
};

void testDefaultInitIsActuallyZero() {
	TestField volatile field{};
	ASSERT_EQUAL(0u,field.word);
}
void testNonZeroInitRemains(){
	TestField field{{42u}};
	ASSERT_EQUAL(42u,field.word);
}
void testAccessingAllSetBitsIsCorrect(){
	TestField field{{0xffu}};
	ASSERT_EQUAL(0xffu, field.word);
	ASSERT_EQUAL(0xfu, field.firstnibble);
	ASSERT_EQUAL(0b1u, field.fourthbit);
	ASSERT_EQUAL(0b111u, field.threebits);
}
void testWritingBitsInAllSetBitsClearsBits(){
	TestField field{{0xffu}};
	field.threebits = 0b101;
	field.fourthbit = 0;
	ASSERT_EQUAL(0xAfu, field.word);
}
void testWritingBitInAllSetBitsClearsBits(){
	TestField field{{0xffu}};
	field.fourthbit = 0;
	ASSERT_EQUAL(0xefu, field.word);
}

void testWritingBitsInAllClearBitsSetsBits(){
	TestField volatile field{};
	field.threebits=5;
	ASSERT_EQUAL(5u<<5,field.word);
	ASSERT_EQUAL(5u,field.threebits);
}

void testWritingMultipleFieldsInAllClearBitsSetsBits(){
	TestField  field{};
	field.firstnibble = 0b1010u;
	field.fourthbit = 1;
	field.threebits = 0b10;
	ASSERT_EQUAL(0x5Au,field.word);
}

}

namespace demonstration{
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
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(testDefaultInitIsActuallyZero));
	s.push_back(CUTE(testNonZeroInitRemains));

	s.push_back(CUTE(testAccessingAllSetBitsIsCorrect));
	s.push_back(CUTE(testWritingBitsInAllSetBitsClearsBits));
	s.push_back(CUTE(testWritingBitInAllSetBitsClearsBits));
	s.push_back(CUTE(testWritingBitsInAllClearBitsSetsBits));
	s.push_back(CUTE(testWritingMultipleFieldsInAllClearBitsSetsBits));
	s.push_back(CUTE(b64::testDefaultInitIsActuallyZero));
	s.push_back(CUTE(b64::testNonZeroInitRemains));
	s.push_back(CUTE(b64::testAccessingAllSetBitsIsCorrect));
	s.push_back(CUTE(b64::testWritingBitsInAllSetBitsClearsBits));
	s.push_back(CUTE(b64::testWritingBitInAllSetBitsClearsBits));
	s.push_back(CUTE(b64::testWritingBitsInAllClearBitsSetsBits));
	s.push_back(CUTE(b64::testWritingMultipleFieldsInAllClearBitsSetsBits));
	s.push_back(CUTE(b16::testDefaultInitIsActuallyZero));
	s.push_back(CUTE(b16::testNonZeroInitRemains));
	s.push_back(CUTE(b16::testAccessingAllSetBitsIsCorrect));
	s.push_back(CUTE(b16::testWritingBitsInAllSetBitsClearsBits));
	s.push_back(CUTE(b16::testWritingBitInAllSetBitsClearsBits));
	s.push_back(CUTE(b16::testWritingBitsInAllClearBitsSetsBits));
	s.push_back(CUTE(b16::testWritingMultipleFieldsInAllClearBitsSetsBits));
	s.push_back(CUTE(b8::testDefaultInitIsActuallyZero));
	s.push_back(CUTE(b8::testNonZeroInitRemains));
	s.push_back(CUTE(b8::testAccessingAllSetBitsIsCorrect));
	s.push_back(CUTE(b8::testWritingBitsInAllSetBitsClearsBits));
	s.push_back(CUTE(b8::testWritingBitInAllSetBitsClearsBits));
	s.push_back(CUTE(b8::testWritingBitsInAllClearBitsSetsBits));
	s.push_back(CUTE(b8::testWritingMultipleFieldsInAllClearBitsSetsBits));	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
