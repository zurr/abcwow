#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* ZulAman.cpp Script													*/
/************************************************************************/

//Nalorakk

#define NALORAKK 23576

//TROLL FORM
#define BRUTAL_SWIPE 42384
#define MANGLE 44955
#define SURGE 42402
//BEAR FORM
#define LACERATING_SLASH 42395
#define REND_FLESH 42397
#define DEAFENING_ROAR 42398


class NALORAKKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NALORAKKAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

	NALORAKKAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		for(int i=spmin;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(BRUTAL_SWIPE); 
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(MANGLE); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(SURGE); 
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 4.0f;
		spells[2].attackstoptimer = 1000;


		spells[3].info = dbcSpell.LookupEntry(LACERATING_SLASH);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 7.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(REND_FLESH); 
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 4.0f;
		spells[4].attackstoptimer = 1000;


		spells[5].info = dbcSpell.LookupEntry(DEAFENING_ROAR); 
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].perctrigger = 6.0f;
		spells[5].attackstoptimer = 1000;

		nrspells = 3;
		spmin = 0;
		m_phase = 0;
		m_phasechange = 45;
		m_enrage = 600;
		enraged = 0;
	}

	void OnCombatStart(Unit* mTarget)
	{
		nrspells = 3;
		spmin = 0;
		m_phase = 0;
		m_enrage = 600;
		enraged = 0;
		_unit->PlaySoundToSet(12070);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You be dead soon enough!");
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach=RandomUInt(1000)%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->PlaySoundToSet(12076);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Di Amani gonna rule again!");
				break;
			case 1:
				_unit->PlaySoundToSet(12075);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Now whatchoo got to say?");
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631);  //change to Troll
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(12077);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... be waitin' on da udda side....");
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		if (!m_enrage && !enraged)
		{
			_unit->DamageDoneModPCT[0] = 2;
			_unit->DamageDoneModPCT[1] = 2;
			_unit->DamageDoneModPCT[2] = 2;
			_unit->DamageDoneModPCT[3] = 2;
			_unit->DamageDoneModPCT[4] = 2;
			_unit->DamageDoneModPCT[5] = 2;
			enraged = 1;
		}
		if (!m_phasechange)
		{
			switch (m_phase)
			{
			case 0:
				//bear
				nrspells = 7;
				spmin = 3;
				m_phase = 1;
				m_phasechange = 30;
				_unit->PlaySoundToSet(12072);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You call on da beast, you gonna get more dan you bargain for!");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 706);
				break;
			case 1:
				//human
				nrspells = 3;
				spmin = 3;
				m_phase = 0;
				m_phasechange = 45;
				//_unit->PlaySoundToSet(11313);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Make way for Nalorakk!");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631);
				break;
			}
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
		m_enrage--;
		m_phasechange--;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
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

	int nrspells;
	int m_phase;
	int m_phasechange;
	int m_enrage;
	int enraged;
	int spmin;
};

//Akil'zon <Eagle Avatar>

#define AKILZON 23574
#define SOARING_EAGLE 24858


#define STATIC_DISRUPTION 44008
#define CALL_LIGHTING 43661
#define GUST_OF_WIND 43621
#define ELECTRICAL_STORM 43648
#define ELECTRICAL_STORM_CLOUD 44007


class AKILZONAI : public CreatureAIScript, public EventableObject
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AKILZONAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	AKILZONAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(STATIC_DISRUPTION);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CALL_LIGHTING);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 3.5f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(GUST_OF_WIND);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;


		m_electicalstorm = 50 + RandomUInt(1000)%10;
		m_enrage = 600;
		enraged = 0;
	}

	void OnCombatStart(Unit* mTarget)
	{
		m_electicalstorm = 50 + RandomUInt(1000)%10;
		m_enrage = 600;
		enraged = 0;
		_unit->PlaySoundToSet(12013);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I be da predator! You da prey!");
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach = RandomUInt(1000)%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->PlaySoundToSet(12018);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop your cryin'!");
				break;
			case 1:
				_unit->PlaySoundToSet(12017);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya got nothin'!");
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(12019);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can't... kill... me spirit!");
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		if (!m_enrage && !enraged)
		{
			_unit->DamageDoneModPCT[0] = 5;
			_unit->DamageDoneModPCT[1] = 5;
			_unit->DamageDoneModPCT[2] = 5;
			_unit->DamageDoneModPCT[3] = 5;
			_unit->DamageDoneModPCT[4] = 5;
			_unit->DamageDoneModPCT[5] = 5;
			enraged = 1;
		}
		/* Disabled fucked up :(
		m_electicalstorm--;
		if (!m_electicalstorm)
		{
		m_electicalstorm = 50 + RandomUInt(1000)%10;
		Player *target = (Player *) RandomTarget(false, true, 10000);
		if (target)
		{
		_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), dbcSpell.LookupEntry(ELECTRICAL_STORM_CLOUD), true);
		target->_Relocate(_unit->GetMapId(), LocationVector(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 10, target->GetOrientation()), false, false, _unit->GetInstanceID());
		_unit->CastSpell(target, ELECTRICAL_STORM, true);
		sEventMgr.AddEvent(this, &AKILZONAI::summonEagles, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
		}
		}
		*/
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
		m_enrage--;
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
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
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
	void summonEagles()
	{
		_unit->PlaySoundToSet(12019);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Feed, me bruddahs!");
		Creature *cre = NULL;
		for (int x = 0; x < 6;x++)
		{
			cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(SOARING_EAGLE, _unit->GetPositionX() + RandomFloat(5) - 5, _unit->GetPositionY() + RandomFloat(5) - 5, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
			cre->GetAIInterface()->setOutOfCombatRange(5000);
		}

	}
protected:

	int nrspells;
	int m_electicalstorm;
	int m_enrage;
	int enraged;
};

//Mobs

//SOARING_EAGLE Summon Akil'zon 

#define EAGLE_SWOOP 44732 //INSTANT


class SOARINGEAGLEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SOARINGEAGLEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	SOARINGEAGLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(EAGLE_SWOOP);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(100,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(100,0);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//Halazzi <Lynx Avatar>

#define HALAZZI 23577
#define HALAZZILYNX 23812
#define CORRUPTED_LIGHTING_TOTEM 24224 

//Phase 1
#define SABER_LASH 43267 //43267 //43268 ///40810 //40816
//Phase 2
#define FLAME_SHOCK 43303
#define EARTH_SHOCK 43305

class HALAZZIAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HALAZZIAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	HALAZZIAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[1].targettype = TARGET_RANDOM_SINGLE; 
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(EARTH_SHOCK);
		spells[2].targettype = TARGET_RANDOM_SINGLE; 
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;

		enrage = false;
		m_enrage = 600;
		m_phase =1;
		m_phasestep = 1;
	}

	void OnCombatStart(Unit* mTarget)
	{
		nrspells = 1;
		spmin = 0;
		m_phase =1;
		m_phasestep = 1;
		enrage = false;
		m_enrage = 600;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Get on your knees and bow to da fang and claw!");
		_unit->PlaySoundToSet(12020);
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cant fight da power...");
				_unit->PlaySoundToSet(12026);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You all gonna fail...");
				_unit->PlaySoundToSet(12027);
				break;
			}
		}

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Chaga... choka'jinn.");
		_unit->PlaySoundToSet(12028);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		m_enrage--;
		if (!enrage && !m_enrage)
		{
			_unit->DamageDoneModPCT[0] = 2;
			_unit->DamageDoneModPCT[1] = 2;
			_unit->DamageDoneModPCT[2] = 2;
			_unit->DamageDoneModPCT[3] = 2;
			_unit->DamageDoneModPCT[4] = 2;
			_unit->DamageDoneModPCT[5] = 2;
			enrage = true;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
		switch (m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		case 3:
			PhaseThree();
			break;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
		m_enrage--;
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
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
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
	void PhaseOne()
	{
		switch (m_phasestep)
		{
		case 1:
			if (_unit->GetHealthPct() <= 75)
			{
				spmin = 1;
				nrspells = 2;
				m_phase = 2;
				m_totemcd = 50 + RandomUInt(1000)%15;
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
			}
			break;
		case 2:
			if (_unit->GetHealthPct() <= 50)
			{
				spmin = 1;
				nrspells = 2;
				m_phase = 2;
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
			}
			break;
		case 3:
			if (_unit->GetHealthPct() <= 25)
			{
				spmin = 1;
				nrspells = 2;
				m_phase = 2;
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				lynx = _unit->GetMapMgr()->GetInterface()->SpawnCreature(HALAZZILYNX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
			}
			break;
		}
	}
	void PhaseTwo()
	{
		m_totemcd--;
		if (_unit->GetHealthPct() <= 10 || lynx->GetHealthPct() <= 10) 
		{
			switch (m_phasestep)
			{
			case 1:
				nrspells = 1;
				spmin = 0;
				m_phase =1;
				m_phasestep = 2;
				lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.75));
				break;
			case 2:
				nrspells = 1;
				spmin = 0;
				m_phase =1;
				m_phasestep = 3;
				lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.50));
				break;
			case 3:
				lynx->Despawn(100, 0);
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.25));
				m_phase = 3;
				spmin = 0;
				nrspells = 3;
				break;
			}
		}
		if (!m_totemcd)
		{
			Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, _unit->GetPositionX()+2, _unit->GetPositionY()+2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
			m_totemcd = 30 + RandomUInt(1000)%15;
		}
	}
	void PhaseThree()
	{
		if (!m_totemcd)
		{
			Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CORRUPTED_LIGHTING_TOTEM, _unit->GetPositionX()+2, _unit->GetPositionY()+2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
			m_totemcd = 20 + RandomUInt(1000)%15;
		}
	}
protected:

	int nrspells;
	int spmin;
	int m_phase;
	int m_phasestep;
	uint32 m_totemcd;
	int m_enrage;
	bool enrage;
	Creature *lynx;
};

//Corrupted Lightning Totem
#define CLT_LIGHTING 43301 //43526 //43301  //43661 //


class CLTOTEMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CLTOTEMAI);

	CLTOTEMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->GetAIInterface()->m_canMove = false;
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(100,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(100,0);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		if (_unit->GetCurrentSpell() == NULL)
			_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), CLT_LIGHTING, false);
	}
protected:
};


//Jan'alai <Dragonhawk Avatar>
#define JANALAI 23578
#define AMANIDRAGONHAWKHATCHLING 23598
#define FIREBOMBTARGET 23920

//Phase 1
#define FLAME_BREATH 43582 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
#define FLAME_BUFFET 43299 
//#define TELEPORT ????
//Phase2 
//#define FLAME_BREATH 23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define FLAME_BUFFET 43299 
//phase3 
//#define FLAME_BREATH 23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define FLAME_BUFFET 43299 
#define JANALAI_ENRAGE 44779




class JANALAIAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JANALAIAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	JANALAIAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		spells[0].info = dbcSpell.LookupEntry(FLAME_BREATH);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		nrspells = 1;
		spmin = 0;
		softenrage = false;
		enraged = false;
		m_enrage = 600;
		m_softenrage = 300;
		m_firebombs = 35 + RandomUInt(100)%25;
		m_adds = 14;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}


	}

	void OnCombatStart(Unit* mTarget)
	{
		m_firebombs = 35 + RandomUInt(100)%25;
		m_softenrage = 300;
		m_adds = 14;
		softenrage = false;
		enraged = false;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Spirits Of the wind,be your doom!");
		_unit->PlaySoundToSet(12031);
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It all be over now, mon!");
				_unit->PlaySoundToSet(12036);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you!");
				_unit->PlaySoundToSet(12041);
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Zul'jin... got a surprise for you...");
		_unit->PlaySoundToSet(12038);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		m_adds--;
		m_enrage--;
		m_firebombs--;
		m_softenrage--;
		if (!m_enrage && !enraged)
		{
			enraged = true;
			_unit->DamageDoneModPCT[0] = 1;
			_unit->DamageDoneModPCT[1] = 1;
			_unit->DamageDoneModPCT[2] = 1;
			_unit->DamageDoneModPCT[3] = 1;
			_unit->DamageDoneModPCT[4] = 1;
			_unit->DamageDoneModPCT[5] = 1;
		}
		if (!softenrage && (_unit->GetHealthPct() <= 25 || !m_softenrage)) 
		{
			softenrage = true;
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(JANALAI_ENRAGE), true);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where ma hatcha? Get to work on dem eggs!");
			_unit->PlaySoundToSet(12033);
		}
		if (!m_firebombs)
		{
			float x;
			float y;

			for (int z = 0; z < 16;z++)
			{
				x = -51.0f + RandomUInt(100)%39;
				y = 1132.0f + RandomUInt(100)%35;
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(FIREBOMBTARGET, x, y, _unit->GetPositionZ(), 0, true, false, 0, 0);
			}
			m_firebombs = 35 + RandomUInt(100)%25;
		}
		if (!m_adds)
		{
			Creature *summon;
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -34.23f, 1099.19f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -34.23f, 1099.19f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -33.18f, 1201.79f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(AMANIDRAGONHAWKHATCHLING, -33.18f, 1201.79f, 18.71f, 0, true, false, 0, 0);
			summon->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			m_adds = 80 + RandomUInt(100)%25;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
			Unit *temp = _unit->GetMapMgr()->GetUnit(itr->first);
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

	int m_enrage;
	bool enraged;
	bool softenrage;
	int nrspells;
	int spmin;
	int m_firebombs;
	int m_softenrage;
	int m_adds;
};

//HEX_LORD_MALACRASS

#define CN_HEX_LORD_MALACRASS 24239
//summons
#define GAZAKROTH 24244
#define THURG 24241 
#define LORD_RAADAN 24243 
#define DARKHEART 24246 
#define ALYSON_ANTILLE 24240
#define SLITHER 24242 
#define FENSTALKER 24245 
#define KORAGG 24247 

//spells
#define HEX_LORD_MALACRASS_SOUL_DRAIN 41303
#define HEX_LORD_MALACRASS_SPIRIT_BOLTS 43382 //43383 

class HEXLORDMALACRASSAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HEXLORDMALACRASSAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HEXLORDMALACRASSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SOUL_DRAIN);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SPIRIT_BOLTS);
		spells[1].targettype = TARGET_DESTINATION; 
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you...");
		_unit->PlaySoundToSet(12041);
		timer = 5;
		mobs = 0;
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {
				if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Azzaga choogo zinn!");
				_unit->PlaySoundToSet(12044);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis a nightmare ya don' wake up from!");
				_unit->PlaySoundToSet(12043);
				break;
			}
		}
	
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 2;
		timer = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis not da end for me..");
		_unit->PlaySoundToSet(12051);
       RemoveAIUpdateEvent();

	   GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(123.256f, 914.422f, 34.1441f, 186306);
        if(pDoor == 0)
            return;

       pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0); // Open the door
    }

    void AIUpdate()
	{
		if (!timer && !mobs)
		{
			for(int j=0;j<4;j++)
			{
				int RandomSpeach;
				RandomUInt(1000);
				RandomSpeach=rand()%8;
				switch (RandomSpeach)
				{
					case 0:
						summon = GAZAKROTH;
						break;
					case 1:
						summon = DARKHEART;
						break;
					case 2:
						summon = FENSTALKER;
						break;
					case 3:
						summon = SLITHER;
						break;
					case 4:
						summon = ALYSON_ANTILLE;
						break;
					case 5:
						summon = THURG;
						break;
					case 6:
						summon = KORAGG;
						break;
					case 7:
						summon = LORD_RAADAN;
						break;
				}
				Creature *cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(summon, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			mobs = 1;
		}
		if (timer)
		{
			timer--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
	int timer;
	uint32 summon;
	bool mobs;
};

//ZUL’JIN

#define CN_ZULJIN 23863

//Phase 1 troll
#define ZULJIN_GRIEVOUS_THROW 43093 
#define ZULJIN_WHIRLWIND 17207 // 44949 - work,43442,41400, 41399,41195, 41194,41098,  // from dbc, untested
#define ZULJIN_MODEL_TROLL 21899 
//Phase2 bear
#define ZULJIN_CREEPING_PARALYSIS 43095 
#define ZULJIN_OVERPOWER 43456
#define ZULJIN_MODEL_BEAR 21736
//phase3  eagle
#define ZULJIN_ENERGY_STORM 43983 //not Work
#define ZULJIN_MODEL_EAGLE 22257
#define ZULJIN_FEATHER_VORTEX 24136 //summons npc 4 begin phase
//phase4  lynx
#define ZULJIN_CLAW_RAGE  43150 
#define ZULJIN_LYNX_RUSH 43153
#define ZULJIN_MODEL_LYNX 21907
//phase5  dragonhawk
#define ZULJIN_FLAME_WHIRL 43213 //43208, 43213
#define ZULJIN_FLAME_BREATH 43215 
#define ZULJIN_FLAME_SHOCK 43303
#define ZULJIN_MODEL_DRAGONHAWK 21901




class ZULJINAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ZULJINAI);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    ZULJINAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ZULJIN_GRIEVOUS_THROW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ZULJIN_WHIRLWIND); 
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = false;
		spells[1].perctrigger = 25.0f;
		spells[1].attackstoptimer = 1000;
		
        spells[2].info = dbcSpell.LookupEntry(ZULJIN_CREEPING_PARALYSIS); 
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		
        spells[3].info = dbcSpell.LookupEntry(ZULJIN_OVERPOWER);
		spells[3].targettype = TARGET_VARIOUS; 
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(ZULJIN_ENERGY_STORM); 
		spells[4].targettype = TARGET_VARIOUS;  
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 1000;
		
		spells[5].info = dbcSpell.LookupEntry(ZULJIN_CLAW_RAGE);
		spells[5].targettype = TARGET_ATTACKING; 
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 1000;
		
		spells[6].info = dbcSpell.LookupEntry(ZULJIN_LYNX_RUSH);
		spells[6].targettype = TARGET_ATTACKING; 
		spells[6].instant = true;
		spells[6].perctrigger = 10.0f;
		spells[6].attackstoptimer = 1000;
		
        spells[7].info = dbcSpell.LookupEntry(ZULJIN_FLAME_WHIRL); 
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].perctrigger = 3.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(ZULJIN_FLAME_BREATH); 
		spells[8].targettype = TARGET_ATTACKING; 
		spells[8].instant = true;
		spells[8].perctrigger = 5.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ZULJIN_FLAME_SHOCK);
		spells[9].targettype = TARGET_ATTACKING; 
		spells[9].instant = true;
		spells[9].perctrigger = 5.0f;
		spells[9].attackstoptimer = 1000;			

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nobody badduh dan me.");
		_unit->PlaySoundToSet(12091);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
					if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Di Amani Di chuga");
				_unit->PlaySoundToSet(12098);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lot more gonna fall like you!");
				_unit->PlaySoundToSet(12099);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mebbe me fall...but da Amani empire...never gonna die...");
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 80 && phase == 1) 
		{
			spmin = 2;
			nrspells = 4;
			phase = 2;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_BEAR);  //change to Bear
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Got me some new tricks...like me bruddah bear!");
			_unit->PlaySoundToSet(12092);
		}
		if (_unit->GetHealthPct() <= 60 && phase == 2) 
		{
			spmin = 4;
			nrspells = 5;
			phase = 3;
			for(int j=0;j<4;j++)
			{
				Creature *cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(ZULJIN_FEATHER_VORTEX, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_EAGLE);  //change to Eagle
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dere be no hidin' from da eagle!");
			_unit->PlaySoundToSet(12093);
		}
		if (_unit->GetHealthPct() <= 40 && phase == 3) 
		{
			spmin = 5;
			nrspells = 7;
			phase = 4;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_LYNX);  //change to Lynx
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let me introduce to you my new bruddahs: fang and claw!");
			_unit->PlaySoundToSet(12094);
		}
		if (_unit->GetHealthPct() <= 20 && phase == 4) 
		{
			spmin = 7;
			nrspells = 10;
			phase = 5;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_DRAGONHAWK);  //change to Dragonhawk
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya don' have to look to da sky to see da dragonhawk!");
			_unit->PlaySoundToSet(12095);
		}
		if (_unit->GetHealthPct() <= 1 && phase == 5) 
		{
			_unit->PlaySoundToSet(12100);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
			phase = 6;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
	int spmin;
	int phase;
};

//Gazakroth
#define G_FIREBOLT 39023 


class GAZAKROTHAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GAZAKROTHAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	GAZAKROTHAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(G_FIREBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}


	void OnTargetDied(Unit* mTarget)
	{

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->Despawn(3000,0);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//Lord Raadan
#define LR_FLAME_BREATH 43215 
#define LR_THUNDERCLAP  30633 //38537, 36706 // 36214  //44033


class LORDRAADANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LORDRAADANAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    LORDRAADANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(LR_FLAME_BREATH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;
		
       /* spells[1].info = dbcSpell.LookupEntry(LR_THUNDERCLAP); all types spells to crash server
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//SLITHER

#define S_POISON_BOLT 38459 


class SLITHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SLITHERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SLITHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(S_POISON_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//THURG

#define TH_CLEAVE 43273


class THURGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(THURGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    THURGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(TH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//DARKHEART

#define DH_FEAR 39415
#define DH_CLEAVE 43273

class DARKHEARTAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DARKHEARTAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    DARKHEARTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(DH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000; 
		
        /*spells[1].info = dbcSpell.LookupEntry(DH_FEAR);   crash
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//FENSTALKER

#define FEN_TOXIC_SLIME 37615  //40818

class FENSTALKERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FENSTALKERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FENSTALKERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(FEN_TOXIC_SLIME);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//ALYSON_ANTILLE
#define ALYS_HEAL_GRONN 36678

class ALYSONANTILLEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ALYSONANTILLEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ALYSONANTILLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ALYS_HEAL_GRONN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//KORAGG
#define KOR_FIREBALL 40877  

class KORAGGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KORAGGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    KORAGGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(KOR_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

//FEATHER_VORTEX
#define CYCLONE_STRIKE 42495 

class FEATHERVORTEXAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FEATHERVORTEXAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FEATHERVORTEXAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(CYCLONE_STRIKE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
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
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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

void SetupZulAman(ScriptMgr * mgr)
{
	//Nalorakk
	mgr->register_creature_script(NALORAKK, &NALORAKKAI::Create);

	//Akilzon
	mgr->register_creature_script(AKILZON, &AKILZONAI::Create);
	mgr->register_creature_script(SOARING_EAGLE, &SOARINGEAGLEAI::Create);

	//Halazzi
	mgr->register_creature_script(HALAZZI, &HALAZZIAI::Create);
	mgr->register_creature_script(CORRUPTED_LIGHTING_TOTEM, &CLTOTEMAI::Create);

	//Jan'alai
	mgr->register_creature_script(JANALAI, &JANALAIAI::Create);

	/*
	mgr->register_creature_script(HEX_LORD_MALACRASS, &HEXLORDMALACRASSAI::Create);
	mgr->register_creature_script(ZULJIN, &ZULJINAI::Create);
	//MOBS
	mgr->register_creature_script(GAZAKROTH, &GAZAKROTHAI::Create);
	mgr->register_creature_script(LORD_RAADAN, &LORDRAADANAI::Create);
	mgr->register_creature_script(SLITHER, &SLITHERAI::Create);
	mgr->register_creature_script(THURG, &THURGAI::Create);
	mgr->register_creature_script(DARKHEART, &DARKHEARTAI::Create);
	mgr->register_creature_script(FENSTALKER, &FENSTALKERAI::Create);
	mgr->register_creature_script(ALYSON_ANTILLE, &ALYSONANTILLEAI::Create);
	mgr->register_creature_script(KORAGG, &KORAGGAI::Create);
	mgr->register_creature_script(FEATHER_VORTEX, &FEATHERVORTEXAI::Create);*/
}
