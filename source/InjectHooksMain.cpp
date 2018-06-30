#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef int (__thiscall*  hCStreamingInfo_AddToList)
(
    //signed __int16 * pThis, signed __int16 *a2
    CStreamingInfo * pThis, CStreamingInfo * listStart
);

hCStreamingInfo_AddToList OLD_CStreamingInfo_AddToList = (hCStreamingInfo_AddToList)0x15674C0;

//int __fastcall CStreamingInfo__AddToList(CStreamingInfo * pThis, void* padding, CStreamingInfo * listStart);

void InjectHooksMain(void)
{
    InjectHook(0x15674C0, &CStreamingInfo::AddToList, PATCH_JUMP);

    /*
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    //DetourAttach(&(PVOID&)OLD_CStreamingInfo_AddToList, CStreamingInfo__AddToList);
    DetourAttach(&(PVOID&)OLD_CStreamingInfo_AddToList, CStreamingInfo::AddToList);

    //InjectHook(0x407480, &CStreamingInfo::AddToList, PATCH_JUMP);
    //InjectHook(0x4076A0, &CStreaming::IsVeryBusy, PATCH_JUMP);
    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    DetourTransactionCommit();
   */
}

/*
// Working Hook :)

template < class T >
int GET_INDEX_FROM_BASE ( T pThis, T ArrayBase )
{
    unsigned __int64 indexToCalculate = (1717986919i64 * (reinterpret_cast<signed int>(pThis) - reinterpret_cast<signed int>(ArrayBase))) >> 32;
    return static_cast < int > ( (indexToCalculate >> 3)  + (indexToCalculate >> 31) );
}

int __fastcall CStreamingInfo__AddToList(CStreamingInfo * pThis, void* padding, CStreamingInfo * listStart)
{   
    std::printf("CStreamingInfo::AddToList called\n");

    pThis->m_nNextIndex = listStart->m_nNextIndex;
    pThis->m_nPrevIndex = GET_INDEX_FROM_BASE < CStreamingInfo * > ( listStart, CStreamingInfo::ms_pArrayBase );
    listStart->m_nNextIndex = GET_INDEX_FROM_BASE < CStreamingInfo * >(pThis, CStreamingInfo::ms_pArrayBase);
    CStreamingInfo::ms_pArrayBase[pThis->m_nNextIndex].m_nPrevIndex = listStart->m_nNextIndex;
    return pThis->m_nNextIndex;
}

*/
