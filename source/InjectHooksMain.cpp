#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef signed int(__cdecl *hCRenderer_SetupEntityVisibility)(CEntity *pEntity, float *outDistance);
auto OLD_CRenderer_SetupEntityVisibility = (hCRenderer_SetupEntityVisibility)0x554230;
signed int __cdecl CRenderer_SetupEntityVisibility(CEntity *pEntity, float *outDistance);

void InjectHooksMain(void)
{
    CAnimManager::InjectHooks();
    CTaskManager::InjectHooks();
    CStreaming::InjectHooks();
    CRenderer::InjectHooks();

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CRenderer_SetupEntityVisibility, CRenderer_SetupEntityVisibility);

    DetourTransactionCommit(); 
    
}


signed int __cdecl CRenderer_SetupEntityVisibility(CEntity *pEntity, float *outDistance)
{
    std::printf("CRenderer_SetupEntityVisibility called! |  distance fade: %d\n", pEntity->m_bDistanceFade);
    //return OLD_CRenderer_SetupEntityVisibility(pEntity, outDistance);

    CEntity *pEntity0; // esi
    int modelIndex; // eax
    CBaseModelInfo *pBaseModelInfo; // edi
    CBaseModelInfo * pBaseAtomicModelInfo; // eax
    unsigned int v6; // ecx
    DWORD dwDirectionWasLooking; // edi
    __int16 modelIndex0; // ax
    int someFlagsMaybe; // ecx
    unsigned int entityFlags; // eax
    CMatrixLink *entityMatrix; // eax
    CVector *vecPosition; // eax
    CVector *entityPosAndVectorPos_sub_result; // eax
    float magnitudeOfPositionAndCameraPos0; // ST0C_4
    tTimeInfo* pModelTimeInfo; // eax
    int wOtherTimeModel; // ebx
    unsigned __int8 entityInterior; // al
    CEntity *pEntityLod; // eax
    CMatrixLink *entityMatrix2; // ecx
    CVector *vecPosition0; // eax
    unsigned int entityFlags0; // eax
    unsigned int entityFlags1; // eax
    CMatrixLink *entityMatrix4; // eax
    CVector *vecPosition1; // eax
    CVector *entityPosAndVectorPos_sub_result0; // eax
    float magnitudeOfPositionAndCameraPos; // ST0C_4
    CMatrixLink *entityMatrix3; // eax
    float posX; // ecx
    float posY; // edx
    float posZ; // eax
    float lodMultiplierAndDrawDistance; // [esp+10h] [ebp-14h]
    char bDoSomething; // [esp+14h] [ebp-10h]
    CVector out; // [esp+18h] [ebp-Ch]
    float distanceBetweenCameraAndEntity; // [esp+28h] [ebp+4h]

    pEntity0 = pEntity;
    modelIndex = pEntity->m_nModelIndex;
    pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[modelIndex];
    pBaseAtomicModelInfo = pBaseModelInfo->AsAtomicModelInfoPtr();
    bDoSomething = 1;
    if ((pEntity->m_nType & 7) == 2)
    {
        v6 = pEntity->m_nFlags;
        if (v6 >= 0
            && (!CRenderer::ms_bRenderTunnels && v6 & 0x40000000 || !CRenderer::ms_bRenderOutsideTunnels && !(v6 & 0x40000000)))
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
                        
                        dwDirectionWasLooking = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_nDirectionWasLooking;
                        CAutomobile * pVehicle = FindPlayerVehicle(-1, 0);
                        if (pVehicle->m_nVehicleClass != CLASS_LEISUREBOAT
                            || !(pVehicle->m_doors[3].m_nDoorFlags & 0x80))
                        {
                            if (dwDirectionWasLooking == 3)
                                return 2;
                            modelIndex0 = pEntity->m_nModelIndex;
                            if (modelIndex0 == 432 || modelIndex0 == 437 || TheCamera.m_bInATunnelAndABigVehicle)
                                return 2;
                            if (dwDirectionWasLooking)
                                goto LABEL_81;
                            if (pVehicle->m_pHandlingData->m_nModelFlags & 0x4000)
                                return 2;
                            if (pVehicle->m_nVehicleClass != CLASS_BIG
                                || modelIndex0 == 453
                                || modelIndex0 == 454
                                || modelIndex0 == 430
                                || modelIndex0 == 460)
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
            entityFlags = pEntity->m_nFlags;
            if (entityFlags & 0x40)
            {
                pEntity->m_nFlags = entityFlags & 0xFFFF7FFF;
                entityMatrix = pEntity->m_matrix;
                if (entityMatrix)
                    vecPosition = &entityMatrix->pos;
                else
                    vecPosition = &pEntity->m_placement.m_vPosn;
                entityPosAndVectorPos_sub_result = VectorSub(&out, vecPosition, &CRenderer::ms_vecCameraPosition);
                magnitudeOfPositionAndCameraPos0 = entityPosAndVectorPos_sub_result->Magnitude();
                CRenderer::AddEntityToRenderList(pEntity, magnitudeOfPositionAndCameraPos0);
                return 0;
            }
            return 1;
        }
        goto LABEL_49;
    }
    if (pBaseModelInfo->GetModelType() == 3)
    {
        pModelTimeInfo = pBaseModelInfo->GetTimeInfo();
        wOtherTimeModel = pModelTimeInfo->m_wOtherTimeModel;          // m_wOtherTimeModel
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
        entityInterior = pEntity->m_nAreaCode;
        if (entityInterior == CGame::currArea || entityInterior == 13)
        {
            pEntityLod = pEntity->m_pLod;
            if (pEntityLod)
            {
                entityMatrix2 = pEntityLod->m_matrix;
                if (entityMatrix2)
                    vecPosition0 = &entityMatrix2->pos;
                else
                    vecPosition0 = &pEntityLod->m_placement.m_vPosn;
            }
            else
            {
                entityMatrix3 = pEntity->m_matrix;
                if (entityMatrix3)
                    vecPosition0 = &entityMatrix3->pos;
                else
                    vecPosition0 = &pEntity->m_placement.m_vPosn;
            }
            posX = vecPosition0->x; 
            posY = vecPosition0->y; 
            posZ = vecPosition0->z; 
            out.x = posX;
            out.y = posY;
            out.z = posZ;
            distanceBetweenCameraAndEntity = sqrt(
                (posZ - CRenderer::ms_vecCameraPosition.z)
                * (posZ - CRenderer::ms_vecCameraPosition.z)
                + (posY - CRenderer::ms_vecCameraPosition.y)
                * (posY - CRenderer::ms_vecCameraPosition.y)
                + (posX - CRenderer::ms_vecCameraPosition.x)
                * (posX - CRenderer::ms_vecCameraPosition.x));
            *outDistance = distanceBetweenCameraAndEntity;
            if (distanceBetweenCameraAndEntity > 300.0)
            {
                lodMultiplierAndDrawDistance = TheCamera.m_fLODDistMultiplier * pBaseModelInfo->m_fDrawDistance;
                if (lodMultiplierAndDrawDistance > 300.0
                    && lodMultiplierAndDrawDistance + 20.0 > distanceBetweenCameraAndEntity)
                {
                    *outDistance = lodMultiplierAndDrawDistance - 300.0 + distanceBetweenCameraAndEntity;
                }
            }
            return CRenderer::SetupMapEntityVisibility(pEntity0, pBaseModelInfo, *outDistance, bDoSomething);
        }
        return 0;
    }
    entityFlags0 = pEntity->m_nFlags;
    if (!(entityFlags0 & 0x80000))
        goto LABEL_49;
    if (!pEntity->m_pRwObject
        || (entityFlags0 & 0x80u) == 0 && (!CMirrors::TypeOfMirror || pEntity->m_nModelIndex))
    {
        return 0;
    }
    if (!pEntity->GetIsOnScreen() || pEntity->IsEntityOccluded())
        return 2;
    entityFlags1 = pEntity->m_nFlags;
    if (!(entityFlags1 & 0x40))
        return 1;
    entityMatrix4 = pEntity->m_matrix;
    if (entityMatrix4)
        vecPosition1 = &entityMatrix4->pos;
    else
        vecPosition1 = &pEntity->m_placement.m_vPosn;
    entityPosAndVectorPos_sub_result0 = VectorSub(&out, vecPosition1, &CRenderer::ms_vecCameraPosition);
    magnitudeOfPositionAndCameraPos = entityPosAndVectorPos_sub_result0->Magnitude();
    CVisibilityPlugins::InsertEntityIntoSortedList(pEntity, magnitudeOfPositionAndCameraPos);
    pEntity->m_nFlags &= 0xFFFF7FFF;
    return 0;
}