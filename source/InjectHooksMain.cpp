#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")


typedef void (__cdecl*  hCStreaming__LoadAllRequestedModels)
(
    bool bOnlyPriorityRequests
);

hCStreaming__LoadAllRequestedModels OLD_CStreaming__LoadAllRequestedModels = (hCStreaming__LoadAllRequestedModels)0x40A45E;

void __cdecl CStreaming__LoadAllRequestedModels(bool bOnlyPriorityRequests);

void InjectHooksMain(void)
{
    InjectHook(0x15674C0, &CStreamingInfo::AddToList, PATCH_JUMP);
    InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

   
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CStreaming__LoadAllRequestedModels, CStreaming__LoadAllRequestedModels);
   

    //InjectHook(0x407480, &CStreamingInfo::AddToList, PATCH_JUMP);
    //InjectHook(0x4076A0, &CStreaming::IsVeryBusy, PATCH_JUMP);
    //InjectHook(0x4087E0, &CStreaming::RequestModel, PATCH_JUMP);

    DetourTransactionCommit(); 
   
}


void __cdecl CStreaming__LoadAllRequestedModels(bool bOnlyPriorityRequests)
{
    std::printf("CStreaming__LoadAllRequestedModels called\n");

   // OLD_CStreaming__LoadAllRequestedModels(bOnlyPriorityRequests);
    int channelIndex; // esi
    int numModelsToLoad; // ebx
    __int16 endRequestPreviousIndex; // ax
    CStreamingInfo *v4; // eax
    unsigned int pStreamingChannel; // edi

    channelIndex = 0;
    if (!CStreaming::m_bLoadingAllRequestedModels)
    {
        CStreaming::m_bLoadingAllRequestedModels = true;
        CStreaming::FlushChannels();
        numModelsToLoad = 10;
        if (2 * CStreaming::ms_numModelsRequested >= 10)
            numModelsToLoad = 2 * CStreaming::ms_numModelsRequested;
        while (1)
        {
            endRequestPreviousIndex = CStreaming::ms_pEndRequestedList->m_nPrevIndex;
            v4 = endRequestPreviousIndex == -1 ? 0 : &CStreamingInfo::ms_pArrayBase[endRequestPreviousIndex];

           if (v4 == CStreaming::ms_pStartRequestedList
                && !CStreaming::ms_channel[0].LoadStatus
                && !CStreaming::ms_channel[1].LoadStatus
                || numModelsToLoad <= 0)
            {
                break;
            }

            if (CStreaming::ms_bLoadingBigModel)
                channelIndex = 0;
        
            if (CStreaming::ms_channel[channelIndex].LoadStatus)
            {
                CdStreamSync(channelIndex);
                CStreaming::ms_channel[channelIndex ].iLoadingLevel = 100;
            }
            if (CStreaming::ms_channel[channelIndex ].LoadStatus == 1)
            {
                CStreaming::ProcessLoadingChannel(channelIndex);
                if (CStreaming::ms_channel[channelIndex ].LoadStatus == 2)
                    CStreaming::ProcessLoadingChannel(channelIndex);
            }
            if (bOnlyPriorityRequests && !CStreaming::ms_numPriorityRequests)
                break;
            if (!CStreaming::ms_bLoadingBigModel)
            {
               // if (!CStreaming::ms_channel[channelIndex / 0xFFFFFF68 + 1].LoadStatus)
               //     CStreaming::RequestModelStream(1 - channelIndex);
                int32_t edi15 = channelIndex * 0x98;
                if (!*reinterpret_cast<int32_t*>(0x8e4b78 - edi15))
                    CStreaming::RequestModelStream(1 - channelIndex);

                if (!CStreaming::ms_channel[channelIndex].LoadStatus && !CStreaming::ms_bLoadingBigModel)
                    CStreaming::RequestModelStream(channelIndex);
            }
            if (!CStreaming::ms_channel[0].LoadStatus && !CStreaming::ms_channel[1].LoadStatus)
                break;
            channelIndex = 1 - channelIndex;
            --numModelsToLoad;
        }
        CStreaming::FlushChannels();
        CStreaming::m_bLoadingAllRequestedModels = false;
    }
}


/*void __cdecl CStreaming__LoadAllRequestedModels(bool bOnlyPriorityRequests)
{
    std::printf("CStreaming__LoadAllRequestedModels called\n");

    // OLD_CStreaming__LoadAllRequestedModels(bOnlyPriorityRequests);
    int channelIndex; // esi
    int numModelsToLoad; // ebx
    __int16 endRequestPreviousIndex; // ax
    CStreamingInfo *v4; // eax
    unsigned int pStreamingChannel; // edi

    channelIndex = 0;
    if (!CStreaming::m_bLoadingAllRequestedModels)
    {
        CStreaming::m_bLoadingAllRequestedModels = true;
        CStreaming::FlushChannels();
        numModelsToLoad = 10;
        if (2 * CStreaming::ms_numModelsRequested >= 10)
            numModelsToLoad = 2 * CStreaming::ms_numModelsRequested;
        while (1)
        {
            endRequestPreviousIndex = CStreaming::ms_pEndRequestedList->m_nPrevIndex;
            v4 = endRequestPreviousIndex == -1 ? 0 : &CStreamingInfo::ms_pArrayBase[endRequestPreviousIndex];
            if (v4 == CStreaming::ms_pStartRequestedList
                && !CStreaming::ms_channel[0].LoadStatus
                && !CStreaming::ms_channel[1].LoadStatus
                || numModelsToLoad <= 0)
            {
                break;
            }
            if (CStreaming::ms_bLoadingBigModel)
                channelIndex = 0;
            pStreamingChannel = 0x98 * channelIndex;
            if (CStreaming::ms_channel[channelIndex].LoadStatus)
            {
                CdStreamSync(channelIndex);
                CStreaming::ms_channel[pStreamingChannel / 0x98].iLoadingLevel = 100;
            }
            if (CStreaming::ms_channel[pStreamingChannel / 0x98].LoadStatus == 1)
            {
                CStreaming::ProcessLoadingChannel(channelIndex);
                if (CStreaming::ms_channel[pStreamingChannel / 0x98].LoadStatus == 2)
                    CStreaming::ProcessLoadingChannel(channelIndex);
            }
            if (bOnlyPriorityRequests && !CStreaming::ms_numPriorityRequests)
                break;
            if (!CStreaming::ms_bLoadingBigModel)
            {
                if (!CStreaming::ms_channel[pStreamingChannel / 0xFFFFFF68 + 1].LoadStatus)
                    CStreaming::RequestModelStream(1 - channelIndex);
                if (!CStreaming::ms_channel[pStreamingChannel / 0x98].LoadStatus && !CStreaming::ms_bLoadingBigModel)
                    CStreaming::RequestModelStream(channelIndex);
            }
            if (!CStreaming::ms_channel[0].LoadStatus && !CStreaming::ms_channel[1].LoadStatus)
                break;
            channelIndex = 1 - channelIndex;
            --numModelsToLoad;
        }
        CStreaming::FlushChannels();
        CStreaming::m_bLoadingAllRequestedModels = false;
    }
}*/