#ifndef _THUNK_H_
#define _THUNK_H_
#pragma once

// TODO: support x64 platform.
//

#define __u8 unsigned char
#define __u16 unsigned short
#define __u32 unsigned long
#define __u64 unsigned long long

class Thunk
{
public:
    Thunk();
    ~Thunk();

    void* Stdcall(void* pThis, __u32 MemberFxnAddr);

    template <typename T>
    static __u32 GetMemberFxnAddr(T MemberFxnName) {
        union
        {
            T From;
            __u32 To;
        }union_cast;

        union_cast.From = MemberFxnName;
        return union_cast.To;
    }

private:
#pragma pack (push, 1)

    typedef struct _BYTECODE_STDCALL {
        __u8 Push[3];  // push dword ptr[esp] ;
        __u32 Move;    // mov dword ptr [esp + 4], ?? ?? ?? ?? ;
        __u32 This;    // this pointer
        __u8 Jmp;      // 0xE9
        __u32 Adrr;    // relative jmp
    }BYTECODE_STDCALL, * PBYTECODE_STDCALL;

#pragma pack (pop)

    BYTECODE_STDCALL  m_BytecodeStdcall;
    Thunk* pthis_;
};

#endif // ! _THUNK_H_
