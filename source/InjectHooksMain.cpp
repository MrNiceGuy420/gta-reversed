#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef char(__cdecl *hCStreaming__ConvertBufferToObject)
(
    unsigned char * pBuffer, int modelId
);
auto OLD_CStreaming__ConvertBufferToObject = (hCStreaming__ConvertBufferToObject)0x40C6B0;

char __cdecl CStreaming__ConvertBufferToObject(unsigned char * pBuffer, int modelId);


void InjectHooksMain(void)
{
   CAnimManager::InjectHooks();
   CTaskManager::InjectHooks();
   CStreaming::InjectHooks();


    
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreaming__ConvertBufferToObject, CStreaming__ConvertBufferToObject);

    DetourTransactionCommit(); 
    
}


char __cdecl CStreaming__ConvertBufferToObject(unsigned char * pBuffer, int modelId)
{

    std::printf("CStreaming::ConvertBufferToObject called! modelId: %d | pBuffer: %s\n", modelId, pBuffer);

    //return OLD_CStreaming__ConvertBufferToObject(pBuffer, modelId);
  

    int BufferSize; // eax
    unsigned int BufferSize1; // ebp
    RwStream *pRwStream0; // eax
    RwStream *pRwStream1; // edi
    CBaseModelInfo *pBaseModelInfo0; // ebp
    int animFileIndex; // eax
    int wTxdIndex; // ecx
    TxdDef *pTxdDef1; // eax
    RwStream *pRwStream2; // eax
    TxdDef *pTxdDef0; // eax
    int txdIndex; // eax
    char bTxdLoaded; // al
    CBaseModelInfo *pBaseModelInfo; // eax
    CStreamingInfo *pStreamingInfo; // ecx
    int streamingMemoryUsed; // eax
    int txdIndex1; // [esp-18h] [ebp-38h]
    CStreamingInfo *pStreamingInfo1; // [esp-14h] [ebp-34h]
    RtDict* pRtDictionary; // [esp+0h] [ebp-20h]
    int data[2]; // [esp+4h] [ebp-1Ch]
    RwChunkHeaderInfo chunkHeaderInfo; // [esp+Ch] [ebp-14h]
    char bLoaded; // [esp+24h] [ebp+4h]


    BufferSize = CStreaming::ms_aInfoForModel[modelId].m_nCdSize << 11;
    BufferSize1 = BufferSize;
    data[0] = (int)pBuffer;
    data[1] = BufferSize;
    pRwStream0 = _rwStreamInitialize(&gRwStream, 0, rwSTREAMMEMORY, rwSTREAMREAD, data);
    pRwStream1 = pRwStream0;
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
                                pStreamedScripts.LoadStreamedScript(pRwStream0, modelId - 26230);
                            }
                            else
                            {
                                CVehicleRecording::Load(pRwStream0, modelId - 25755, BufferSize1);
                            }
                        } 
                        else
                        {
                            if (!(CStreaming::ms_aInfoForModel[modelId].m_nFlags & 0xE)
                                && !CStreaming::AreAnimsUsedByRequestedModels(modelId - 25575))
                            {
                                CStreaming::RemoveModel(modelId);
                                RwStreamClose(pRwStream1, data);
                                return 0;
                            }
                            CAnimManager::LoadAnimFile(pRwStream1, 1, 0);
                            CAnimManager::CreateAnimAssocGroups();
                        }
                    }
                    else
                    {
                        ThePaths.LoadPathFindData(pRwStream0, modelId - 25511);
                    }
                }
                else if (!CIplStore::LoadIpl(modelId - 25255, pBuffer, BufferSize1))
                {
                    CStreaming::RemoveModel(modelId);
                    CStreaming::RequestModel(modelId, CStreaming::ms_aInfoForModel[modelId].m_nFlags);
                    RwStreamClose(pRwStream1, data);
                    return 0;
                }
            }
            else if (!CColStore::LoadCol(modelId - 25000, pBuffer, BufferSize1))
            {
            LABEL_41:
                CStreaming::RemoveModel(modelId);
                CStreaming::RequestModel(modelId, CStreaming::ms_aInfoForModel[modelId].m_nFlags);
                RwStreamClose(pRwStream1, data);
                return 0;
            }
        }
        else
        {
            if ((CTxdStore::ms_pTxdPool->m_byteMap[modelId - 20000].IntValue() & 0x80u) == 0)
                pTxdDef0 = &CTxdStore::ms_pTxdPool->m_pObjects[modelId - 20000]; //m_pObjects + 12 * (modelId - 20000);
            else
                pTxdDef0 = 0;
            txdIndex = pTxdDef0->m_wParentIndex;              
            if (txdIndex != -1 && !CTxdStore::GetTxd(txdIndex))
                goto LABEL_38;
            if (!(CStreaming::ms_aInfoForModel[modelId].m_nFlags & 0xE)
                && !CStreaming::AreTexturesUsedByRequestedModels(modelId - 20000))
            {
                CStreaming::RemoveModel(modelId);
                RwStreamClose(pRwStream1, data);
                return 0;
            }
            txdIndex1 = modelId - 20000;
            if (CStreaming::ms_bLoadingBigModel)
            {
                bTxdLoaded = CTxdStore::StartLoadTxd(txdIndex1, pRwStream1);
                if (!bTxdLoaded)
                {
                LABEL_38:
                    CStreaming::RemoveModel(modelId);
                    CStreaming::RequestModel(modelId, CStreaming::ms_aInfoForModel[modelId].m_nFlags);
                    RwStreamClose(pRwStream1, data);
                    return 0;
                }
                CStreaming::ms_aInfoForModel[modelId].m_nLoadState = 4;
            }
            else
            {
                bTxdLoaded = CTxdStore::LoadTxd(txdIndex1, pRwStream1);
            }
            if (!bTxdLoaded)
                goto LABEL_38;
        }
        pBaseModelInfo0 = (CVehicleModelInfo *)pBuffer;
        goto LABEL_56;
    }
    pBaseModelInfo0 = CModelInfo::ms_modelInfoPtrs[modelId];
    animFileIndex = pBaseModelInfo0->GetAnimFileIndex();
    wTxdIndex = pBaseModelInfo0->m_nTxdIndex;

    if ((CTxdStore::ms_pTxdPool->m_byteMap[wTxdIndex].IntValue() & 0x80u) == 0)
        pTxdDef1 = &CTxdStore::ms_pTxdPool->m_pObjects[wTxdIndex] ;//m_pObjects + 12 * wTxdIndex;
    else
        pTxdDef1 = 0;

    if (!pTxdDef1->m_pRwDictionary || animFileIndex != -1 && !CAnimManager::ms_aAnimBlocks[animFileIndex].bLoaded)
        goto LABEL_41;

    CTxdStore::AddRef(wTxdIndex);
    if (animFileIndex != -1)
        CAnimManager::AddAnimBlockRef(animFileIndex);
    CTxdStore::SetCurrentTxd(pBaseModelInfo0->m_nTxdIndex);
    if (pBaseModelInfo0->GetRwModelType() == 1)
    {
        pRtDictionary = 0;
        RwStreamReadChunkHeaderInfo(pRwStream1, &chunkHeaderInfo);
        if (chunkHeaderInfo.type == 43)
        {
            pRtDictionary = RtDictSchemaStreamReadDict(&RpUVAnimDictSchema, pRwStream1);
            RtDictSchemaSetCurrentDict(&RpUVAnimDictSchema, pRtDictionary);
        }
        RwStreamClose(pRwStream1, data);
        pRwStream2 = _rwStreamInitialize(&gRwStream, 0, rwSTREAMMEMORY, rwSTREAMREAD, data);
        CFileLoader::LoadAtomicFile(pRwStream2, modelId);
        if (pRtDictionary)
            RtDictDestroy(pRtDictionary);
    }
    else
    {
        pBaseModelInfo0->GetModelType();
        bLoaded = CFileLoader::LoadClumpFile(pRwStream1, modelId);
    }
    if (CStreaming::ms_aInfoForModel[modelId].m_nLoadState != 4)
    {
        CTxdStore::RemoveRefWithoutDelete(pBaseModelInfo0->m_nTxdIndex);
        if (animFileIndex != -1)
            CAnimManager::RemoveAnimBlockRefWithoutDelete(animFileIndex);
        if (!bLoaded)
            goto LABEL_23;
        if (pBaseModelInfo0->GetModelType() == 6)
            bLoaded = CStreaming::AddToLoadedVehiclesList(modelId);
    }
    if (!bLoaded)
    {
    LABEL_23:
        CStreaming::RemoveModel(modelId);
        CStreaming::RequestModel(modelId, CStreaming::ms_aInfoForModel[modelId].m_nFlags);
        RwStreamClose(pRwStream1, data);
        return 0;
    }
LABEL_56:
    RwStreamClose(pRwStream1, data);
    if (modelId >= 20000)
    {
        if (modelId >= 25000 && (modelId < 25575 || modelId >= 25755) && modelId < 26230
            || CStreaming::ms_aInfoForModel[modelId].m_nFlags & 6)
        {
            goto LABEL_70;
        }
        pStreamingInfo1 = CStreaming::ms_startLoadedList;
        pStreamingInfo = &CStreaming::ms_aInfoForModel[modelId];
    }
    else
    {
        if (pBaseModelInfo0->GetModelType() == 6
            || pBaseModelInfo0->GetModelType() == 7)
        {
            goto LABEL_70;
        }
        pBaseModelInfo = pBaseModelInfo0->AsAtomicModelInfoPtr();
        if (pBaseModelInfo)
            pBaseModelInfo->m_nAlpha = -((CStreaming::ms_aInfoForModel[modelId].m_nFlags & 0x24) != 0);
        pStreamingInfo = &CStreaming::ms_aInfoForModel[modelId];
        if (CStreaming::ms_aInfoForModel[modelId].m_nFlags & 6)
            goto LABEL_70;
        pStreamingInfo1 = CStreaming::ms_startLoadedList;
    }
    pStreamingInfo->AddToList( pStreamingInfo1);
LABEL_70:
    if (CStreaming::ms_aInfoForModel[modelId].m_nLoadState != 4)
    {
        streamingMemoryUsed = CStreaming::ms_memoryUsed;
        CStreaming::ms_aInfoForModel[modelId].m_nLoadState = 1;
        CStreaming::ms_memoryUsed = (CStreaming::ms_aInfoForModel[modelId].m_nCdSize << 11) + streamingMemoryUsed;
    }
    return 1;//*/
}
