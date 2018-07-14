#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")
    
typedef void(__thiscall*  hCTaskSimpleSwim__ProcessSwimAnims)
(
    CTaskSimpleSwim * pThis, CPed *pLevel
);
auto OLD_CTaskSimpleSwim__ProcessSwimAnims = (hCTaskSimpleSwim__ProcessSwimAnims)0x6899F0;




bool __cdecl CWaterLevel_GetWaterLevel(float x, float y, float z, CVector * pVecPlacementPos, char bUnderWater, CVector* pVector);
void __fastcall CTaskSimpleSwim__ProcessSwimAnims(CTaskSimpleSwim * pThis, void* padding, CPed *pLevel);



void InjectHooksMain(void)
{
    CAnimManager::InjectHooks();
    CStreaming::InjectHooks();

   
   

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CTaskSimpleSwim__ProcessSwimAnims, CTaskSimpleSwim__ProcessSwimAnims);

    DetourTransactionCommit(); 
   
}


void __fastcall CTaskSimpleSwim__ProcessSwimAnims(CTaskSimpleSwim * pThis, void* padding, CPed *pLevel)
{
    std::printf("CTaskSimpleSwim__ProcessSwimAnims called\n");
    //OLD_CTaskSimpleSwim__ProcessSwimAnims(pThis, pLevel);
    CPlayerPed *pLevel1; // esi
  //  int pThis; // edi
    CAnimBlendAssociation *pAnimAssoc12; // ecx
    FxSystem_c *pFixSystem; // ecx
    CMatrixLink *pMatrixLink3; // eax
    CVector * pVec3; // edx
    CVector * pVec3_0; // ecx
    CVector * pVec3_6; // eax
    CMatrixLink *pMatrixLink; // eax
    CVector * pVec3_5; // eax
    CAnimBlock *pAnimBlock; // eax
    char *pBlockName; // eax
    int animID; // eax
    CAnimBlendAssociation *pAnimAssoc13; // eax
    CAnimBlendAssociation *pAnimAssoc15; // eax
    CMatrixLink *pMatrixLink4; // eax
    CVector * pVec3_2; // edx
    CVector * pVec3_1; // ecx
    CVector * pVec3_3; // eax
    CMatrixLink *pMatrixLink1; // eax
    CVector * pVec3_4; // eax
    CEntity *pEntity; // eax
    CAnimBlendAssociation *pAnimAssoc8; // eax
    int animID_0; // eax
    CAnimBlendAssociation *pAnimAssoc11; // ecx
    int animID_1; // eax
    double v28; // st7
    CAnimBlendAssociation *pAnimAssoc9; // eax
    CAnimBlendAssociation *pAnimAssoc14; // ebp
    float buttonSprintResults; // ST20_4
    CAnimBlendAssociation *pAnimAssoc10; // eax
    CAnimBlendAssociation *pAnimAssoc6; // ebp
    CAnimBlendAssociation *pAnimAssoc7; // ebp
    float buttonSprintResults1; // ST20_4
    float taskAnimSpeed; // ST20_4
    CAnimBlendAssociation *pAnimAssoc; // eax
    CAnimBlendAssociation *pAnimAssoc0; // eax
    CAnimBlendAssociation *pAnimAssoc1; // eax
    CAnimBlendAssociation *pAnimAssoc2; // eax
    CAnimBlendAssociation *pAnimAssoc3; // eax
    CAnimBlendAssociation *pAnimAssoc4; // eax
    CAnimBlendAssociation *pAnimAssoc5; // eax
    CPlayerData *pPlayerData; // esi

    pLevel1 = (CPlayerPed *)pLevel;

    pAnimAssoc12 = RpAnimBlendClumpGetAssociation(pLevel->m_pRwClump, 14);
    if (pThis->m_bFinishedBlending) // m_bFinishedBlending
    {
        if (pAnimAssoc12
            && pAnimAssoc12->m_fBlendAmount < 1.0
            && pAnimAssoc12->m_fBlendDelta <= 0.0)
        {
            CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 0, 0xEu, 8.0);
        }
    }
    else
    {
        if (pAnimAssoc12
            || (pAnimAssoc12 = CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 0, 14u, 8.0)) != 0)
        {
            if (pAnimAssoc12->m_fBlendAmount >= 1.0)
                pThis->m_bFinishedBlending = true; //pThis->m_bFinishedBlending = 1;
        }
        RpAnimBlendClumpSetBlendDeltas(pLevel1->m_pRwClump, 0x10, -8.0);
        pFixSystem = pLevel1->m_aWeapons[pLevel1->m_nActiveWeaponSlot].m_pFxSystem;
        if (pFixSystem)
        {
            pFixSystem->Kill(); // FxSystem_c::Kill(pFixSystem);
            pLevel1->m_aWeapons[pLevel1->m_nActiveWeaponSlot].m_pFxSystem = 0;
        }
       /// pLevel->m_placement = 0.0f; //m_placement-> *&pLevel = 0.0;
        // define a new Vec3 variable here for GetWaterLevel

        if (pLevel1->IsPlayer() && !pThis->m_nSwimState)
        {
            pMatrixLink3 = pLevel1->m_matrix;
            pVec3 = &pMatrixLink3->pos;
            if (pMatrixLink3)
            {
                pVec3_0 = &pMatrixLink3->pos;
            }
            else
            {
                pVec3 = &pLevel1->m_placement.m_vPosn; //ped.physical.entity.placeable.m_SimpleCoors;
                pVec3_0 = &pLevel1->m_placement.m_vPosn; // ped.physical.entity.placeable.m_SimpleCoors;
            }

            float waterLevel = 0.0f;
            pVec3_6 = (pMatrixLink3 ? &pMatrixLink3->pos : &pLevel1->m_placement.m_vPosn);
            if (CWaterLevel::GetWaterLevel(pVec3_6->x, pVec3_6->y, pVec3_6->z, &waterLevel, 1, 0))
            {
                pMatrixLink = pLevel1->m_matrix; //ped.physical.entity.placeable.m_pMatrix;
                pVec3_5 = (pMatrixLink ? &pMatrixLink->pos : &pLevel1->m_placement.m_vPosn);
                if (waterLevel - 2.0 > pVec3_5->z) // *(pSimpleTransform + 8)) //m_placement
                    pThis->m_nSwimState = (eSwimState)4;
            }
        }
    }
   
    if (!pThis->m_bAnimBlockRefAdded)
    {
        pAnimBlock = CAnimManager::ms_aAnimAssocGroups[71].m_pAnimBlock;
        if (!pAnimBlock)
        {
            pBlockName = CAnimManager::GetAnimBlockName(71);
            pAnimBlock = CAnimManager::GetAnimationBlock(pBlockName);
        }
        if (pAnimBlock->bLoaded == 1)
        {
            CAnimManager::AddAnimBlockRef(pAnimBlock - CAnimManager::ms_aAnimBlocks);
            pThis->m_bAnimBlockRefAdded = true;
        }
        else
        {
            CStreaming::RequestModel(pAnimBlock - CAnimManager::ms_aAnimBlocks + 25575, 8);
        }
    }
    
    if (pThis->m_bFinishedBlending && pThis->m_bAnimBlockRefAdded) //if (pThis->m_bFinishedBlending && pThis->m_bAnimBlockRefAdded)
    {
        std::printf(" pThis->m_nSwimState: %d\n", pThis->m_nSwimState);
        switch (pThis->m_nSwimState)//(*(pThis + 10))
        {
        case 0: // SWIM_TREAD 
            if (pThis->m_AnimID != 14) // if (pThis->m_AnimID != 14)
            {
                pAnimAssoc = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 311);
                if (pAnimAssoc)
                    pAnimAssoc->m_fBlendDelta = -1.0;
                pAnimAssoc0 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 312);
                if (pAnimAssoc0)
                    pAnimAssoc0->m_fBlendDelta = -1.0;
                pAnimAssoc1 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 313);
                if (pAnimAssoc1)
                    pAnimAssoc1->m_fBlendDelta = -4.0;
                pAnimAssoc2 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 314);
                if (pAnimAssoc2)
                    pAnimAssoc2->m_fBlendDelta = -2.0;
                pAnimAssoc3 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 315);
                if (pAnimAssoc3)
                    pAnimAssoc3->m_fBlendDelta = -2.0;
                pAnimAssoc4 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 316);
                if (pAnimAssoc4)
                    pAnimAssoc4->m_fBlendDelta = -4.0;
                pAnimAssoc5 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 128);
                if (pAnimAssoc5)
                    pAnimAssoc5->m_fBlendDelta = -4.0;
                pThis->m_AnimID = 14;
            }
            goto LABEL_118;
        case 1: // SWIM_SPRINT
            if (pThis->m_AnimID == 311)
            {
                pAnimAssoc6 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 311);
                if (!pAnimAssoc6)
                    goto LABEL_52;
                if (pLevel1->IsPlayer())
                {
                    pAnimAssoc6->m_fSpeed = pLevel1->m_pPlayerData->m_fMoveBlendRatio;
                }
                else if (pThis->m_fAnimSpeed > 0.0 && pThis->m_fAnimSpeed <= 1.0)
                {
                    pAnimAssoc6->m_fSpeed = pThis->m_fAnimSpeed;
                }
            }
            else
            {
                CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x137u, 2.0);
                pThis->m_AnimID = 311;
            }
            goto LABEL_118;
        case 2:
            if (pThis->m_AnimID == 312)
            {
                pAnimAssoc7 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 312);
                if (!pAnimAssoc7)
                    goto LABEL_52;
                if (pLevel1->m_pPlayerData)
                {
                    buttonSprintResults1 = pLevel1->GetButtonSprintResults( (eSprintType) 2);
                    pAnimAssoc7->m_fSpeed = max(1.0, buttonSprintResults1);
                }
                else if (pThis->m_fAnimSpeed > 1.0)
                {
                    taskAnimSpeed = pThis->m_fAnimSpeed - 1.0;
                    pAnimAssoc7->m_fSpeed = min(1.5, taskAnimSpeed);
                }
            }
            else
            {
                CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x138u, 2.0);
                pThis->m_AnimID = 312;
            }
            goto LABEL_118;
        case 3:
            if (pThis->m_AnimID == 313)
            {
                pAnimAssoc8 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 313);
                if (!pAnimAssoc8)
                    goto LABEL_52;
                if (pAnimAssoc8->m_pHierarchy->m_fTotalTime == pAnimAssoc8->m_fCurrentTime)
                    pThis->m_nSwimState = (eSwimState)4;
            }
            else
            {
                CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x139u, 8.0);
                pThis->m_AnimID = 313;
            }
            goto LABEL_118;
        case 4:
            animID_0 = pThis->m_AnimID;
            if ((animID_0 == 314 || animID_0 == 315) && pThis->flt_34 >= 0.0)
            {
                if (pLevel1->m_pPlayerData && pLevel1->GetButtonSprintResults((eSprintType) 3) >= 1.0)
                {
                    pAnimAssoc9 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 314);
                    pAnimAssoc14 = pAnimAssoc9;
                    if (!pAnimAssoc9
                        || pAnimAssoc9->m_fBlendDelta < 0.0
                        || pAnimAssoc9->m_fBlendAmount == 0.0)
                    {
                        pAnimAssoc14 = CAnimManager::BlendAnimation(
                            pLevel1->m_pRwClump,
                            71,
                            0x13Au,
                            4.0);
                    }
                    if (pAnimAssoc14->m_pHierarchy->m_fTotalTime == pAnimAssoc14->m_fCurrentTime)
                    {
                        pAnimAssoc14->Start( 0.0);
                        buttonSprintResults = pLevel1->GetButtonSprintResults( (eSprintType)3);
                        pAnimAssoc14->m_fSpeed = max(0.69999999, buttonSprintResults);
                    }
                    pThis->m_AnimID = 314;
                }
                else
                {
                    pAnimAssoc10 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 315);
                    if (!pAnimAssoc10
                        || pAnimAssoc10->m_fBlendDelta < 0.0
                        || pAnimAssoc10->m_fBlendAmount == 0.0)
                    {
                        CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x13Bu, 4.0);
                    }
                    pThis->m_AnimID = 315;
                }
            }
            else
            {
                pAnimAssoc11 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 314);
                if (pAnimAssoc11)
                {
                    if (pThis->flt_34 < 0.0 && pAnimAssoc11->m_fBlendAmount >= 0.99000001)
                    {
                        if (pThis->flt_34 > -2.0)
                        {
                            //v28 = flt_8D2FC4;
                            pThis->flt_34 = 0;
                            pThis->flt_24 = (float)(CTaskSimpleSwim::SWIM_DIVE_UNDER_ANGLE * 3.1416) / 180.0; //v28 * 0.017453292;
                        }
                        else
                        {
                            pThis->flt_24 = 1068677315;
                            pThis->flt_34 = 0;
                        }
                    }
                }
                else
                {
                    CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x13Au, 1000.0);
                    animID_1 = pThis->m_AnimID;
                    if (animID_1 == 14 || animID_1 == 191)
                    {
                        pThis->flt_34 = -1073741824;
                        pThis->m_AnimID = 314;
                    }
                    else
                    {
                        pThis->flt_34 = -1082130432;
                        pThis->m_AnimID = 314;
                    }
                }
            }
            goto LABEL_118;
        case 5:
            animID = pThis->m_AnimID;
            if (animID == 316)
            {
                pAnimAssoc13 = RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 316);
                if (pAnimAssoc13)
                {
                    if (pAnimAssoc13->m_pHierarchy->m_fTotalTime * 0.25 <= pAnimAssoc13->m_fTimeStep
                        + pAnimAssoc13->m_fCurrentTime)
                    {
                        pAnimAssoc15 = CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 0, 0x80u, 8.0);
                        pAnimAssoc15->m_nFlags |= 8u;//LOBYTE(pAnimAssoc15->sFlags) |= 8u;
                        pThis->m_AnimID = 128;
                    }
                    goto LABEL_118;
                }
            }
            else
            {
                if (animID != 128)
                {
                    CAnimManager::BlendAnimation(pLevel1->m_pRwClump, 71, 0x13Cu, 8.0);
                    pThis->m_AnimID = 316;
                    pLevel1->m_vecMoveSpeed.z = 8.0 / pLevel1->m_fMass;
                    pEntity = CTaskSimpleClimb::TestForClimb(pLevel1, pThis->m_pClimbPos, (pThis->m_fAngle), (pThis->m_nSurfaceType), 1);
                    pThis->m_pEntity = pEntity;
                    if (pEntity)
                        pEntity->RegisterReference( &pThis->m_pEntity);
                    goto LABEL_118;
                }
                if (RpAnimBlendClumpGetAssociation(pLevel1->m_pRwClump, 128))
                {
                    pMatrixLink4 = pLevel1->m_matrix;
                
                    pVec3_2 = &pMatrixLink4->pos;
                    if (pMatrixLink4)
                    {
                        pVec3_1 = &pMatrixLink4->pos;
                    }
                    else
                    {
                        pVec3_2 = &pLevel1->m_placement.m_vPosn;
                        pVec3_1 = &pLevel1->m_placement.m_vPosn;
                    }
                    pVec3_3 = (pMatrixLink4 ? &pMatrixLink4->pos : &pLevel1->m_placement.m_vPosn);
                    float waterLevel = 0.0f;
                    if (!CWaterLevel::GetWaterLevel(pVec3_3->x, pVec3_3->y, pVec3_3->z, &waterLevel, 1, 0)
                        || ((pMatrixLink1 = pLevel1->m_matrix) == 0 ? (pVec3_4 = &pLevel1->m_placement.m_vPosn) : (pVec3_4 = &pMatrixLink1->pos),
                            pVec3_4->z + 0.5 >= waterLevel))
                    {
                    LABEL_118:
                        pPlayerData = pLevel1->m_pPlayerData;
                        if (pPlayerData && (pPlayerData->m_fMoveBlendRatio > 0.5 || pThis->m_nSwimState == 4))
                            CStats::UpdateStatsWhenSwimming(pThis->m_nSwimState == 4, pThis->m_nSwimState == 2);
                        return;
                    }
                }
            }
        LABEL_52:
            pThis->m_nSwimState = (eSwimState)0;
            goto LABEL_118;
        default:
            goto LABEL_118;
        }
    }
}