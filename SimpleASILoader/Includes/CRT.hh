#pragma once
#define _CORECRT_BUILD
#include <Windows.h>
#include <intrin.h>

//=======================================================================
// 
//	Description:	Handle everything thats related to C++ Runtime
// 
//=======================================================================


//=======================================================================
// C++ Runtime Stuff
// https://wiki.osdev.org/Visual_C++_Runtime

typedef void(__cdecl* _PVFV)(void);
typedef int(__cdecl* _PIFV)(void);

#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)

#pragma comment(linker, "/merge:.CRT=.rdata")

__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = { 0 };
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = { 0 };
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = { 0 };
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = { 0 };

extern __declspec(allocate(".CRT$XIA")) _PIFV __xi_a[];
extern __declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[];
extern __declspec(allocate(".CRT$XCA")) _PVFV __xc_a[];
extern __declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[];

// Call C constructors
static int _initterm_e(_PIFV* pfbegin, _PIFV* pfend)
{
    int ret = 0;

    while (pfbegin < pfend && ret == 0)
    {
        if (*pfbegin != 0) {
            ret = (**pfbegin)();
        }
        ++pfbegin;
    }

    return ret;
}

// Call C++ constructors
static void _initterm(_PVFV* pfbegin, _PVFV* pfend)
{
    while (pfbegin < pfend)
    {
        if (*pfbegin != 0) {
            (**pfbegin)();
        }
        ++pfbegin;
    }
}

//=======================================================================
// Memory Stuff

void* __cdecl malloc(size_t size)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void __cdecl free(void* block)
{
    HeapFree(GetProcessHeap(), 0, block);
}

void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void* p) noexcept
{
    free(p);
}

void operator delete[](void* p) noexcept
{
    free(p);
}

void* memset(void* dest, int ch, size_t len)
{
    asm volatile(
        "rep stosb"
        : "+D"(dest), "+c"(len)
        : "a"(ch)
        : "memory"
    );
    return dest;
}

//=======================================================================

namespace CRT
{
    void Initialize()
    {
        // Do C initialization
        if (_initterm_e(__xi_a, __xi_z)) {
            return;
        }

        // Do C++ initialization
        _initterm(__xc_a, __xc_z);
    }
}