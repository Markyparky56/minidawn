#pragma once
#include <cstdint>
#include <cmath>
#include <intrin.h>

static uint32_t CountTrailingZeros(uint32_t Value)
{
    if (Value == 0)
    {
        return 32;
    }
    uint32_t BitIndex; // 0-based, where LSB is 0 and MSB is 31
    _BitScanForward(reinterpret_cast<unsigned long*>(&BitIndex), Value); // Scans from LSB to MSB
    return BitIndex;
}

template<typename T>
static inline void Valswap(T& A, T& B)
{
    // For when MoveTemp isn't necessary (e.g. fundamental tyoes)
    T Tmp = A;
    A = B;
    B = Tmp;
}

static void MemswapImpl(void* Ptr1, void* Ptr2, size_t Size);

// Memswap function used by SharedRef
static inline void Memswap(void* Ptr1, void* Ptr2, size_t Size)
{
    switch (Size)
    {
    case 1:
        Valswap(*(static_cast<uint8_t*>(Ptr1)), *(static_cast<uint8_t*>(Ptr2)));
        break;

    case 2:
        Valswap(*(static_cast<uint16_t*>(Ptr1)), *(static_cast<uint16_t*>(Ptr2)));
        break;

    case 4:
        Valswap(*(static_cast<uint32_t*>(Ptr1)), *(static_cast<uint32_t*>(Ptr2)));
        break;

    case 8:
        Valswap(*(static_cast<uint64_t*>(Ptr1)), *(static_cast<uint64_t*>(Ptr2)));
        break;

    case 16:
        Valswap((static_cast<uint64_t*>(Ptr1)[0]), (static_cast<uint64_t*>(Ptr2)[0]));
        Valswap((static_cast<uint64_t*>(Ptr1)[1]), (static_cast<uint64_t*>(Ptr2)[1]));
        break;
    }
}