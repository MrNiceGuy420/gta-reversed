#include "StdInc.h"

CStreamingInfo *&CStreamingInfo::ms_pArrayBase = *reinterpret_cast<CStreamingInfo **>(0x9654B4);

int CStreamingInfo::AddToList(CStreamingInfo *listStart) {
    int v2; // esi
    signed int v3; // edx
    unsigned int v4; // eax
    int result; // eax

    std::printf("CStreamingInfo::AddToList called loadstate, flags, nCdSize: %d, %d, %d\n", m_nLoadState, m_nFlags, m_nCdSize );
    
   // return plugin::CallMethod<0x406380, CStreamingInfo *, CStreamingInfo*>(this, listStart);

    v2 = this - CStreamingInfo::ms_pArrayBase;
    v3 = ((1717986919i64 * (listStart - CStreamingInfo::ms_pArrayBase)) >> 32) >> 3;
    v4 = ((1717986919i64 * (listStart - CStreamingInfo::ms_pArrayBase)) >> 32) >> 31;
    this->m_nNextIndex = listStart->m_nNextIndex;
    this->m_nPrevIndex = v4 + v3;
    listStart->m_nNextIndex = v2;
    result = this->m_nNextIndex;
    CStreamingInfo::ms_pArrayBase[result].m_nPrevIndex = v2;
    return result;
}