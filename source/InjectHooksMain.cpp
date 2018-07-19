#include "StdInc.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef void(__thiscall*  hCTaskSimpleSwim__ProcessPed)
(
    CTaskSimpleSwim * pThis, CPed *pPed
);
auto OLD_CTaskSimpleSwim__ProcessPed = (hCTaskSimpleSwim__ProcessPed)0x68B1C0;

bool __fastcall CTaskSimpleSwim__ProcessPed(CTaskSimpleSwim * pThis, void* padding, CPed *pPed);



void InjectHooksMain(void)
{
    CAnimManager::InjectHooks();
    CTaskManager::InjectHooks();
    CStreaming::InjectHooks();

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    std::printf("GOING TO HOOK FUNC NOW\n");
    DetourAttach(&(PVOID&)OLD_CTaskSimpleSwim__ProcessPed, CTaskSimpleSwim__ProcessPed);

    DetourTransactionCommit(); 
    
}


bool __fastcall CTaskSimpleSwim__ProcessPed(CTaskSimpleSwim * pThis, void* padding, CPed *pPed)
{
    if (pThis->m_pEntity) //*(pThis + 14))
    {
        CAnimManager::BlendAnimation(pPed->m_pRwClump, pPed->m_nAnimGroup, 3u, 8.0);
        pPed->m_nMoveState = 1;
        pPed->field_538 = 1;
    LABEL_44:
        FxSystem_c * pFxSystem = pThis->m_pFxSystem;
        if (pFxSystem)
        {
            pFxSystem->Kill();
            pThis->m_pFxSystem = 0;
        }
        return 1;
    }
    
    if (pThis->m_vecMoveBlendRatio.z > CTaskSimpleSwim::SWIM_STOP_TIME || (pPed->m_nPedFlags & 1))
    {
        CAnimBlendAssociation * pAnimAssociation = nullptr;
        if (pThis->m_AnimID != 191)
            pAnimAssociation = RpAnimBlendClumpGetAssociation(pPed->m_pRwClump, pThis->m_AnimID);
        unsigned int animId = 3;
        pPed->field_538 = 1;
        pPed->m_nMoveState = 1;
        if (pAnimAssociation)
        {
            if (pAnimAssociation->m_nAnimId == 128)
                pAnimAssociation->m_nFlags |= 8u;
            else
                pAnimAssociation->m_fBlendDelta = -4.0;
            if (pThis->m_AnimID == 311)                         // pThis->m_AnimID
            {
                animId = 0;
                pPed->field_538 = 4;
                pPed->m_nMoveState = 4;
            }
            else if (pThis->m_AnimID == 312)
            {
                animId = 1;
                pPed->field_538 = 6;
                pPed->m_nMoveState = 6;
            }
        }
        CAnimManager::BlendAnimation(pPed->m_pRwClump, pPed->m_nAnimGroup, animId, 4.0);
        pPed->RestoreHeadingRate();
        goto LABEL_44;
    }

    pPed->m_nPedFlags &= 0xFFFFFDFF;

    if (pPed->IsPlayer())
    {
        // pThis->m_vecMoveBlendRatio.z seems to remain 0.0f for some reason, so this code seems not to run at all
        if (pThis->m_vecMoveBlendRatio.z && pThis->m_nSwimState != SWIM_UNDERWATER_SPRINTING)
        {
            pThis->ProcessControlAI(pPed);
            float v6 = (CTimer::ms_fTimeStep / 50.0) * 1000.0;
            if (pThis->m_vecMoveBlendRatio.z <= v6)
            {
                pThis->m_vecMoveBlendRatio.z = 0.0f;
            }
            else
            {
                pThis->m_vecMoveBlendRatio.z -= v6;
            }

            CVector *pVecPosition = &pPed->m_placement.m_vPosn;
            if (pPed->m_matrix)
            {
                pVecPosition = &pPed->m_matrix->pos;
            }
            CVector pVecOut;
            VectorSub(&pVecOut, &pThis->m_vecMoveBlendRatio, pVecPosition);
            pPed->m_pPlayerData->m_fMoveBlendRatio = sqrt(pVecOut.x * pVecOut.x + pVecOut.y * pVecOut.y);
            CPlayerData * pPlayerData = pPed->m_pPlayerData;
            if (pPlayerData->m_fMoveBlendRatio < 0.5)
            {
                pPlayerData->m_fMoveBlendRatio = 0.0;
                CAnimManager::BlendAnimation(pPed->m_pRwClump, pPed->m_nAnimGroup, 3u, 4.0);
                pPed->RestoreHeadingRate();
                pThis->DestroyFxSystem();
                return 1;
            }
            if (pPlayerData->m_fMoveBlendRatio > 1.0)
                pPlayerData->m_fMoveBlendRatio = 1.0;
        }
        else
        {
            pThis->ProcessControlInput(reinterpret_cast<CPlayerPed*>(pPed));
            pThis->m_vecMoveBlendRatio.z = 0.0f;
        }
        bool bDecreaseAir = false;
        float fDecreaseAirMultiplicator = 1.0;
        if (pThis->m_nSwimState == SWIM_UNDERWATER_SPRINTING)
        {
            bDecreaseAir = true;
            CAnimBlendAssociation * pAnimAssociation = RpAnimBlendClumpGetAssociation(pPed->m_pRwClump, 314);
            if (pAnimAssociation)
            {
                fDecreaseAirMultiplicator = pAnimAssociation->m_fSpeed * pAnimAssociation->m_fBlendAmount + 1.0f;
            }
        }

        CPlayerPed * pPlayerPed = reinterpret_cast<CPlayerPed *> (pPed);
        pPlayerPed->HandlePlayerBreath(bDecreaseAir, fDecreaseAirMultiplicator);
        if (pThis->m_pPed)
        {
            if (pThis->m_nSwimState != SWIM_UNDERWATER_SPRINTING)
            {
                CPlayerData * pPlayerData = pPed->m_pPlayerData;
                if (CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG) * 0.5f > pPlayerData->m_fBreath)
                    pPed->Say(356, 0, 1.0, 0, 0, 0);
            }
        }
        pPed->SetMoveState(PEDMOVE_NONE);
    }
    else
    {
        pThis->ProcessControlAI(pPed);

        // I didn't really get a chance to this this part of code, if someone knows how to make 
        // other peds swim, then let me know because this code will be needed. I did try spawning peds in the
        // sea but they die lol.
        if (pThis->m_nSwimState == SWIM_UNDERWATER_SPRINTING)
        {
            eWeaponType weaponType = static_cast<eWeaponType>(WEAPON_ARMOUR | WEAPON_BASEBALLBAT);

            CPedDamageResponseCalculator pedDamageResponseCalculator;
            pedDamageResponseCalculator.Constructor1(0, CTimer::ms_fTimeStep, weaponType, PED_PIECE_TORSO, false);

            unsigned char pedFlags = (pPed->m_nPedFlags >> 8) & 1;

            CEventDamage eventDamage;
            eventDamage.Constructor1(0, CTimer::m_snTimeInMilliseconds, weaponType, PED_PIECE_TORSO, 0, 0, pedFlags);
            CPedDamageResponse damageResponseInfo;
            if (eventDamage.AffectsPed(pPed))
            {
                pedDamageResponseCalculator.ComputeDamageResponse(pPed, &damageResponseInfo, true);
            }
            else
            {
                damageResponseInfo.bDamageCalculated = true;
            }
            pPed->m_pIntelligence->m_eventGroup.Add(reinterpret_cast <CEvent&>(eventDamage), false);
            eventDamage.Destructor1();
            pedDamageResponseCalculator.Destructor1();
        }
    }
    pThis->ProcessSwimAnims(pPed);
    pThis->ProcessSwimmingResistance(pPed);
    pThis->ProcessEffects(pPed);
    return 0;
}


