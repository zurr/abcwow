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

/************************************************************************/
/* Raid_CoT_BattleOfMountHyjal.cpp Script								*/
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

/*  * Might of the Defiler - Smashed into the ground for 6000 fire damage, 
		and 3000 every second for 10 seconds.
    * Grip of the Legion - 250,000 damage over 5 mins. [833.33dps].
    * Finger of Death - 20,000 shadow damage on a single target. Archimonde 
		will begin casting this if nobody is in melee range.
    * Soul Charge - All damage taken increased by 50%. Non-dispellable.
    * Doomfire - Leaves a trail of fire on the ground, which does 2400 fire 
		damage per second (occasionally feared people run into these and die) */

// ArchimondeAI

#define CN_ARCHIMONDE 17968

#define AIR_BURST 32014 
#define GRIP_OF_THE_LEGION 31972
#define FINGER_OF_DEATH 31984 // 31984 or 32111 // should be casted only when no one in melee range
#define SOUL_CHARGE 32054
#define DOOMFIRE 31944 // 31945 -> 31943 -> 31944 // bugged as it's targetting only caster :O
#define HAND_OF_DEATCH 35354 //99,999 shadow damage....
// TO DO: Add rest of the sounds (first I must gather infos about to which spell or
// event there are connected.

class ARCHIMONDEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ARCHIMONDEAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    ARCHIMONDEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(AIR_BURST);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(GRIP_OF_THE_LEGION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(FINGER_OF_DEATH);
		spells[2].targettype = TARGET_ATTACKING; 
		spells[2].instant = false;
		spells[2].perctrigger = 6.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SOUL_CHARGE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = false;
		spells[3].perctrigger = 7.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DOOMFIRE);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 2000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your resistance is insignificant.");
		_unit->PlaySoundToSet(10987);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul will languish for eternity.");
				_unit->PlaySoundToSet(10993);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All creation will be devoured!");
				_unit->PlaySoundToSet(10994);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am the coming of the end!");
				_unit->PlaySoundToSet(10995);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No, it cannot be! Nooo!");
		_unit->PlaySoundToSet(10992);
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

// AnetheronAI


#define CN_ANETHERON 17808

#define CARRION_SWARM 31306 
#define VAMPIRIC_AURA 38196
#define INFERNO 31299
#define SLEEP 12098


class ANETHERONAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ANETHERONAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];


    ANETHERONAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(CARRION_SWARM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(VAMPIRIC_AURA);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(INFERNO);
		spells[2].targettype =  TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 3000;
		
		spells[3].info = dbcSpell.LookupEntry(SLEEP);
		spells[3].targettype =  TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 1.0f;
		spells[3].attackstoptimer = 3000;
		
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		hm=100;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are defenders of a doomed world. Flee here and perhaps you will prolong your pathetic lives!");
		_unit->PlaySoundToSet(10977);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your hopes are lost.");
				_unit->PlaySoundToSet(10981);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scream for me.");
				_unit->PlaySoundToSet(11038);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You look tired");
				_unit->PlaySoundToSet(10978);
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		hm=100;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The clock... is still...ticking!!!");
		_unit->PlaySoundToSet(10982);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 75 && hm == 100) 
        {
            Creature *cre = NULL;
            for(int i = 0; i == 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17818, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hit he, no time for a slow death");
		_unit->PlaySoundToSet(11039);
		hm=75;
		}
		else if(_unit->GetHealthPct() <= 50 && hm == 75)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 3; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17818, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hit he, no time for a slow death");
		_unit->PlaySoundToSet(11039);
		hm=50;
		}
		else if(_unit->GetHealthPct() <= 25 && hm == 50)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 3; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17818, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hit he, no time for a slow death");
		_unit->PlaySoundToSet(11039);
		hm=25;
		}
	    else
        {
		    float val = (float)RandomFloat(100.0f);
            SpellCast(val);
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
	int hm;
};

// AzgalorAI

#define CN_AZGALOR 17842

#define UNQUENCHABLE_FLAMES 31341 
#define HOWL_OF_AZGALOR 31344
#define DOOM 31347
#define AZGALOR_CLEAVE 31345
#define RAINF_OF_FIRE 31340 



class AZGALORAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AZGALORAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];
	


    AZGALORAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(RAINF_OF_FIRE);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 3000;

		spells[1].info = dbcSpell.LookupEntry(HOWL_OF_AZGALOR);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(DOOM);
		spells[2].targettype = TARGET_DESTINATION; 
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		
		spells[3].info = dbcSpell.LookupEntry(AZGALOR_CLEAVE);
		spells[3].targettype = TARGET_ATTACKING; 
		spells[3].instant = false;
		spells[3].perctrigger = 4.0f;
		spells[3].attackstoptimer = 1000;



    }
    
    void OnCombatStart(Unit* mTarget)
    {
		hm=100;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Reesh, hokta!");
				_unit->PlaySoundToSet(11001);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No one is going to save you!");
				_unit->PlaySoundToSet(11047);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't fight it");
				_unit->PlaySoundToSet(11048);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		hm=100;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your time is almost... up!");
		_unit->PlaySoundToSet(11002);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 75 && hm == 100) 
        {
            Creature *cre = NULL;
            for(int i = 0; i < 3; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17864, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=75;
		}
		else if(_unit->GetHealthPct() <= 50 && hm == 75)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 3; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17864, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=50;
		}
		else if(_unit->GetHealthPct() <= 25 && hm == 50)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 3; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17864, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(3000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=25;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
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
	int hm;
};

// kAZROGALAI

#define CN_KAZROGAL 17888

#define CLEAVE 31345
#define WAR_STOMP 31480
#define MARK_OF_KAZROGAL 31447



class  KAZROGALAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KAZROGALAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    KAZROGALAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 3000;

		spells[1].info = dbcSpell.LookupEntry(WAR_STOMP);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(MARK_OF_KAZROGAL);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = false;
		spells[2].perctrigger = 1.0f;
		spells[2].attackstoptimer = 3000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Cry for mercy! Your meaningless lives will soon be forfeit.");
		_unit->PlaySoundToSet(11015);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your death will be a painful one.");
				_unit->PlaySoundToSet(11016);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You... are marked");
				_unit->PlaySoundToSet(11052);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You... are nothing!");
				_unit->PlaySoundToSet(11053);
				break;
			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Miserable nuisance!");
				_unit->PlaySoundToSet(11054);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "hahahahaa aahaah");
		_unit->PlaySoundToSet(11018);
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

// Rage WinterchillAI

#define CN_RAGE_WINTERCHILL 17767

#define FROSTBOLT 41486 
#define DEATCH_AND_DECAY 39658
#define FROST_NOVA 36989
#define FROST_ARMOR 12544


class RAGEWINTERCHILLAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RAGEWINTERCHILLAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    RAGEWINTERCHILLAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DEATCH_AND_DECAY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 0.5f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(FROST_NOVA);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = false;
		spells[2].perctrigger = 3.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FROST_ARMOR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 2.0f;
		spells[3].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion's final conquest has begun! Once again the subjugation of this world is within our grasp. Let none survive!");
		_unit->PlaySoundToSet(11022);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%5;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Crumble and rot!");
				_unit->PlaySoundToSet(11023);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ashes to ashes, dust to dust");
				_unit->PlaySoundToSet(11055);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All life must perish!");
				_unit->PlaySoundToSet(11025);
				break;
			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your world is ours now!");
				_unit->PlaySoundToSet(11056);
				break;
			case 4:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory to the Legion!");
				_unit->PlaySoundToSet(11057);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have won this battle, but not... the...war");
		_unit->PlaySoundToSet(11026);
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

void SetupBattleOfMountHyjal(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_ARCHIMONDE, &ARCHIMONDEAI::Create);
	mgr->register_creature_script(CN_ANETHERON, &ANETHERONAI::Create);
	mgr->register_creature_script(CN_AZGALOR, &AZGALORAI::Create);
	mgr->register_creature_script(CN_KAZROGAL, &KAZROGALAI::Create);
	mgr->register_creature_script(CN_RAGE_WINTERCHILL, &RAGEWINTERCHILLAI::Create);
}