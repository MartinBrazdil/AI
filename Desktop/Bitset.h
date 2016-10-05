#pragma once
//
//#include "Interval.h"
//#include <cstdint>
//
//template<class OWNER, class MEMORY_T, int BITS>
//class Bitset
//{
//public:
//	static const size_t T_BYTES = sizeof(MEMORY_T);
//	static const size_t T_BITS = 8 * T_BYTES;
//
//	// determines size, i.e. how many MEMORY_T types are needed to hold bitSize number of bits
//	static const int SIZE = size_t(CEIL(BITS / double(T_BITS)));
//
//	static MEMORY_T bitset[SIZE];
//
//	inline bool GetBit(size_t x) const noexcept { return bitset[x / T_BITS] & (1LLU << (x % T_BITS)); }
//	inline void SetBit(size_t x)  noexcept { bitset[x / T_BITS] |= (1LLU << (x % T_BITS)); }
//
//	void Reset() { memset(bitset, 0, SIZE * T_BYTES); }
//
//	bool SearchStart(const size_t memIdx, uint8_t& idx) const noexcept;
//	bool SearchStop(const size_t memIdx, uint8_t& idx) const noexcept;
//
//	template<class IDX_T>
//	IDX_T ToIntervals(Interval<IDX_T>* memory) const noexcept;
//};
//
//template<class OWNER, class MEMORY_T, int BITS>
//MEMORY_T Bitset<OWNER, MEMORY_T, BITS>::bitset[];
//
//template<class OWNER, class MEMORY_T, int BITS>
//inline bool Bitset<OWNER, MEMORY_T, BITS>::SearchStart(const size_t memIdx, uint8_t& idx) const noexcept
//{
//	// if MEMORY_T is 0 then all bits are 0 so there will be no 1
//	if (bitset[memIdx] == 0) { idx += T_BITS; return true; }
//	// iterate until it finds bit which equals 1
//	while (!(bitset[memIdx] & (1LLU << idx)) && idx < T_BITS) idx++;
//	// return value signalises whether to search for start or stop
//	// true == searchStart, false == searchStop
//	return idx == T_BITS;
//}
//
//template<class OWNER, class MEMORY_T, int BITS>
//inline bool Bitset<OWNER, MEMORY_T, BITS>::SearchStop(const size_t memIdx, uint8_t& idx) const noexcept
//{
//	// if MEMORY_T is max then all bits are 1 so there will be no 0
//	if (bitset[memIdx] == std::numeric_limits<MEMORY_T>::max()) { idx += T_BITS; return false; }
//	// iterate until it finds bit which equals 0
//	while (bitset[memIdx] & (1LLU << idx) && idx < T_BITS) idx++;
//	// return value signalises whether to search for start or stop
//	// true == searchStart, false == searchStop
//	return !(idx == T_BITS);
//}
//
//template<class OWNER, class MEMORY_T, int BITS>
//template<class IDX_T>
//inline IDX_T Bitset<OWNER, MEMORY_T, BITS>::ToIntervals(Interval<IDX_T>* memory) const noexcept
//{
//	// current bit index (into MEMORY_T)
//	uint8_t idx = 0;
//	// count how many intervals were built
//	IDX_T cntr = 0;
//	// signalize whether to search for start or stop
//	bool searchStart = true;
//	// memIdx is MEMORY_T memory index, next cycle is made when idx == T_BITS
//	for (IDX_T memIdx = 0; memIdx < SIZE; memIdx++, idx %= T_BITS)
//	{
//		do
//		{
//			// search for idx of first bit set to 1
//			if (searchStart)
//			{
//				// idx is passed by reference and incremented
//				searchStart = SearchStart(memIdx, idx);
//				// if start has been found, write interval begin
//				if (!searchStart) memory[cntr]._begin = memIdx * T_BITS + idx;
//			}
//			// search for idx of first bit set to 0
//			else
//			{
//				// idx is passed by reference and incremented
//				searchStart = SearchStop(memIdx, idx);
//				// if stop has been found, write interval end and advance interval cntr
//				if (searchStart) memory[cntr++]._end = memIdx * T_BITS + idx;
//			}
//		} while (idx < T_BITS);
//	}
//	// if end has not been found and we reach end of memory, write end and advance interval cntr
//	if (!searchStart) memory[cntr++]._end = SIZE * T_BITS;
//
//	// return number of intervals built
//	return cntr;
//}
