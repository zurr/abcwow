/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "StdAfx.h"
#include "Setup.h"

//Theka the Martyr

// casts the spell Theka Transform 11089 at %30  hp
// casts the spell fevered plague around each 17 second
/*
Fevered Plague 8600 = Inflicts 250 Nature damage to an enemy, then an additional 11 damage every 5 sec. for 3 min.
Fevered Plague 16186 =  Inflicts 72 to 78 Nature damage to an enemy, then an additional 10 damage every 3 sec. for 30 sec. */

#define theka_transform 11089
#define fevered_plague 16186
//#define fevered_plague 8600  i dont know wich one it is he casts

class thekaAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(thekaAI);

    thekaAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{

        morph = dbcSpell.LookupEntry(theka_transform);

		plague = dbcSpell.LookupEntry(fevered_plague);
	}

	void OnCombatStart(Unit* mTarget) 
	{
		morphcheck = true;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget) 
	{
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		morphcheck = false;
		plaguecount = 0;
        RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller) 
	{
		morphcheck = false;
		plaguecount = 0;
        RemoveAIUpdateEvent();
	}

	void AIUpdate() 
	{
		plaguecount++;
		randomplague = 16 + RandomUInt(3);
		if(plaguecount <= randomplague)
		{
			plaguecount = 0;
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target, plague, true);
		}
		else if(_unit->GetHealthPct() <= 30 && morphcheck)
		{
			morphcheck = false;

			_unit->CastSpell(_unit, morph, false);
		}
	}

    void Destroy()
    {
        delete (thekaAI*)this;
    }

protected:
	int plaguecount, randomplague;
	bool morphcheck;
	SpellEntry * morph;
	SpellEntry * plague;
};


//----------------------------------------------------------------------------------------------------------

// Antu'sul

/* 
needs a aggro trigger outside cave

yells

Lunch has arrived, my beutiful childern. Tear them to pieces!   // on aggro
Rise and defend your master!  //  at 70% when his add spawn
The children of sul will protect their master. Rise once more Sul'lithuz! // at  20% when his adds start spawning again but he says it only once and adds keep on spawning

spells

Healing Ward 11889  random around 18 sec
Earthgrab Totem 8376 random around 18 sec

misc info

he summons 8 Sul'lithuz Broodling 8138 on aggro
he summons Servant of antu'sul 8156 70% with spell 11894
he summons Servant of antu'sul 8156 20% with spell 11894 each 15 second  

------------------------------------------------------------------------------------------------
*/

// Witch Doctor Zum'Rah


void SetupZulFarrak(ScriptMgr * mgr)
{
	mgr->register_creature_script(7272, &thekaAI::Create);

}