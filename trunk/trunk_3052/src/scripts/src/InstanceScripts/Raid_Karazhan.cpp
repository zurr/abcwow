#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_Karazhan.cpp Script                                             */
/************************************************************************/

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Midnight AI
#define CN_MIDNIGHT 16151

#define CN_ATTUMEN 15550

/*
#define SUMMON_ATTUMEN 29714 // summons Attumen but wrong faction (neutral)
#define SUMMON_MOUNTED_ATTUMEN 29799 // summons mounted Attumen; wrong faction
#define MOUNT_ATTUMEN 29770 // broken or maybe should be called from Attumen (so also wont work because missing core support)
#define MOUNT_ATTUMEN 29769 // broken?!
*/

class MIDNIGHTAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MIDNIGHTAI);

	MIDNIGHTAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		attumen = NULL;
		m_phase = 1;

		//        infoSummonAttumen = dbcSpell.LookupEntry(SUMMON_ATTUMEN);
		//        infoMountAttumen = dbcSpell.LookupEntry(MOUNT_ATTUMEN);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		m_phase = 1;
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive() && (attumen != NULL))
		{
			attumen->Despawn(100, 0);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
		if(m_phase==2)
		{
			_unit->PlaySoundToSet(9173);
			attumen->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Well done Midnight!");
		}
	}
	void AIUpdate()
	{
		switch(m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		case 3:
			break;
		default:
			m_phase = 1;
		};
	}

	void PhaseOne()
	{
		if(_unit->GetHealthPct() <= 95)
		{
			// summon Attumen
			attumen = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ATTUMEN,
				_unit->GetPositionX(), _unit->GetPositionY(),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, 0, 0);
			//            _unit->CastSpell(_unit, infoSummonAttumen, true);
			m_phase = 2;
		}
	}

	void PhaseTwo()
	{
		if(_unit->GetHealthPct() <= 25)
		{
			// mount Attumen
			_unit->PlaySoundToSet(9168);
			attumen->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, 
				"Come Midnight, let's disperse this petty rabble!");

			attumen->SetUInt32Value(UNIT_FIELD_DISPLAYID , 16040);
			_unit->Despawn(100, 0);
			//            _unit->CastSpell(_unit, infoMountAttumen, true);
			m_phase = 3;
		}
	}

protected:
	uint32 m_phase;
	Creature *attumen;
	//    SpellEntry *infoSummonAttumen;
	//    SpellEntry *infoMountAttumen;
};


// Attumen AI

#define SHADOW_CLEAVE 29832
#define BERSERKER_CHARGE 26561
#define INTANGIBLE_PRESENCE 29833
#define ENRAGE 19953

class ATTUMENAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ATTUMENAI);
	bool m_spellcheck[3];
	SP_AI_Spell spells[3];

	ATTUMENAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		m_phase = 1;
		nrspells = 2;
		for(int i=0;i<3;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SHADOW_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(INTANGIBLE_PRESENCE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BERSERKER_CHARGE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		infoEnrage = dbcSpell.LookupEntry(ENRAGE);
	}

	void OnCombatStart(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%3;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9167;
			text = "Cowards! Wretches!";
			break;
		case 1:
			sound = 9298;
			text = "Who dares attack the steed of the Huntsman?";
			break;
		case 2:
			sound = 9299;
			text = "Perhaps you would rather test yourselves against a more formidable opponent?!";
			break;
		}
		CastTime();
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9165);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Always knew... someday I would become... the hunted.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9169;
			text = "Your impurity must be cleansed.";
			break;
		case 1:
			sound = 9300;
			text = "Another trophy to add to my collection!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		switch(m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			break;
		default:
			m_phase = 1;
		};
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void PhaseOne()
	{
		if(_unit->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 16040)
		{
			// we've mounted so reset hp
			static_cast<Creature*>(_unit)->RegenerateHealth();
			_unit->GetAIInterface()->WipeHateList();
			m_phase = 2;
		}
		else if(_unit->GetHealthPct() <= 25)
		{
			_unit->CastSpell(_unit, infoEnrage, true);
			m_phase = 2;
			nrspells = 3;
		}
	}

	void CastTime()
	{
		for(int i=0;i<3;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	uint32 m_phase;
	int nrspells;
	SpellEntry *infoEnrage;
};

// Moroes AI
#define CN_MOROES 15687

#define VANISH 31619
#define GARROTE 37066 
#define BLIND 34694
#define GOUGE 36862
#define ENRAGE 19953

class MOROESAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MOROESAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	MOROESAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		} 

		spells[0].info = dbcSpell.LookupEntry(BLIND);
		spells[0].targettype = TARGET_ATTACKING; // TODO: should be random target in melee range
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(GOUGE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		infoVanish = dbcSpell.LookupEntry(VANISH);
		infoGarrote = dbcSpell.LookupEntry(GARROTE);
		infoEnrage = dbcSpell.LookupEntry(ENRAGE);
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		m_enraged = false;
		m_time_vanish = 30;
		_unit->PlaySoundToSet(9211);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, 
			"Hmm, unannounced visitors? Preparations must be made.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9213);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How terribly clumsy of me...");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%3;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9214;
			text = "One more for dinner this evening.";
			break;
		case 1:
			sound = 9314;
			text = "Time... Never enough time.";
			break;
		case 2:
			sound = 9315;
			text = "I've gone and made a mess.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		m_time_vanish--;
		if(vanished && _unit->GetAIInterface()->GetNextTarget()) {
			_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoGarrote, true);
			vanished = false;
			m_time_vanish = 30;
		}
		else if(!m_time_vanish) {
			CastVanish();
		}
		else if(!m_enraged && _unit->GetHealthPct() <= 30)
		{
			_unit->CastSpell(_unit, infoEnrage, true);
			m_enraged = true;
		}
		else {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastVanish()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9215;
			text = "Now, where was I? Oh yes...";
			break;
		case 1:
			sound = 9316;
			text = "You rang?";
			break;
		}
		_unit->CastSpell(_unit, infoVanish, true);
		vanished = true;
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

protected:
	bool m_enraged;
	bool vanished;
	int nrspells;
	uint32 m_time_vanish;
	SpellEntry *infoGarrote;
	SpellEntry *infoVanish;
	SpellEntry *infoEnrage;
};

// Maiden of Virtue AI
#define CN_MAIDEN 16457

#define REPENTANCE 29511
#define HOLY_FIRE 29522
#define HOLY_WRATH 28883
#define HOLY_GROUND 29512

class MAIDENOFVIRTUEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MAIDENOFVIRTUEAI);
	bool m_spellcheck[3];
	SP_AI_Spell spells[3];

	MAIDENOFVIRTUEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		// spell buggy: should be VARIOUS but this targets the maiden?!
		spells[0].info = dbcSpell.LookupEntry(HOLY_FIRE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		// emsy: removed, 100yrd range and infinite jumps
		/*	spells[1].info = dbcSpell.LookupEntry(HOLY_WRATH);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; */

		// temporary fix for broken spell: cast holy ground every 2 secs since its triggered only once
		spells[1].info = dbcSpell.LookupEntry(HOLY_GROUND);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 3;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		infoRepentance = dbcSpell.LookupEntry(REPENTANCE);
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		m_time_repentance = 25 + RandomUInt(100)%10;
		_unit->PlaySoundToSet(9204);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your behavior will not be tolerated!");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9206);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Death comes. Will your conscience be clear?");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%3;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9207;
			text = "Anh ah ah...";
			break;
		case 1:
			sound = 9311;
			text = "This is for the best.";
			break;
		case 2:
			sound = 9312;
			text = "Impure thoughts lead to profane actions.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		if(!RepentanceTrigger()) {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	bool RepentanceTrigger()
	{
		if(!m_time_repentance) {
			uint32 sound, val = (uint32) RandomUInt(100)%2;
			char *text;
			switch(val)
			{
			case 0:
				sound = 9208;
				text = "Your impurity must be cleansed.";
				break;
			case 1:
				sound = 9313;
				text = "Cast out your corrupt thoughts";
				break;
			}
			_unit->PlaySoundToSet(sound);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
			_unit->CastSpell(_unit, infoRepentance, true);
			m_time_repentance = 25 + RandomUInt(100)%5;
			return true;
		}
		else
			m_time_repentance--;
		return  false;
	}

protected:
	int nrspells;
	uint32 m_time_repentance;
	SpellEntry *infoRepentance;
};

// The Big Bad Wolf
#define CN_THEBIGBADWOLF 17521

#define TERRIFYING_HOWL 30752
#define MORPH_LITTLE_RED_RIDING_HOOD 30768
#define DEBUFF_LITTLE_RED_RIDING_HOOD 30756

class THEBIGBADWOLFAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(THEBIGBADWOLFAI);
	bool m_spellcheck[3];
	SP_AI_Spell spells[3];

	THEBIGBADWOLFAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(TERRIFYING_HOWL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(MORPH_LITTLE_RED_RIDING_HOOD);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 60;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(DEBUFF_LITTLE_RED_RIDING_HOOD);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].soundid = 9278;
		spells[2].speech = "Run away little girl, run away!";
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		_unit->PlaySoundToSet(9276);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The better to own you with!");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9275);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "AArrhhh.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->PlaySoundToSet(9277);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mmmm... delicious.");
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

// The Curator
#define CN_THECURATOR 15691
#define CN_ASTRALFLARE 17096

#define HATEFUL_BOLT 30383
#define EVOCATION 30254
#define ENRAGE 19953
#define BERSERK 26662

class THECURATORAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(THECURATORAI);
	SP_AI_Spell spellBolt;
	bool m_spellcheck;

	THECURATORAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		m_spellcheck = false;
		spellBolt.info = dbcSpell.LookupEntry(HATEFUL_BOLT);
		spellBolt.instant = true;
		spellBolt.cooldown = 15;

		infoEvocation = dbcSpell.LookupEntry(EVOCATION);
		infoEnrage = dbcSpell.LookupEntry(ENRAGE);
		infoBerserk = dbcSpell.LookupEntry(BERSERK);
	}

	void OnCombatStart(Unit* mTarget)
	{
		m_time_summon = 10;
		m_time_berserk = 720;
		evocation = false;
		enrage = false;
		berserk = false;
		_unit->PlaySoundToSet(9183);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Menagerie is for guests only.");
		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9184);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This Curator is no longer op... er... ation... al.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9186;
			text = "Do not touch the displays.";
			break;
		case 1:
			sound = 9308;
			text = "You are not a guest.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		if(!evocation)
		{
			if(_unit->GetManaPct() <= 10)
			{
				_unit->PlaySoundToSet(9186);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your request cannot be processed.");
				_unit->CastSpell(_unit, infoEvocation, true);
				evocation = true;
			}
			else if(!enrage && _unit->GetHealthPct() <= 16)
			{
				_unit->CastSpell(_unit, infoEnrage, true);
				_unit->PlaySoundToSet(9185);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Failure to comply will result in offensive action.");
				enrage = true;
			}
			else if(!TimedTrigger())
				CastBolt();
		}
		else if(_unit->GetManaPct() > 90)
			evocation = false;
	}

	void CastTime()
	{
		spellBolt.casttime = spellBolt.cooldown;
	}

	void CastBolt()
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *target = NULL;
			spellBolt.casttime--;
			if(spellBolt.casttime) return;

			if (!spellBolt.casttime && m_spellcheck)
			{
				spellBolt.casttime = spellBolt.cooldown;
				target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(_unit, spellBolt.info, spellBolt.instant);
				m_spellcheck = false;
				return;
			}
			m_spellcheck = true;
		}
	}

	bool TimedTrigger()
	{
		m_time_berserk--;
		m_time_summon--;
		if(!berserk && !m_time_berserk) {
			_unit->CastSpell(_unit, infoBerserk, true);
			berserk = true;
			return true;
		}
		else if(!enrage && !m_time_summon) {
			uint32 sound, val = (uint32) RandomUInt(100)%2;
			char *text;
			switch(val)
			{
			case 0:
				sound = 9188;
				text = "Gallery rules will be strictly enforced.";
				break;
			case 1:
				sound = 9309;
				text = "This curator is equipped for gallery protection.";
				break;
			}
			_unit->PlaySoundToSet(sound);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
			// spawn a new flare
			// TODO: summon on n/e/s/w positions
			// Temporary hack to reduce mana by 10%
			_unit->SetUInt32Value(UNIT_FIELD_POWER1,
				_unit->GetUInt32Value(UNIT_FIELD_POWER1) - (_unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1) / 10));
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE,
				_unit->GetPositionX(), _unit->GetPositionY(),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, 0, 0);
			m_time_summon = 10;
			return true;
		}
		return  false;
	}

protected:
	bool evocation;
	bool enrage;
	bool berserk;
	uint32 m_time_summon;
	uint32 m_time_berserk;
	SpellEntry *infoEvocation;
	SpellEntry *infoEnrage;
	SpellEntry *infoBerserk;
};

// Astral Flare
#define ASTRAL_FLARE_PASSIVE 30234
#define ARCING_SCAR 30325
#define ASTRAL_FLARE_VISUAL 30237

class ASTRALFLAREAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ASTRALFLAREAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	ASTRALFLAREAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		/*	spells[0].info = dbcSpell.LookupEntry(ASTRAL_FLARE_PASSIVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 3;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		*/
		spells[0].info = dbcSpell.LookupEntry(ARCING_SCAR);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ASTRAL_FLARE_VISUAL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

// Shade of Aran
#define CN_SHADEOFARAN 16524

#define FROSTBOLT 29954
#define FIREBALL 29953
#define ARCMISSLE 29955
#define CHAINSOFICE 29991
#define DRAGONSBREATH 29964
#define CIRCULAR_BLIZZAR 29952
#define MASSSLOW 30035
#define MAGNETIC_PULL 29979
#define FLAME_WREATH 29946 // detonate -> 30004
#define AOE_COUNTERSPELL 29961
#define AEXPLOSION 29973
#define MASS_POLYMORPH 29963
#define BLINK_CENTER 29967
#define SUMMON_ELEMENTALS 29962
#define CONJURE 29975
#define AOE_PYROBLAST 29978

class SHADEOFARANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SHADEOFARANAI);
	bool m_spellcheck[6];
	SP_AI_Spell spells[6];

	SHADEOFARANAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FIREBALL);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(ARCMISSLE);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(CHAINSOFICE);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].cooldown = 10;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DRAGONSBREATH);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = 10;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(AOE_COUNTERSPELL);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].cooldown = 15;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		info_flame_wreath = dbcSpell.LookupEntry(FLAME_WREATH);
		info_blink_center = dbcSpell.LookupEntry(BLINK_CENTER);
		info_massslow = dbcSpell.LookupEntry(MASSSLOW);
		info_magnetic_pull = dbcSpell.LookupEntry(MAGNETIC_PULL);
		info_aexplosion = dbcSpell.LookupEntry(AEXPLOSION);
		info_blizzard = dbcSpell.LookupEntry(CIRCULAR_BLIZZAR);
		info_summon_elementals = dbcSpell.LookupEntry(SUMMON_ELEMENTALS);
		info_mass_polymorph = dbcSpell.LookupEntry(MASS_POLYMORPH);
		info_conjure = dbcSpell.LookupEntry(CONJURE);
		info_pyroblast = dbcSpell.LookupEntry(AOE_PYROBLAST);

		GameObject* pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11162.56f, -1913.954f, 232.008f, 184517);
	}

	void OnCombatStart(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%3;
		char *text;
		switch(val)
		{
			// TODO: extra emote if Atiesh is equipped by a raid member
			// 9249 - "Where did you get that?! Did HE send you?!"
		case 0:
			sound = 9241;
			text = "Please, no more. My son... he's gone mad!";
			break;
		case 1:
			sound = 9323;
			text = "I'll not be tortured again!";
			break;
		case 2:
			sound = 9324;
			text = "Who are you? What do you want? Stay away from me!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		_unit->GetAIInterface()->disable_melee = true;
		RegisterAIUpdateEvent(1000);
		m_time_enrage = 900;
		m_time_special = 30 + (uint32) RandomUInt(10)%5;
		m_time_pyroblast = 0;
		CastTime();
		drinking = false;
		enraged = false;
		summoned = false;
		explode = false;
		slow = false;

		if (pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
			_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
		CastTime();
		if (pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9244);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last... The nightmare is.. over...");
		if (pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0: {
			sound = 9250;
			text = "I want this nightmare to be over!";
				} break;
		case 1: {
			sound = 9328;
			text = "Torment me no more!";
				} break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		if(!drinking) 
		{
			if(explode)
			{
				if(slow)
				{
					_unit->CastSpell(_unit, info_massslow, true);
					slow = false;
				}
				else
				{
					_unit->CastSpell(_unit, info_aexplosion, false);
					explode = false;
				}
			}
			else if(!summoned && _unit->GetHealthPct() <= 40)
			{
				_unit->CastSpell(_unit, info_summon_elementals, true);
				_unit->PlaySoundToSet(9251);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm not finished yet! No, I have a few more tricks up me sleeve.");
				summoned = true;
			}
			else if(_unit->GetManaPct() <= 20)
			{
				if(!m_time_pyroblast) 
				{
					_unit->PlaySoundToSet(9248);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Surely you would not deny an old man a replenishing drink? No, no I thought not.");
					m_time_pyroblast = 10;
					_unit->CastSpell(_unit, info_mass_polymorph, false);
				}
				else
				{
					m_time_pyroblast--;
					// TODO: conjure spell broken :(
					_unit->CastSpell(_unit, info_conjure, false);
					drinking = true;
				}
			}
			else
				SpellTrigger();
		}
		else
		{
			m_time_pyroblast--;
			if(!m_time_pyroblast)
			{
				_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
				_unit->CastSpell(_unit, info_pyroblast, false);
				drinking = false;
			}
		}
	}

	void SpellTrigger()
	{
		m_time_enrage--;
		m_time_special--;
		if(!enraged && !m_time_enrage) {
			// TODO: Enrage => Summon Shadows of Aran after 15 min
			_unit->PlaySoundToSet(9247);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You've wasted enough of my time. Let these games be finished!");
			enraged = true;
			return;
		}
		else if(!m_time_special)
		{
			CastSpecial();
			m_time_special = 30 + (uint32) RandomUInt(10)%5;
			return;
		}
		else {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void EmoteBlizzard()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9246;
			text = "I'll freeze you all!";
			break;
		case 1:
			sound = 9327;
			text = "Back to the cold dark with you!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void EmoteWreath()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9245;
			text = "I'll show you this beaten dog still has some teeth!";
			break;
		case 1:
			sound = 9326;
			text = "Burn you hellish fiends!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void EmoteExplosion()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9242;
			text = "Yes, yes, my son is quite powerful... but I have powers of my own!";
			break;
		case 1:
			sound = 9325;
			text = "I am not some simple jester! I am Nielas Aran!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void CastSpecial()
	{
		uint32 val = (uint32) RandomUInt(100)%3;
		switch(val)
		{
		case 0:
			EmoteExplosion();
			_unit->CastSpell(_unit, info_blink_center, true);
			_unit->CastSpell(_unit, info_magnetic_pull, true);
			explode = true;
			slow = true;
			break;
		case 1:
			EmoteBlizzard();
			_unit->CastSpell(_unit, info_blizzard, true);
			break;
		case 2:
			EmoteWreath();
			_unit->CastSpell(_unit, info_flame_wreath, true);
			break;
		}
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(true, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	bool drinking;
	bool enraged;
	bool summoned;
	bool explode;
	bool slow;
	int nrspells;
	uint32 m_time_enrage;
	uint32 m_time_special;
	uint32 m_time_pyroblast;
	SpellEntry *info_flame_wreath;
	SpellEntry *info_aexplosion;
	SpellEntry *info_blizzard; 
	SpellEntry *info_magnetic_pull;
	SpellEntry *info_blink_center;
	SpellEntry *info_massslow;
	SpellEntry *info_conjure;
	SpellEntry *info_summon_elementals;
	SpellEntry *info_pyroblast;
	SpellEntry *info_mass_polymorph;

	GameObject *pDoor;
};

// Terestian Illhoof
#define CN_ILLHOOF 15688

#define SHADOW_BOLT 19729
#define SUMMON_DEMONCHAINS 30120 // maybe there's some Sacrifice id?!
#define SUMMON_KIL_REK 30066
//#define ENRAGE ?????
// TODO: couldnt find Demonic-Portal spell

class ILLHOOFAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ILLHOOFAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	ILLHOOFAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SUMMON_DEMONCHAINS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		summonImp = 0;
		summonSpeech = 0;
		CastTime();
		_unit->PlaySoundToSet(9260);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, you're just in time. The rituals are about to begin.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9262);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My life, is yours. Oh great one.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9264;
			text = "Your blood will anoint my circle.";
			break;
		case 1:
			sound = 9329;
			text = "The great one will be pleased.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		summonImp++;
		if (summonImp > 2) //its already nerfed (hope enough)
		{
			SummonImp();
			summonImp = 0;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SummonImp()
	{
		summonSpeech++;
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17267, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f, false, false, 0, 0);
		if(summonSpeech == 1)
		{
			uint32 sound, val = (uint32) RandomUInt(100)%2;
			char *text;
			switch(val)
			{
			case 0:
				sound = 9265;
				text = "Come, you dwellers in the dark. Rally to my call!";
				break;
			case 1:
				sound = 9331;
				text = "Gather, my pets. There is plenty for all.";
				break;
			}
			_unit->PlaySoundToSet(sound);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		}
	}

	void SacrificePlayer()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9263;
			text = "Please, accept this humble offering, oh great one.";
			break;
		case 1:
			sound = 9330;
			text = "Let the sacrifice serve his testament to my fealty.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void CastTime()
	{
		for(int i=0;i<2;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
	int summonImp;
	int summonSpeech;
};

// Imps
#define CN_FIENDISH_IMP 17267

#define IMP_FIREBALL 38692

class FiendishImpAI : public CreatureAIScript
	{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FiendishImpAI);
		bool m_spellcheck[1];
		SP_AI_Spell spells[1];

		FiendishImpAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(IMP_FIREBALL);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].cooldown = 0;
			spells[0].perctrigger = 100.0f;
			spells[0].attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(1000);
		}

		void OnCombatStop(Unit *mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			_unit->Despawn(100, 0);
		}

		void OnDied(Unit * mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}

		void CastTime()
		{
			for(int i=0;i<nrspells;i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void SpellCast(float val)
			{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
				float comulativeperc = 0;
				Unit *target = NULL;
				for(int i=0;i<nrspells;i++)
					{
					spells[i].casttime--;

					if (m_spellcheck[i])
						{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->GetNextTarget();
						switch(spells[i].targettype)
							{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
							}

						if (spells[i].speech != "")
							{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid); 
							}

						m_spellcheck[i] = false;
						return;
						}

					if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
						{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
						}
					comulativeperc += spells[i].perctrigger;
					}
				}
			}

	protected:
		int nrspells;
	};

// Kil'Rek
#define CN_KIL_REK 17229

#define AMPLIFY_FLAMES 30053
#define BROKEN_PACT 30065

class KILREKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KILREKAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	KILREKAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(AMPLIFY_FLAMES);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BROKEN_PACT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

// Prince Malchezaar
#define CN_MALCHEZAAR 15690
#define CN_INFERNAL 89 // not shure if correct id
#define CN_MALCHEZAARS_AXES 17650

#define ENFEEBLE 30843
#define SHADOWNOVA 30852
#define SW_PAIN 30854
#define THRASH_PASSIVE 12787 // should self-buff prince with 3391
#define SUNDER_ARMOR 25225
#define CLEAVE 15663
#define AMPLIFY_DAMAGE 12738 //Axe's use this spell; maybe wrong spell => test 39095
#define HELLFIRE 39132 // Infernals cast this once every 3 seconds
#define SUMMON_INFERNAL ????? // WTF is this spells id?!

#define SUMMON_AXES 30891
#define WIELD_AXES 30857

/* Emotes:
SPECIAL? - 9223 - 9320
AXETOSS2? - 9317
*/

class MALCHEZAARAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MALCHEZAARAI);
	bool m_spellcheck[5];
	SP_AI_Spell spells[5];

	MALCHEZAARAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		info_thrash = dbcSpell.LookupEntry(THRASH_PASSIVE);
		info_shadownova = dbcSpell.LookupEntry(SHADOWNOVA);
		info_enfeeble = dbcSpell.LookupEntry(ENFEEBLE);
		info_sw_pain = dbcSpell.LookupEntry(SW_PAIN);
		info_cleave = dbcSpell.LookupEntry(CLEAVE);
		info_sunder_armor = dbcSpell.LookupEntry(SUNDER_ARMOR);

		info_wield_axes = dbcSpell.LookupEntry(WIELD_AXES);
		info_summon_axes = dbcSpell.LookupEntry(SUMMON_AXES);
	}

	void SetupSpells()
	{
		m_phase = 1;
		nrspells = 4;
		m_time_infernal = 45;

		for(int i=0;i<5;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = info_thrash;
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = info_enfeeble;
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = info_shadownova;
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = false;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = info_sw_pain;
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 15;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = info_cleave;
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].cooldown = 6;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		SetupSpells();
		_unit->PlaySoundToSet(9218);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Madness has brought you here to me. I shall be your undoing.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9221);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I refuse to concede defeat. I am a prince of the Eredar! I am...");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound, val = (uint32) RandomUInt(100)%3;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9222;
			text = "You are, but a plaything, unfit even to amuse.";
			break;
		case 1:
			sound = 9318;
			text = "Your greed, your foolishness has brought you to this end.";
			break;
		case 2:
			sound = 9319;
			text = "Surely you did not think you could win.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		switch(m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		case 3: {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
				} break;
		default:
			m_phase = 1;
		};

		m_time_infernal--;
		if(!m_time_infernal) {
			SummonInfernal();
			if(m_phase==3)
				m_time_infernal = 20;
			else
				m_time_infernal = 45;
		}
	}

	void PhaseOne()
	{
		if(_unit->GetHealthPct() <= 60)
		{
			_unit->PlaySoundToSet(9220);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time is the fire in which you'll burn!");
			spells[3].info = info_sunder_armor;
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = true;
			spells[3].cooldown = 10;
			spells[3].casttime = 10;

			// equip axes...not shure if this spells works :>
			_unit->CastSpell(_unit, info_wield_axes, true);

			m_phase = 2;
			nrspells = 5;
		}
		else {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseTwo()
	{
		if(_unit->GetHealthPct() <= 30)
		{
			_unit->PlaySoundToSet(9321);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How can you hope to withstand against such overwhelming power?");            m_phase = 2;
			spells[1].info = info_sw_pain;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 15;
			spells[1].casttime = 15;
			// summon axes...not shure if this spells works :>
			_unit->CastSpell(_unit, info_summon_axes, true);
			m_phase = 3;
			nrspells = 3;
		}
		else {
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void SummonInfernal()
	{
		uint32 sound, val = (uint32) RandomUInt(100)%2;
		char *text;
		switch(val)
		{
		case 0:
			sound = 9322;
			text = "You face not Malchezaar alone, but the legions I command!";
			break;
		case 1:
			sound = 9224;
			text = "All realities, all dimensions are open to me!";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		// TODO: Spawn Infernal
	}

	void CastTime()
	{
		for(int i=0;i<5;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	uint32 m_phase;
	int nrspells;
	uint32 m_time_infernal;
	SpellEntry *info_wield_axes;
	SpellEntry *info_summon_axes;
	SpellEntry *info_thrash;
	SpellEntry *info_enfeeble;
	SpellEntry *info_shadownova;
	SpellEntry *info_sw_pain;
	SpellEntry *info_cleave;
	SpellEntry *info_sunder_armor;
};

// Netherspite
#define CN_NETHERSPITE 15689

#define NETHERBURN 30523
#define VOIDZONE 30533
#define NETHERBREATH 36631
#define BERSERK 26662
#define NETHERBURN_AURA 30522
// TODO: Perservance, Serenity and Dominance

class NETHERSPITEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NETHERSPITEAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	NETHERSPITEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(NETHERBURN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(VOIDZONE);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = false;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(NETHERBREATH);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 5;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(BERSERK);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 540;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

// Nightbane
#define CN_NIGHTBANE 17225
#define CN_RESTLESS_SKELETON 17261 // not needed if spell works

#define WALK 0
#define RUN 256
#define FLY 768

// ground spells
#define BELLOWING_ROAR 36922
#define CHARRED_EARTH 30129 //Also 30209 (Target Charred Earth) triggers this
#undef CLEAVE
#define CLEAVE 31043 // fixme: correct spell?!
#define SMOLDERING_BREATH 39385
#define TAIL_SWEEP 25653 // TODO: how to use this spell???
#define DISTRACTING_ASH 30280

// air spells
#define DISTRACTING_ASH_FLY 30130 // all guides say ground spell but animation is flying?!
#define RAIN_OF_BONES 37091 // Spell bugged: should debuff with 37098
#define SMOKING_BLAST 37057
#define FIREBALL_BARRAGE 30282
#define SUMMON_BONE_SKELETONS 30170

static Coords coords[] =
{
	{ 0, 0, 0, 0 },
	{ -11173.719727f, -1863.993164f, 130.390396f, 5.343079f }, // casting point
	{ -11125.542969f, -1926.884644f, 139.349365f, 3.982360f },
	{ -11166.404297f, -1950.729736f, 114.714726f, 1.537812f },
	{ -11167.497070f, -1922.315918f, 91.473755f, 1.390549f } // landing point
};

class NIGHTBANEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NIGHTBANEAI);
	bool m_spellcheck[5];
	SP_AI_Spell spells[5];

	NIGHTBANEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->setMoveType(4);
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));

		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		info_tail_sweep = dbcSpell.LookupEntry(TAIL_SWEEP);
		info_distracting_ash = dbcSpell.LookupEntry(DISTRACTING_ASH);
		info_rain_of_bones = dbcSpell.LookupEntry(RAIN_OF_BONES);
		info_smoking_blast = dbcSpell.LookupEntry(SMOKING_BLAST);
		info_summon_skeletons = dbcSpell.LookupEntry(SUMMON_BONE_SKELETONS);
		info_fireball_barrage = dbcSpell.LookupEntry(FIREBALL_BARRAGE);

		spells[0].info =  dbcSpell.LookupEntry(BELLOWING_ROAR);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARRED_EARTH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 8;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SMOLDERING_BREATH);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 7;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(CLEAVE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].cooldown = 7;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DISTRACTING_ASH);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].cooldown = 20;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		m_phase = 0;
		m_time_hover = 0;
		m_currentWP = 4;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setMoveType(0);
		_unit->GetAIInterface()->setWaypointToMove(0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 1:
			{
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
				//_unit->m_pacified--;
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);
				WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
				data << _unit->GetNewGUID();
				data << uint32(0);
				_unit->SendMessageToSet(&data, false);
				m_currentWP = 1;
			}break;
		case 4:
			{
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);
				/*_unit->m_pacified--;
				if(_unit->m_pacified > 0)
				_unit->m_pacified--;*/
				WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
				data << _unit->GetNewGUID();
				data << uint32(0);
				_unit->SendMessageToSet(&data, false);
				Land();
				m_currentWP = 4;
			}break;
		default:
			{
				iWaypointId++;
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId);
			}break;
		};
	}

	void AIUpdate()
	{
		if(m_phase%2)
		{
			if(m_time_hover)
			{
				if(m_currentWP == 1)
				{
					m_time_hover--;
					Unit *target = _unit->GetAIInterface()->GetNextTarget();
					if(target && m_time_hover < 17)
						_unit->CastSpell(target, info_smoking_blast, true);
					else if(target) {
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), info_rain_of_bones, true);
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), info_summon_skeletons, true);
					}
					// TODO: cast fireball barrage if a player gets out of range
					// _unit->CastSpell(target, info_fireball_barrage, true);
				}
			}
			else
			{
				if(_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
				//_unit->m_pacified++;
				_unit->GetAIInterface()->StopMovement(0);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(2);
				m_phase++;
			}
		}
		else if((m_phase == 0 && _unit->GetHealthPct() <= 75) 
			|| (m_phase == 2 && _unit->GetHealthPct() <= 50) 
			|| (m_phase == 4 && _unit->GetHealthPct() <= 25))
		{
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			//_unit->m_pacified++;            
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			Fly();
			m_phase++;
			m_time_hover = 17;
		}
		else if(m_currentWP == 4)
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
			// TODO: cast tail-sweep if any player gets behind us
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		//WayPoint* wp = new WayPoint;
		//WayPoint * wp = _unit->GetMapMgr()->GetInterface()->CreateWaypoint();
		//WayPoint * wp = sStructFactory.CreateWaypoint();
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = coords[id].x;
		wp->y = coords[id].y;
		wp->z = coords[id].z;
		wp->o = coords[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

	void Fly()
	{
		_unit->Emote(EMOTE_ONESHOT_LIFTOFF);
		//Do we need hover really? Check it :D
		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = true;
	}

	void Land()
	{
		_unit->Emote(EMOTE_ONESHOT_LAND);
		//Do we need hover really? Check it :D
		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = false;
	}


	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
	uint32 m_phase;
	uint32 m_time_hover;
	uint32 m_currentWP;
	SpellEntry *info_smoking_blast;
	SpellEntry *info_summon_skeletons;
	SpellEntry *info_fireball_barrage;
	SpellEntry *info_rain_of_bones;
	SpellEntry *info_distracting_ash;
	SpellEntry *info_tail_sweep;
};

void SetupKarazhan(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_MIDNIGHT, &MIDNIGHTAI::Create);
	mgr->register_creature_script(CN_ATTUMEN, &ATTUMENAI::Create);
	mgr->register_creature_script(CN_MOROES, &MOROESAI::Create);
	mgr->register_creature_script(CN_MAIDEN, &MAIDENOFVIRTUEAI::Create);
	mgr->register_creature_script(CN_THEBIGBADWOLF, &THEBIGBADWOLFAI::Create);
	mgr->register_creature_script(CN_THECURATOR, &THECURATORAI::Create);
	mgr->register_creature_script(CN_ASTRALFLARE, &ASTRALFLAREAI::Create);
	mgr->register_creature_script(CN_SHADEOFARAN, &SHADEOFARANAI::Create);
	mgr->register_creature_script(CN_ILLHOOF, &ILLHOOFAI::Create);
	mgr->register_creature_script(CN_KIL_REK, &KILREKAI::Create);
	mgr->register_creature_script(CN_FIENDISH_IMP, &FiendishImpAI::Create);
	mgr->register_creature_script(CN_MALCHEZAAR, &MALCHEZAARAI::Create);
	mgr->register_creature_script(CN_NETHERSPITE, &NETHERSPITEAI::Create);
	mgr->register_creature_script(CN_NIGHTBANE, &NIGHTBANEAI::Create);
}
