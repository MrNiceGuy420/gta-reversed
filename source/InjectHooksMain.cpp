#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef int(__cdecl* _hCdStreamRead)(DWORD streamNum, int lpBuffer, int streamIndex, int sectorCount);
_hCdStreamRead _CdStreamRead = (_hCdStreamRead)0x406A20;


bool __cdecl CdStreamReadHook(DWORD streamNum, int lpBuffer, int streamIndex, int sectorCount)
{
    std::printf("calling CdStreamRead\n");

    return _CdStreamRead(streamNum, lpBuffer, streamIndex, sectorCount);
}

    
typedef char(__cdecl*  hCStreaming__RequestModelStream)
(
    DWORD channelIndex
);

auto OLD_CStreaming__RequestModelStream = (hCStreaming__RequestModelStream)0x40CBA0;
//void __cdecl CStreaming__RequestModelStream(int32_t channelIndex);

char __cdecl CStreaming__RequestModelStream_test(DWORD channelIndex);

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
    DetourAttach(&(PVOID&)OLD_CStreaming__RequestModelStream, CStreaming__RequestModelStream_test);
  
    //DetourAttach(&(PVOID&)_CdStreamRead, CdStreamReadHook);
    

    DetourTransactionCommit(); 
   
}



struct s0 {
    signed char pad7 [7];
    unsigned char f7;
    int32_t f8;
};

//uint32_t (*(uint32_t*)0x8e4ca8) = 0xffffffff;

struct s1 {
    signed char pad6 [6];
    unsigned char f6;
    signed char pad16 [9];
    signed char f16;
};

//int32_t (*(int32_t*)0x8e4ba0) = -1;

//signed char (*(signed char*)0xb5f852) = -1;

//signed char (*(signed char*)0x8e4d5c) = -1;

struct s3 {
    signed char pad16 [16];
    int32_t f16;
    signed char pad56 [36];
    int32_t f56;
};

struct s2 {
    struct s3* f0;
    signed char pad10 [6];
    int16_t f10;
};



//int32_t (*(int32_t*)0x8e4cb8) = -1;

struct s4 {
    signed char pad16 [16];
    int32_t f16;
};

struct s5 {
    signed char pad16 [16];
    int32_t f16;
};



//signed char (*(signed char*)0x9654c4) = -1;

//uint32_t (*(uint32_t*)0x8e4b78) = 0xffffffff;

//signed char (*(signed char*)0x8e4a58) = -1;

/* CStreaming::RequestModelStream(int) */
void CStreaming__RequestModelStream(int32_t channelIndex) {
    
    std::printf("CStreaming__RequestModelStream called!\n");
    uint32_t sectorCount = 0;
    uint32_t CdStreamLastPosn = 0;
    signed char isModelTypePed = 0;
    signed char isBlockCountGreaterThan200 = 0;
    int32_t modelId = 0;
    int32_t modelId4 = 0;
    int32_t modelId5 = 0;
    signed char al9 = 0;
    signed char al10 = 0;
    uint32_t m_nCdSize = 0;
    int32_t blockOffsetMimg2 = 0;
    int32_t blockOffsetMimg = 0;
    uint32_t eax14 = 0;
    uint32_t m_nCdSize2 = 0;
    int32_t eax16 = 0;
    int32_t eax17 = 0;
    uint32_t edx18 = 0;
    struct s0* eax19 = 0;
    uint32_t ecx20 = 0;
    bool below_or_equal21 = 0;
    uint32_t loopIndex = 0;
    int32_t ebx23 = 0;
    struct s1* ebp24 = 0;
    int32_t eax25 = 0;
    signed char al26 = 0;
    bool zf27 = 0;
    struct s2* edi28 = 0;
    struct s3* edx29 = 0;
    signed char al30 = 0;
    struct s3* eax31 = 0;
    signed char al32 = 0;
    int32_t eax33 = 0;
    struct s3* edx34 = 0;
    int32_t eax35 = 0;
    int32_t eax36 = 0;
    uint32_t edi37 = 0;
    uint32_t eax38 = 0;
    uint32_t ecx39 = 0;
    int32_t ecx40 = 0;
    bool zf41 = 0;
    int32_t eax42 = 0;
    uint32_t loopIndex2 = 0;
    struct s4* edx44 = 0;
    signed char al45 = 0;
    struct s5* eax46 = 0;
    signed char al47 = 0;
    uint32_t ecx48 = 0;
    int32_t* edi49 = 0;
    int32_t ecx50 = 0;
    int32_t modelId6 = 0;
    signed char al52 = 0;
    bool zf53 = 0;


    sectorCount = 0;
    CdStreamLastPosn = CdStreamGetLastPosn();
    isModelTypePed = 0;
    isBlockCountGreaterThan200 = 0;
    modelId = CStreaming::GetNextFileOnCd(CdStreamLastPosn, 1);
    modelId4 = modelId;
    if (modelId4 == -1) {
    returnFromFunction:
        return;
    }
    else {
        modelId5 = modelId4 ;
        if (!(14 & *reinterpret_cast<unsigned char*>(modelId5 + 0x8e4cc6))) {
            do {
                if (modelId4 < 0x4e20)
                    goto addr_40cc0b_5;
                if (modelId4 >= 0x61a8)
                    goto addr_40cc0b_5;
                al9 = CStreaming::AreTexturesUsedByRequestedModels(modelId4 - 0x4e20);
                if (!al9)
                    goto addr_40cc32_8;
            addr_40cc0b_5:
                if (modelId4 < 0x63e7)
                    break;
                if (modelId4 >= 0x649b)
                    break;
                al10 = CStreaming::AreAnimsUsedByRequestedModels(modelId4 - 0x63e7);
                if (al10)
                    break;
            addr_40cc32_8:
                CStreaming::RemoveModel(modelId4);
                m_nCdSize = *reinterpret_cast<uint32_t*>(modelId5 + 0x8e4ccc);
                if (!m_nCdSize) {
                    blockOffsetMimg2 = blockOffsetMimg;
                }
                else {
                    eax14 = *reinterpret_cast<unsigned char*>(modelId5 + 0x8e4cc7);
                    blockOffsetMimg2 = *reinterpret_cast<int32_t*>((eax14 ) + 0x8e4904) + *reinterpret_cast<int32_t*>(modelId5 + 0x8e4cc8);
                    blockOffsetMimg = blockOffsetMimg2;
                    m_nCdSize2 = m_nCdSize;
                }
                eax16 = CStreaming::GetNextFileOnCd(blockOffsetMimg2 + m_nCdSize2, 1);
                modelId4 = eax16;
                if (modelId4 == -1)
                    goto addr_40cf77_14;
                modelId5 = modelId4 ;
            } while (!(*reinterpret_cast<unsigned char*>(modelId5 + 0x8e4cc6) & 14));
        }
        if (modelId4 == -1)
            goto addr_40cf77_14;
        eax17 = modelId4 ;
        edx18 = *reinterpret_cast<uint32_t*>(eax17  + 0x8e4ccc);
        eax19 = reinterpret_cast<struct s0*>(eax17  + 0x8e4cc0);
        if (edx18) {
            ecx20 = eax19->f7;
            blockOffsetMimg = *reinterpret_cast<int32_t*>((ecx20 ) + 0x8e4904) + eax19->f8;
            m_nCdSize2 = edx18;
        }
        below_or_equal21 = m_nCdSize2 <= (*(uint32_t*)0x8e4ca8);
        if (!below_or_equal21)
            goto addr_40cce1_20;
    }
addr_40ccff_21:
    loopIndex = 0;
    while (modelId4 != -1 && (ebx23 = modelId4 , *reinterpret_cast<signed char*>(ebx23 + 0x8e4cd0) == 2)) {
        ebp24 = reinterpret_cast<struct s1*>(ebx23 + 0x8e4cc0);
        if (*reinterpret_cast<uint32_t*>(ebx23 + 0x8e4ccc)) {
            m_nCdSize2 = *reinterpret_cast<uint32_t*>(ebx23 + 0x8e4ccc);
        }
        eax25 = (*(int32_t*)0x8e4ba0);
        if (!eax25)
            goto addr_40cd56_26;
        if (!(*reinterpret_cast<unsigned char*>(ebx23 + 0x8e4cc6) & 16))
            break;
    addr_40cd56_26:
        if (modelId4 >= 0x4e20) {
            if (modelId4 < 0x63e7 || modelId4 >= 0x649b) {
                if (!isBlockCountGreaterThan200)
                    goto addr_40ce1d_30;
                if (m_nCdSize2 > 0xc8)
                    break;
            }
            else {
                al26 = (*(signed char*)0xb5f852);
                if (al26)
                    break;
                zf27 = (*(signed char*)0x8e4d5c) == 1;
                if (!zf27)
                    break;
            }
        }
        else {
         

            CBaseModelInfo * pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId4];
            int modelType = pBaseModelInfo->GetModelType();
            int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            edi28 = *reinterpret_cast<struct s2**>(modelId4  + 0xa9b0c8);
            if (!isModelTypePed)
                goto addr_40cd7c_36;
            edx29 = edi28->f0;
            al30 = static_cast<signed char>(modelType);
            if (al30 == 7)
                break;
        addr_40cd7c_36:
            if (!isBlockCountGreaterThan200)
                goto addr_40cd93_38;
            eax31 = edi28->f0;
            al32 = static_cast<signed char>(modelType);
            if (al32 == 6)
                break;
        addr_40cd93_38:
            eax33 = edi28->f10 + 0x4e20;
            if (*reinterpret_cast<signed char*>((eax33 )+ 0x8e4cd0) == 1)
                goto addr_40cdb2_40;
            if (*reinterpret_cast<signed char*>((eax33 )+ 0x8e4cd0) != 3)
                break;
        addr_40cdb2_40:
            edx34 = edi28->f0;
            eax35 = animFileIndex;
            if (eax35 == -1)
                goto addr_40ce1d_30;
            eax36 = eax35 + 0x63e7;
            if (*reinterpret_cast<signed char*>(eax36   + 0x8e4cd0) != 1)
                goto addr_40cdd1_43;
        }
    addr_40ce1d_30:
        edi37 = (*(uint32_t*)0x8e4ca8);
        eax38 = channelIndex * 38 + loopIndex;
        *reinterpret_cast<uint32_t*>(eax38 + 0x8e4aa0) = sectorCount;
        *reinterpret_cast<int32_t*>(eax38 + 0x8e4a60) = modelId4;
        ecx39 = sectorCount + m_nCdSize2;
        sectorCount = ecx39;
        if (static_cast<int32_t>(ecx39) <= static_cast<int32_t>(edi37))
            goto addr_40ce59_44;
        if (!static_cast<unsigned char>(static_cast<unsigned char>(static_cast<int32_t>(loopIndex) < 0) | static_cast<unsigned char>(loopIndex == 0)))
            goto addr_40cef2_46;
    addr_40ce59_44:
        if (modelId4 >= 0x4e20) {
            if (m_nCdSize2 <= 0xc8) {
            addr_40cea1_48:
                ebp24->f16 = 3;
                CStreamingInfo * pStreamingInfo = (CStreamingInfo *)ebp24;
                pStreamingInfo->RemoveFromList();
                //_ZN14CStreamingInfo14RemoveFromListEv(ebp24);
                ecx40 = (*(int32_t*)0x8e4cb8);
                zf41 = (*reinterpret_cast<unsigned char*>(ebx23 + 0x8e4cc6) & 16) == 0;
                (*(int32_t*)0x8e4cb8) = ecx40 - 1;
                if (!zf41) {
                    eax42 = (*(int32_t*)0x8e4ba0);
                    ebp24->f6 = static_cast<unsigned char>(ebp24->f6 & 0xef);
                    (*(int32_t*)0x8e4ba0) = eax42 - 1;
                }
            }
            else {
            addr_40ce9c_50:
                isBlockCountGreaterThan200 = 1;
                goto addr_40cea1_48;
            }
            modelId4 = *reinterpret_cast<int16_t*>(ebx23 + 0x8e4cc4);
            loopIndex2 = loopIndex + 1;
            loopIndex = loopIndex2;
            if (static_cast<int32_t>(loopIndex2) < 16)
                continue; else
                goto addr_40cef0_52;
        }
        else {

            CBaseModelInfo *  pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId4];
    

            edx44 = **reinterpret_cast<struct s4***>(modelId4 + 0xa9b0c8);
            al45 = static_cast<signed char>(pBaseModelInfo->GetModelType());
            if (al45 == 7) {
                isModelTypePed = 1;
            }
            eax46 = **reinterpret_cast<struct s5***>(modelId4 + 0xa9b0c8);
            al47 = static_cast<signed char>(pBaseModelInfo->GetModelType());
            if (al47 != 6)
                goto addr_40cea1_48;
            goto addr_40ce9c_50;
        }
    addr_40cdd1_43:
        if (*reinterpret_cast<signed char*>(eax36  + 0x8e4cd0) != 3)
            break;
        goto addr_40ce1d_30;
    }
addr_40cef8_58:
    if (static_cast<int32_t>(loopIndex) < 16) {
        ecx48 = 16 - loopIndex;
        edi49 = reinterpret_cast<int32_t*>((channelIndex * 38 + loopIndex) * 4 + 0x8e4a60);
        while (ecx48) {
            --ecx48;
            *edi49 = -1;
            ++edi49;
        }
    }
addr_40cf1d_62:
    ecx50 = *reinterpret_cast<int32_t*>(channelIndex * 4 + 0x8e4cac);

    //CdStreamRead(channelIndex, ecx50, blockOffsetMimg, sectorCount);
    CdStreamRead(channelIndex, (int)((&CStreaming::ms_pStreamingBuffer)[channelIndex]), (int)blockOffsetMimg, sectorCount);

    modelId6 = channelIndex * 0x98;
    *reinterpret_cast<int32_t*>(modelId6 + 0x8e4ae0) = 1;
    *reinterpret_cast<int32_t*>(modelId6 + 0x8e4ae4) = 0;
    *reinterpret_cast<uint32_t*>(modelId6 + 0x8e4aec) = sectorCount;
    *reinterpret_cast<int32_t*>(modelId6 + 0x8e4ae8) = blockOffsetMimg;
    *reinterpret_cast<int32_t*>(modelId6 + 0x8e4af0) = 0;
    al52 = (*(signed char*)0x9654c4);
    if (al52) {
        (*(signed char*)0x9654c4) = 0;
        goto addr_40cf77_14;
    }
addr_40cef2_46:
    sectorCount = ecx39 - m_nCdSize2;
    goto addr_40cef8_58;
addr_40cef0_52:
    goto addr_40cf1d_62;
addr_40cce1_20:
    if (channelIndex == 1 || (zf53 = (*(uint32_t*)0x8e4b78) == 0, !zf53)) {
    addr_40cf77_14:
        goto returnFromFunction;
    }
    else {
        (*(signed char*)0x8e4a58) = 1;
        goto addr_40ccff_21;
    }

    

//OLD_CStreaming__RequestModelStream(channelIndex);
}


char __cdecl CStreaming__RequestModelStream_test(DWORD channelIndex)
{
    std::printf("CStreaming__RequestModelStream_test called\n");

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

    int32_t blockOffsetMimg = 0;
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
                blockOffsetMimg = streamingInfo->m_nCdPosn + CStreaming::ms_files[streamingInfo->m_nImgId].m_StreamHandle;
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

    if (CStreaming::ms_aInfoForModel[modelId].m_nCdSize)
    {
        blockOffsetMimg = streamingInfo->m_nCdPosn + CStreaming::ms_files[streamingInfo->m_nImgId].m_StreamHandle;
        blockCount = CStreaming::ms_aInfoForModel[modelId].m_nCdSize;
    }
    if (blockCount > CStreaming::ms_streamingBufferSize)
    {
        if (channelIndex == 1 || CStreaming::ms_channel[1].LoadStatus)
        {
            //int SomeSizeToReturn = 20 * modelId + 9325760;
            ///return SomeSizeToReturn;
            return 1;
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
            unsigned char loadState = CStreaming::ms_aInfoForModel[pBaseModelInfo->m_nTxdIndex + 20000].m_nLoadState;
            if (loadState != 1 && loadState != 3)
                break;
            int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
            if (animFileIndex != -1)
            {
                unsigned char loadState2 = CStreaming::ms_aInfoForModel[animFileIndex + 25575].m_nLoadState;
                if (loadState2 != 1 && loadState2 != 3)
                    break;
            }
        }

    LABEL_PutStuffInChannel:

        streamingBufferSize = CStreaming::ms_streamingBufferSize;
      //  modelIndexForChannel = (loopIndex + 38 * channelIndex) * 4; //  loopIndex << 2 here? 4 * (loopIndex + 38 * channelIndex)
        CStreaming::ms_channel[channelIndex].modelStreamingBufferOffsets[loopIndex] = sectorCount;
        CStreaming::ms_channel[channelIndex].modelIds[loopIndex] = modelId;

        sectorCount += blockCount;      // sectorCount += blockCount
        if (sectorCount > streamingBufferSize && loopIndex > 0)
        {
            sectorCount = sectorCount - blockCount;
            break;
        }
       //  *v26 = modelId;
       //  ++v26;

        CBaseModelInfo *  pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
        if (modelId >= 20000)
        {
            if (blockCount <= 200)  
            {
            LABEL_BlockCountIsLessThanOrEqualTo200:
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
            }
            else
            {
            LABEL_BlockCountGreaterThan200:

                isBlockCountGreaterThan200 = true;
                goto LABEL_BlockCountIsLessThanOrEqualTo200;
            }

            modelId = streamingInfo->m_nNextIndexOnCd;
            loopIndex = loopIndex + 1;

            if (loopIndex < 16)
            {
                continue;
            }
            else
            {
                goto LABEL_62; // label right before CdStreamRead 
            }
        }
        else
        {
            if (pBaseModelInfo->GetModelType() == 7)
            {
                isModelTypePed = true;
            }
            if (pBaseModelInfo->GetModelType() != 6)
            {
                goto LABEL_BlockCountIsLessThanOrEqualTo200;
            }
            goto LABEL_BlockCountGreaterThan200;
        }

        int animFileIndex = pBaseModelInfo->GetAnimFileIndex();
        if (animFileIndex != -1)
        {
            unsigned char loadState2 = CStreaming::ms_aInfoForModel[animFileIndex + 25575].m_nLoadState;
            if (loadState2 != 3)
            {
                break;
            }
        }

        goto LABEL_PutStuffInChannel;

        /*CBaseModelInfo *  pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelId];
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

  

        loopIndex = loopIndex + 1;
        if (!(loopIndex < 16))
        {
            goto LABEL_62;
        }*/

    }

    if (loopIndex < 16)
    {
        tStreamingChannel & streamingChannel = CStreaming::ms_channel[channelIndex];
        memset (&streamingChannel.modelIds[loopIndex], 0xFFu, 4 * (16 - loopIndex)); // 0xFFu = -1
    }
LABEL_62:

    std::printf("calling CdStreamRead\n");
     //   ecx50 = *reinterpret_cast<int32_t*>(a1 * 4 + 0x8e4cac);
    //_Z12CdStreamReadiPvjj(a1, ecx50, v13, sectorCount);
   
   //CdStreamRead(channelIndex, (int)((&CStreaming::ms_pStreamingBuffer)[channelIndex]), (int)blockOffsetMimg, sectorCount);

   
    //CdStreamRead(channelIndex, (int)((&CStreaming::ms_pStreamingBuffer)[channelIndex * 4]), (int)blockOffsetMimg, sectorCount);

    int32_t ecx50 = *reinterpret_cast<int32_t*>(channelIndex * 4 + 0x8e4cac);
    CdStreamRead(channelIndex, ecx50, (int)blockOffsetMimg, sectorCount);


    tStreamingChannel & streamingChannel = CStreaming::ms_channel[channelIndex];
    streamingChannel.LoadStatus = LOADSTATE_LOADED;
    streamingChannel.iLoadingLevel = 0;
    streamingChannel.iBlockCount = sectorCount;
    streamingChannel.iBlockOffset = (int)blockOffsetMimg;
    streamingChannel.OnBeginRead = 0;

    // LOBYTE(modelId) = (*(BYTE*)0x9654C4); // byte_9654C4; //(*(DWORD*)0xB5F878)
    if ((*(BYTE*)0x9654C4))
        (*(BYTE*)0x9654C4) = 0;

    return (*(BYTE*)0x9654C4); // modelId;*/

          return OLD_CStreaming__RequestModelStream(channelIndex);
}

