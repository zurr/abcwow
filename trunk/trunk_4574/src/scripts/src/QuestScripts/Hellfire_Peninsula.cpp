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

// Fel Orc Scavengers Quest
class FelOrcScavengersQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FelOrcScavengersQAI);
	FelOrcScavengersQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = NULL;
			en = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10482);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
			}
		}
	}
};

// A Burden Of Souls
class BurdenOfSoulsQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BurdenOfSoulsQAI);
	BurdenOfSoulsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if(mKiller->GetTypeId() != TYPEID_PLAYER) 
			return;

		QuestLogEntry *en = NULL;
		en = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10864);
		if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			uint32 newcount = en->GetMobCount(0) + 1;
			en->SetMobCount(0, newcount);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
		}
	}
};

bool TestingTheAntidote(uint32 i, Spell* pSpell)
{
	if(!pSpell->GetUnitTarget() || pSpell->GetUnitTarget()->GetTypeId() != TYPEID_UNIT)
		return true;

	Creature *target = NULL;
	target = static_cast<Creature*>(pSpell->GetUnitTarget());
	if(!target || target->GetEntry() != 16880) // Hulking Helboar
		return true;

	Creature *spawned = NULL;
	spawned = target->GetMapMgr()->GetInterface()->SpawnCreature(16992, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), false, false, 0, 0);
	if(!spawned)
		return true;

	target->Despawn(0, 300000);

	spawned->GetAIInterface()->SetNextTarget(pSpell->u_caster);

	return true;
}

class Dreadtusk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Dreadtusk);
	Dreadtusk(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnDied(Unit *mKiller)
	{
		if(mKiller->GetTypeId() != TYPEID_PLAYER)
			return;

		QuestLogEntry *qle = NULL;
		qle = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10255);
		if(qle)
		{
			qle->SetMobCount(0,1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
};

void SetupHellfirePeninsula(ScriptMgr * mgr)
{
	mgr->register_creature_script(16772, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(19701, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16876, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16925, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(18952, &FelOrcScavengersQAI::Create);

	mgr->register_creature_script(19411, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19410, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16867, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16870, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19413, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19414, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16878, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19415, &BurdenOfSoulsQAI::Create);
	mgr->register_dummy_spell(34665, &TestingTheAntidote);
	mgr->register_creature_script(16992, &Dreadtusk::Create);
}
