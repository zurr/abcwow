#include "StdAfx.h"
#include "Setup.h"


//ZulAman.cpp 


//Nalorakk

#define NALORAKK 23576


#define BRUTAL_SWIPE 42384 //INSTANT
//TROLL FORM
#define MANGLE 44955 //INSTANT 
#define SURGE 42402 //INSTANT
//BEAR FORM
#define LACERATING_SLASH 42395 //INSTANT
#define REND_FLESH 42397 //INSTANT
#define DEAFENING_ROAR 42398 //INSTANT


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
		spells[0].perctrigger = 0.5f;
		spells[0].attackstoptimer = 1000;


		spells[1].info = dbcSpell.LookupEntry(MANGLE); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;
		//spells[1].soundid = 11315;
		//spells[1].speech = "That's right! Yes!";

		spells[2].info = dbcSpell.LookupEntry(SURGE); 
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = true;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;
		//spells[2].soundid = 11290;
		//spells[2].speech = "Better, much better.";

        spells[3].info = dbcSpell.LookupEntry(BRUTAL_SWIPE); 
		spells[3].targettype = TARGET_ATTACKING; 
		spells[3].instant = true;
		spells[3].perctrigger = 0.5f;
		spells[3].attackstoptimer = 1000;
		//spells[3].soundid = 11316;
		//spells[3].speech = "Who's the master now?";

		spells[4].info = dbcSpell.LookupEntry(LACERATING_SLASH);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 1000;
		//spells[1].soundid = 11315;
		//spells[1].speech = "That's right! Yes!";

		spells[5].info = dbcSpell.LookupEntry(REND_FLESH); 
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].perctrigger = 4.0f;
		spells[5].attackstoptimer = 1000;
		//spells[2].soundid = 11290;
		//spells[2].speech = "Better, much better.";

		spells[6].info = dbcSpell.LookupEntry(DEAFENING_ROAR); 
		spells[6].targettype = TARGET_VARIOUS;
		spells[6].instant = true;
		spells[6].perctrigger = 6.0f;
		spells[6].attackstoptimer = 1000;
		//spells[2].soundid = 11290;
		//spells[2].speech = "Better, much better.";
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 3;
		troll = 0;
		bear = 0;
		spmin = 0;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally my banishment ends!");
		//_unit->PlaySoundToSet(11312);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
/*		if (_unit->GetHealthPct() > 0)	
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perish, mortal.");
				_unit->PlaySoundToSet(11307);
				break;
			}
		}*/
    }

    void OnCombatStop(Unit *mTarget)
    {
		troll = 0;
		spmin = 0;
		nrspells = 3;
		bear = 0;
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631);  //change to Troll
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Better, much better.");
		//_unit->PlaySoundToSet(11290);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		//_unit->PlaySoundToSet(11317);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		/*if(_unit->GetHealthPct() <= 20 && enrage != 1)
		{
		enrage = 1;
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(LEO_ENRAGE), true);
		}*/
		if((_unit->GetHealthPct() <= 85 && troll == 0) || (_unit->GetHealthPct() <= 55 && troll == 1) || (_unit->GetHealthPct() <= 25 && troll == 2))
		{
		troll++;
		nrspells = 7;
		spmin = 3;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone trifling elf. I'm in control now");
		//_unit->PlaySoundToSet(11304);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 706); // to Bear // 5487,706
		}
		if((_unit->GetHealthPct() <= 70 && bear == 0) || (_unit->GetHealthPct() <= 40 && bear == 1))
		{
		bear++;
		nrspells = 3;
		spmin = 0;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no equal");
		//_unit->PlaySoundToSet(11306);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631); //to Troll
		}
		if(_unit->GetHealthPct() <= 15 && bear == 2)
		{
		troll++;
		bear++;
		nrspells = 4;
		spmin = 0;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No! NO! What have you done?! I am the master, do you hear me? I... aaghh... Can't... contain him...");
		//_unit->PlaySoundToSet(11313);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21631); // to troll
		/*Creature *cre = NULL;
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21875, 
		_unit->GetPositionX(), _unit->GetPositionY(),
        _unit->GetPositionZ(), _unit->GetOrientation(),
        true, false, 0, 0); 
		cre->GetAIInterface()->SetAllowedToEnterCombat(true);
		cre->GetAIInterface()->setOutOfCombatRange(3000);*/
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
	int bear;
	int troll;
	int spmin;
};

//Akil'zon <Eagle Avatar>

#define AKILZON 23574


#define STATIC_DISRUPTION 44008 //INSTANT
#define CALL_LIGHTING 43661 //INSTANT 
#define GUST_OF_WIND 43621 //INSTANT
#define ELECTRICAL_STORM 43648
#define SUMMON_EAGLE 44038 // DBC



class AKILZONAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AKILZONAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    AKILZONAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 4;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(STATIC_DISRUPTION); 
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		//spells[0].soundid = 11316;
		//spells[0].speech = "Who's the master now?";

		spells[1].info = dbcSpell.LookupEntry(CALL_LIGHTING); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;
		//spells[1].soundid = 11315;
		//spells[1].speech = "That's right! Yes!";

		spells[2].info = dbcSpell.LookupEntry(GUST_OF_WIND); //crash
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		//spells[2].soundid = 11290;
		//spells[2].speech = "Better, much better.";

        spells[3].info = dbcSpell.LookupEntry(ELECTRICAL_STORM);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 3.0f;
		spells[3].attackstoptimer = 1000;
		//spells[3].soundid = 11316;
		//spells[3].speech = "Who's the master now?";

		spells[4].info = dbcSpell.LookupEntry(SUMMON_EAGLE);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;
		//spells[1].soundid = 11315;
		//spells[1].speech = "That's right! Yes!";

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 4;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally my banishment ends!");
		//_unit->PlaySoundToSet(11312);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
/*		if (_unit->GetHealthPct() > 0)	
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perish, mortal.");
				_unit->PlaySoundToSet(11307);
				break;
			}
		}*/
    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 4;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		//_unit->PlaySoundToSet(11317);
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

//Summon Akil'zon 

/*#define SUMMON_EAGLE_AKILZON ?????   //SUMMONS  Akil'zon <Eagle Avatar>


#define EAGLE_SWOOP 44732 //INSTANT


class SUMMONEAGLEAKILZONAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SUMMONEAGLEAKILZONAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SUMMONEAGLEAKILZONAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(EAGLE_SWOOP);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
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
};*/

//Halazzi <Lynx Avatar>

#define HALAZZI 23577

//Phase 1
#define SABER_LASH 40810 //40816
//Phase 2
#define LIGHTING_TOTEM 99999 // ??
#define FLAME_SHOCK 43303 //INSTANT , VARIOUS
//Phase 3
//Halazzi now drops his totems more frequently as well as doing more damage. 




class HALAZZIAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HALAZZIAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HALAZZIAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			for(int i=spmin;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 0.5f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(LIGHTING_TOTEM);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;


		spells[2].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[2].targettype = TARGET_DESTINATION; //+ at
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 1;
		spmin = 0;
		phase =1;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally my banishment ends!");
		//_unit->PlaySoundToSet(11312);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {
		nrspells = 1;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		//_unit->PlaySoundToSet(11317);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 75 && phase == 1) 
		{
			spmin = 1;
			nrspells = 3;
			phase = 2;
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone trifling elf. I'm in control now");
			//_unit->PlaySoundToSet(11304);
		}
		if (_unit->GetHealthPct() <= 25 && phase == 2) 
		{
		spmin = 0;
		nrspells = 3;
		phase = 3;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone trifling elf. I'm in control now");
		//_unit->PlaySoundToSet(11304);
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

void SetupZulAman(ScriptMgr * mgr)
{
	mgr->register_creature_script(NALORAKK, &NALORAKKAI::Create);
	mgr->register_creature_script(AKILZON, &AKILZONAI::Create);
	mgr->register_creature_script(HALAZZI, &HALAZZIAI::Create);
	//MOBS
	//mgr->register_creature_script(SUMMON_EAGLE_AKILZON, &SUMMONEAGLEAKILZONAI::Create);
	
}