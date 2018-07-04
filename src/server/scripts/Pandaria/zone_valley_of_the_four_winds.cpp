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
SDName: Valley_Of_The_Four_Winds
SD%Complete: 100%
SDComment: Placeholder
SDCategory: Valley_Of_The_Four_Winds
EndScriptData 
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"

enum eBonobosSpells
{
    SPELL_GOING_BANANAS     = 125363,
    SPELL_BANANARANG        = 125311,
    SPELL_TOSS_FILTH        = 125365,
};

enum eBonobosEvents
{
    EVENT_GOING_BANANAS         = 1,
    EVENT_BANANARANG            = 2,
    EVENT_TOSS_FILTH            = 3,
};

class npc_bonobos : public CreatureScript
{
public:
    npc_bonobos() : CreatureScript("npc_bonobos") {}

    struct npc_bonobosAI : public ScriptedAI
    {
        npc_bonobosAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        {
        	events.Reset();
            events.ScheduleEvent(EVENT_GOING_BANANAS,       12000);
            events.ScheduleEvent(EVENT_BANANARANG,           8000);
            events.ScheduleEvent(EVENT_TOSS_FILTH,          15000);
        }

        void JustDied(Unit* /*killer*/) { }

        void JustSummoned(Creature* summon)
        {
        	summon->DespawnOrUnsummon(12000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GOING_BANANAS:
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                             me->CastSpell(target, SPELL_GOING_BANANAS, false);
                         events.ScheduleEvent(EVENT_GOING_BANANAS,      10000);
                         break;
                    
                    case EVENT_BANANARANG:
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                             me->CastSpell(target, SPELL_BANANARANG, false);
                         events.ScheduleEvent(EVENT_BANANARANG, 20000);
                         break;
                    
                    case EVENT_TOSS_FILTH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_TOSS_FILTH, false);
                        events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
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
            return new npc_bonobosAI(creature);
    }
};

enum eSeleNaSpells
{
    SPELL_RAIN_DANCE    = 124860,
    SPELL_TORRENT       = 124935,
    SPELL_WATER_BOLT    = 124854,
};

enum eSeleNaEvents
{
    EVENT_RAIN_DANCE        = 1,
    EVENT_TORRENT           = 2,
    EVENT_WATER_BOLT        = 3,
};

class npc_sele_na : public CreatureScript
{
public:
	npc_sele_na() : CreatureScript("npc_sele_na") { }

    struct npc_sele_naAI : public ScriptedAI
    {
       	npc_sele_naAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        {
        	events.Reset();
           	events.ScheduleEvent(EVENT_RAIN_DANCE, 5000);
            events.ScheduleEvent(EVENT_TORRENT, 15000);
            events.ScheduleEvent(EVENT_WATER_BOLT, 25000);
        }

        void JustDied(Unit* /*killer*/) {}

        void JustSummoned(Creature* summon)
        {
       		summon->DespawnOrUnsummon(12000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RAIN_DANCE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_RAIN_DANCE, false);
                        events.ScheduleEvent(EVENT_RAIN_DANCE,       5000);
                        break;

                    case EVENT_TORRENT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_TORRENT, false);
                        events.ScheduleEvent(EVENT_TORRENT, 15000);
                        break;

                    case EVENT_WATER_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_WATER_BOLT, false);
                        events.ScheduleEvent(EVENT_WATER_BOLT, 25000);
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
    	return new npc_sele_naAI(creature);
    }
};

enum eBlackhoofSpells
{
    SPELL_BELLOWING_RAGE    = 124297,
    SPELL_RUSHING_RAGE      = 124302,
    SPELL_YAUNGOL_STOMP     = 124289,
};

enum eBlackhoofEvents
{
    EVENT_BELLOWING_RAGE        = 1,
    EVENT_RUSHING_RAGE          = 2,
    EVENT_YAUNGOL_STOMP         = 3,
};

class npc_blackhoof : public CreatureScript
{
public:
    npc_blackhoof() : CreatureScript("npc_blackhoof") {}

    struct npc_blackhoofAI : public ScriptedAI
    {
    	npc_blackhoofAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        { 
        	events.Reset();
            events.ScheduleEvent(EVENT_RUSHING_RAGE,         5000);
            events.ScheduleEvent(EVENT_YAUNGOL_STOMP,       15000);
            events.ScheduleEvent(EVENT_BELLOWING_RAGE,      25000);
        }

        void JustDied(Unit* /*killer*/) { }

        void JustSummoned(Creature* summon)
        {
        	summon->DespawnOrUnsummon(12000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BELLOWING_RAGE:
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                             me->CastSpell(target, SPELL_BELLOWING_RAGE, false);
                         events.ScheduleEvent(EVENT_BELLOWING_RAGE,       25000);
                         break;
                    
                    case EVENT_RUSHING_RAGE:
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                             me->CastSpell(target, SPELL_RUSHING_RAGE, false);
                         events.ScheduleEvent(EVENT_RUSHING_RAGE, 5000);
                         break;
                    
                    case EVENT_YAUNGOL_STOMP:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_YAUNGOL_STOMP, false);
                        events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 15000);
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
            return new npc_blackhoofAI(creature);
    }
};

enum eIkThikWarriorSpells
{
    SPELL_PIERCE_ARMOR      = 6016,
    SPELL_SHOCK_AND_AWE		= 118538,
};

enum eIkThikWarriorEvents
{
    EVENT_PIERCE_ARMOR          = 1,
    EVENT_SHOCK_AND_AWE			= 2,
};

class npc_ik_thik_warrior : public CreatureScript
{
public:
    npc_ik_thik_warrior() : CreatureScript("npc_ik_thik_warrior") { }

    struct npc_ik_thik_warriorAI : public ScriptedAI
    {
        npc_ik_thik_warriorAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset()
        {
        	events.Reset();
            events.ScheduleEvent(EVENT_PIERCE_ARMOR,		 5000);
            events.ScheduleEvent(EVENT_SHOCK_AND_AWE,		15000);
        }

        void JustDied(Unit* /*killer*/) { }

        void JustSummoned(Creature* summon)
        {
        	summon->DespawnOrUnsummon(12000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                 return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_PIERCE_ARMOR:
                    	if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_PIERCE_ARMOR, false);
                        events.ScheduleEvent(EVENT_PIERCE_ARMOR,       25000);
                        break;

                    case EVENT_SHOCK_AND_AWE:
                    	if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SHOCK_AND_AWE, false);
                        events.ScheduleEvent(EVENT_SHOCK_AND_AWE, 40000);
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
    	return new npc_ik_thik_warriorAI(creature);
    }
};

void AddSC_valley_of_the_four_winds()
{
	// Rare Mobs
	new npc_bonobos();;
    new npc_sele_na();
    new npc_blackhoof();

    // Standard Mobs
    new npc_ik_thik_warrior();
}