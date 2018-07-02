#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef void (__cdecl*  hCStreaming__RequestModel)
(
    //signed int dwModelId, char flags
    int dwModelId, char streamingFlags
);

hCStreaming__RequestModel OLD_CStreaming__RequestModel = (hCStreaming__RequestModel)0x4087E0;

void __cdecl CStreaming__RequestModel(int dwModelId, char streamingFlags);

void InjectHooksMain(void)
{
    //InjectHook(0x15674C0, &CStreamingInfo::AddToList, PATCH_JUMP);

    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreaming__RequestModel, CStreaming__RequestModel);
   

    //InjectHook(0x407480, &CStreamingInfo::AddToList, PATCH_JUMP);
    //InjectHook(0x4076A0, &CStreaming::IsVeryBusy, PATCH_JUMP);
    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    DetourTransactionCommit();
   
}

//int __cdecl getTXDEntryParentIndex(int index);
typedef int(__cdecl* hgetTXDEntryParentIndex)
(
    int index
    );
//hgetTXDEntryParentIndex getTXDEntryParentIndex = (hgetTXDEntryParentIndex)0x00408370;

extern hgetTXDEntryParentIndex getTXDEntryParentIndex;


void __cdecl CStreaming__RequestModel(int dwModelId, char streamingFlags) //CStreaming::RequestModel(uint32_t modelId, uint32_t streamingFlags)
{
    int flags; // ebx
    signed int dwModelId_1; // edi
    char loadState; // al
    unsigned __int8 v5; // al
    CStreamingInfo *pModelStreamingInfo; // esi
    char loadState2; // al
    CBaseModelInfo *pBaseModelInfo; // ebp
    int animFileIndex; // eax
    int txdEntryParentIndex; // eax

    //std::printf("CStreaming::RequestModel called dwModelId: %d\n", dwModelId);

    flags = streamingFlags;
    dwModelId_1 = dwModelId;

    pModelStreamingInfo = &CStreaming::ms_aInfoForModel[dwModelId_1];
    loadState = pModelStreamingInfo->m_nLoadState;
    if (loadState == 2)                         // if loading
    {
        if (streamingFlags & 16 && !(CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags & 16))
        {
            ++CStreaming::ms_numPriorityRequests;
            pModelStreamingInfo->m_nFlags |= 16;
        }
    }
    else if (loadState)
    {
        flags = streamingFlags & 0xEF;
    }

    pModelStreamingInfo->m_nFlags |= flags;
    loadState2 = pModelStreamingInfo->m_nLoadState;
    if (loadState2 == 1)                        // if loaded
    {
        if (pModelStreamingInfo->m_nNextIndex != -1)   // CStreamingInfo::InList
        {
            CStreamingInfo::ms_pArrayBase[pModelStreamingInfo->m_nNextIndex].m_nPrevIndex = pModelStreamingInfo->m_nPrevIndex;
            CStreamingInfo::ms_pArrayBase[pModelStreamingInfo->m_nPrevIndex].m_nNextIndex = pModelStreamingInfo->m_nNextIndex;
            pModelStreamingInfo->m_nNextIndex = -1;
            pModelStreamingInfo->m_nPrevIndex = -1;

            if ((dwModelId < 20000))
            {
                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(dwModelId);
                size_t modelType =  modelInfo->GetModelType();
                if (modelType == 7 && modelType == 6)
                {
                    return;
                }
            }

            if (!(pModelStreamingInfo->m_nFlags & 6))
            {
                pModelStreamingInfo->AddToList(CStreaming::ms_startLoadedList);
            }
        }
    }
    else if (loadState2 != 3 && loadState2 != 2 && loadState2 != 4)// if not loaded
    {
      //  std::printf("CStreaming__RequestModel: lmao, exeuting wrong code again, fix this!\n");
        if (!loadState2)                          // if not loaded
        {
            if (dwModelId >= 20000)
            {
                if (dwModelId < 25000)                // txd
                {
                    txdEntryParentIndex = getTXDEntryParentIndex(dwModelId - 20000);
                    if (txdEntryParentIndex != -1)
                        CStreaming::RequestTxdModel(txdEntryParentIndex, flags);
                }
            }
            else
            {
                pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[dwModelId];


                CStreaming::RequestTxdModel(pBaseModelInfo->m_nTxdIndex, flags);
                animFileIndex = pBaseModelInfo->GetAnimFileIndex();
                if (animFileIndex != -1)
                    CStreaming::RequestModel(animFileIndex + 25575, 8);
            }
            pModelStreamingInfo->AddToList(CStreaming::ms_pStartRequestedList);
            ++CStreaming::ms_numModelsRequested;
            if (flags & 0x10)
                ++CStreaming::ms_numPriorityRequests;
        }
        pModelStreamingInfo->m_nFlags = flags;
        pModelStreamingInfo->m_nLoadState = 2;// requested, loading
    }
}

