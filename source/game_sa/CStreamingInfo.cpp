#include "StdInc.h"

CStreamingInfo *&CStreamingInfo::ms_pArrayBase = *reinterpret_cast<CStreamingInfo **>(0x9654B4);


template < class T >
int GET_INDEX_FROM_BASE(T pThis, T ArrayBase)
{
    unsigned __int64 indexToCalculate = (1717986919i64 * (reinterpret_cast<signed int>(pThis) - reinterpret_cast<signed int>(ArrayBase))) >> 32;
    return static_cast < int > ((indexToCalculate >> 3) + (indexToCalculate >> 31));
}
/*
int __fastcall CStreamingInfo__AddToList(CStreamingInfo * pThis, void* padding, CStreamingInfo * listStart)
{
    std::printf("CStreamingInfo::AddToList called\n");

    pThis->m_nNextIndex = listStart->m_nNextIndex;
    pThis->m_nPrevIndex = GET_INDEX_FROM_BASE < CStreamingInfo * >(listStart, CStreamingInfo::ms_pArrayBase);
    listStart->m_nNextIndex = GET_INDEX_FROM_BASE < CStreamingInfo * >(pThis, CStreamingInfo::ms_pArrayBase);
    CStreamingInfo::ms_pArrayBase[pThis->m_nNextIndex].m_nPrevIndex = listStart->m_nNextIndex;
    return pThis->m_nNextIndex;
}*/

int CStreamingInfo::AddToList(CStreamingInfo *listStart) {
    std::printf("CStreamingInfo::AddToList called :)\n");

    m_nNextIndex = listStart->m_nNextIndex;
    m_nPrevIndex = GET_INDEX_FROM_BASE < CStreamingInfo * >(listStart, CStreamingInfo::ms_pArrayBase);
    listStart->m_nNextIndex = GET_INDEX_FROM_BASE < CStreamingInfo * >(this, CStreamingInfo::ms_pArrayBase);
    CStreamingInfo::ms_pArrayBase[m_nNextIndex].m_nPrevIndex = listStart->m_nNextIndex;
    return m_nNextIndex;
}