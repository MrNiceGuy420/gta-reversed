#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef char(__cdecl*  hCStreaming__ProcessLoadingChannel)
(
    int channelIndex
);

auto OLD_CStreaming__ProcessLoadingChannel = (hCStreaming__ProcessLoadingChannel)0x40E170;
char __cdecl CStreaming__ProcessLoadingChannel(int channelIndex);

void InjectHooksMain(void)
{
    InjectHook(0x15674C0, &CStreamingInfo::AddToList, PATCH_JUMP);
    InjectHook(0x40A45E, &CStreaming::LoadAllRequestedModels, PATCH_JUMP);
    InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);
   
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreaming__ProcessLoadingChannel, CStreaming__ProcessLoadingChannel);
   

    //InjectHook(0x407480, &CStreamingInfo::AddToList, PATCH_JUMP);
    //InjectHook(0x4076A0, &CStreaming::IsVeryBusy, PATCH_JUMP);
    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    DetourTransactionCommit(); 
   
}


char __cdecl CStreaming__ProcessLoadingChannel(int channelIndex)
{
    int streamStatus; // eax
    int channelIndex2; // ebp
    bool bStreamInQueue; // zf
    tStreamingChannel *pStreamChannel; // ebx
    int modelId; // esi
    int modelId2; // edi
    int nCdSize; // eax
    unsigned __int8 streamingInfoFlags; // al
    bool loopIndex; // sf
   // unsigned __int8 v10; // of
    int channelIndex1; // ecx
    int loopIndex2; // [esp+0h] [ebp-8h]
    int nCdSize2; // [esp+4h] [ebp-4h]
    int wModelTxdIndex; // [esp+4h] [ebp-4h]


    //std::printf("CStreaming__ProcessLoadingChannel called! channelIndex: %d\n", channelIndex);

    streamStatus = CdStreamGetStatus(channelIndex);
    if (streamStatus)
    {
        if (streamStatus == 255)
            return 0;
        if (streamStatus == 250)
            return 0;
        channelIndex1 = channelIndex;
        CStreaming::ms_channel[channelIndex1].m_nCdStreamStatus = streamStatus;
        bStreamInQueue = CStreaming::ms_channelError == -1;
        CStreaming::ms_channel[channelIndex1].LoadStatus = LOADSTATE_Channeled; // 3;
        if (!bStreamInQueue)
            return 0;
        CStreaming::ms_channelError = channelIndex;
        CStreaming::RetryLoadFile(channelIndex);
        return 1;
    }
    
    channelIndex2 = channelIndex;
    bStreamInQueue = CStreaming::ms_channel[channelIndex].LoadStatus == 2;
    CStreaming::ms_channel[channelIndex].LoadStatus = LOADSTATE_NOT_LOADED; // 0;
    if (!bStreamInQueue)
    {
        loopIndex2 = 0;
        pStreamChannel = &CStreaming::ms_channel[channelIndex2];
        std::printf("CdStreamStatus: %d\n", pStreamChannel->m_nCdStreamStatus);

        while (1)
        {
            modelId = pStreamChannel->modelIds[loopIndex2];
            if (modelId != -1)
            {
                // --------------REMOVE THIS LATER
                //return OLD_CStreaming__ProcessLoadingChannel(channelIndex);
                //[END] --------------REMOVE THIS LATER

                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
                modelId2 = modelId;
                nCdSize = CStreaming::ms_aInfoForModel[modelId].m_nCdSize;
                nCdSize2 = CStreaming::ms_aInfoForModel[modelId].m_nCdSize;
                if (modelId >= 20000)
                    goto LABEL_16;
                if (modelInfo->GetModelType() != 6
                    || CStreaming::ms_vehiclesLoaded.CountMembers() < CStreaming::desiredNumVehiclesLoaded
                    || CStreaming::RemoveLoadedVehicle()
                    || CStreaming::ms_aInfoForModel[modelId2].m_nFlags & 6)
                {
                    nCdSize = nCdSize2;
                LABEL_16:



                    if (modelId < 25255 || modelId >= 25511)
                        CStreaming::MakeSpaceFor(nCdSize << 11);

                    std::printf("CStreaming__ProcessLoadingChannel: About to call CStreaming::ConvertBufferToObject\n");
                    CStreaming::ConvertBufferToObject(
                        &(&CStreaming::ms_pStreamingBuffer)[channelIndex][2048 * pStreamChannel->modelStreamingBufferPositions[loopIndex2]],
                        modelId);


                    if (CStreaming::ms_aInfoForModel[modelId2].m_nLoadState != 4
                        || (CStreaming::ms_channel[channelIndex2].LoadStatus = LOADSTATE_Requested, //2,
                            CStreaming::ms_channel[channelIndex2].modelStreamingBufferPositions[loopIndex2] = pStreamChannel->modelStreamingBufferPositions[loopIndex2],
                            CStreaming::ms_channel[channelIndex2].modelIds[loopIndex2] = modelId,
                            loopIndex2))
                    {
                        pStreamChannel->modelIds[loopIndex2] = -1;

                    }
                    goto LABEL_22;
                }
                wModelTxdIndex = modelInfo->m_nTxdIndex; //CModelInfo::ms_modelInfoPtrs[modelId]->clump.base.m_wTxdIndex;
                CStreaming::RemoveModel(modelId);
                streamingInfoFlags = CStreaming::ms_aInfoForModel[modelId2].m_nFlags;
                if (streamingInfoFlags & 6)
                {
                    CStreaming::RequestModel(modelId, streamingInfoFlags);
                }
                else if (!CTxdStore::GetNumRefs(wModelTxdIndex))
                {
                    CStreaming::RemoveTxdModel(wModelTxdIndex);
                }
            }
        LABEL_22:

            //pStreamChannel++;
            //v10 = __OFSUB__(loopIndex2 + 1, 16);
            //loopIndex = loopIndex2++ - 15 < 0;
            //if (!(loopIndex ^ v10))
                //goto LABEL_23;
            loopIndex2++;
            if (!(loopIndex2 < 16) )
                goto LABEL_23;
        }
    }
    std::printf("CStreaming__ProcessLoadingChannel: calling CStreaming::FinishLoadingLargeFile now\n");

    CStreaming::FinishLoadingLargeFile(
        &(&CStreaming::ms_pStreamingBuffer)[channelIndex][2048
        * CStreaming::ms_channel[channelIndex2].modelStreamingBufferPositions[0]],
        CStreaming::ms_channel[channelIndex2].modelIds[0]);
    CStreaming::ms_channel[channelIndex2].modelIds[0] = -1;
LABEL_23:
    if (CStreaming::ms_bLoadingBigModel)
    {
        if (CStreaming::ms_channel[channelIndex2].LoadStatus != 2)
        {
            CStreaming::ms_bLoadingBigModel = 0;
            memset(&CStreaming::ms_channel[1], 0xFFu, 64u);
        }
    }
    return 1;
    
    //return OLD_CStreaming__ProcessLoadingChannel(channelIndex);
}