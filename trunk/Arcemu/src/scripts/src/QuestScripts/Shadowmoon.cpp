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
#include "EAS/EasyFunctions.h"

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

class KneepadsQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KneepadsQAI);
    KneepadsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if(!mKiller)
			return;

		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = NULL;
			en = ((Player*)mKiller)->GetQuestLogForEntry(10703);
			if (en == NULL)
			{
				en = ((Player*)mKiller)->GetQuestLogForEntry(10702);
				if (en == NULL)
				{
					return;
				}
			}

			if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
			}
		}
		return;
	}
};

bool ToLegionHold(uint32 i, Aura* pAura, bool apply)
{
	if ( pAura == NULL || pAura->GetUnitCaster() == NULL || !pAura->GetUnitCaster()->IsPlayer() )
		return true;

	Player *pPlayer = static_cast< Player* >( pAura->GetUnitCaster() );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10563 );
	if ( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry( 10596 );
		if ( pQuest == NULL )
			return true;
	}

	if ( apply )
	{

		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, 20366 );
		pPlayer->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 1194 );
		pPlayer->_setFaction();
		Creature *pJovaan = sEAS.SpawnCreature( pPlayer, 21633, -3306.26f, 2933.84f, 170.934f, 5.47423f, 52000 );	// Jovaan
		Creature *pRazuun = sEAS.SpawnCreature( pPlayer, 21502, -3300.47f, 2927.22f, 173.870f, 2.42924f, 52000 );	// Razuun
		if ( pJovaan != NULL )
		{
			pJovaan->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
			if ( pJovaan->GetAIInterface() != NULL )
			{
				pJovaan->GetAIInterface()->SetAllowedToEnterCombat( false );
			}
		}
		if ( pRazuun != NULL )
		{
			pRazuun->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
			if ( pRazuun->GetAIInterface() != NULL )
			{
				pRazuun->GetAIInterface()->SetAllowedToEnterCombat( false );
			}
		}
	}
	else
	{
		if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
		{
			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
			pQuest->SendUpdateAddKill( 2 );
			pQuest->UpdatePlayerFields();
		}
	
		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, pPlayer->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		pPlayer->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 0 ); //reset faction?
		pPlayer->_setFaction();
	}

	return true;
}
class DeathbringerJovaanAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DeathbringerJovaanAI);
	DeathbringerJovaanAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
	}
	void JovWait(Creature *creat)
    {
		TimedEvent *te0 = TimedEvent::Allocate(this, new CallbackP1<DeathbringerJovaanAI, Creature*>(this, &DeathbringerJovaanAI::JovStart, creat), 0, 1000, 1);
		TimedEvent *te1 = TimedEvent::Allocate(this, new CallbackP1<DeathbringerJovaanAI, Creature*>(this, &DeathbringerJovaanAI::Jov1, creat), 0, 15000, 1);
		TimedEvent *te2 = TimedEvent::Allocate(this, new CallbackP1<DeathbringerJovaanAI, Creature*>(this, &DeathbringerJovaanAI::Jov2, creat), 0, 28000, 1);
		TimedEvent *te3 = TimedEvent::Allocate(this, new CallbackP1<DeathbringerJovaanAI, Creature*>(this, &DeathbringerJovaanAI::Jov3, creat), 0, 40000, 1);
		creat->event_AddEvent(te0);		
		creat->event_AddEvent(te1);		
		creat->event_AddEvent(te2);		
		creat->event_AddEvent(te3);
	}
	void JovStart(Creature *creat)
    {
		creat->SetStandState(STANDSTATE_KNEEL);
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Everything is in readiness, warbringer.");
	}
	void Jov1(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Warbringer, that will require the use of all the hold's infernals. It may leave us vulnerable to a counterattack.");
	}
	void Jov2(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It shall be as you say, warbringer. One last question, if I may...");
	}
	void Jov3(Creature *creat)
    {
		creat->SetStandState(STANDSTATE_STAND);
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's in the crate?");
	}	
	void OnLoad(){
		JovWait(_unit);
	}
};

class WarbringerRazuunAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WarbringerRazuunAI);
	WarbringerRazuunAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
	}
	void RaazunWait(Creature *creat)
    {
		TimedEvent *rte0 = TimedEvent::Allocate(this, new CallbackP1<WarbringerRazuunAI, Creature*>(this, &WarbringerRazuunAI::RaazunStart, creat), 0, 8000, 1);
		TimedEvent *rte1 = TimedEvent::Allocate(this, new CallbackP1<WarbringerRazuunAI, Creature*>(this, &WarbringerRazuunAI::Raazun1, creat), 0, 22000, 1);
		TimedEvent *rte2 = TimedEvent::Allocate(this, new CallbackP1<WarbringerRazuunAI, Creature*>(this, &WarbringerRazuunAI::Raazun2, creat), 0, 35000, 1);
		TimedEvent *rte3 = TimedEvent::Allocate(this, new CallbackP1<WarbringerRazuunAI, Creature*>(this, &WarbringerRazuunAI::Raazun3, creat), 0, 44000, 1);
		creat->event_AddEvent(rte0);
		creat->event_AddEvent(rte1);
		creat->event_AddEvent(rte2);
		creat->event_AddEvent(rte3);	

	}
	void RaazunStart(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Doom Lord Kazzak will be pleased. You are to increase the pace of your attacks. Destroy the orcish and dwarven strongholds with all haste.");
	}
	void Raazun1(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Don't worry about that. I've increased production at the Deathforge. You'll have all the infernals you need to carry out your orders. Don't fail, Jovaan.");
	}
	void Raazun2(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Yes?");
	}
	void Raazun3(Creature *creat)
    {
		creat->Emote(EMOTE_ONESHOT_TALK);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Crate? I didn't send you a crate, Jovaan. Don't you have more important things to worry about? Go see to them!");
	}
	void OnLoad(){
		RaazunWait(_unit);
	}
	
};

void SetupShadowmoon(ScriptMgr * mgr)
{
	mgr->register_creature_script(11980, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21718, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21719, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21720, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22253, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22274, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22331, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(23188, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21717, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21878, &KneepadsQAI::Create);
	mgr->register_creature_script(21879, &KneepadsQAI::Create);
	mgr->register_creature_script(21864, &KneepadsQAI::Create);
	mgr->register_creature_script(21633, &DeathbringerJovaanAI::Create);
	mgr->register_creature_script(21502, &WarbringerRazuunAI::Create);

	mgr->register_dummy_aura(37097, &ToLegionHold);
	
	mgr->register_dummy_spell(39094, &CrystalOfDeepShadows);
}
