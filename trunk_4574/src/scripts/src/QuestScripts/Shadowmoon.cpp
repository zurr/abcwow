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

// Shadowmoon Valley Wildlife Quest
class ShadowmoonValleyWildlifeQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShadowmoonValleyWildlifeQAI);
	ShadowmoonValleyWildlifeQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			int questID = static_cast<Player*>(mKiller)->GetTeam() ? 10702 : 10703;
			QuestLogEntry *en = NULL;
			en = static_cast<Player*>(mKiller)->GetQuestLogForEntry(questID);
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

bool CrystalOfDeepShadows(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10833);
  
  if(qle == NULL)
    return true;

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}
// Infiltrating Dragonmaw Fortress Quest
class InfiltratingDragonmawFortressQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfiltratingDragonmawFortressQAI);
    InfiltratingDragonmawFortressQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(10836);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

void SetupShadowmoon(ScriptMgr * mgr)
{
	mgr->register_creature_script(21878, &ShadowmoonValleyWildlifeQAI::Create);
	mgr->register_creature_script(21879, &ShadowmoonValleyWildlifeQAI::Create);
	mgr->register_creature_script(21864, &ShadowmoonValleyWildlifeQAI::Create);

  mgr->register_dummy_spell(39094, &CrystalOfDeepShadows);
  mgr->register_creature_script(11980, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(21718, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(21719, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(21720, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(22253, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(22274, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(22331, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(23188, &InfiltratingDragonmawFortressQAI::Create);
  mgr->register_creature_script(21717, &InfiltratingDragonmawFortressQAI::Create);
}
