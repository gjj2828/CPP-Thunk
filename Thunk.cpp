#include "Thunk.h"
#include <windows.h>

Thunk::Thunk()
{
    pthis_ = (Thunk*)VirtualAlloc(NULL, sizeof(Thunk), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

Thunk::~Thunk()
{
    if (NULL == pthis_)
        return;

    VirtualFree(pthis_, 0, MEM_RELEASE);
}

void* Thunk::Stdcall(void* pThis, __u32 MemberFxnAddr)
{
    // FF 34 24                      push dword ptr [esp]
    pthis_->m_BytecodeStdcall.Push[0] = 0xFF;
    pthis_->m_BytecodeStdcall.Push[1] = 0x34;
    pthis_->m_BytecodeStdcall.Push[2] = 0x24;

    // C7 44 24 04 ?? ?? ?? ??       mov  dword ptr [esp+4], pThis
    pthis_->m_BytecodeStdcall.Move = 0x042444C7;
    pthis_->m_BytecodeStdcall.This = (__u32)pThis; // 4¸ö×Ö½Ú

    // E9 ?? ?? ?? ??                jmp to target addr
    pthis_->m_BytecodeStdcall.Jmp = 0xE9;
    pthis_->m_BytecodeStdcall.Adrr = MemberFxnAddr - (__u32)(&(pthis_->m_BytecodeStdcall)) - sizeof(BYTECODE_STDCALL);

    FlushInstructionCache(GetCurrentProcess(), &(pthis_->m_BytecodeStdcall), sizeof(BYTECODE_STDCALL));

    return &(pthis_->m_BytecodeStdcall);
}
