#include "GenericMemory.hpp"

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

    uint32_t CommonAlignment = std::min(//countrailingzeros)
}
