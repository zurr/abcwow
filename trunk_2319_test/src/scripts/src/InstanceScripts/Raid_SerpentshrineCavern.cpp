#include "StdAfx.h"
#include "Setup.h"
//------------------------------------
//    -= Hydross the Unstable=-
//------------------------------------
#define CN_HYDROSS_THE_UNSTABLE 21216
#define CN_TAINTED_SPAWN_OF_HYDROSS 22036
#define CN_PURE_SPAWN_OF_HYDROSS 22035

#define PURIFY_ELEMENTAL 36461//
#define MARK_OF_HYDROSS 38215
#define WATER_TOMB 38235//
#define MARK_OF_CORRUPTION 38219
#define VILE_SLUDGE 38246//

class HydrossAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HydrossAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    HydrossAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 5;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(PURIFY_ELEMENTAL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		spells[0].speech = "Casting PURIFY_ELEMENTAL";
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(MARK_OF_HYDROSS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec
		spells[1].speech = "Casting MARK_OF_HYDROSS";

		spells[2].info = dbcSpell.LookupEntry(WATER_TOMB);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].cooldown = 7;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec
		spells[2].speech = "Casting WATER_TOMB";

		spells[3].info = dbcSpell.LookupEntry(MARK_OF_CORRUPTION);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].cooldown = 15;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
		spells[3].speech = "Casting MARK_OF_CORRUPTION";

		spells[4].info = dbcSpell.LookupEntry(VILE_SLUDGE);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].cooldown = 15;
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 2000; // 2sec
		spells[4].speech = "Casting VILE_SLUDGE";
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I cannot allow you to interfere!");
		_unit->PlaySoundToSet(11289);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no choice.");
		_unit->PlaySoundToSet(11292);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am... released...");
		_unit->PlaySoundToSet(11293);
	}

	void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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

//------------------------------------
//    -= The Lurker Below=-
//------------------------------------
#define CN_THE_LURKER_BELOW 21217
#define CN_COILFANG_AMBUSHER 21865
#define CN_COILFANG_GUARDIAN 21873

#define WHIRL 37363
#define GEYSER 37478
#define SPOUT 37433
#define SUBMERGE 37433 // Didn't find the spell id
//#define WATER_BOLT 37138  

class LurkerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LurkerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    LurkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(WHIRL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(GEYSER);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SPOUT);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(SUBMERGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 120;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(Unit *mTarget)
    {
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

	}

	void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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




//------------------------------------
//    -= Leotheras the Blind =-
//------------------------------------
#define CN_LEOTHERAS_THE_BLIND 21215
#define CN_INNER_DEMON 21857 


#define WHIRLWINDLEO 40653
#define INSIDIOUS_WHISPER 37676 //Debuff that summons an Inner Demon from up to five raid members. Each Inner Demon can be attacked only by the person it spawned from. If you do not kill your Inner Demon before Leotheras gets back into humanoid form you will become Mind Controlled for 10 minutes and can't get out of it unless killed. Inner Demons take increased damage from arcane, nature, and holy spells.
#define CHAOS_BLAST 37675 //Chaos Blast is Leotheras' only form of attacking while in demon form. It is a single target nuke that deals AoE damage around the target. Chaos Blast also leaves a stacking debuff that increases fire damage taken.
#define LEO_ENRAGE 41447 //13048



class LeotherasAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LeotherasAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    LeotherasAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			// --- Initialization ---
			for(int i=spmin;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
		
        spells[0].info = dbcSpell.LookupEntry(WHIRLWINDLEO);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(INSIDIOUS_WHISPER);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(CHAOS_BLAST);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 1;
		spmin = 0;
		demon = 0;
		human = 0;
		sumh = 0;
		sumd = 0;
		model = 0;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally my banishment ends!");
		_unit->PlaySoundToSet(11312);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
//Transition to Demon phase 
//"Be gone trifling elf. I'm in control now." 

//Insidious Whisper (inner demons) 
//"We all have our demons..." 
	void OnCombatStop(Unit *mTarget)
    {
		nrspells = 1;
		demon = 0;
		human = 0;
		sumh = 0;
		sumd = 0;
		model = 0;
		spmin = 0;
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
      	_unit->GetAIInterface()->SetAIState(STATE_IDLE);
     		RemoveAIUpdateEvent();
    }
//	Killing a Player (demon form) 
//"I have no equal." - soundid(11306)
//"Perish, mortal."  - soundid(11307)
//"Yes, YES! Ahahah!"  - sound id(11308)
//Killing a Player (blood elf form) 
//"Kill! KILL!"  -soundif (11314)
//"That's right! Yes!"  - soundid (11315)
//"Who's the master now?"  - soundid (11316)

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0 && !model)	
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Kill! KILL!");
				_unit->PlaySoundToSet(11314);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That's right! Yes!");
				_unit->PlaySoundToSet(11315);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who's the master now?");
				_unit->PlaySoundToSet(11316);
				break;
			}
		}
		if (_unit->GetHealthPct() > 0 && model)	
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no equal.");
				_unit->PlaySoundToSet(11306);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perish, mortal.");
				_unit->PlaySoundToSet(11307);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, YES! Ahahah!");
				_unit->PlaySoundToSet(11308);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		_unit->PlaySoundToSet(11317);
		RemoveAIUpdateEvent();
    }

//15% Leotheras/Demon split 
//"No! NO! What have you done?! I am the master, do you hear me? I... aaghh... Can't... contain him..." - Leotheras 
//"At last I am liberated. It has been too long since I have tasted true freedom!" - Shadow of Leotheras (demon) 

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 20 && enrage != 1)
		{
		enrage = 1;
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(LEO_ENRAGE), true);
		}
		if (_unit->GetHealthPct() <= 90 && sumh == 0)
		{
			sumh++;
			Creature *cre = NULL;
           		cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(15516, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
			if (_unit->GetHealthPct() <= 60 && sumh == 1)
			{
			sumh++;
			Creature *cre = NULL;
            	cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(15516, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
			if (_unit->GetHealthPct() <= 30 && sumh == 2)
		{
			sumh++;
			Creature *cre = NULL;
            	cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(15516, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
		if((_unit->GetHealthPct() <= 85 && human == 0) || (_unit->GetHealthPct() <= 55 && human == 1) || (_unit->GetHealthPct() <= 25 && human == 2))
		{
			human++;
			nrspells = 3;
			spmin = 1;
			model = 1;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone trifling elf. I'm in control now");
			_unit->PlaySoundToSet(11304);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 20125); //model to demon
		}
		if((_unit->GetHealthPct() <= 80 && sumd == 0) || (_unit->GetHealthPct() <= 75 && sumd == 1) ||(_unit->GetHealthPct() <= 71 && sumd == 2))
		{
			sumd++;
			Creature *cre = NULL;
            	cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21857, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
			if((_unit->GetHealthPct() <= 50 && sumd == 3) || (_unit->GetHealthPct() <= 45 && sumd == 4) ||(_unit->GetHealthPct() <= 41 && sumd == 5))
		{
			sumd++;
			Creature *cre = NULL;
            	cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21857, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
		if((_unit->GetHealthPct() <= 20 && sumd == 6) || (_unit->GetHealthPct() <= 15 && sumd == 7) ||(_unit->GetHealthPct() <= 11 && sumd == 8))
		{
			sumd++;
			Creature *cre = NULL;
            	cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21857, 
			_unit->GetPositionX(), _unit->GetPositionY(),
            	_unit->GetPositionZ(), _unit->GetOrientation(),
            	true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
		if((_unit->GetHealthPct() <= 70 && demon == 0) || (_unit->GetHealthPct() <= 40 && demon == 1))
		{
			demon++;
			nrspells = 1;
			spmin = 0;
			model = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no equal");
			_unit->PlaySoundToSet(11306);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 20514); //model to human
		}
		if(_unit->GetHealthPct() <= 15 && demon == 2)
		{
			human++;
			demon++;
			nrspells = 3;
			spmin = 0;
			model = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No! NO! What have you done?! I am the master, do you hear me? I... aaghh... Can't... contain him...");
			_unit->PlaySoundToSet(11313);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 20514); //model to human
			Creature *cre = NULL;
        		cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21875, 
			_unit->GetPositionX(), _unit->GetPositionY(),
        		_unit->GetPositionZ(), _unit->GetOrientation(),
        		true, false, 0, 0); 
			cre->GetAIInterface()->SetAllowedToEnterCombat(true);
			cre->GetAIInterface()->setOutOfCombatRange(3000);
		}
		float val = (float)sRand.rand(100.0f);
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
	int demon;
	int human;
	int sumd;
	int sumh;
	bool enrage;
	int spmin;
	bool model;
};


//------------------------------------
//    -= Fathom-Lord Karathress =-
//------------------------------------
#define CN_FANTHOM_LORD_KARATHRESS 21214

#define CATACLYSMIC_BOLT 38441

class KarathressAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KarathressAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    KarathressAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(CATACLYSMIC_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Guards, attention! We have visitors ...");
		_unit->PlaySoundToSet(11277);
    }
/*
Killing Caribdis 

"More knowledge, more power!" 
Killing Sharkkis 

"I am more powerful than ever!" 
Killing Tidalvess 

"Go on, kill them! I'll be the better for it!" 
Misc 

"Alana be'lendor!" 
*/
	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Her ... excellency ... awaits!");
		_unit->PlaySoundToSet(11285);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am rid of you.");
		_unit->PlaySoundToSet(11284);
	}

	void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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


//------------------------------------
//    -= Morogrim Tidewalker =-
//------------------------------------
#define CN_MOROGRIM_TIDEWALKER 21213

#define TIDAL_WAVE 37730
#define WATERY_GRAVE 38049
#define EARTHQUAKE 37764
#define SUMMON_WATER_GLOBULE 37854

class MorogrimAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MorogrimAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MorogrimAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(TIDAL_WAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(WATERY_GRAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;

		spells[2].info = dbcSpell.LookupEntry(EARTHQUAKE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;

		spells[3].info = dbcSpell.LookupEntry(SUMMON_WATER_GLOBULE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 10;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000; // 1sec
		m_spellcheck[3] = true;

    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flood of the Deep, take you!");
		_unit->PlaySoundToSet(11321);
    }
/*
Aggro: "Flood of the Deep, take you!" 

Kills a Player: "Only the strong survive." 

Kills a Player: "Struggling only makes it worse." 

Kills a Player: "It is done!" 

Summoning Murlocs: "By the Tides, kill them at once!" 

Summoning Murlocs: "Destroy them my subjects!" 

Globules: "It will soon be over." 

Globules: "There is nowhere to hide!" 

Death: "Great... currents of... Ageon." 

*/
	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Great... currents of... Ageon.");
		_unit->PlaySoundToSet(11329);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Only the strong survive.");
				_unit->PlaySoundToSet(11328);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Struggling only makes it worse.");
				_unit->PlaySoundToSet(11327);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is done!");
				_unit->PlaySoundToSet(11326);
				break;
			}
		}

	}

	void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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

//------------------------------------
//    -= Lady Vashj =-
//------------------------------------
/*Intro:
Water is life. It has become a rare commodity here in Outland. A commodity that we alone shall control. We are the Highborne, and the time has come at last for us to retake our rightful place in the world!
Aggro:
I'll split you from stem to stern!
Victory to Lord Illidan!
I spit on you, surface filth!
Death to the outsiders!
Phase 1:
I did not wish to lower myself by engaging your kind, but you leave me little choice!
Phase 2:
The time is now! Leave none standing!
Phase 3:
You may want to take cover.
Archery:
Straight to the heart!
Seek your mark!
Slaying:
Your time ends now!
You have failed!
Death:
Lord Illidan,I... I am... sorry.
*/

#define CN_LADY_VASHJ 21212
#define CN_ENCHANTED_ELEMENTAL 21958
#define CN_COILFANG_STRIDER 22056
#define CN_TAINTED_ELEMENTAL 22009
#define CN_COILFANG_ELITE 22055

#define SHOOT 37770
#define ENTANGLE 38316
#define SPAWN_ELEMENTAL 37053 
#define MULTI_SHOT 29576
#define STATIC_CHARGE 38280
#define FORKED_LIGHTNING 40088
#define VASHJ_SHIELD 33405

// Toxic Spore Bat AI
#define CN_TOXIC_SPORE_BAT 22140
#define TOXIC_SPORES 38575
#define WALK 0
#define RUN 256
#define FLY 768

class VashjAI : public CreatureAIScript
	{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(VashjAI);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];
		
		VashjAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 6;
			
			// --- Initialization ---
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------
			
			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
			
			spells[0].info = dbcSpell.LookupEntry(SHOOT);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000; // 1sec
			m_spellcheck[0] = true;
			
			spells[1].info = dbcSpell.LookupEntry(ENTANGLE);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 2000; // 2sec
			
			spells[2].info = dbcSpell.LookupEntry(SPAWN_ELEMENTAL);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].cooldown = 7;
			spells[2].instant = true;
			spells[2].perctrigger = 5.0f;
			spells[2].attackstoptimer = 2000; // 2sec
			
			spells[3].info = dbcSpell.LookupEntry(MULTI_SHOT);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = true;
			spells[3].perctrigger = 10.0f;
			spells[3].attackstoptimer = 1000; // 1sec
			spells[3].speech = "Seek your mark!";
			
			spells[4].info = dbcSpell.LookupEntry(STATIC_CHARGE);
			spells[4].targettype = TARGET_VARIOUS;
			spells[4].instant = true;
			spells[4].cooldown = 15;
			spells[4].perctrigger = 10.0f;
			spells[4].attackstoptimer = 2000; // 2sec
			
			spells[5].info = dbcSpell.LookupEntry(FORKED_LIGHTNING);
			spells[5].targettype = TARGET_VARIOUS;
			spells[5].cooldown = 7;
			spells[5].instant = true;
			spells[5].perctrigger = 5.0f;
			spells[5].attackstoptimer = 2000; // 2sec
		}
		
		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory to Lord Illidan!");
			_unit->PlaySoundToSet(11533);
		}
		
		void OnCombatStop(Unit *mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}
		
		void OnDied(Unit * mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan, I... I am... sorry.");
			_unit->PlaySoundToSet(11544);
			RemoveAIUpdateEvent();
		}
		
		void OnTargetDied(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your time ends now!");
			_unit->PlaySoundToSet(11541);
		}
		
		void AIUpdate()
		{

			if(_unit->GetHealthPct() <= 80)
			{
				m_phase = 2;
			}
			if(_unit->GetHealthPct() <= 50)
			{
				m_phase = 3;
			}
			
			switch(m_phase)
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
				default:
					m_phase = 1;
					break;
			}
		}
		
		void PhaseOne()
		{
			phaseSetup = 2;
			float val = (float)sRand.rand(100.0f);
			SpellCast(val);
		}
		void PhaseTwo()
		{
			if(phaseSetup == 2)
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(VASHJ_SHIELD), true); // Cast her shield
				_unit->GetAIInterface()->MoveTo(29.798161f, -923.358276f, 42.900517f, 0.0f); //Move to center
				_unit->GetAIInterface()->m_canMove = false;
				phaseSetup = 3;
			}
			timer_fork++;
			if(timer_fork > 6)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), dbcSpell.LookupEntry(FORKED_LIGHTNING), false);
				timer_fork = 0;
			}
			spawncreatures();
		}
		void PhaseThree()
		{
			if(phaseSetup == 3)
			{
				_unit->GetAIInterface()->m_canMove = true;
			}
			float val = (float)sRand.rand(100.0f);
			SpellCast(val);
			spawnSporeBats();
		}
		
		void spawncreatures()
		{
			timer_elemental++;
			timer_strider++;
			timer_naga++;
			timer_tainted_elemental++;
			
			if(timer_elemental > 10)
			{
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTED_ELEMENTAL, -24.096014f, -908.627014f, 41.097122f, 0.0f, false, false, 0, 0);
				cre->GetAIInterface()->MoveTo(24.103367f, -925.333496f, 41.097122f, 0.0f);
				cre->GetAIInterface()->setOutOfCombatRange(100000);
				timer_elemental = 0;
			}
			if(timer_strider > 60)
			{
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANG_STRIDER, -29.761278f, -980.252930f, 41.097122f, 0.0f, false, false, 0, 0);
				cre->GetAIInterface()->MoveTo(24.103367f, -925.333496f, 41.097122f, 0.0f);
				cre->GetAIInterface()->setOutOfCombatRange(100000);
				timer_strider=0;
			}
			if(timer_naga > 50)
			{
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANG_ELITE, 67.146439f, -884.181641f, 41.097122f, 0.0f, false, false, 0, 0);
				cre->GetAIInterface()->MoveTo(24.103367f, -925.333496f, 41.097122f, 0.0f);
				cre->GetAIInterface()->setOutOfCombatRange(100000);
				timer_naga=0;
			}
			if(timer_tainted_elemental > 60)
			{
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_ELEMENTAL, 14.451158f, -870.376831f, 41.097122f, 0.0f, false, false, 0, 0);
				cre->GetAIInterface()->MoveTo(24.103367f, -925.333496f, 41.097122f, 0.0f);
				cre->GetAIInterface()->setOutOfCombatRange(100000);
				timer_tainted_elemental=0;
			}
			
		}
		
		void spawnSporeBats()
		{
			timer_spore++;
			if(timer_spore > 10)
			{
				Creature *cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TOXIC_SPORE_BAT,  29.769f, -866.190f, 43, 0, false, false, 0, 0);
				cre->GetAIInterface()->setOutOfCombatRange(1);		
				timer_spore = 0;
			}

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
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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
		int m_phase;
		int timer_elemental;
		int timer_strider;
		int timer_naga;
		int timer_tainted_elemental;
		int timer_spore;
		int phaseSetup;
		int timer_fork;
	};
	

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords fly[] = 
{
	{ 29.769f, -866.190f, 43, 0 },
	{ 1.135f, -874.345f, 43, 0 },
	{ -19.719f, -894.950f, 43, 0 },
	{ -27.4222f, -923.572f, 43, 0 },
	{ -19.739f, -951.907f, 43, 0 },
    { 1.059f, -973.314f, 43, 0 },
	{ 30.071f, -980.424f, 43, 0 },
	{ 58.665f, -973.410f, 43, 0 },
	{ 79.353f, -952.011f, 43, 0 },
	{ 87.552f, -923.175f, 43, 0 },
	{ 79.068f, -894.570f, 43, 0 },
	{ 58.503f, -873.295f, 43, 0 }
};

class ToxicSporeBatAI : public CreatureAIScript
	{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ToxicSporeBatAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];
		
		ToxicSporeBatAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			/************************ Waypoint Place ************************/
			
			m_entry = pCreature->GetEntry();
			
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(11, 0, FLY));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(12, 0, FLY));
			
			/************************** Spells ****************************/
			
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
			
			spells[0].info = dbcSpell.LookupEntry(TOXIC_SPORES);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;
			
			/******************* Additional Settings *******************/
			
			Phase = 0;
			FlameQuills = false;
			Meteor = false;
			PositionChange=rand()%8+15;
			PhoenixSummon=rand()%6+17;
			_unit->GetAIInterface()->m_moveFly = true;
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		}
		
		void OnCombatStart(Unit* mTarget)
		{
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
			_unit->PlaySoundToSet(11243);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			CastTime();
			Phase = 1;
			FlameQuills = false;
			Meteor = false;
			PositionChange=rand()%8+15;	// 30-45sec /*** if attack time 1000 (%15+31) ***/
			PhoenixSummon=rand()%6+17;	// 34-44sec /*** if attack time 1000 (%11+34) ***/
			FlyWay=rand()%2;
			switch (FlyWay)
			{
				case 0:	// Clock like
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(6);
				}break;
					
				case 1:	// hmm... other?
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(9);
				}break;
			}
			//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));	// Taken from here to add more abilities to code
		}
		
		void CastTime()
		{
			for(int i=0;i<nrspells;i++)
				spells[i].casttime = spells[i].cooldown;
		}
		
		void OnCombatStop(Unit *mTarget)
		{
			Phase = 0;
			FlameQuills = false;
			Meteor = false;
			PhoenixSummon=rand()%6+17;
			PositionChange=rand()%8+15;
			CastTime();
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			//RemoveAIUpdateEvent();
		}
		
		void OnDied(Unit * mKiller)
		{
			Phase = 0;
			FlameQuills = false;
			Meteor = false;
			PositionChange=rand()%8+15;
			PhoenixSummon=rand()%6+17;
			CastTime();
			//RemoveAIUpdateEvent();
		}
		
		void AIUpdate()
		{
			if (FlameQuills == true)
			{
				QuillsCount++;
				if (QuillsCount == 9)
				{
					FlameQuills = false;
					switch (FlyWay)
					{
						case 0:	// Clock like
						{
							_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
							_unit->GetAIInterface()->setWaypointToMove(6);
						}break;
							
						case 1:	// hmm... other?
						{
							_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
							_unit->GetAIInterface()->setWaypointToMove(9);
						}break;
					}
				}
				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}
			
			if (Meteor == true)
			{
			}
			
			else 
			{
				switch (Phase)
				{
					case 0: return;
					case 1: 
					{
						PhaseOne();
					}break;
					case 2:
					{
						PhaseTwo();
					}break;
					default:
					{
						Phase = 0;
					};
				};
			}
			//float val = (float)sRand.rand(100.0f);
			//SpellCast(val);
		}
		
		void PhaseOne()
		{
			PositionChange--;
			PhoenixSummon--;
			
			if (_unit->GetHealthPct() == 0)
			{
				Phase = 2;
				_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
			}
			
			if (!PhoenixSummon--)
			{
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				PhoenixSummon=rand()%6+17;
			}
			
			if (!PositionChange)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(NextWP);
				PositionChange=rand()%8+17;	// added 4 sec fit time + time needed to move to next pos.
			}
			
			else
			{
				uint32 val = sRand.randInt(100);
				
				if (val > 0 && val < 5)	// Flame Quills wp here!
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(10);
				}
			}
		}
		
		void PhaseTwo()
		{
			
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
		
		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if (Phase == 1)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(6);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
				_unit->PlaySoundToSet(11243);
			}
			
			switch(iWaypointId)
			{
				case 1:	// First fly point
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(2);
				}break;
					
				case 2:
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(3);
				}break;
					
				case 3:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(4);
			    }break;
					
				case 4:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(5);
			    }break;
					
				case 5:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(1);	// Last fly point (flyback to point 1 - reset)
			    }break;
					
				case 6:
			    {
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
						case 0:
						{
							NextWP = 7;
						}break;
							
						case 1:
						{	
							NextWP = 9;
						}break;
					}
				}break;
					
				case 7:
			    {
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
						case 0:
						{
							NextWP = 8;
						}break;
							
						case 1:
						{	
							NextWP = 6;
						}break;
					}
			    }break;
					
				case 8:
				{
					_unit->GetAIInterface()->m_canMove = false;
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					switch (FlyWay)
					{
						case 0:
						{
							NextWP = 9;
						}break;
							
						case 1:
						{	
							NextWP = 7;
						}break;
					}
				}break;
					
				case 9:
				{
					_unit->GetAIInterface()->m_canMove = false;
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					switch (FlyWay)
					{
						case 0:
						{
							NextWP = 6;
						}break;
							
						case 1:
						{	
							NextWP = 8;
						}break;
					}
				}break;
					
				case 10:
				{
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					if (Phase == 1)
					{
						FlameQuills = true;
						QuillsCount = 0;
					}
					
					if (Phase == 2)
					{
						Meteor = true;
					}
					
				}break;
			}
		}
		
		inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
		{
			WayPoint * wp = _unit->CreateWaypointStruct();
			wp->id = id;
			wp->x = fly[id].x;
			wp->y = fly[id].y;
			wp->z = fly[id].z;
			wp->o = fly[id].o;
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
		
	protected:
		
		bool FlameQuills;
		uint32 QuillsCount;
		bool Meteor;
		int PositionChange;
		int PhoenixSummon;
		uint32 NextWP;
		uint32 m_entry;
		uint32 FlyWay;
		uint32 Phase;
		int nrspells;
	};




//Trash mobs.
	
// COILFANG AMBUSHER AI

#define COILFANG_AMBUSHER 21865

#define CA_MULTI_SHOT 27021 //


class COILFANGAMBUSHERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGAMBUSHERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	

    COILFANGAMBUSHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CA_MULTI_SHOT); //+
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 4500;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
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
		float val = (float)sRand.rand(100.0f);
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


// COILFANG FATHOM WITCH AI


#define COILFANG_FATHOM_WITCH 21299

#define SHADOW_BOLT 27209 //
#define WHIRLWIND_KNOCKBACK 34109 //


class COILFANGFATHOMWITCHAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGFATHOMWITCHAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    COILFANGFATHOMWITCHAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT); //+
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(WHIRLWIND_KNOCKBACK); //+
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
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
		float val = (float)sRand.rand(100.0f);
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

// COILFANG GUARDIAN AI

#define COILFANG_GUARDIAN 21873  // 


#define CLEAVE 38260


class COILFANGGUARDIANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGGUARDIANAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGGUARDIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
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
		float val = (float)sRand.rand(100.0f);
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


// COILFANG PRIESTESS AI

#define COILFANG_PRIESTESS 21220

#define HOLY_NOVA 38589 
#define SMITE 25364 
#define SPIRIT_OF_REDEMPTION 35618



class COILFANGPRIESTESSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGPRIESTESSAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    COILFANGPRIESTESSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HOLY_NOVA); //+
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SMITE); //+
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SPIRIT_OF_REDEMPTION); //+
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
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
		float val = (float)sRand.rand(100.0f);
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


// UNDERBOG COLOSSUS 

#define UNDERBOG_COLOSSUS 21251

#define ACID_GEYSER 37959 
#define ATROPIC_BLOW 39015 
#define SPORE_QUAKE 38976
#define TOXIC_POOL 38718
#define FRENZY 37605
#define RAMPANT_INFECTION 39042



class UNDERBOGCOLOSSUSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGCOLOSSUSAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    UNDERBOGCOLOSSUSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ACID_GEYSER); //
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ATROPIC_BLOW);//+
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SPORE_QUAKE); //++
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;
		
		spells[3].info = dbcSpell.LookupEntry(TOXIC_POOL); //++
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 1.0f;
		spells[3].attackstoptimer = 1000;
		
		spells[4].info = dbcSpell.LookupEntry(FRENZY); //++
		spells[4].targettype = TARGET_ATTACKING; 
		spells[4].instant = true;
		spells[4].perctrigger = 1.0f;
		spells[4].attackstoptimer = 1000;
		
		spells[5].info = dbcSpell.LookupEntry(RAMPANT_INFECTION); //
		spells[5].targettype = TARGET_VARIOUS; 
		spells[5].instant = true;
		spells[5].perctrigger = 1.0f;
		spells[5].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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


// TIDEWALKER WARRIOR

#define TIDEWALKER_WARRIOR 21225

#define TW_CLEAVE 38260 
#define TW_BLOODTHIRST 30335 //INSTANT
#define TW_FRENZY 37605


class TIDEWALKERWARRIORAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TIDEWALKERWARRIORAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    TIDEWALKERWARRIORAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(TW_CLEAVE); //
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(TW_BLOODTHIRST);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(TW_FRENZY); //++
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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

//COILFANG SERPENTGUARD     

#define COILFANG_SERPENTGUARD 21298

#define CSERP_CLEAVE 38260 
#define CSERP_REFLECTION 36096 //INSTANT
#define CSERP_DEVOTION 38603 //INSTANT


class COILFANGSERPENTGUARDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSERPENTGUARDAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    COILFANGSERPENTGUARDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CSERP_CLEAVE); //
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CSERP_REFLECTION); //+
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 0.5f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CSERP_DEVOTION); //
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 1.0f;
		spells[2].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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


//COILFANG_SHATTERER   

#define COILFANG_SHATTERER 21301

#define CSHATT_ARMOR 38591 



class COILFANGSHATTERERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSHATTERERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGSHATTERERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CSHATT_ARMOR); //++
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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

//COILFANG STRIDER     

#define COILFANG_STRIDER 22056

#define CSTRID_SCREAM 10890 //instant



class COILFANGSTRIDERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSTRIDERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    COILFANGSTRIDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CSTRID_SCREAM); //
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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

//FATHOM GUARD CARIBDIS     

#define FATHOM_GUARD_CARIBDIS 21964

#define FGCARIB_WATER_BOLT 38335 //INSTANT
#define FGCARIB_WHIRLWIND 1680 //INSTANT
#define FGCARIB_TIDAL 38357 //INSTANT


class FATHOMGUARDCARIBDISAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FATHOMGUARDCARIBDISAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    FATHOMGUARDCARIBDISAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FGCARIB_WATER_BOLT); //++
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FGCARIB_WHIRLWIND);//++
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(FGCARIB_TIDAL); //
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 1000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)sRand.rand(100.0f);
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
				if (m_spellcheck[i])
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



void SetupSerpentshrineCavern(ScriptMgr * mgr)
{
	//Bosses
	mgr->register_creature_script(CN_HYDROSS_THE_UNSTABLE, &HydrossAI::Create);
	mgr->register_creature_script(CN_THE_LURKER_BELOW, &LurkerAI::Create);
	mgr->register_creature_script(CN_LEOTHERAS_THE_BLIND, &LeotherasAI::Create);
	mgr->register_creature_script(CN_FANTHOM_LORD_KARATHRESS, &KarathressAI::Create);
	mgr->register_creature_script(CN_MOROGRIM_TIDEWALKER, &MorogrimAI::Create);
	mgr->register_creature_script(CN_LADY_VASHJ, &VashjAI::Create);
	//boss related mobs
	mgr->register_creature_script(CN_TOXIC_SPORE_BAT, &ToxicSporeBatAI::Create);
	mgr->register_creature_script(COILFANG_STRIDER, &COILFANGSTRIDERAI::Create);
	//trash
	mgr->register_creature_script(COILFANG_AMBUSHER, &COILFANGAMBUSHERAI::Create);
    mgr->register_creature_script(COILFANG_FATHOM_WITCH, &COILFANGFATHOMWITCHAI::Create);
    mgr->register_creature_script(COILFANG_GUARDIAN, &COILFANGGUARDIANAI::Create);
	mgr->register_creature_script(COILFANG_PRIESTESS, &COILFANGPRIESTESSAI::Create);
	mgr->register_creature_script(UNDERBOG_COLOSSUS, &UNDERBOGCOLOSSUSAI::Create);
	mgr->register_creature_script(TIDEWALKER_WARRIOR, &TIDEWALKERWARRIORAI::Create);
	mgr->register_creature_script(COILFANG_SERPENTGUARD, &COILFANGSERPENTGUARDAI::Create);
	mgr->register_creature_script(COILFANG_SHATTERER, &COILFANGSHATTERERAI::Create);
	mgr->register_creature_script(FATHOM_GUARD_CARIBDIS, &FATHOMGUARDCARIBDISAI::Create);
	

}