#include "StdInc.h"

unsigned int &CStreaming::ms_memoryAvailable = *reinterpret_cast<unsigned int *>(0x8A5A80);
unsigned int &CStreaming::desiredNumVehiclesLoaded = *reinterpret_cast<unsigned int *>(0x8A5A84);
bool &CStreaming::ms_bLoadVehiclesInLoadScene = *reinterpret_cast<bool *>(0x8A5A88);
int *CStreaming::ms_aDefaultCopCarModel = reinterpret_cast<int *>(0x8A5A8C);
int &CStreaming::ms_DefaultCopBikeModel = *reinterpret_cast<int *>(0x8A5A9C);
int *CStreaming::ms_aDefaultCopModel = reinterpret_cast<int *>(0x8A5AA0);
int &CStreaming::ms_DefaultCopBikerModel = *reinterpret_cast<int *>(0x8A5AB0);
signed int *CStreaming::ms_aDefaultAmbulanceModel = reinterpret_cast<signed int *>(0x8A5AB4);
signed int *CStreaming::ms_aDefaultMedicModel = reinterpret_cast<signed int *>(0x8A5AC4);
signed int *CStreaming::ms_aDefaultFireEngineModel = reinterpret_cast<signed int *>(0x8A5AD4);
signed int *CStreaming::ms_aDefaultFiremanModel = reinterpret_cast<signed int *>(0x8A5AE4);
signed int *CStreaming::ms_aDefaultCabDriverModel = reinterpret_cast<signed int *>(0x8A5AF4);
CDirectory *&CStreaming::ms_pExtraObjectsDir = *reinterpret_cast<CDirectory **>(0x8E48D0);
tStreamingFileDesc *CStreaming::ms_files = reinterpret_cast<tStreamingFileDesc *>(0x8E48D8);
bool &CStreaming::ms_bLoadingBigModel = *reinterpret_cast<bool *>(0x8E4A58);
tStreamingChannel *CStreaming::ms_channel = reinterpret_cast<tStreamingChannel *>(0x8E4A60);
signed int &CStreaming::ms_channelError = *reinterpret_cast<signed int *>(0x8E4B90);
bool &CStreaming::m_bHarvesterModelsRequested = *reinterpret_cast<bool *>(0x8E4B9C);
bool &CStreaming::m_bStreamHarvesterModelsThisFrame = *reinterpret_cast<bool *>(0x8E4B9D);
unsigned int &CStreaming::ms_numPriorityRequests = *reinterpret_cast<unsigned int *>(0x8E4BA0);
int &CStreaming::ms_lastCullZone = *reinterpret_cast<int *>(0x8E4BA4);
unsigned short &CStreaming::ms_loadedGangCars = *reinterpret_cast<unsigned short *>(0x8E4BA8);
unsigned short &CStreaming::ms_loadedGangs = *reinterpret_cast<unsigned short *>(0x8E4BAC);
unsigned int &CStreaming::ms_numPedsLoaded = *reinterpret_cast<unsigned int *>(0x8E4BB0);
unsigned int **CStreaming::ms_pedsLoaded = reinterpret_cast<unsigned int **>(0x8E4C00);
int &CStreaming::ms_currentZoneType = *reinterpret_cast<int *>(0x8E4C20);
CLoadedCarGroup &CStreaming::ms_vehiclesLoaded = *reinterpret_cast<CLoadedCarGroup *>(0x8E4C24);
CStreamingInfo *&CStreaming::ms_pEndRequestedList = *reinterpret_cast<CStreamingInfo **>(0x8E4C54);
CStreamingInfo *&CStreaming::ms_pStartRequestedList = *reinterpret_cast<CStreamingInfo **>(0x8E4C58);
CStreamingInfo *&CStreaming::ms_pEndLoadedList = *reinterpret_cast<CStreamingInfo **>(0x8E4C5C);
CStreamingInfo *&CStreaming::ms_startLoadedList = *reinterpret_cast<CStreamingInfo **>(0x8E4C60);
int &CStreaming::ms_lastImageRead = *reinterpret_cast<int *>(0x8E4C64);
signed int *CStreaming::ms_imageOffsets = reinterpret_cast<signed int *>(0x8E4C8C);
bool &CStreaming::ms_bEnableRequestListPurge = *reinterpret_cast<bool *>(0x8E4CA4);
unsigned int &CStreaming::ms_streamingBufferSize = *reinterpret_cast<unsigned int *>(0x8E4CA8);
char *&CStreaming::ms_pStreamingBuffer = *reinterpret_cast<char **>(0x8E4CAC);
unsigned int &CStreaming::ms_memoryUsed = *reinterpret_cast<unsigned int *>(0x8E4CB4);
unsigned int &CStreaming::ms_numModelsRequested = *reinterpret_cast<unsigned int *>(0x8E4CB8);
CStreamingInfo *CStreaming::ms_aInfoForModel = reinterpret_cast<CStreamingInfo *>(0x8E4CC0);
bool &CStreaming::ms_disableStreaming = *reinterpret_cast<bool *>(0x9654B0);
int &CStreaming::ms_bIsInitialised = *reinterpret_cast<int *>(0x9654B8);
bool &CStreaming::m_bBoatsNeeded = *reinterpret_cast<bool *>(0x9654BC);
bool &CStreaming::m_bLoadingScene = *reinterpret_cast<bool *>(0x9654BD);
bool &CStreaming::m_bCopBikeLoaded = *reinterpret_cast<bool *>(0x9654BE);
bool &CStreaming::m_bDisableCopBikes = *reinterpret_cast<bool *>(0x9654BF);
CLinkList<CEntity*> &CStreaming::ms_rwObjectInstances = *reinterpret_cast<CLinkList<CEntity*> *>(0x9654F0);
RwStream &gRwStream = *reinterpret_cast<RwStream *>(0x8E48AC);


//int __cdecl getTXDEntryParentIndex(int index);
typedef int(__cdecl* hgetTXDEntryParentIndex)
(
    int index
);
hgetTXDEntryParentIndex getTXDEntryParentIndex = (hgetTXDEntryParentIndex)0x00408370;



bool CStreaming::IsVeryBusy() {
    std::printf("Streaming::IsVeryBusy called\n");
    return CRenderer::m_loadingPriority || CStreaming::ms_numModelsRequested > 5;
}

void CStreaming::RequestModel(int dwModelId, int streamingFlags) //CStreaming::RequestModel(uint32_t modelId, uint32_t streamingFlags)
{
 /*   int flags; // ebx
    signed int dwModelId_1; // edi
    char loadState; // al
    unsigned __int8 v5; // al
    CStreamingInfo *pModelStreamingInfo; // esi
    char loadState2; // al
    CBaseModelInfo *pBaseModelInfo; // ebp
    int animFileIndex; // eax
    int txdEntryParentIndex; // eax

    std::printf("CStreaming::RequestModel called dwModelId: %d\n", dwModelId);

    flags = streamingFlags;
    dwModelId_1 = dwModelId;
    loadState = CStreaming::ms_aInfoForModel[dwModelId].m_nLoadState;
    if (loadState == 2)                         // if loading
    {
        if (streamingFlags & 16 && !(CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags & 16))
        {
            v5 = CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags | 16;
            ++CStreaming::ms_numPriorityRequests;
            CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags = v5;
        }
    }
    else if (loadState)
    {
        flags = streamingFlags & 0xFFFFFFEF;
    }
    pModelStreamingInfo = &CStreaming::ms_aInfoForModel[dwModelId_1];
    CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags |= flags;
    loadState2 = CStreaming::ms_aInfoForModel[dwModelId_1].m_nLoadState;
    if (loadState2 == 1)                        // if loaded
    {
        if (pModelStreamingInfo->m_nNextIndex != -1)   // CStreamingInfo::InList
        {
            CStreamingInfo::ms_pArrayBase[pModelStreamingInfo->m_nNextIndex].m_nPrevIndex = pModelStreamingInfo->m_nPrevIndex;
            CStreamingInfo::ms_pArrayBase[pModelStreamingInfo->m_nPrevIndex].m_nNextIndex = pModelStreamingInfo->m_nNextIndex;
            pModelStreamingInfo->m_nNextIndex = -1;
            pModelStreamingInfo->m_nPrevIndex = -1;

           // CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(dwModelId); 
            size_t modelType = (*(int(**)(void))(*(DWORD *)*(&(*(DWORD*)0xA9B0C8) + dwModelId) + 16))(); //modelInfo->GetModelType();
            if ((dwModelId >= 20000
                || modelType != 7 && modelType != 6)
                && !(CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags & 6))
            {
                pModelStreamingInfo->AddToList( CStreaming::ms_startLoadedList);
            }
        }
    }
    else if (loadState2 != 3 && loadState2 != 2 && loadState2 != 4)// if not loaded
    {
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
            pModelStreamingInfo->AddToList( CStreaming::ms_pStartRequestedList);
            ++CStreaming::ms_numModelsRequested;
            if (flags & 0x10)
                ++CStreaming::ms_numPriorityRequests;
        }
        CStreaming::ms_aInfoForModel[dwModelId_1].m_nFlags = 0;
        pModelStreamingInfo->m_nFlags |= flags;
        CStreaming::ms_aInfoForModel[dwModelId_1].m_nLoadState = 2;// requested, loading
    }*/
}


void CStreaming::RequestTxdModel(int TxdModelID, int Streamingflags) {
    //char __cdecl CStreaming::requestTxd(int a1, char a2)
    typedef int(__cdecl* hRequestTxdModel)
    (
            int a1, char a2
    );
    hRequestTxdModel _RequestTxdModel = (hRequestTxdModel)0x0407100;

    //plugin::CallDynGlobal<int, int>(0x407100, TxdModelID, Streamingflags);
    _RequestTxdModel ( TxdModelID, Streamingflags);
}

/*
{
    if (ms_aInfoForModel[modelId].m_loadStatus == 2)
    {
        if (flags & 0x10 && !(ms_aInfoForModel[modelId].m_flags & 0x10))
        {
            ms_aInfoForModel[modelId].m_flags |= 0x10;
            ms_numPriorityRequests++;
        }
    }
    else if (ms_aInfoForModel[modelId].m_loadStatus != 0)
    {
        ms_aInfoForModel[modelId].m_flags |= flags & 0xEF;
        if (ms_aInfoForModel[modelId].m_loadStatus == 1)
        {
            if (ms_aInfoForModel[modelId].InList())
            {
                ms_aInfoForModel[modelId].RemoveFromList();
                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
                size_t modelType = modelInfo->GetModelType();
                if (modelId > 19999 ||
                    (modelType != 7 && modelType != 6))
                {
                    if (!(ms_aInfoForModel[modelId].m_flags & (2 | 4)))
                    {
                        ms_aInfoForModel[modelId].AddToList(ms_pStartLoadedList);
                    }
                }
            }
        }
        else if (ms_aInfoForModel[modelId].m_loadStatus != 3 && ms_aInfoForModel[modelId].m_loadStatus != 4)
        {
            ms_aInfoForModel[modelId].m_flags = flags;
            ms_aInfoForModel[modelId].m_loadStatus = 2;
        }
    }
    else
    {
        ms_aInfoForModel[modelId].m_flags |= flags;
        if (modelId > 20000 + 4999)
        {
            if (modelId <= 19999)
            {
                v7 = (*(int(**)(void))(*(_DWORD *)CModelInfo::ms_modelInfoPtrs[temp] + 60))();
                if (v7 != -1)
                    CStreaming::RequestModel(v7 + 25575, 8);
            }
            ms_aInfoForModel[modelId].AddToList(ms_pStartRequestedList);
            ms_numModelsRequested++;
            if (flags & 0x10)
            {
                ms_numPriorityRequests++;
            }
            ms_aInfoForModel[modelId].m_flags = flags;
            ms_aInfoForModel[modelId].m_loadStatus = 2;
        }
        else
        {
            ms_aInfoForModel[modelId].m_loadStatus = 1;
        }
    }
}*/