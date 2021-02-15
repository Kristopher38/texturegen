#include "kisstoys.h"

inline void SeqMWC_seed(SeqMWC* mwc, ui32 zSeed, ui32 wSeed)
{
	mwc->z = zSeed;
	mwc->w = wSeed;
}

inline void SeqMWC_seed_bySeqMWC(SeqMWC* mwc, SeqMWC* seqSeeder)
{
	mwc->z = SeqMWC_takeU32Sample(seqSeeder);
	mwc->w = SeqMWC_takeU32Sample(seqSeeder);
	SeqMWC_genNextValue(mwc);
}

inline void SeqMWC_genNextValue(SeqMWC* mwc)
{
	mwc->z = 36969*(mwc->z & 65535) + (mwc->z >> 16); //
	mwc->w = 18000*(mwc->w & 65535) + (mwc->w >> 16); // generate next state
	mwc->value = (mwc->z << 16) + mwc->w;
}

inline ui32 SeqMWC_takeU32Sample(SeqMWC* mwc) // expect range: {1, ..., MAX_U32}
{ 
	SeqMWC_genNextValue(mwc);
	return mwc->value;
}

inline fp32 SeqMWC_takeF32Sample(SeqMWC* mwc) // approx range: [0.0, 1.0]
{
	SeqMWC_genNextValue(mwc);
	return ((fp32)mwc->value) / ((fp32)__maxRngValue);
}

inline void SeqCONG_seed(SeqCONG* cong, ui32 seed)
{
	cong->value = seed;
}

inline void SeqCONG_seed_bySeqMWC(SeqCONG* cong, SeqMWC* seqSeeder)
{
	cong->value = SeqMWC_takeU32Sample(seqSeeder);
}

inline void SeqCONG_genNextValue(SeqCONG* cong)
{ 
	cong->value = 69069*cong->value + 1234567; /* state = value */ 
}

inline ui32 SeqCONG_takeU32Sample(SeqCONG* cong) // expect range: {0, ..., MAX_U32}
{ 
	SeqCONG_genNextValue(cong);
	return cong->value;
}

inline fp32 SeqCONG_takeF32Sample(SeqCONG* cong) // approx range: [0.0, 1.0]
{
	SeqCONG_genNextValue(cong);
	return ((fp32)cong->value) / ((fp32)__maxRngValue);
}

inline void SeqSHR3_seed(SeqSHR3* shr3, ui32 seed)
{
	shr3->value = seed;
}

inline void SeqSHR3_seed_bySeqMWC(SeqSHR3* shr3, SeqMWC* seqSeeder)
{
	shr3->value = SeqMWC_takeU32Sample(seqSeeder);
}

inline void SeqSHR3_genNextValue(SeqSHR3* shr3) 
{
	shr3->value ^= (shr3->value << 17),
	shr3->value ^= (shr3->value >> 13),
	shr3->value ^= (shr3->value << 5 );
}

inline ui32 SeqSHR3_takeU32Sample(SeqSHR3* shr3) // expect range: {1, ..., MAX_U32}
{
	SeqSHR3_genNextValue(shr3);
	return shr3->value; /* state = value */
}

inline fp32 SeqSHR3_takeF32Sample(SeqSHR3* shr3) // approx range: [0.0, 1.0]
{
	SeqSHR3_genNextValue(shr3);
	return ((fp32)shr3->value) / ((fp32)__maxRngValue);
}

inline void SeqKISS_seed_bySeqMWC(SeqKISS* kiss, SeqMWC* seqSeeder)
{
    SeqMWC_seed_bySeqMWC(&kiss->rngMWC, seqSeeder);
    SeqCONG_seed_bySeqMWC(&kiss->rngCONG, seqSeeder);
    SeqSHR3_seed_bySeqMWC(&kiss->rngSHR3, seqSeeder);
	SeqKISS_genNextValue(kiss);
}

inline void SeqKISS_genNextValue(SeqKISS* kiss)
{
    SeqMWC_genNextValue(&kiss->rngMWC);
    SeqCONG_genNextValue(&kiss->rngCONG);
    SeqSHR3_genNextValue(&kiss->rngSHR3);
	kiss->value = (kiss->rngMWC.value ^ kiss->rngCONG.value) + kiss->rngSHR3.value;
}

inline ui32 SeqKISS_takeU32Sample(SeqKISS* kiss) // expect range: {1, ..., MAX_U32}
{ 
	SeqKISS_genNextValue(kiss); 
	return kiss->value;
}

inline fp32 SeqKISS_takeF32Sample(SeqKISS* kiss) // approx range: [0.0, 1.0]
{
	SeqKISS_genNextValue(kiss); 
	return ((fp32)kiss->value) / ((fp32)__maxRngValue);
}
