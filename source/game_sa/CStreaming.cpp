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
// There are only two channels within CStreaming::ms_channel
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
bool &CStreaming::m_bLoadingAllRequestedModels = *reinterpret_cast<bool *>(0x965538);
bool &CStreaming::m_bModelStreamNotLoaded = *reinterpret_cast<bool *>(0x9654C4);
unsigned int &CStreaming::ms_numberOfBytesRead = *reinterpret_cast<unsigned int *>(0x965534);

void CStreaming::InjectHooks()
{
    CStreamingInfo::InjectHooks();

    InjectHook(0x40A45E, &CStreaming::LoadAllRequestedModels, PATCH_JUMP);
    InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);
    InjectHook(0x40E170, &CStreaming::ProcessLoadingChannel, PATCH_JUMP);
    InjectHook(0x40E460, &CStreaming::FlushChannels, PATCH_JUMP);
    InjectHook(0x40CBA0, &CStreaming::RequestModelStream, PATCH_JUMP);
    InjectHook(0x40E3A0, &CStreaming::LoadRequestedModels, PATCH_JUMP);
    //////////////
    // NOT TESTED
    //////////////
    //InjectHook(0x40E4E0, &CStreaming::FlushRequestList, PATCH_JUMP);

    InjectHook(0x40C6B0, &CStreaming::ConvertBufferToObject, PATCH_JUMP);
    InjectHook(0x40E120, &CStreaming::MakeSpaceFor, PATCH_JUMP);
    //InjectHook(0x408CB0, &CStreaming::FinishLoadingLargeFile, PATCH_JUMP);
}

bool CStreaming::AreAnimsUsedByRequestedModels(int AnimFileIndex) {
    return plugin::CallAndReturnDynGlobal<bool, int>(0x407AD0, AnimFileIndex);
}

bool CStreaming::AreTexturesUsedByRequestedModels(int txdIndex) {
    return plugin::CallAndReturnDynGlobal<bool, int>(0x409A90, txdIndex);
}

int CStreaming::GetNextFileOnCd(int pos, bool bNotPriority) {
    return plugin::CallAndReturnDynGlobal<int, int, bool>(0x408E20, pos, bNotPriority);
}

bool CStreaming::ConvertBufferToObject(unsigned char* pFileBuffer, int modelId)
{
    CStreamingInfo *pStartLoadedListStreamingInfo = ms_startLoadedList;;
    CBaseModelInfo *pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
    CStreamingInfo *pModelStreamingInfo = &ms_aInfoForModel[modelId];

    unsigned int bufferSize = pModelStreamingInfo->m_nCdSize * 2048;
    tRwStreamInitializeData rwStreamInitializationData = { pFileBuffer, bufferSize };

    RwStream* pRwStream = _rwStreamInitialize(&gRwStream, 0, rwSTREAMMEMORY, rwSTREAMREAD, &rwStreamInitializationData);
    if (modelId >= 20000)
    {
        if (modelId >= 25000)
        {
            if (modelId >= 25255)
            {
                if (modelId >= 25511)
                {
                    if (modelId >= 25575)
                    {
                        if (modelId >= 25755)
                        {
                            if (modelId >= 26230)
                            {
                                CStreamedScripts & pStreamedScripts = CTheScripts::StreamedScripts;
                                pStreamedScripts.LoadStreamedScript(pRwStream, modelId - 26230);
                            }
                            else
                            {
                                CVehicleRecording::Load(pRwStream, modelId - 25755, bufferSize);
                            }
                        }
                        else
                        {
                            if (!(pModelStreamingInfo->m_nFlags & 0xE)
                                && !AreAnimsUsedByRequestedModels(modelId - 25575))
                            {
                                RemoveModel(modelId);
                                RwStreamClose(pRwStream, &rwStreamInitializationData);
                                return false;
                            }
                            CAnimManager::LoadAnimFile(pRwStream, true, 0);
                            CAnimManager::CreateAnimAssocGroups();
                        }
                    }
                    else
                    {
                        ThePaths.LoadPathFindData(pRwStream, modelId - 25511);
                    }
                }
                else if (!CIplStore::LoadIpl(modelId - 25255, pFileBuffer, bufferSize))
                {
                    RemoveModel(modelId);
                    RequestModel(modelId, pModelStreamingInfo->m_nFlags);
                    RwStreamClose(pRwStream, &rwStreamInitializationData);
                    return false;
                }
            }
            else if (!CColStore::LoadCol(modelId - 25000, pFileBuffer, bufferSize))
            {
                RemoveModel(modelId);
                RequestModel(modelId, pModelStreamingInfo->m_nFlags);
                RwStreamClose(pRwStream, &rwStreamInitializationData);
                return false;
            }
        }
        else
        {
            int modelTxdIndex = modelId - 20000;
            TxdDef *pTxdDef0 = nullptr;
            if ((CTxdStore::ms_pTxdPool->m_byteMap[modelTxdIndex].IntValue() & 0x80u) == 0)
            {
                pTxdDef0 = &CTxdStore::ms_pTxdPool->m_pObjects[modelTxdIndex];
                if (pTxdDef0)
                {
                    int txdIndex = pTxdDef0->m_wParentIndex;
                    if (txdIndex != -1 && !CTxdStore::GetTxd(txdIndex))
                    {
                        RemoveModel(modelId);
                        RequestModel(modelId, pModelStreamingInfo->m_nFlags);
                        RwStreamClose(pRwStream, &rwStreamInitializationData);
                        return false;
                    }
                }
            }

            if (!(pModelStreamingInfo->m_nFlags & 0xE)
                && !AreTexturesUsedByRequestedModels(modelTxdIndex))
            {
                RemoveModel(modelId);
                RwStreamClose(pRwStream, &rwStreamInitializationData);
                return false;
            }

            bool bTxdLoaded = false;
            if (ms_bLoadingBigModel)
            {
                bTxdLoaded = CTxdStore::StartLoadTxd(modelTxdIndex, pRwStream);
                if (bTxdLoaded)
                {
                    pModelStreamingInfo->m_nLoadState = LOADSTATE_Finishing;
                }
            }
            else
            {
                bTxdLoaded = CTxdStore::LoadTxd(modelTxdIndex, pRwStream);
            }
            if (!bTxdLoaded)
            {
                RemoveModel(modelId);
                RequestModel(modelId, pModelStreamingInfo->m_nFlags);
                RwStreamClose(pRwStream, &rwStreamInitializationData);
                return false;
            }
        }
    }
    else
    {
        int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
        short wTxdIndex = pBaseModelInfo->m_nTxdIndex;


        TxdDef* pTxdDef1 = nullptr;
        if ((CTxdStore::ms_pTxdPool->m_byteMap[wTxdIndex].IntValue() & 0x80u) == 0)
            pTxdDef1 = &CTxdStore::ms_pTxdPool->m_pObjects[wTxdIndex];

        CAnimBlock& AnimBlock = CAnimManager::ms_aAnimBlocks[animFileIndex];
        if ((pTxdDef1 && !pTxdDef1->m_pRwDictionary) || animFileIndex != -1 && !AnimBlock.bLoaded)
        {
            RemoveModel(modelId);
            RequestModel(modelId, pModelStreamingInfo->m_nFlags);
            RwStreamClose(pRwStream, &rwStreamInitializationData);
            return false;
        }

        CTxdStore::AddRef(wTxdIndex);
        if (animFileIndex != -1)
        {
            CAnimManager::AddAnimBlockRef(animFileIndex);
        }

        CTxdStore::SetCurrentTxd(pBaseModelInfo->m_nTxdIndex);

        bool bFileLoaded = false;
        if (pBaseModelInfo->GetRwModelType() == RWMODEL_INFO_ATOMIC)
        {
            RtDict* pRtDictionary = nullptr;
            RwChunkHeaderInfo chunkHeaderInfo;
            RwStreamReadChunkHeaderInfo(pRwStream, &chunkHeaderInfo);
            if (chunkHeaderInfo.type == rwID_UVANIMDICT)
            {
                pRtDictionary = RtDictSchemaStreamReadDict(&RpUVAnimDictSchema, pRwStream);
                RtDictSchemaSetCurrentDict(&RpUVAnimDictSchema, pRtDictionary);
            }
            RwStreamClose(pRwStream, &rwStreamInitializationData);
            RwStream* pRwStream2 = _rwStreamInitialize(&gRwStream, 0, rwSTREAMMEMORY, rwSTREAMREAD, &rwStreamInitializationData);
            bFileLoaded = CFileLoader::LoadAtomicFile(pRwStream2, modelId);
            if (pRtDictionary)
            {
                RtDictDestroy(pRtDictionary);
            }
        }
        else
        {
            bFileLoaded = CFileLoader::LoadClumpFile(pRwStream, modelId);
        }
        if (pModelStreamingInfo->m_nLoadState != LOADSTATE_Finishing)
        {
            CTxdStore::RemoveRefWithoutDelete(pBaseModelInfo->m_nTxdIndex);
            if (animFileIndex != -1)
            {
                CAnimManager::RemoveAnimBlockRefWithoutDelete(animFileIndex);
            }

            if (bFileLoaded && pBaseModelInfo->GetModelType() == MODEL_INFO_VEHICLE)
            {
                if (!AddToLoadedVehiclesList(modelId))
                {
                    RemoveModel(modelId);
                    RequestModel(modelId, pModelStreamingInfo->m_nFlags);
                    RwStreamClose(pRwStream, &rwStreamInitializationData);
                    return false;
                }
            }
        }
        if (!bFileLoaded)
        {
            RemoveModel(modelId);
            RequestModel(modelId, pModelStreamingInfo->m_nFlags);
            RwStreamClose(pRwStream, &rwStreamInitializationData);
            return false;
        }
    }

    RwStreamClose(pRwStream, &rwStreamInitializationData);
    if (modelId >= 20000)
    {
        if (!(modelId >= 25000 && (modelId < 25575 || modelId >= 25755) && modelId < 26230 || pModelStreamingInfo->m_nFlags & 6))
        {
            pModelStreamingInfo->AddToList(pStartLoadedListStreamingInfo);
        }
    }
    else
    {
        if (pBaseModelInfo->GetModelType() != MODEL_INFO_VEHICLE && pBaseModelInfo->GetModelType() != MODEL_INFO_PED)
        {
            CBaseModelInfo * pAsAtomicModelInfo = pBaseModelInfo->AsAtomicModelInfoPtr();
            if (pAsAtomicModelInfo)
            {
                pAsAtomicModelInfo->m_nAlpha = -((pModelStreamingInfo->m_nFlags & 0x24) != 0);
            }
            if (!(pModelStreamingInfo->m_nFlags & 6))
            {
                pModelStreamingInfo->AddToList(pStartLoadedListStreamingInfo);
            }
        }
    }

    if (pModelStreamingInfo->m_nLoadState != LOADSTATE_Finishing)
    {
        pModelStreamingInfo->m_nLoadState = LOADSTATE_LOADED;
        ms_memoryUsed += bufferSize;
    }
    return true;
}



bool CStreaming::IsVeryBusy() {
    std::printf("Streaming::IsVeryBusy called\n");
    return CRenderer::m_loadingPriority || ms_numModelsRequested > 5;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CStreaming::LoadAllRequestedModels
//
// There are only 2 streaming channels within CStreaming::ms_channel. In this function, 
// if your current channelIndex is zero then "1 - channelIndex" will give you the other 
// streaming channel within CStreaming::ms_channel which is 1 (second streaming channel). 
//////////////////////////////////////////////////////////////////////////////////////////
void CStreaming::LoadAllRequestedModels(bool bOnlyPriorityRequests)
{
    if (!m_bLoadingAllRequestedModels)
    {
        m_bLoadingAllRequestedModels = true;
        FlushChannels();
        int numModelsToLoad = 10;
        if (2 * ms_numModelsRequested >= 10)
            numModelsToLoad = 2 * ms_numModelsRequested;

        int channelIndex = 0;
        while (1)
        {
            tStreamingChannel& firstStreamingChannel = ms_channel[0];
            tStreamingChannel& secondStreamingChannel = ms_channel[1];

            short endRequestPreviousIndex = ms_pEndRequestedList->m_nPrevIndex;
            CStreamingInfo * pStreamingInfo = endRequestPreviousIndex == -1 ? 0 : &CStreamingInfo::ms_pArrayBase[endRequestPreviousIndex];

            if (pStreamingInfo == ms_pStartRequestedList
                && !firstStreamingChannel.LoadStatus
                && !secondStreamingChannel.LoadStatus
                || numModelsToLoad <= 0)
            {
                break;
            }

            if (ms_bLoadingBigModel)
            {
                channelIndex = 0;
            }

            tStreamingChannel& streamingChannel = ms_channel[channelIndex];
            if (streamingChannel.LoadStatus)
            {
                CdStreamSync(channelIndex);
                streamingChannel.iLoadingLevel = 100;
            }

            if (streamingChannel.LoadStatus == LOADSTATE_LOADED)
            {
                ProcessLoadingChannel(channelIndex);
                if (streamingChannel.LoadStatus == LOADSTATE_Requested)
                {
                    ProcessLoadingChannel(channelIndex);
                }
            }

            if (bOnlyPriorityRequests && !ms_numPriorityRequests)
            {
                break;
            }

            if (!ms_bLoadingBigModel)
            {
                tStreamingChannel& otherStreamingChannel = ms_channel[1 - channelIndex];
                if (!otherStreamingChannel.LoadStatus)
                {
                    RequestModelStream(1 - channelIndex);
                }

                if (!streamingChannel.LoadStatus && !ms_bLoadingBigModel)
                {
                    RequestModelStream(channelIndex);
                }
            }
            if (!firstStreamingChannel.LoadStatus && !secondStreamingChannel.LoadStatus)
            {
                break;
            }

            channelIndex = 1 - channelIndex;
            --numModelsToLoad;
        }
        FlushChannels();
        m_bLoadingAllRequestedModels = false;
    }

}

void CStreaming::RequestModel(int modelId, char streamingFlags)
{
    int flags = streamingFlags;
    CStreamingInfo & modelStreamingInfo = ms_aInfoForModel[modelId];
    char loadState = modelStreamingInfo.m_nLoadState;
    if (loadState == LOADSTATE_Requested)
    {
        if ((streamingFlags & PRIORITY_REQUEST) && !(modelStreamingInfo.m_nFlags & PRIORITY_REQUEST))
        {
            ++ms_numPriorityRequests;
            modelStreamingInfo.m_nFlags |= PRIORITY_REQUEST;
        }
    }
    else if (loadState)
    {
        flags = streamingFlags & 0xEF;
    }

    modelStreamingInfo.m_nFlags |= flags;
    if (loadState == LOADSTATE_LOADED)
    {
        if (modelStreamingInfo.InList())
        {
            modelStreamingInfo.RemoveFromList();
            if ((modelId < 20000))
            {
                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
                size_t modelType = modelInfo->GetModelType();
                if (modelType == MODEL_INFO_TYPE_PED && modelType == MODEL_INFO_TYPE_VEHICLE)
                {
                    return;
                }
            }

            if (!(modelStreamingInfo.m_nFlags & (GAME_REQUIRED || MISSION_REQUIRED)))
            {
                modelStreamingInfo.AddToList(ms_startLoadedList);
            }
        }
    }
    else if (loadState != LOADSTATE_Channeled && loadState != LOADSTATE_Requested && loadState != LOADSTATE_Finishing)
    {
        if (loadState == LOADSTATE_NOT_LOADED)
        {
            if (modelId >= 20000)
            {
                if (modelId < 25000)                // txd
                {
                    int txdEntryParentIndex = CTxdStore::GetParentTxdSlot(modelId - 20000);
                    if (txdEntryParentIndex != -1)
                        RequestTxdModel(txdEntryParentIndex, flags);
                }
            }
            else
            {
                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
                RequestTxdModel(modelInfo->m_nTxdIndex, flags);
                int animFileIndex = modelInfo->GetAnimFileIndex();
                if (animFileIndex != -1)
                    RequestModel(animFileIndex + 25575, KEEP_IN_MEMORY);
            }
            modelStreamingInfo.AddToList(ms_pStartRequestedList);
            ++ms_numModelsRequested;
            if (flags & PRIORITY_REQUEST)
                ++ms_numPriorityRequests;
        }
        modelStreamingInfo.m_nFlags = flags;
        modelStreamingInfo.m_nLoadState = LOADSTATE_Requested;// requested, loading
    }
}

void CStreaming::RequestTxdModel(int txdModelID, int streamingFlags) {
    RequestModel(txdModelID + 20000, streamingFlags);
}

bool CStreaming::FinishLoadingLargeFile(unsigned char * pFileBuffer, int modelId)
{
    std::printf("FinishLoadingLargeFile called\n");
    bool bFinishedLoadingLargeFile = 0;
    CBaseModelInfo *pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
    CStreamingInfo& streamingInfo = CStreaming::ms_aInfoForModel[modelId];
    if (streamingInfo.m_nLoadState == 4)
    {

        unsigned int bufferSize = streamingInfo.m_nCdSize << 11;
        tRwStreamInitializeData rwStreamInitializationData = { pFileBuffer, bufferSize };
        RwStream * pRwStream = _rwStreamInitialize(&gRwStream, 0, rwSTREAMMEMORY, rwSTREAMREAD, &rwStreamInitializationData);
        bool bLoaded = false;
        if (modelId >= 20000)
        {
            if (modelId >= 25000)
            {
                bLoaded = modelId;
            }
            else
            {
                CTxdStore::AddRef(modelId - 20000);
                bLoaded = CTxdStore::FinishLoadTxd(modelId - 20000, pRwStream);
                CTxdStore::RemoveRefWithoutDelete(modelId - 20000);
            }
        }
        else
        {
            CTxdStore::SetCurrentTxd(pBaseModelInfo->m_nTxdIndex);
            bLoaded = CFileLoader::FinishLoadClumpFile(pRwStream, modelId);
            if (bLoaded)
            {
                bLoaded = CStreaming::AddToLoadedVehiclesList(modelId);
            }
            pBaseModelInfo->RemoveRef();
            CTxdStore::RemoveRefWithoutDelete(pBaseModelInfo->m_nTxdIndex);
            int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            if (animFileIndex != -1)
            {
                CAnimManager::RemoveAnimBlockRefWithoutDelete(animFileIndex);
            }
        }
        RwStreamClose(pRwStream, &pFileBuffer);
        streamingInfo.m_nLoadState = LOADSTATE_LOADED;
        CStreaming::ms_memoryUsed += bufferSize;
        if (bLoaded)
        {
            bFinishedLoadingLargeFile = true;
        }
        else
        {
            CStreaming::RemoveModel(modelId);
            CStreaming::RequestModel(modelId, streamingInfo.m_nFlags);
            bFinishedLoadingLargeFile = false;
        }
    }
    else
    {
        if (modelId < 20000)
        {
            pBaseModelInfo->RemoveRef();
        }
        bFinishedLoadingLargeFile = false;
    }
    return bFinishedLoadingLargeFile;
}

bool CStreaming::FlushChannels()
{
    char channelsFlushed = false;
    if (ms_channel[1].LoadStatus == LOADSTATE_Requested)
        channelsFlushed = ProcessLoadingChannel(1);
    if (ms_channel[0].LoadStatus == LOADSTATE_LOADED)
    {
        CdStreamSync(0);
        ms_channel[0].iLoadingLevel = 100;
        channelsFlushed = ProcessLoadingChannel(0);
    }
    if (ms_channel[0].LoadStatus == LOADSTATE_Requested)
        channelsFlushed = ProcessLoadingChannel(0);
    if (ms_channel[1].LoadStatus == LOADSTATE_LOADED)
    {
        CdStreamSync(1u);
        ms_channel[1].iLoadingLevel = 100;
        channelsFlushed = ProcessLoadingChannel(1);
    }
    if (ms_channel[1].LoadStatus == LOADSTATE_Requested)
        channelsFlushed = ProcessLoadingChannel(1);
    return channelsFlushed;
}

void CStreaming::RequestModelStream(int channelIndex)
{
    int CdStreamLastPosn = CdStreamGetLastPosn();
    int modelId = GetNextFileOnCd(CdStreamLastPosn, 1);
    if (modelId == -1)
    {
        return;
    }

    int32_t blockOffsetMimg = 0;
    unsigned int blockCount = 0;

    CStreamingInfo * streamingInfo = &ms_aInfoForModel[modelId];

    if (!(streamingInfo->m_nFlags & (GAME_REQUIRED | MISSION_REQUIRED | KEEP_IN_MEMORY)))
    {
        do
        {
            bool areTexturesUsedByRequestedModels = false;
            if (modelId < 20000 || modelId >= 25000
                || (areTexturesUsedByRequestedModels = AreTexturesUsedByRequestedModels(modelId - 20000), modelId))
            {
                if (modelId < 25575)
                    break;
                if (modelId >= 25755)
                    break;

                bool areAnimsUsedByRequestedModels = AreAnimsUsedByRequestedModels(modelId - 25575);
                if (areAnimsUsedByRequestedModels)
                    break;
            }
            RemoveModel(modelId);
            if (streamingInfo->m_nCdSize)
            {
                blockOffsetMimg = streamingInfo->m_nCdPosn + ms_files[streamingInfo->m_nImgId].m_StreamHandle;
                blockCount = streamingInfo->m_nCdSize;
            }

            modelId = GetNextFileOnCd(blockCount + blockOffsetMimg, 1);
            if (modelId == -1)
            {
                return;
            }

            streamingInfo = &ms_aInfoForModel[modelId];

        } while (!(streamingInfo->m_nFlags & 14));
    }


    if (modelId == -1)
    {
        return;
    }

    if (streamingInfo->m_nCdSize)
    {
        blockOffsetMimg = streamingInfo->m_nCdPosn + ms_files[streamingInfo->m_nImgId].m_StreamHandle;
        blockCount = streamingInfo->m_nCdSize;
    }
    if (blockCount > ms_streamingBufferSize)
    {
        if (channelIndex == 1 || ms_channel[1].LoadStatus)
        {
            return;
        }
        ms_bLoadingBigModel = 1;
    }

    unsigned int sectorCount = 0;
    bool isVehcileModelORBlockCountGreaterThan200 = false;
    bool isModelTypePed = false;

    int modelIndex = 0;
    int numberOfModelIds = sizeof(tStreamingChannel::modelIds) / sizeof(tStreamingChannel::modelIds[0]);
    while (modelIndex < numberOfModelIds)
    {
        streamingInfo = &ms_aInfoForModel[modelId];
        if (streamingInfo->m_nLoadState != LOADSTATE_Requested)
        {
            break;
        }
        if (streamingInfo->m_nCdSize)
        {
            blockCount = streamingInfo->m_nCdSize;
        }
        if (ms_numPriorityRequests && !(streamingInfo->m_nFlags & PRIORITY_REQUEST))
        {
            break;
        }
        if (modelId >= 20000)
        {
            if (modelId < 25575 || modelId >= 25755)
            {
                if (isVehcileModelORBlockCountGreaterThan200 && blockCount > 200)
                {
                    break;
                }
            }
            else if (CCutsceneMgr::ms_cutsceneProcessing || ms_aInfoForModel[7].m_nLoadState != LOADSTATE_LOADED)
            {
                break;
            }
        }
        else
        {
            CBaseModelInfo * pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
            ModelInfoType modelType = pBaseModelInfo->GetModelType();
            if (isModelTypePed && modelType == MODEL_INFO_PED)
            {
                break;
            }
            if (isVehcileModelORBlockCountGreaterThan200 && modelType == MODEL_INFO_VEHICLE)
            {
                break;
            }
            unsigned char loadState = ms_aInfoForModel[pBaseModelInfo->m_nTxdIndex + 20000].m_nLoadState;
            if (loadState != LOADSTATE_LOADED && loadState != LOADSTATE_Channeled)
            {
                break;
            }
            int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            if (animFileIndex != -1)
            {
                unsigned char loadState2 = ms_aInfoForModel[animFileIndex + 25575].m_nLoadState;
                if (loadState2 != LOADSTATE_LOADED && loadState2 != LOADSTATE_Channeled)
                    break;
            }
        }

        tStreamingChannel & streamingChannel = ms_channel[channelIndex];
        streamingChannel.modelStreamingBufferOffsets[modelIndex] = sectorCount;
        streamingChannel.modelIds[modelIndex] = modelId;

        sectorCount += blockCount;
        if (sectorCount > ms_streamingBufferSize && modelIndex > 0)
        {
            sectorCount = sectorCount - blockCount;
            break;
        }


        CBaseModelInfo *  pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
        if (modelId >= 20000)
        {
            if (blockCount > 200)
            {
                isVehcileModelORBlockCountGreaterThan200 = true;
            }
        }
        else
        {
            if (pBaseModelInfo->GetModelType() == MODEL_INFO_PED)
            {
                isModelTypePed = true;
            }
            if (pBaseModelInfo->GetModelType() == MODEL_INFO_VEHICLE)
            {
                isVehcileModelORBlockCountGreaterThan200 = true;
            }
        }

        streamingInfo->m_nLoadState = LOADSTATE_Channeled;
        streamingInfo->RemoveFromList();
        --ms_numModelsRequested;

        if (streamingInfo->m_nFlags & PRIORITY_REQUEST)
        {
            int numPriorityRequests = ms_numPriorityRequests - 1;
            streamingInfo->m_nFlags &= 0xEFu;
            ms_numPriorityRequests = numPriorityRequests;
        }

        modelId = streamingInfo->m_nNextIndexOnCd;
        modelIndex++;
    }

    if (modelIndex < numberOfModelIds)
    {
        tStreamingChannel & streamingChannel = ms_channel[channelIndex];
        memset(&streamingChannel.modelIds[modelIndex], 0xFFu, 4 * (numberOfModelIds - modelIndex)); // 0xFFu = -1
    }

    CdStreamRead(channelIndex, (int)((&ms_pStreamingBuffer)[channelIndex * 4]), blockOffsetMimg, sectorCount);

    tStreamingChannel & streamingChannel = ms_channel[channelIndex];
    streamingChannel.LoadStatus = LOADSTATE_LOADED;
    streamingChannel.iLoadingLevel = 0;
    streamingChannel.iBlockCount = sectorCount;
    streamingChannel.iBlockOffset = blockOffsetMimg;
    streamingChannel.OnBeginRead = 0;


    if (m_bModelStreamNotLoaded)
        m_bModelStreamNotLoaded = false;
}

bool CStreaming::ProcessLoadingChannel(int channelIndex)
{
    tStreamingChannel& streamingChannel = ms_channel[channelIndex];
    int streamStatus = CdStreamGetStatus(channelIndex);
    if (streamStatus)
    {
        if (streamStatus == 255)
            return false;
        if (streamStatus == 250)
            return false;
        streamingChannel.m_nCdStreamStatus = streamStatus;
        streamingChannel.LoadStatus = LOADSTATE_Channeled;

        bool isChannelErrorFree = ms_channelError == -1;
        if (!isChannelErrorFree)
            return false;
        ms_channelError = channelIndex;
        RetryLoadFile(channelIndex);
        return true;
    }

    bool isRequested = streamingChannel.LoadStatus == LOADSTATE_Requested;
    streamingChannel.LoadStatus = LOADSTATE_NOT_LOADED; // 0;
    if (!isRequested)
    {
        int numberOfModelIds = sizeof(tStreamingChannel::modelIds) / sizeof(tStreamingChannel::modelIds[0]);
        for (int modelIndex = 0; modelIndex < numberOfModelIds; modelIndex++)
        {
            int modelId = streamingChannel.modelIds[modelIndex];
            if (modelId != -1)
            {
                CBaseModelInfo* baseModelInfo = CModelInfo::GetModelInfo(modelId);
                CStreamingInfo& streamingInfo = ms_aInfoForModel[modelId];
                int nCdSize = streamingInfo.m_nCdSize;

                if (modelId >= 20000
                    || baseModelInfo->GetModelType() != MODEL_INFO_VEHICLE
                    || ms_vehiclesLoaded.CountMembers() < desiredNumVehiclesLoaded
                    || RemoveLoadedVehicle()
                    || streamingInfo.m_nFlags & 6)
                {
                    if (modelId < 25255 || modelId >= 25511)
                        MakeSpaceFor(nCdSize << 11); // MakeSpaceFor(nCdSize * (2^11))

                    int bufferOffset = streamingChannel.modelStreamingBufferOffsets[modelIndex];
                    unsigned char * pFileBuffer = reinterpret_cast <unsigned char*> (&(&ms_pStreamingBuffer)[channelIndex][2048 * bufferOffset]);

                    ConvertBufferToObject(pFileBuffer, modelId);

                    if (streamingInfo.m_nLoadState != LOADSTATE_Finishing
                        || (streamingChannel.LoadStatus = LOADSTATE_Requested,
                            streamingChannel.modelStreamingBufferOffsets[modelIndex] = bufferOffset,
                            streamingChannel.modelIds[modelIndex] = modelId,
                            modelIndex))
                    {
                        streamingChannel.modelIds[modelIndex] = -1;
                    }
                }
                else
                {
                    int modelTxdIndex = baseModelInfo->m_nTxdIndex;
                    RemoveModel(modelId);

                    if (streamingInfo.m_nFlags & 6)
                    {
                        RequestModel(modelId, streamingInfo.m_nFlags);
                    }
                    else if (!CTxdStore::GetNumRefs(modelTxdIndex))
                    {
                        RemoveTxdModel(modelTxdIndex);
                    }
                }
            }
        }
    }
    else
    {
        int bufferOffset = streamingChannel.modelStreamingBufferOffsets[0];
        unsigned char * pFileContents = reinterpret_cast<unsigned char*>(&(&ms_pStreamingBuffer)[channelIndex][2048 * bufferOffset]);
        FinishLoadingLargeFile(pFileContents, streamingChannel.modelIds[0]);
        streamingChannel.modelIds[0] = -1;
    }

    if (ms_bLoadingBigModel)
    {
        if (streamingChannel.LoadStatus != LOADSTATE_Requested)
        {
            ms_bLoadingBigModel = false;
            memset(&ms_channel[1], 0xFFu, 64u);
        }
    }
    return true;
}

void CStreaming::RemoveModel(int Modelindex)
{
    plugin::CallDynGlobal<int>(0x4089A0, Modelindex);
}

void CStreaming::RemoveTxdModel(int Modelindex)
{
    RemoveModel(Modelindex + 20000);
}

void CStreaming::MakeSpaceFor(int memoryToCleanInBytes)
{
    if (ms_memoryUsed >= (ms_memoryAvailable - memoryToCleanInBytes))
    {
        while (RemoveLeastUsedModel(0x20u))
        {
            if (ms_memoryUsed < (ms_memoryAvailable - memoryToCleanInBytes))
                return;
        }
        DeleteRwObjectsBehindCamera(ms_memoryAvailable - memoryToCleanInBytes);
    }
}

bool CStreaming::RemoveLoadedVehicle() {
    return plugin::CallAndReturnDynGlobal<bool>(0x40C020);
}

void CStreaming::RetryLoadFile(int streamNum) {
    plugin::CallDynGlobal<int>(0x4076C0, streamNum);
}


DWORD CStreaming::LoadRequestedModels()
{
    DWORD channelIndex = 0;

    if (ms_bLoadingBigModel)
    {
        channelIndex = 0;
        ms_numberOfBytesRead = 0;
    }
    else
    {
        channelIndex = ms_numberOfBytesRead;
    }
    if (ms_channel[channelIndex].LoadStatus)
    {
        ProcessLoadingChannel(channelIndex);
        channelIndex = ms_numberOfBytesRead;
    }
    if (!ms_bLoadingBigModel)
    {
        int otherChannelIndex = 1 - channelIndex;
        if (!ms_channel[otherChannelIndex].LoadStatus)
        {
            RequestModelStream(1 - channelIndex);
            channelIndex = ms_numberOfBytesRead;
        }
        if (!ms_channel[channelIndex].LoadStatus && !ms_bLoadingBigModel)
        {
            RequestModelStream(channelIndex);
            channelIndex = ms_numberOfBytesRead;
        }
    }
    if (ms_channel[channelIndex].LoadStatus != LOADSTATE_Requested)
    {
        ms_numberOfBytesRead = 1 - channelIndex;
    }
    return channelIndex;
}

//////////////
// NOT TESTED
//////////////
bool CStreaming::FlushRequestList()
{
    std::printf(" CStreaming::FlushRequestList called\n");

    CStreamingInfo *streamingInfo = nullptr;
    CStreamingInfo *nextStreamingInfo = nullptr;

    if (ms_pStartRequestedList->m_nNextIndex == -1)
    {
        streamingInfo = nullptr;
    }
    else
    {
        streamingInfo = &CStreamingInfo::ms_pArrayBase[ms_pStartRequestedList->m_nNextIndex];
    }
    if (streamingInfo != ms_pEndRequestedList)
    {
        do
        {
            if (streamingInfo->m_nNextIndex == -1)
            {
                nextStreamingInfo = nullptr;
            }
            else
            {
                nextStreamingInfo = &CStreamingInfo::ms_pArrayBase[streamingInfo->m_nNextIndex];
            }
            RemoveModel(streamingInfo - ms_aInfoForModel);
            streamingInfo = nextStreamingInfo;
        } while (nextStreamingInfo != ms_pEndRequestedList);
    }
    return FlushChannels();
}

bool CStreaming::AddToLoadedVehiclesList(int modelIndex) {
    return plugin::CallAndReturnDynGlobal<bool, int>(0x408000, modelIndex);
}

void CStreaming::DeleteRwObjectsBehindCamera(int memoryToCleanInBytes) {
    plugin::CallDynGlobal<int>(0x40D7C0, memoryToCleanInBytes);
}

bool CStreaming::RemoveLeastUsedModel(unsigned int StreamingFlags) {
    return plugin::CallAndReturnDynGlobal<bool, unsigned int>(0x40CFD0, StreamingFlags);
}
