#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef int (__thiscall*  hCStreamingInfo_AddToList)
(
    signed __int16 * pThis, signed __int16 *a2
);

hCStreamingInfo_AddToList OLD_CStreamingInfo_AddToList = (hCStreamingInfo_AddToList)0x15674C0;

int __fastcall CStreamingInfo__AddToList(signed __int16 * pThis, void* padding, signed __int16 *a2);

void InjectHooksMain(void)
{

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreamingInfo_AddToList, CStreamingInfo__AddToList);

    //InjectHook(0x407480, &CStreamingInfo::AddToList, PATCH_JUMP);
    //InjectHook(0x4076A0, &CStreaming::IsVeryBusy, PATCH_JUMP);
    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    DetourTransactionCommit();
   
}

/*
template < class T >
unsigned __int64 GET_INDEX_FROM_BASE ( T pThis, T ArrayBase )
{
    unsigned __int64 weirdOperation = ((1717986919i64 * (pThis - ArrayBase)) >> 32);
    return (weirdOperation >> 3)  + (weirdOperation >> 31);
}

int __fastcall CStreamingInfo__AddToList(signed __int16 * pThis, void* padding, signed __int16 *a2)
{

   
    std::printf("CStreamingInfo::AddToList called\n");

    int v2; // esi
    signed int v3; // edx
    unsigned int v4; // eax
    int result; // eax

    CStreamingInfo *listStart = (CStreamingInfo *)a2;

    unsigned __int64 weirdOperation = ( (1717986919i64 * (listStart - CStreamingInfo::ms_pArrayBase)) >> 32 );

    v2 = ((signed int)pThis - (*(DWORD*)0x9654B4) ) / 20;
    v3 = (signed int)weirdOperation >> 3;
    v4 = (unsigned int)weirdOperation >> 31;
    *pThis = *a2;
    pThis[1] = v4 + v3; // static_cast <short> (GET_INDEX_FROM_BASE < CStreamingInfo * >(listStart, CStreamingInfo::ms_pArrayBase));//v4 + v3;
    *a2 = v2;
    result = *pThis;
    //*(WORD *)( (*(DWORD*)0x9654B4)  + 20 * result + 2) = v2;

    CStreamingInfo::ms_pArrayBase[result].m_nPrevIndex = v2;

    return result;
   // return OLD_CStreamingInfo_AddToList( ppThis, a2);

}
*/

/*
// working HOOK

typedef int (__thiscall*  hCStreamingInfo_AddToList)
(
signed __int16 * pThis, signed __int16 *a2
);

hCStreamingInfo_AddToList OLD_CStreamingInfo_AddToList = (hCStreamingInfo_AddToList)0x15674C0;

int __fastcall CStreamingInfo__AddToList(signed __int16 * pThis, void* padding, signed __int16 *a2);
*/
int __fastcall CStreamingInfo__AddToList(signed __int16 * pThis, void* padding, signed __int16 *a2)
{

std::printf("CStreamingInfo::AddToList called\n");

int v2; // esi
signed int v3; // edx
unsigned int v4; // eax
int result; // eax


v2 = ((signed int)pThis - (*(DWORD*)0x9654B4) ) / 20;
v3 = (signed int)((unsigned __int64)(1717986919i64 * ((signed int)a2 - (*(DWORD*)0x9654B4) )) >> 32) >> 3;
v4 = (unsigned int)((unsigned __int64)(1717986919i64 * ((signed int)a2 - (*(DWORD*)0x9654B4) )) >> 32) >> 31;
*pThis = *a2;
pThis[1] = v4 + v3;
*a2 = v2;
result = *pThis;
*(WORD *)( (*(DWORD*)0x9654B4)  + 20 * result + 2) = v2;
return result;
// return OLD_CStreamingInfo_AddToList( ppThis, a2);

}
//*/