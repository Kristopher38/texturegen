//## The quartet of simple PRNGs invented by George Marsagalia.
 //##
  //## It goes: MWC + CONG + SHR3 + KISS

typedef unsigned int ui32; // sizeof(ui32) must be 4
typedef float fp32;        // 4byte-wide float
#define __maxRngValue 0xFFFFFFFFUL

//==[ Multiply-With-Carry ]====================================================
//  period: 2^60
//  description:
//    The MWC generator concatenates two 16-bit multiply-
//    with-carry generators, x(n)=36969x(n-1)+carry,
//    y(n)=18000y(n-1)+carry mod 2^16, has period about
//    2^60 and seems to pass all tests of randomness. A
//    favourite stand-alone generator---faster than KISS,
//    which contains it.
//
//=============================================================================
typedef struct SeqMWC
{
	ui32 z, w, value;
} SeqMWC;

void SeqMWC_seed(SeqMWC* mwc, ui32 zSeed, ui32 wSeed);
void SeqMWC_seed_bySeqMWC(SeqMWC* mwc, SeqMWC* seqSeeder);
void SeqMWC_genNextValue(SeqMWC* mwc);
ui32 SeqMWC_takeU32Sample(SeqMWC* mwc); // expect range: {1, ..., MAX_U32}
fp32 SeqMWC_takeF32Sample(SeqMWC* mwc); // approx range: [0.0, 1.0]

//==[ Multiply-With-Carry ]== END


//==[ Congruential ]===========================================================
//  period: 2^32
//  description:
//    CONG is a congruential generator with the widely used 69069
//    multiplier: x(n)=69069x(n-1)+1234567. It has period
//    2^32. The leading half of its 32 bits seem to pass
//    tests, but bits in the last half are too regular.
//
//=============================================================================
typedef struct SeqCONG
{
	ui32 value;
} SeqCONG;

void SeqCONG_seed(SeqCONG* cong, ui32 seed);
void SeqCONG_seed_bySeqMWC(SeqCONG* cong, SeqMWC* seqSeeder);
void SeqCONG_genNextValue(SeqCONG* cong);
ui32 SeqCONG_takeU32Sample(SeqCONG* cong); // expect range: {0, ..., MAX_U32}
fp32 SeqCONG_takeF32Sample(SeqCONG* cong); // approx range: [0.0, 1.0]

//==[ Congruential ]== END


//==[ 3-shift-register ]=======================================================
//  period: 2^32-1
//  description:
//    SHR3 is a 3-shift-register generator with period
//    2^32-1. It uses y(n)=y(n-1)(I+L^17)(I+R^13)(I+L^5),
//    with the y's viewed as binary vectors, L the 32x32
//    binary matrix that shifts a vector left 1, and R its
//    transpose. SHR3 seems to pass all except those
//    related to the binary rank test, since 32 successive
//    values, as binary vectors, must be linearly
//    independent, while 32 successive truly random 32-bit
//    integers, viewed as binary vectors, will be linearly
//    independent only about 29% of the time.
//
//=============================================================================
typedef struct SeqSHR3
{
	ui32 value;
} SeqSHR3;

void SeqSHR3_seed(SeqSHR3 *shr3, ui32 seed);
void SeqSHR3_seed_bySeqMWC(SeqSHR3* shr3, SeqMWC* seqSeeder);
void SeqSHR3_genNextValue(SeqSHR3* shr3);
ui32 SeqSHR3_takeU32Sample(SeqSHR3* shr3); // expect range: {1, ..., MAX_U32}
fp32 SeqSHR3_takeF32Sample(SeqSHR3* shr3); // approx range: [0.0, 1.0]

//==[ 3-shift-register ]== END


//==[ Keep It Simple Stupid ]==================================================
//  period: 2^123
//  description:
//    The KISS generator, (Keep It Simple Stupid), is
//    designed to combine the two multiply-with-carry
//    generators in MWC with the 3-shift register SHR3 and
//    the congruential generator CONG, using addition and
//    exclusive-or. Period about 2^123.
//    It is one of my favorite generators.
//=============================================================================
typedef struct SeqKISS
{
	SeqMWC  rngMWC;
	SeqCONG rngCONG;
	SeqSHR3 rngSHR3;
	ui32    value;
} SeqKISS;

void SeqKISS_seed_bySeqMWC(SeqKISS* kiss, SeqMWC* seqSeeder);
void SeqKISS_genNextValue(SeqKISS* kiss);
ui32 SeqKISS_takeU32Sample(SeqKISS* kiss); // expect range: {1, ..., MAX_U32}
fp32 SeqKISS_takeF32Sample(SeqKISS* kiss); // approx range: [0.0, 1.0]

//==[ Keep It Simple Stupid ]== END
