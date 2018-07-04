/*
* Copyright (C) 2013-2018 NuskyCore <http://www.nuskycore.org/>
* Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2018 MaNGOS <https://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ScriptData
SDName: Vale_Of_Eternal_Blossoms
SD%Complete: 0
SDComment: Placeholder
SDCategory: Vale_Of_Eternal_Blossoms
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"

enum eZhaoSpells
{
    SPELL_LAVA_BURST        =  75068,
    SPELL_LIGHTNING_SPEAR   = 116570,
};

enum eZhaoEvents
{
    EVENT_LAVA_BURST        = 1,
    EVENT_LIGHTNING_SPEAR   = 2,
};

enum eSpells
{
    SPELL_JADE_FIRE         = 127422,
    SPELL_JADE_STRENGHT     = 127462,
    SPELL_LIGHTNING_BREATH  = 126491,
    SPELL_LIGHTNING_POOL    = 129695,
    SPELL_LIGHTNING_WHIRL   = 126522,
};

class npc_zhao_jin : public CreatureScript
{
public:
   	npc_zhao_jin() : CreatureScript("npc_zhao_jin") { }

    struct npc_zhao_jinAI : public ScriptedAI
    {
        npc_zhao_jinAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        {
        	events.Reset();
                
            events.ScheduleEvent(EVENT_LAVA_BURST,      10000);
            events.ScheduleEvent(EVENT_LIGHTNING_SPEAR, 15000);
        }

        void JustDied(Unit* /*killer*/)
       	{

        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                 return;

            events.Update(diff);  

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_LAVA_BURST:
                    	if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        	me->CastSpell(target, SPELL_LAVA_BURST, false);

                       	events.ScheduleEvent(EVENT_LAVA_BURST,      10000);
                        break;

                    case EVENT_LIGHTNING_SPEAR:
                   		if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_LIGHTNING_SPEAR, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_SPEAR, 15000);
                        break;

                    default:
                    	break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
    	return new npc_zhao_jinAI(creature);
    }
};

#define SAY_LAO_1   "Come quickly. There is a gap in their defenses."
#define SAY_LAO_2   "Let's go while the way is clear. The cave is just ahead."
#define SAY_LAO_3   "Let's wait for the next wave."
#define SAY_LAO_4   "Stop. Let them pass."
#define SAY_LAO_5   "Wait for a safe moment, then plant the explosives at the mouth of the cave."
#define SAY_LAO_6   "Well, that should put a damper on their plans. Good work. I trust you can get back on your own."

class npc_lao_softfoot : public CreatureScript
{
public:
    npc_lao_softfoot() : CreatureScript("npc_lao_softfoot") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
    	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Start", GOSSIP_SENDER_MAIN,  GOSSIP_ACTION_INFO_DEF); 
       	player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
    	if (action == GOSSIP_ACTION_INFO_DEF)
        	creature->AI()->DoAction(1);
            return true;
    }

    struct npc_lao_softfootAI : public npc_escortAI
    {
        npc_lao_softfootAI(Creature* creature) : npc_escortAI(creature) { }
            
        void Reset() { }

        void DoAction(const int32 action)
        {
        	if (action == 1)
                Start(true, true);
        }

        void WaypointReached(uint32 waypointId) { }

        void JustDied(Unit* /*killer*/)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                if (Player* player = GetPlayerForEscort())
                    player->FailQuest(30634); // Barring Entry
        }

        void UpdateAI(const uint32 uiDiff)
        {
        	npc_escortAI::UpdateAI(uiDiff);
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
    	return new npc_lao_softfootAI(creature);
    }
};

class npc_reanimated_jade_warrior : public CreatureScript
{
public:
    npc_reanimated_jade_warrior() : CreatureScript("npc_reanimated_jade_warrior") { }

    struct npc_reanimated_jade_warriorAI : public ScriptedAI
    {
        npc_reanimated_jade_warriorAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 jadeFireTimer;
        uint32 jadeStrenghtTimer;

        void Reset()
        {
        	jadeFireTimer	  = urand(10000, 12000);
            jadeStrenghtTimer = urand(5000, 7000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                    return;
            
            if (jadeFireTimer <= diff)
            {
                if (Unit* target = me->SelectNearestTarget(5.0f))
                    if (!target->IsFriendlyTo(me))
                        me->CastSpell(target, SPELL_JADE_FIRE, true);
                jadeFireTimer = urand(20000, 22000);
            }
            else
                jadeFireTimer -= diff;

            if (jadeStrenghtTimer <= diff)
            {
            	me->CastSpell(me, SPELL_JADE_STRENGHT, true);
                jadeStrenghtTimer = urand(20000, 22000);
           	}
            else
                jadeStrenghtTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
    	return new npc_reanimated_jade_warriorAI(creature);
    }
};


void AddSC_vale_of_eternal_blossoms()
{
	new npc_zhao_jin();
	new npc_lao_softfoot();
	new npc_reanimated_jade_warrior();
}