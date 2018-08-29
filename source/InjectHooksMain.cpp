#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef signed int(__cdecl *hCRenderer_SetupEntityVisibility)(CEntity *pEntity, float *outDistance);
auto OLD_CRenderer_SetupEntityVisibility = (hCRenderer_SetupEntityVisibility)0x554230;
signed int __cdecl CRenderer_SetupEntityVisibility(CEntity *pEntity, float *outDistance);

typedef void(__cdecl *hCRenderer_AddEntityToRenderList)(CEntity *pEntity, float fDistance);
auto OLD_CRenderer_AddEntityToRenderList = (hCRenderer_AddEntityToRenderList)0x05534B0;
void __cdecl CRenderer_AddEntityToRenderList(CEntity *pEntity, float fDistance);


void InjectHooksMain(void)
{
    //CAnimManager::InjectHooks();
   // CTaskManager::InjectHooks();
    //std::printf("okay, only CAnimManager and CTaskManager hooks\n ");
    //CStreaming::InjectHooks();
    CRenderer::InjectHooks();

   DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    //DetourAttach(&(PVOID&)OLD_CRenderer_AddEntityToRenderList, CRenderer_AddEntityToRenderList);

     DetourAttach(&(PVOID&)OLD_CRenderer_SetupEntityVisibility, CRenderer_SetupEntityVisibility);

    DetourTransactionCommit();
    
}


struct s0 {
    signed char pad28[28];
    uint32_t f28;
    signed char pad34[2];
    int16_t f34;
    signed char pad52[16];
    signed char f52;
};

struct s1 {
    signed char pad18[18];
    unsigned char f18;
};

void CRenderer_AddEntityToRenderList(CEntity *pEntity, float fDistance)
{
    std::printf("00000CRe00nd0e0000000r0er_AddEntityToRenderList called modelIndex: %d | distance: %f\n  ", pEntity->m_nModelIndex, fDistance);

    CBaseModelInfo* pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[pEntity->m_nModelIndex];
    pBaseModelInfo->m_nFlags &= 0xFE; // set first 7 bits to true, and 8th bit (bIsLod) to false

    if (pEntity->m_bDistanceFade >= 0)
    {
        if (pEntity->m_bDrawLast && CVisibilityPlugins::InsertEntityIntoSortedList(pEntity, fDistance))
        {
            pEntity->m_nFlags &= 0xFFFF7FFF; // bDistanceFade
            return;
        }
    }
    else
    {
        if (CVisibilityPlugins::InsertEntityIntoSortedList(pEntity, fDistance))
        {
            return;
        }
    }

    if (pEntity->m_nNumLodChildren && !pEntity->m_bUnderwater)
    {
        CRenderer::ms_aVisibleLodPtrs[CRenderer::ms_nNoOfVisibleLods] = pEntity;
        CRenderer::ms_nNoOfVisibleLods++;
    }
    else
    {
        CRenderer::ms_aVisibleEntityPtrs[CRenderer::ms_nNoOfVisibleEntities] = pEntity;
        CRenderer::ms_nNoOfVisibleEntities++;
    }
}

signed int __cdecl CRenderer_SetupEntityVisibility(CEntity *pEntity, float *outDistance)
{
    std::printf("CRenderer_SetupEntityVisibility called! |  distance fade: %d\n", pEntity->m_bDistanceFade);

    int modelIndex = pEntity->m_nModelIndex;
    CBaseModelInfo *pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelIndex];
    CBaseModelInfo *pBaseAtomicModelInfo = pBaseModelInfo->AsAtomicModelInfoPtr();
    bool bDoSomething = 1;
    if ((pEntity->m_nType & 7) == 2)
    {
        unsigned int entityFlags = pEntity->m_nFlags;
        if (entityFlags >= 0
            && (!CRenderer::ms_bRenderTunnels && entityFlags & 0x40000000 || !CRenderer::ms_bRenderOutsideTunnels && !(entityFlags & 0x40000000)))
        {
            return 0;
        }
    }

    if (!pBaseAtomicModelInfo)
    {
        if (pBaseModelInfo->GetModelType() != 5 && pBaseModelInfo->GetModelType() != 4)
        {
            if (FindPlayerVehicle(-1, 0) == pEntity)
            {
                if (gbFirstPersonRunThisFrame)
                {
                    if (CReplay::Mode != 1)
                    {
                        
                        DWORD dwDirectionWasLooking = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_nDirectionWasLooking;
                        CAutomobile * pVehicle = FindPlayerVehicle(-1, 0);
                        if (pVehicle->m_nVehicleClass != CLASS_LEISUREBOAT
                            || !(pVehicle->m_doors[3].m_nDoorFlags & 0x80))
                        {
                            if (dwDirectionWasLooking == 3)
                                return 2;
                            if (modelIndex == 432 || modelIndex == 437 || TheCamera.m_bInATunnelAndABigVehicle)
                                return 2;
                            if (dwDirectionWasLooking)
                                goto LABEL_81;
                            if (pVehicle->m_pHandlingData->m_nModelFlags & 0x4000)
                                return 2;
                            if (pVehicle->m_nVehicleClass != CLASS_BIG
                                || modelIndex == 453
                                || modelIndex == 454
                                || modelIndex == 430
                                || modelIndex == 460)
                            {
                            LABEL_81:
                                CRenderer::m_pFirstPersonVehicle = static_cast<CVehicle*>(pEntity);
                                return 2;
                            }
                        }
                    }
                }
            }
            if (!pEntity->m_pRwObject
                || !(pEntity->m_nFlags & 0x80) && (!CMirrors::TypeOfMirror || pEntity->m_nModelIndex)
                || !pEntity->IsCurrentAreaOrBarberShopInterior() && (pEntity->m_nType & 7) == 2)
            {
                return 0;
            }
            if (!pEntity->GetIsOnScreen() || pEntity->IsEntityOccluded())
                return 2;

            if (pEntity->m_nFlags & 0x40)
            {
                pEntity->m_nFlags = pEntity->m_nFlags & 0xFFFF7FFF;
                CMatrixLink * entityMatrix = pEntity->m_matrix;
                CVector *vecPosition;
                if (entityMatrix)
                    vecPosition = &entityMatrix->pos;
                else
                    vecPosition = &pEntity->m_placement.m_vPosn;

                CVector out;
                CVector * vectorSubResult = VectorSub(&out, vecPosition, &CRenderer::ms_vecCameraPosition);
                CRenderer::AddEntityToRenderList(pEntity, vectorSubResult->Magnitude());
                return 0;
            }
            return 1;
        }
        goto LABEL_49;
    }
    if (pBaseModelInfo->GetModelType() == 3)
    {
        tTimeInfo* pModelTimeInfo = pBaseModelInfo->GetTimeInfo();
        int wOtherTimeModel = pModelTimeInfo->m_wOtherTimeModel;          // m_wOtherTimeModel
        if (CClock::GetIsTimeInRange(pModelTimeInfo->m_nTimeOn, pModelTimeInfo->m_nTimeOff))// m_nTimeOn, m_nTimeOff
        {
            if (wOtherTimeModel != -1 && CModelInfo::ms_modelInfoPtrs[wOtherTimeModel]->m_pRwObject)
                pBaseModelInfo->m_nAlpha = -1;
        }
        else
        {
            if (wOtherTimeModel == -1 || CModelInfo::ms_modelInfoPtrs[wOtherTimeModel]->m_pRwObject)
            {
                pEntity->DeleteRwObject();
                return 0;
            }
            bDoSomething = 0;
        }
    LABEL_49:
        unsigned __int8 entityInterior = pEntity->m_nAreaCode;
        if (entityInterior == CGame::currArea || entityInterior == 13)
        {
            CVector * vecPosition;
            CEntity * pEntityLod = pEntity->m_pLod;
            if (pEntityLod)
            {
                CMatrixLink *entityMatrix = pEntityLod->m_matrix;
                if (entityMatrix)
                    vecPosition = &entityMatrix->pos;
                else
                    vecPosition = &pEntityLod->m_placement.m_vPosn;
            }
            else
            {
                CMatrixLink *entityMatrix = pEntity->m_matrix;
                if (entityMatrix)
                    vecPosition = &entityMatrix->pos;
                else
                    vecPosition = &pEntity->m_placement.m_vPosn;
            }
            float posX = vecPosition->x;
            float posY = vecPosition->y;
            float posZ = vecPosition->z;
            float distanceBetweenCameraAndEntity = sqrt(
                (posZ - CRenderer::ms_vecCameraPosition.z)
                * (posZ - CRenderer::ms_vecCameraPosition.z)
                + (posY - CRenderer::ms_vecCameraPosition.y)
                * (posY - CRenderer::ms_vecCameraPosition.y)
                + (posX - CRenderer::ms_vecCameraPosition.x)
                * (posX - CRenderer::ms_vecCameraPosition.x));
            *outDistance = distanceBetweenCameraAndEntity;
            if (distanceBetweenCameraAndEntity > 300.0)
            {
                float lodMultiplierAndDrawDistance = TheCamera.m_fLODDistMultiplier * pBaseModelInfo->m_fDrawDistance;
                if (lodMultiplierAndDrawDistance > 300.0
                    && lodMultiplierAndDrawDistance + 20.0 > distanceBetweenCameraAndEntity)
                {
                    *outDistance = lodMultiplierAndDrawDistance - 300.0 + distanceBetweenCameraAndEntity;
                }
            }
            return CRenderer::SetupMapEntityVisibility(pEntity, pBaseModelInfo, *outDistance, bDoSomething);
        }
        return 0;
    }
    if (!(pEntity->m_nFlags & 0x80000))
        goto LABEL_49;
    if (!pEntity->m_pRwObject
        || (pEntity->m_nFlags & 0x80u) == 0 && (!CMirrors::TypeOfMirror || pEntity->m_nModelIndex))
    {
        return 0;
    }
    if (!pEntity->GetIsOnScreen() || pEntity->IsEntityOccluded())
        return 2;

    if (!(pEntity->m_nFlags & 0x40))
        return 1;
    CMatrixLink *entityMatrix = pEntity->m_matrix;
    CVector * vecPosition;
    if (entityMatrix)
        vecPosition = &entityMatrix->pos;
    else
        vecPosition = &pEntity->m_placement.m_vPosn;
    CVector out;
    CVector * vectorSubResult = VectorSub(&out, vecPosition, &CRenderer::ms_vecCameraPosition);
    CVisibilityPlugins::InsertEntityIntoSortedList(pEntity, vectorSubResult->Magnitude());
    pEntity->m_nFlags &= 0xFFFF7FFF;
    return 0;
}