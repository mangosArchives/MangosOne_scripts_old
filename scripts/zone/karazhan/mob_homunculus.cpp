/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Mob_Homunculus
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_FIREBOLT  18086 // Blasts a target for 150 Fire damage.

struct MANGOS_DLL_DECL mob_homunculusAI : public ScriptedAI
{
    mob_homunculusAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Firebolt_Timer;

    bool InCombat;

    void Reset()
    {
        Firebolt_Timer = 0;      

        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();

        //DoGoHome();
    }

    int RandTime(int time) {return ((rand()%time)*1000);}

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            if (!InCombat) InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim()) return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if (!InCombat) InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Chain cast
        if (!m_creature->IsNonMeleeSpellCasted(false) && !m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
            DoCast(m_creature->getVictim(),SPELL_FIREBOLT);
        else DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_homunculus(Creature *_Creature)
{
    return new mob_homunculusAI (_Creature);
}

void AddSC_mob_homunculus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_homunculus";
    newscript->GetAI = GetAI_mob_homunculus;
    m_scripts[nrscripts++] = newscript;
}
