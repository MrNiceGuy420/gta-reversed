#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef char(__cdecl*  hCStreaming__RequestModelStream)
(
    DWORD channelIndex
);

auto OLD_CStreaming__RequestModelStream = (hCStreaming__RequestModelStream)0x40CBA0;
char __cdecl CStreaming__RequestModelStream(DWORD channelIndex);

void InjectHooksMain(void)
{
    InjectHook(0x15674C0, &CStreamingInfo::AddToList, PATCH_JUMP);
    InjectHook(0x40A45E, &CStreaming::LoadAllRequestedModels, PATCH_JUMP);
    InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);
    InjectHook(0x40E170, &CStreaming::ProcessLoadingChannel, PATCH_JUMP);
    InjectHook(0x40E460, &CStreaming::FlushChannels, PATCH_JUMP);
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreaming__RequestModelStream, CStreaming__RequestModelStream);
  
    DetourTransactionCommit(); 
   
}


char __cdecl CStreaming__RequestModelStream(DWORD channelIndex)
{
    int streamingBufferSize = 0; // edi
    DWORD modelIndexForChannel; // eax
    unsigned __int8 modelFlags; // al
    int numPriorityRequests; // eax
    signed int loopIndex = 0 ; // [esp+8h] [ebp-50h]
    int sectorCount = 0; // [esp+Ch] [ebp-4Ch]

    //int *v26; // [esp+14h] [ebp-44h]
    //int v27; // [esp+18h] [ebp-40h]



    int CdStreamLastPosn = CdStreamGetLastPosn();
    int modelId = CStreaming::GetNextFileOnCd(CdStreamLastPosn, 1);
    if (modelId == -1)
    {
        return modelId;
    }

    HANDLE blockOffsetMimg = 0;
    unsigned int blockCount = 0;

    CStreamingInfo * streamingInfo = &CStreaming::ms_aInfoForModel[modelId];
    if (!(streamingInfo->m_nFlags & 14))
    {
        do
        {
            bool areTexturesUsedByRequestedModels = false;
            if (modelId < 20000 || modelId >= 25000
                || (areTexturesUsedByRequestedModels = CStreaming::AreTexturesUsedByRequestedModels(modelId - 20000), modelId))
            {
                if (modelId < 25575)
                    break;
                if (modelId >= 25755)
                    break;

                bool areAnimsUsedByRequestedModels = CStreaming::AreAnimsUsedByRequestedModels(modelId - 25575);
                if (areAnimsUsedByRequestedModels)
                    break;
            }
            CStreaming::RemoveModel(modelId);
            if (streamingInfo->m_nCdSize)
            {
                /*blockOffsetMimg = CStreaming::ms_aInfoForModel[modelId4].m_nCdPosn
                + dword_8E4904[12 * CStreaming::ms_aInfoForModel[modelId4].m_nImgId];
                blockOffsetMimg = (CStreaming::ms_aInfoForModel[modelId4].m_nCdPosn
                + dword_8E4904[12 * CStreaming::ms_aInfoForModel[modelId4].m_nImgId]);*/

                // needs verification
                blockOffsetMimg = (HANDLE)(streamingInfo->m_nCdPosn + CStreaming::ms_files[12 * streamingInfo->m_nImgId].m_StreamHandle);

                blockCount = streamingInfo->m_nCdSize;
            }

            modelId = CStreaming::GetNextFileOnCd(blockCount + (int)blockOffsetMimg, 1);
            if (modelId == -1)
            {
                std::printf("modelId is -1 in while\n");
                return modelId;
            }

            streamingInfo = &CStreaming::ms_aInfoForModel[modelId];

        } while (!(streamingInfo->m_nFlags & 14));
    }


    if (modelId == -1)
    {
        std::printf("modelId is -1 after while\n");
        return modelId;
    }

    if (blockCount > CStreaming::ms_streamingBufferSize)
    {
        if (channelIndex == 1 || CStreaming::ms_channel[1].LoadStatus)
        {
            int SomeSizeToReturn = 20 * modelId + 9325760;
            return SomeSizeToReturn;
        }
        CStreaming::ms_bLoadingBigModel = 1;
    }

    //loopIndex = 0;
    //v26 = &v27;


    bool isBlockCountGreaterThan200 = false; // [esp+2h] [ebp-56h]
    bool isModelTypePed = false; // [esp+3h] [ebp-55h]

    while (modelId != -1)
    {
        streamingInfo = &CStreaming::ms_aInfoForModel[modelId];
        if (streamingInfo->m_nLoadState != 2)
            break;

        if (streamingInfo->m_nCdSize)
            blockCount = streamingInfo->m_nCdSize;
        if (CStreaming::ms_numPriorityRequests && !(streamingInfo->m_nFlags & 0x10))
            break;
        if (modelId >= 20000)
        {
            if (modelId < 25575 || modelId >= 25755)
            {
                if (isBlockCountGreaterThan200 && blockCount > 200)
                    break;
            }
            else if (CCutsceneMgr::ms_cutsceneProcessing || CStreaming::ms_aInfoForModel[7].m_nLoadState != 1)// CCutsceneMgr::ms_cutsceneProcessing || CStreaming::ms_aInfoForModel[7].uiLoadFlag
            {
                break;
            }
        }
        else
        {
            CBaseModelInfo * pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
            if (isModelTypePed && pBaseModelInfo->GetModelType() == 7)
                break;
            if (isBlockCountGreaterThan200 && pBaseModelInfo->GetModelType() == 6)
                break;
            eStreamingLoadState loadState = (eStreamingLoadState)CStreaming::ms_aInfoForModel[pBaseModelInfo->m_nTxdIndex + 20000].m_nLoadState;
            if (loadState != 1 && loadState != 3)
                break;
            int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            if (animFileIndex != -1)
            {
                eStreamingLoadState loadState2 = (eStreamingLoadState)CStreaming::ms_aInfoForModel[animFileIndex + 25575].m_nLoadState;
                if (loadState2 != 1 && loadState2 != 3)
                    break;
            }
        }
        streamingBufferSize = CStreaming::ms_streamingBufferSize;
        modelIndexForChannel = loopIndex + 38 * channelIndex;
        CStreaming::ms_channel[0].modelStreamingBufferOffsets[modelIndexForChannel] = sectorCount;
        CStreaming::ms_channel[0].modelIds[modelIndexForChannel] = modelId;

        sectorCount += blockCount;      // sectorCount += blockCount
        if (sectorCount > streamingBufferSize && loopIndex > 0)
        {
            sectorCount = sectorCount - blockCount;
            break;
        }
       //  *v26 = modelId;
       //  ++v26;

        CBaseModelInfo *  pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
        if (modelId < 20000)
        {
            if (pBaseModelInfo->GetModelType() == 7)
                isModelTypePed = true;
            if (pBaseModelInfo->GetModelType() != 6)
                goto LABEL_55;
        LABEL_54:
            isBlockCountGreaterThan200 = true;
            goto LABEL_55;
        }
        if (blockCount > 200)
            goto LABEL_54;
    LABEL_55:
        streamingInfo->m_nLoadState = LOADSTATE_Channeled;
        streamingInfo->RemoveFromList();
        modelFlags = streamingInfo->m_nFlags;
        --CStreaming::ms_numModelsRequested;
        if (modelFlags & 0x10)
        {
            numPriorityRequests = CStreaming::ms_numPriorityRequests - 1;
            streamingInfo->m_nFlags &= 0xEFu;
            CStreaming::ms_numPriorityRequests = numPriorityRequests;
        }
        modelId = streamingInfo->m_nNextIndexOnCd;

        /*v18 = __OFSUB__(loopIndex + 1, 16);
        v17 = loopIndex++ - 15 < 0;
        if (!(v17 ^ v18))
        goto LABEL_62;*/

        if (!(loopIndex < 16))
        {
            goto LABEL_62;
        }
    }
    if (loopIndex < 16)
    {
        memset(((char*)CStreaming::ms_channel) + 4 * (loopIndex + 38 * channelIndex), 0xFFu, 4 * (16 - loopIndex)); 
    }
LABEL_62:

    CdStreamRead(channelIndex, (int)((&CStreaming::ms_pStreamingBuffer)[channelIndex]), (int)blockOffsetMimg, sectorCount);


    tStreamingChannel & streamingChannel = CStreaming::ms_channel[channelIndex];
    streamingChannel.LoadStatus = LOADSTATE_LOADED;
    streamingChannel.iLoadingLevel = 0;
    streamingChannel.iBlockCount = sectorCount;
    streamingChannel.iBlockOffset = (int)blockOffsetMimg;
    streamingChannel.OnBeginRead = 0;

    // LOBYTE(modelId) = (*(BYTE*)0x9654C4); // byte_9654C4; //(*(DWORD*)0xB5F878)
    if ((*(BYTE*)0x9654C4))
        (*(BYTE*)0x9654C4) = 0;

    return (*(BYTE*)0x9654C4); // modelId;

                               //return OLD_CStreaming__RequestModelStream(channelIndex);
}

char __cdecl CStreaming__RequestModelStream_test(DWORD channelIndex)
{
    int CdStreamLastPosn; // eax
    int modelId; // eax
    int modelId2; // esi
    int modelId4; // edi

    int modelId3; // ebx
    CStreamingInfo *pStreamingInfo; // ebp
    CBaseModelInfo *pBaseModelInfo; // edi
    char loadState; // al
    int animFileIndex; // eax
    char loadState2; // al
    int streamingBufferSize; // edi
    DWORD modelIndexForChannel; // eax
    int BlockAndSectorCount; // ecx
    unsigned __int8 modelFlags; // al
    int numPriorityRequests; // eax
    bool v17; // sf
    unsigned __int8 v18; // of
    bool isBlockCountGreaterThan200; // [esp+2h] [ebp-56h]
    bool isModelTypePed; // [esp+3h] [ebp-55h]
    unsigned int blockCount; // [esp+4h] [ebp-54h]
    signed int loopIndex; // [esp+8h] [ebp-50h]
    int sectorCount; // [esp+Ch] [ebp-4Ch]
    HANDLE blockOffsetMimg; // [esp+10h] [ebp-48h]
    int *v26; // [esp+14h] [ebp-44h]
    int v27; // [esp+18h] [ebp-40h]

   // std::printf("CStreaming__RequestModelStream called\n");

    sectorCount = 0;
    CdStreamLastPosn = CdStreamGetLastPosn();
    isModelTypePed = 0;
    isBlockCountGreaterThan200 = 0;
    modelId = CStreaming::GetNextFileOnCd(CdStreamLastPosn, 1);




    modelId2 = modelId;
    if (modelId == -1)
    {
        std::printf("modelId is -1 before while \n");
        return modelId;
    }
    modelId4 = modelId;
    unsigned char streamingInfoFlags = CStreaming::ms_aInfoForModel[modelId].m_nFlags;
    if (!(streamingInfoFlags & 14))
    {
        /// ------ Remove LATER
        //return OLD_CStreaming__RequestModelStream(channelIndex);
        /// [END] ------ Remove LATER

        do
        {
            bool areTexturesUsedByRequestedModels = false;
            if (modelId2 < 20000
                || modelId2 >= 25000
                || (areTexturesUsedByRequestedModels = CStreaming::AreTexturesUsedByRequestedModels(modelId2 - 20000), modelId))
            {
                if (modelId2 < 25575)
                    break;
                if (modelId2 >= 25755)
                    break;

                bool areAnimsUsedByRequestedModels = CStreaming::AreAnimsUsedByRequestedModels(modelId2 - 25575);
                if (areAnimsUsedByRequestedModels)
                    break;
            }
            CStreaming::RemoveModel(modelId2);
            if (CStreaming::ms_aInfoForModel[modelId4].m_nCdSize)
            {
                /*blockOffsetMimg = CStreaming::ms_aInfoForModel[modelId4].m_nCdPosn
                    + dword_8E4904[12 * CStreaming::ms_aInfoForModel[modelId4].m_nImgId];
                blockOffsetMimg = (CStreaming::ms_aInfoForModel[modelId4].m_nCdPosn
                    + dword_8E4904[12 * CStreaming::ms_aInfoForModel[modelId4].m_nImgId]);*/

                blockOffsetMimg = (HANDLE)(CStreaming::ms_aInfoForModel[modelId4].m_nCdPosn
                    + CStreaming::ms_files[12 * CStreaming::ms_aInfoForModel[modelId4].m_nImgId].m_StreamHandle);

                blockCount = CStreaming::ms_aInfoForModel[modelId4].m_nCdSize;
            }

            modelId = CStreaming::GetNextFileOnCd(blockCount + (int)blockOffsetMimg, 1);
            modelId2 = modelId;
            if (modelId == -1)
            {
                std::printf("modelId is -1 in while\n");
                return modelId;

            }
            modelId4 = modelId;
        } while (!(CStreaming::ms_aInfoForModel[modelId].m_nFlags & 14));
    }


    if (modelId2 == -1)
    {
        std::printf("modelId is -1 after while\n");
        return modelId;
    }

    modelId = 20 * modelId2 + 9325760;
    if (CStreaming::ms_aInfoForModel[modelId2].m_nCdSize)
    {
        //blockOffsetMimg = (CStreaming::ms_aInfoForModel[modelId2].m_nCdPosn
        //    + dword_8E4904[12 * CStreaming::ms_aInfoForModel[modelId2].m_nImgId]);

        blockOffsetMimg = (HANDLE)(CStreaming::ms_aInfoForModel[modelId2].m_nCdPosn
            + CStreaming::ms_files[12 * CStreaming::ms_aInfoForModel[modelId2].m_nImgId].m_StreamHandle);
        
        blockCount = CStreaming::ms_aInfoForModel[modelId2].m_nCdSize;
    }
    if (blockCount > CStreaming::ms_streamingBufferSize)
    {
        if (channelIndex == 1 || CStreaming::ms_channel[1].LoadStatus)
            return modelId;
        CStreaming::ms_bLoadingBigModel = 1;
    }
    loopIndex = 0;
    v26 = &v27;

    /// ------ Remove LATER
    return OLD_CStreaming__RequestModelStream(channelIndex);
    /// [END] ------ Remove LATER

    while (modelId2 != -1)
    {
        modelId3 = modelId2;
        if (CStreaming::ms_aInfoForModel[modelId2].m_nLoadState != 2)
            break;
        pStreamingInfo = &CStreaming::ms_aInfoForModel[modelId3];
        if (CStreaming::ms_aInfoForModel[modelId3].m_nCdSize)
            blockCount = CStreaming::ms_aInfoForModel[modelId3].m_nCdSize;
        if (CStreaming::ms_numPriorityRequests && !(CStreaming::ms_aInfoForModel[modelId3].m_nFlags & 0x10))
            break;
        if (modelId2 >= 20000)
        {
            if (modelId2 < 25575 || modelId2 >= 25755)
            {
                if (isBlockCountGreaterThan200 && blockCount > 200)
                    break;
            }
            else if (CCutsceneMgr::ms_cutsceneProcessing || CStreaming::ms_aInfoForModel[7].m_nLoadState != 1)// CCutsceneMgr::ms_cutsceneProcessing || CStreaming::ms_aInfoForModel[7].uiLoadFlag
            {
                break;
            }
        }
        else
        {
            pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId2];
            if (isModelTypePed && pBaseModelInfo->GetModelType() == 7)
                break;
            if (isBlockCountGreaterThan200 && pBaseModelInfo->GetModelType() == 6)
                break;
            loadState = CStreaming::ms_aInfoForModel[pBaseModelInfo->m_nTxdIndex + 20000].m_nLoadState;
            if (loadState != 1 && loadState != 3)
                break;
            animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            if (animFileIndex != -1)
            {
                loadState2 = CStreaming::ms_aInfoForModel[animFileIndex + 25575].m_nLoadState;
                if (loadState2 != 1 && loadState2 != 3)
                    break;
            }
        }
        streamingBufferSize = CStreaming::ms_streamingBufferSize;
        modelIndexForChannel = loopIndex + 38 * channelIndex;
        CStreaming::ms_channel[0].modelStreamingBufferOffsets[modelIndexForChannel] = sectorCount;
        CStreaming::ms_channel[0].modelIds[modelIndexForChannel] = modelId2;
        BlockAndSectorCount = blockCount + sectorCount;
        sectorCount = BlockAndSectorCount;          // sectorCount += blockCount
        if (BlockAndSectorCount > streamingBufferSize && loopIndex > 0)
        {
            sectorCount = BlockAndSectorCount - blockCount;
            break;
        }
       // *v26 = modelId2;
       // ++v26;

        pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId2];
        if (modelId2 < 20000)
        {
            if (pBaseModelInfo->GetModelType() == 7)
                isModelTypePed = 1;
            if (pBaseModelInfo->GetModelType() != 6)
                goto LABEL_55;
        LABEL_54:
            isBlockCountGreaterThan200 = 1;
            goto LABEL_55;
        }
        if (blockCount > 200)
            goto LABEL_54;
    LABEL_55:
        pStreamingInfo->m_nLoadState = LOADSTATE_Channeled;
        CStreaming::ms_aInfoForModel[modelId3].RemoveFromList();
        modelFlags = CStreaming::ms_aInfoForModel[modelId3].m_nFlags;
        --CStreaming::ms_numModelsRequested;
        if (modelFlags & 0x10)
        {
            numPriorityRequests = CStreaming::ms_numPriorityRequests - 1;
            pStreamingInfo->m_nFlags &= 0xEFu;
            CStreaming::ms_numPriorityRequests = numPriorityRequests;
        }
        modelId2 = CStreaming::ms_aInfoForModel[modelId3].m_nNextIndexOnCd;

        /*v18 = __OFSUB__(loopIndex + 1, 16);
        v17 = loopIndex++ - 15 < 0;
        if (!(v17 ^ v18))
            goto LABEL_62;*/

        if (!(loopIndex < 16))
        {
            goto LABEL_62;
        }
    }
    if (loopIndex < 16)
        memset(CStreaming::ms_channel + 4 * (loopIndex + 38 * channelIndex), 0xFFu, 4 * (16 - loopIndex));
LABEL_62:
    CdStreamRead(channelIndex, (int)((&CStreaming::ms_pStreamingBuffer)[channelIndex]), (int)blockOffsetMimg, sectorCount);


    tStreamingChannel & streamingChannel = CStreaming::ms_channel [channelIndex];
    streamingChannel.LoadStatus = LOADSTATE_LOADED;
    streamingChannel.iLoadingLevel  = 0;
    streamingChannel.iBlockCount  = sectorCount;
    streamingChannel.iBlockOffset  = (int)blockOffsetMimg;
    streamingChannel.OnBeginRead  = 0;
  

   // LOBYTE(modelId) = (*(BYTE*)0x9654C4); // byte_9654C4; //(*(DWORD*)0xB5F878)
    if ((*(BYTE*)0x9654C4))
        (*(BYTE*)0x9654C4) = 0;

    return (*(BYTE*)0x9654C4); // modelId;

    //return OLD_CStreaming__RequestModelStream(channelIndex);
}