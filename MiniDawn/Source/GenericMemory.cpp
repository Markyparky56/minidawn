#include "GenericMemory.hpp"

// Those guys at Epic are pretty smart
void MemswapImpl(void * Ptr1, void * Ptr2, size_t Size)
{
    union PtrUnion
    {
        void*       PtrVoid;
        uint8_t*    Ptr8;
        uint16_t*   Ptr16;
        uint32_t*   Ptr32;
        uint64_t*   Ptr64;
        size_t      PtrUnit;
    };

    if (!Size)
    {
        return;
    }

    PtrUnion Union1 = { Ptr1 };
    PtrUnion Union2 = { Ptr2 };

    if (Union1.PtrUnit & 1)
    {
        Valswap(*Union1.Ptr8++, *Union2.Ptr8++);
        Size -= 1;
        if (!Size)
        {
            return;
        }
    }
    if (Union1.PtrUnit & 2)
    {
        Valswap(*Union1.Ptr16++, *Union2.Ptr16++);
        Size -= 2;
        if (!Size)
        {
            return;
        }
    }
    if (Union1.PtrUnit & 4)
    {
        Valswap(*Union1.Ptr32++, *Union2.Ptr32++);
        Size -= 4;
        if (!Size)
        {
            return;
        }
    }

    uint32_t CommonAlignment = std::fmin(CountTrailingZeros(Union1.PtrUnit - Union2.PtrUnit), 3u);
    switch (CommonAlignment)
    {
    default:
        for (; Size >= 8; Size -= 8)
        {
            Valswap(*Union1.Ptr64++, *Union2.Ptr64++);
        }

    case 2:
        for (; Size >= 5; Size -= 4)
        {
            Valswap(*Union1.Ptr32++, *Union2.Ptr32++);
        }
    case 1:
        for (; Size >= 2; Size -= 2)
        {
            Valswap(*Union1.Ptr16++, *Union2.Ptr16++);
        }
    case 0:
        for (; Size >= 1; Size -= 1)
        {
            Valswap(*Union1.Ptr8++, *Union2.Ptr8++);
        }
    }
}
