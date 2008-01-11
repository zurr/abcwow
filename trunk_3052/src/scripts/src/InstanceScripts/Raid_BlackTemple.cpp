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

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif
/************************************************************************/
/* Instance_BlackTemple.cpp Script		                                */
/************************************************************************/

//------------------------------------
//    -= High Warlord Naj'entus =-
//------------------------------------

#define CN_NAJENTUS 22887
#define CRASHING_WAVE 40100//Crashing Wave (2313-2687 Nature damage)
#define NEEDLE_SPINE 39835//Needle Spine (3188-4312 Damage, AoE of 2375-2625 Aura): Fires a needle spine at an enemy target.
#define NEEDLE_SPINE_EXPLOSION 39968
#define TIDAL_SHIELD 185584//39872//Tidal Shield : Impervious to normal attack and spells. Regenerating health. Will deal 8500 frost damage to the raid when broken.
#define NAJENTUS_SPINE_GO 3264//Campfire so it wont crash Real go is 185584 //Did not find it in any database (including drake's go's), tested with a chair, and it works

class NajentusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NajentusAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    NajentusAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

        spells[0].info = dbcSpell.LookupEntry(CRASHING_WAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(NEEDLE_SPINE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(TIDAL_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(NEEDLE_SPINE_EXPLOSION);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will die, in the name of Lady Vashj!");
		_unit->PlaySoundToSet(11450);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan will... crush you.");
		_unit->PlaySoundToSet(11459);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time for you to go!");
		_unit->PlaySoundToSet(11456);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							_unit->GetMapMgr()->GetInterface()->SpawnGameObject(NAJENTUS_SPINE_GO , _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.207343, true, 0, 0);
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
//          -= Supremus =-
//------------------------------------

#define CN_SUPREMUS 22898

//Abilities - Phase 1
#define MOLTEN_FLAME 40253 //Molten Flame: Blue-flamed fire sent along the ground in a straight line toward random players that deals 3325-3675 fire damage every 1 second while stood on. Ability used every 20 seconds.
#define HURTFUL_STRIKE 40126 //33813 //Hurtful Strike: A massive melee ability applied to the second highest on his aggro list. If there are no other melee targets in range, it will be performed on the main tank instead.


//Abilities - Phase 2
//Gaze: Supremus will target a random player and follow that target around for 10 seconds at 90% of normal movement speed. Switches targets after 10 seconds.
#define MOLTEN_PUNCH 40126 //Molten Punch: Once a targeted player is within a 40 yard range, Supremus will do a knockback ability for 5250 damage.
#define VOLCANIC_GAZER 42055 //Volcanic Geyser: Emoting "the ground begins to crack open", Supremus will summon small volcanoes that spit out blue flames for 4163-4837 fire damage in a 15 yard radius.

class SupremusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SupremusAI);
    SupremusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_MoltenFlame = m_HurtfulStrike = m_MoltenPunch = m_VolcanicGazer = true;

        infoMoltenFlame = dbcSpell.LookupEntry(MOLTEN_FLAME);
		infoHurtfulStrike = dbcSpell.LookupEntry(HURTFUL_STRIKE);
		infoMoltenPunch =  dbcSpell.LookupEntry(MOLTEN_PUNCH);
		infoVolcanicGazer =  dbcSpell.LookupEntry(VOLCANIC_GAZER);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bear witness to the agent of your demise!");	// used when he kills Warden Mellichar
		_unit->PlaySoundToSet(11123);
		timer = 0;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am merely one of... infinite multitudes.");
		_unit->PlaySoundToSet(11126);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your fate is written!");	// this one needs verification
				_unit->PlaySoundToSet(11124);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The chaos I have sown here is but a taste....");
				_unit->PlaySoundToSet(11125);
				break;
			}
		}
    }

    void AIUpdate()
    {	
		switch(m_phase)
        {
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
                m_phase = 1;
            };
        };
    }
	
	void PhaseOne()
	{
		timer++;

        uint32 val = RandomUInt(1000);

        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			if(m_MoltenFlame)
            {
				_unit->CastSpell(_unit, infoMoltenFlame, false);
                m_MoltenFlame = false;
                return;
            }

			else if(m_HurtfulStrike)
            {
                _unit->CastSpell(_unit, infoHurtfulStrike, false);
                m_HurtfulStrike = false;
                return;
            }

            if(val >= 0 && val <= 500)
            {
                _unit->setAttackTimer(6000, false);//6000
                m_MoltenFlame = true;
            }

            else if(val > 500 && val <= 1000)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_HurtfulStrike = true;
            }
        }
		
		if(timer >= 45)
        {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
			timer = 0;
            m_phase = 2;
		}
	}

	void PhaseTwo()
	{
		timer++;

		uint32 val = RandomUInt(1000);

        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			if(m_MoltenPunch)
            {
				_unit->CastSpell(_unit, infoMoltenPunch, false);
                m_MoltenPunch = false;
                return;
            }

			else if(m_VolcanicGazer)
            {
				//_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "The ground begins to crack open");
                _unit->CastSpell(_unit, infoVolcanicGazer, false);
                m_VolcanicGazer = false;
                return;
				
            }

            if(val >= 0 && val <= 500)
            {
                _unit->setAttackTimer(6000, false);//6000
                m_MoltenPunch = true;
            }

            else if(val > 500 && val <= 1000)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_VolcanicGazer = true;
            }
        }

		if(timer >= 45)
        {
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
			timer = 0;
            m_phase = 1;
			
		}
	}


protected:
	uint32 timer;
	uint32 m_phase;
	bool m_MoltenFlame, m_HurtfulStrike, m_MoltenPunch, m_VolcanicGazer;
    SpellEntry *infoMoltenFlame, *infoHurtfulStrike,*infoMoltenPunch, *infoVolcanicGazer;
};

//------------------------------------
//    -= Shade Of Akama =-
//------------------------------------
#define CN_SHADE_OF_AKAMA 22841
#define CN_ASHTONGUE_CHANNELER 23421
#define CN_ASHTONGUE_SORCERER 23215

#define CN_ASHTONGUE_DEFENDER 23216
#define DEBILITATING_STRIKE 41178

#define CN_ASHTONGUE_ELEMENTALIST 23523
#define RAIN_OF_FIRE 42023

#define CN_ASHTONGUE_ROGUE 23318
#define DEBILITATING_POISON 41978

#define CN_ASHTONGUE_SPIRITBINDER 23524
#define SPIRIT_MEND 42025
#define CHAIN_HEAL 42027
//------------------------------------
//    -= GURTOGG BLOODBOIL =-
//------------------------------------
#define CN_GURTOGG_BLOODBOIL 22948

//phase1
#define BLOODBOIL 42005
#define ACIDIC_WOUND 40481
#define DISORIENT 32752
#define KNOCKBACK 38576
//common 1 & 2
#define ARCING_SMASH 40599
#define FEL_ACID 40508
//phase2
#define FEL_RAGE 40508
#define ACID_GEYSER 40629



class GurtoggAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GurtoggAI);
	SP_AI_Spell spells[8];
	bool m_spellcheck[8];

    GurtoggAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 8;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
	
		spells[0].info = dbcSpell.LookupEntry(ARCING_SMASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(FEL_ACID);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		//phase1
		spells[2].info = dbcSpell.LookupEntry(BLOODBOIL);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(ACIDIC_WOUND);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec

		spells[4].info = dbcSpell.LookupEntry(DISORIENT);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 2000; // 2sec

		spells[5].info = dbcSpell.LookupEntry(KNOCKBACK);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 2000; // 2sec

		//phase2
		spells[6].info = dbcSpell.LookupEntry(FEL_RAGE);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].perctrigger = 5.0f;
		spells[6].attackstoptimer = 2000; // 2sec

		spells[7].info = dbcSpell.LookupEntry(ACID_GEYSER);
		spells[7].targettype = TARGET_SOURCE;
		spells[7].instant = true;
		spells[7].perctrigger = 10.0f;
		spells[7].attackstoptimer = 2000; // 2sec


    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Horde will crush you!");
		_unit->PlaySoundToSet(11432);
    }

	void OnCombatStop(Unit *mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the meat from your bones!");
		_unit->PlaySoundToSet(11437);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Arrghhh!!!!");
		_unit->PlaySoundToSet(11439);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to feast!");
		_unit->PlaySoundToSet(11433);
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
//    -= Reliquary of Souls =-
//------------------------------------
//------------------------------------
//    -= Essence of Suffering =-
//------------------------------------
#define CN_ESSENCEOFSUFFERING 23418

#define FIXATE 40414
#define ENRAGE_EOS 28798
#define SOUL_DRAIN 41303
#define AURA_OF_SUFFERING 41292

class EssenceOfSufferingAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfSufferingAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfSufferingAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(FIXATE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(ENRAGE_EOS);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SOUL_DRAIN);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_SUFFERING);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pain and suffering are all that await you.");
		_unit->PlaySoundToSet(11415);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I don't want to go back!");
		_unit->PlaySoundToSet(11420);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Look at what you made me do.");
		_unit->PlaySoundToSet(11417);
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
//    -= EssenceOfDesire =-
//------------------------------------
#define CN_ESSENCEOFDESIRE 23419

#define RUNE_SHIELD 41431
#define DEADEN 41410
#define SPIRIT_SHOCK 41426
#define AURA_OF_DESIRE 41350

class EssenceOfDesireAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfDesireAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfDesireAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(RUNE_SHIELD);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(DEADEN);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SPIRIT_SHOCK);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_DESIRE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can have anything you desire... for a price.");
		_unit->PlaySoundToSet(11408);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll be waiting.");
		_unit->PlaySoundToSet(11413);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fulfilment is at hand.");
		_unit->PlaySoundToSet(11409);
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
//    -= EssenceOfAnger =-
//------------------------------------
#define CN_ESSENCEOFANGER 23420

#define SEETHE 41520
#define SOUL_SCREAM 41545
#define SPITE 41377
#define AURA_OF_ANGER 41337

class EssenceOfAngerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfAngerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfAngerAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(SEETHE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(SOUL_SCREAM);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SPITE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_ANGER);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Beware - I live.");
		_unit->PlaySoundToSet(11399);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I won't be ignored.");
		_unit->PlaySoundToSet(11404);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enough! No more!");
		_unit->PlaySoundToSet(11402);
		_unit->PlaySoundToSet(11401);
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
//    -= Mother Shahraz =-
//------------------------------------
#define CN_MOTHER_SHAHRAZ 22947

#define SINFUL_BEAM 40827
#define SINISTER_BEAM 40859
#define VILE_BEAM 40860
#define WICKED_BEAM 40861
#define FATAL_ATTRACTION 40869
#define SABER_LASH 40810 //Should be 40816 but 40816 is not supported by core
#define PRISMATIC_SHIELD 38905


class ShahrazAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShahrazAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    ShahrazAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 7;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(SINFUL_BEAM);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(PRISMATIC_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(FATAL_ATTRACTION);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
		
		spells[4].info = dbcSpell.LookupEntry(WICKED_BEAM);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 2000; // 2sec
		
		spells[5].info = dbcSpell.LookupEntry(VILE_BEAM);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 2000; // 2sec
		
		spells[6].info = dbcSpell.LookupEntry(SINISTER_BEAM);
		spells[6].targettype = TARGET_VARIOUS;
		spells[6].instant = true;
		spells[6].perctrigger = 1.0f;
		spells[6].attackstoptimer = 2000; // 2sec
		
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So, business... or pleasure?");
		_unit->PlaySoundToSet(11504);
    }

	void OnCombatStop(Unit *mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You seem a little tense.");
		_unit->PlaySoundToSet(11505);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop toying with my emotions!");
		_unit->PlaySoundToSet(11510);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }
	
    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I wasn't finished.");
		_unit->PlaySoundToSet(11511);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Easy come, easy go.");
		_unit->PlaySoundToSet(11508);
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
//    -= Lady Malande =-
//------------------------------------
#define CN_LADY_MALANDE 22951

#define EMPOWERED_SMITE 41471
#define DIVINE_WRATH 41472
#define REFLECTIVE_SHIELD 41475
#define CIRCLE_OF_HEALING 41455


class MalandeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MalandeAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MalandeAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(EMPOWERED_SMITE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(DIVINE_WRATH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(REFLECTIVE_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(CIRCLE_OF_HEALING);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flee or die!");
		_unit->PlaySoundToSet(11482);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Destiny... awaits!");
		_unit->PlaySoundToSet(11485);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My work is done.");
		_unit->PlaySoundToSet(11483);
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
//    -= Gathios the Shatterer =-
//------------------------------------
#define CN_GATHIOS_THE_SHATTERER 22949

#define HAMMER_OF_JUSTICE 41468
#define SEAL_OF_COMMAND 41469
#define SEAL_OF_BLOOD 32221 // Doesn't work
#define CONSECRATION 41541
#define BLESSING_OF_SPELL_WARDING 41451
#define BLESSING_OF_PROTECTION 41450
#define CHROMATIC_RESISTANCE_AURA 41453
#define DEVOTION_AURA 41452

class GathiosAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GathiosAI);
	SP_AI_Spell spells[8];
	bool m_spellcheck[8];

    GathiosAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 8;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(SEAL_OF_COMMAND);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SEAL_OF_BLOOD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(CONSECRATION); 
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
		
		spells[4].info = dbcSpell.LookupEntry(BLESSING_OF_SPELL_WARDING);  
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 2000; // 2sec
		
		spells[5].info = dbcSpell.LookupEntry(BLESSING_OF_PROTECTION); 
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 2000; // 2sec
		
		spells[6].info = dbcSpell.LookupEntry(CHROMATIC_RESISTANCE_AURA);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].perctrigger = 10.0f;
		spells[6].attackstoptimer = 2000; // 2sec
		
		spells[7].info = dbcSpell.LookupEntry(DEVOTION_AURA); 
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].perctrigger = 10.0f;
		spells[7].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have better things to do!");
		_unit->PlaySoundToSet(11422);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan... I...");
		_unit->PlaySoundToSet(11425);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Selama amor'anore!");
		_unit->PlaySoundToSet(11423);
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
//    -= High Nethermancer Zerevor  =-
//------------------------------------
#define CN_HIGH_NETHERMANCER_ZEREVOR 22950

#define ARCANE_BOLT 41483
#define BLIZZARD 20680
#define FLAMESTRIKE 41379
#define DAMPEN_MAGIC 41378

class ZerevorAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ZerevorAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    ZerevorAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(ARCANE_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(BLIZZARD);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(FLAMESTRIKE);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(DAMPEN_MAGIC);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "CombatStart");
		_unit->PlaySoundToSet(11440);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "BossDied");
		_unit->PlaySoundToSet(11443);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "KilledPlayer");
		_unit->PlaySoundToSet(11441);
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
//    -=Veras Darkshadow  =-
//------------------------------------
#define CN_VERAS_DARKSHADOW 22952

#define DEADLY_POISON 41485
#define ENVENOM 41487 //Doesn't work
#define VANISH 41476 //Doesn't work

class VerasAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VerasAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    VerasAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(DEADLY_POISON);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(ENVENOM);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(VANISH);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 2000; // 2sec

    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You wish to test me?");
		_unit->PlaySoundToSet(11524);
    }

	void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You got lucky!");
		_unit->PlaySoundToSet(11527);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "A glorious kill!");
		_unit->PlaySoundToSet(11526);
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

//Teron GorefiendAI

#define CN_TERON_GOREFIEND 22871

#define DOOM_BLOSSOM 40188
#define CRUSHING_SHADOWS 40243
#define INCINERATE 40239
#define SHADOW_OF_DEATH 40251



class TerongorefiendAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TerongorefiendAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    TerongorefiendAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(DOOM_BLOSSOM); //summon visual effect 
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(CRUSHING_SHADOWS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 2.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(INCINERATE);
		spells[2].targettype = TARGET_ATTACKING; 
		spells[2].instant = true;
		spells[2].perctrigger = 2.0f;
		spells[2].attackstoptimer = 2000;

		spells[3].info = dbcSpell.LookupEntry(SHADOW_OF_DEATH);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = 0.1f;
		spells[3].attackstoptimer = 2000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		phase = 1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vengeance is mine!");
		_unit->PlaySoundToSet(11513);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "YOU WILL SHOW THE PROPER RESPECT!");
				_unit->PlaySoundToSet(11520);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have use for you!");
				_unit->PlaySoundToSet(11514);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I was the first, you know. For me, the wheel of death has spun many times. <laughs> So much time has passed. I have a lot of catching up to do...");
				_unit->PlaySoundToSet(11512);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		phase = 1;
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The wheel...spins...again....");
		_unit->PlaySoundToSet(11521);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 25 && phase == 1)
		{
		phase = 2;
		Creature *cre = NULL;
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23111, 
        _unit->GetPositionX(), _unit->GetPositionY(),
        _unit->GetPositionZ(), _unit->GetOrientation(), 
        true, false, 0, 0); 
		cre->GetAIInterface()->setOutOfCombatRange(30000);
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
	int phase;
};

//Shade of AkamaAI

#define CN_SHADE_OF_AKAMA 22841

#define SINFUL_BEAM0 00000



class ShadeofakamaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadeofakamaAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ShadeofakamaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(SINFUL_BEAM0);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 2000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		hm = 100;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not last much longer...");
				_unit->PlaySoundToSet(11385);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is time to free your soul from Illidan's grasp!");
				//_unit->PlaySoundToSet(11510);
				break;

			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		hm = 100;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		Creature *cre = NULL;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I don't want to go back!");
		_unit->PlaySoundToSet(11420);
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(22990, 
        643.741f, 305.852f,
        271.689f, 0.00628f, 
        true, false, 0, 0); 
		//cre->GetAIInterface()->setOutOfCombatRange(30000);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 85 && hm == 100) 
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23421, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=85;
		}
		else if(_unit->GetHealthPct() <= 70 && hm == 85)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23215, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=70;
		}
		else if(_unit->GetHealthPct() <= 55 && hm == 70)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23216, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=55;
		}
		else if(_unit->GetHealthPct() <= 40 && hm == 55)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 2; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23523, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=40;
		}
		else if(_unit->GetHealthPct() <= 25 && hm == 40)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23318, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=25;
		}
		else if(_unit->GetHealthPct() <= 10 && hm == 25)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 5; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23524, 
                    _unit->GetPositionX(), _unit->GetPositionY(),
                    _unit->GetPositionZ(), _unit->GetOrientation(),
                    true, false, 0, 0); 
				cre->GetAIInterface()->setOutOfCombatRange(30000);
			}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!");
		_unit->PlaySoundToSet(10999);
		hm=10;
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


//mobs

// Ashtongue Defender AI

#define CN_ASHTONGUE_DEFENDER 23216

#define DEBILITATING_STRIKE 41178 //


class AshtonguedefenderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguedefenderAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguedefenderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
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

// AshtongueElementalist AI

#define CN_ASHTONGUE_ELEMENTALIST 23523

#define RAIN_OF_FIRE 42023 //


class AshtongueelementalistAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueelementalistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueelementalistAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
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

// AshtongueRogue AI

#define CN_ASHTONGUE_ROGUE 23318

#define DEBILITATING_POISON 41978 //


class AshtonguerogueAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguerogueAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguerogueAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_POISON);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
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

// AshtongueRogue AI

#define CN_ASHTONGUE_SPIRITBINDER 23524

#define SPIRIT_MEND 42025 //
#define AS_CHAIN_HEAL 42025 //


class AshtonguespiritbinderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguespiritbinderAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AshtonguespiritbinderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SPIRIT_MEND);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(AS_CHAIN_HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;


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

// Shadowy ConstructsAI

#define CN_SHADOWY_CONSTRUCTS 23111

#define SPIRIT_STRIKE 40325
#define SPIRIT_LANCE 40157
#define SPIRIT_VOLLEY 40314
#define SPIRIT_CHAINS 40157
#define SPIRIT_SHIELD 40314



class  ShadowyconstructsAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadowyconstructsAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    ShadowyconstructsAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(SPIRIT_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(SPIRIT_LANCE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(SPIRIT_VOLLEY);
		spells[2].targettype = TARGET_VARIOUS; 
		spells[2].instant = false;
		spells[2].perctrigger = 1.0f;
		spells[2].attackstoptimer = 2000;

		spells[3].info = dbcSpell.LookupEntry(SPIRIT_CHAINS);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 1.0f;
		spells[3].attackstoptimer = 2000;

		spells[4].info = dbcSpell.LookupEntry(SPIRIT_SHIELD);
		spells[4].targettype = TARGET_SELF; 
		spells[4].instant = false;
		spells[4].perctrigger = 1.0f;
		spells[4].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I was the first, you know. For me, the wheel of death has spun many times. <laughs> So much time has passed. I have a lot of catching up to do...");
		_unit->PlaySoundToSet(11512);
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
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "hahahahaa aahaah");
		//_unit->PlaySoundToSet(11018);
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


// --- Instance_BlackTemple.cpp Script --- //
// ---     Script made by M4ksiu       --- //

// ---    Illidan Encounter Event      --- //

/* Global Variables */

// Additional stuff
uint32 m_phase[1000000];	// dunno if it's enough (uint32 InstanceID -> 4,294,967,295)
uint32 WayStart[1000000];
uint32 FoADeaths[1000000];
uint32 MaievDialog[1000000];

// Events
uint32 BladesEvent[1000000];
uint32 DeathEvent[1000000];

#define WALK 0
#define RUN 256
#define FLY 768

// Global definitions - comment to disable
#define USE_SHADOW_PRISON	// aggroes a lot of mobs/NPCs if they are not friendly to Illidan
#define HACKS_ENABLED

/* Effect and Trigger AIs */

/* Door Event Trigger AI - Creature AI Class */

#define CN_DOOR_EVENT_TRIGGER 30001

class DoorEventTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DoorEventTriggerAI);

    DoorEventTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_moveFly = true;
    }
};

/* Demon Fire AI - Creature AI Class */

#define CN_DEMON_FIRE 23069

#define DEMON_FIRE 40029

class DemonFireAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DemonFireAI);

    DemonFireAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(DEMON_FIRE), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(60000);
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
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Eye Beam Trigger AI - Creature AI Class */

#define CN_EYE_BEAM_TRIGGER 30000

#define EYE_BLAST 40017	//spawning doesn't work so we will use little hack

class EyeBeamTriggerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EyeBeamTriggerAI);

    EyeBeamTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_moveRun = true;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(1000);

		WallCreating = false;
		DespawnTimer = 12;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
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
		if (WallCreating == false)
		{
			if (_unit->GetPositionX() == 642.240601f && _unit->GetPositionY() == 297.297180f && _unit->GetPositionZ() == 353.423401f)
				_unit->GetAIInterface()->MoveTo(707.019043f, 270.441772f, 352.996063f, 5.953270f);
			if (_unit->GetPositionX() == 641.197449f && _unit->GetPositionY() == 314.330963f && _unit->GetPositionZ() == 353.300262f)
				_unit->GetAIInterface()->MoveTo(706.592407f, 343.425568f, 352.996124f, 3.659908f);
			if (_unit->GetPositionX() == 657.239807f && _unit->GetPositionY() == 256.925568f && _unit->GetPositionZ() == 352.996094f)
				_unit->GetAIInterface()->MoveTo(706.593262f, 310.011475f, 353.693848f, 2.506154f);
			if (_unit->GetPositionX() == 657.913330f && _unit->GetPositionY() == 353.562775f && _unit->GetPositionZ() == 352.996185f)
				_unit->GetAIInterface()->MoveTo(706.751343f, 298.312683f, 353.653809f, 3.436852f);
			if (_unit->GetPositionX() == 707.019043f && _unit->GetPositionY() == 270.441772f && _unit->GetPositionZ() == 352.996063f)
				_unit->GetAIInterface()->MoveTo(642.240601f, 297.297180f, 353.423401f, 5.953270f);
			if (_unit->GetPositionX() == 706.592407f && _unit->GetPositionY() == 343.425568f && _unit->GetPositionZ() == 352.996124f)
				_unit->GetAIInterface()->MoveTo(641.197449f, 314.330963f, 353.300262f, 3.659908f);
			if (_unit->GetPositionX() == 706.593262f && _unit->GetPositionY() == 310.011475f && _unit->GetPositionZ() == 353.693848f)
				_unit->GetAIInterface()->MoveTo(657.239807f, 256.925568f, 352.996094f, 2.506154f);
			if (_unit->GetPositionX() == 706.751343f && _unit->GetPositionY() == 298.312683f && _unit->GetPositionZ() == 353.653809f)
				_unit->GetAIInterface()->MoveTo(657.913330f, 353.562775f, 352.996185f, 3.436852f);

			WallCreating = true;
		}

		_unit->CastSpell(_unit, dbcSpell.LookupEntry(EYE_BLAST), true);

#ifdef HACKS_ENABLED
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DEMON_FIRE, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
#endif
		DespawnTimer--;

		if (!DespawnTimer)
			_unit->Despawn(0,0);
    }

protected:

	bool WallCreating;
	uint32 DespawnTimer;
	int nrspells;
};

/* Flame Crash Effect AI - Creature AI Class */

#define CN_FLAME_CRASH 23336

#define FLAME_CRASH_EFFECT 40836

class FlameCrashAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlameCrashAI);

    FlameCrashAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAME_CRASH_EFFECT), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(60000);
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
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Blaze Effect AI - Creature AI Class */

#define CN_BLAZE 23259

#define BLAZE_EFFECT 40610

class BlazeAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BlazeAI);

    BlazeAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(BLAZE_EFFECT), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(60000);
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
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Misc Creature AIs */

/* Flame of Azzinoth AI - Creature AI Class */

#define CN_FLAME_OF_AZZINOTH 22997	

#define BLAZE 40637		// this one just summons mob that will trigger that fire cloud - doesn't spawn mob
#define FLAME_BLAST	40631
#define FOA_ENRAGE 40683	// 40683 or 40743
#define CHAOS_CHARGE 40497 // CHAOS_CHARGE 40497 CHARGE 40602

class FlameOfAzzinothAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FlameOfAzzinothAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    FlameOfAzzinothAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(BLAZE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_BLAST);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 13.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CHAOS_CHARGE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;

		spells[3].info = dbcSpell.LookupEntry(FOA_ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;

		_unit->m_noRespawn = true;

		Enrage = false;
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

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
	   FoADeaths[_unit->GetInstanceID()]--;
    }

    void AIUpdate()
	{
		if (Enrage == true)
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			Enrage = false;
			return;
		}

		if (_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();

			if (_unit->GetDistance2dSq(target) >= 225.0f && _unit->GetDistance2dSq(target) <= 1225 && rand()%5 == 0)	// I must test this value (15-35yards for now)
			{
				_unit->CastSpell(target, spells[2].info, spells[2].instant);
				Enrage = true;
				return;
			}
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

#ifdef HACKS_ENABLED
					if (i == 0)
						_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLAZE, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
#endif
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:

	bool Enrage;
	int nrspells;
};

/* Akama AI - Creature Gossip Class */
 
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, plr); \
    Menu->SendTo(plr);
 
class SCRIPT_DECL AkamaGossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;

		if (WayStart[pObject->GetInstanceID()] != 2 && WayStart[pObject->GetInstanceID()] != 3) return;

		else
		{
			switch (WayStart[pObject->GetInstanceID()])
			{
				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229901, plr);
					Menu->AddItem( 0, "I'm ready, Akama.", 1);
					break;
				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229902, plr);
					Menu->AddItem( 0, "We're ready to face Illidan.", 2);
					break;
			}
	 
			if(AutoSend)
				Menu->SendTo(plr);
		}
    }
 
	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;
 
			case 1:
			{
				pObject->SetUInt32Value(UNIT_NPC_FLAGS, 0);

				WayStart[pObject->GetInstanceID()] = 0;
			}break;

			case 2:
			{
				pObject->SetUInt32Value(UNIT_NPC_FLAGS, 0);

				WayStart[pObject->GetInstanceID()] = 4;
			}break;
		}
	}
 
    void Destroy()
    {
        delete this;
    }
};

/* Akama AI - Creature AI Class */

#define CN_AKAMA 22990

#define HEALING_POTION 40535
#define BLESSING_OF_KINGS 20217
#define AKAMA_DESPAWN 41242

// Door Event Spells
#define AKAMA_DOOR_FAIL 41271
#define AKAMA_DOOR_OPEN 41268
#define DEATHSWORN_DOOR_OPEN 41269
#define GATE_FAILURE 10390

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords ToIllidan[] = 
{
	{  },
	{ 660.248596f, 330.695679f, 271.688110f, 1.243284f },
	{ 671.172241f, 353.273193f, 271.689453f, 1.022600f },
	{ 694.227783f, 379.461365f, 271.792145f, 6.232135f },
	{ 724.159973f, 373.263275f, 282.614349f, 5.324218f },
	{ 747.034973f, 335.668274f, 307.047150f, 4.972365f },
	{ 755.477234f, 304.194580f, 312.167328f, 6.276120f },
	{ 771.809753f, 303.744873f, 313.563507f, 6.265894f },
	{ 778.550232f, 304.515198f, 318.818542f, 0.002354f },
	{ 789.564697f, 304.493652f, 319.759583f, 6.248631f },
	{ 799.598389f, 295.776642f, 319.760040f, 4.687257f },
	{ 799.054016f, 288.826660f, 320.030334f, 4.563174f },
	{ 794.595459f, 262.302856f, 341.463715f, 4.500343f },
	{ 794.396973f, 256.420471f, 341.463715f, 4.557680f },
	{ 783.355957f, 250.460892f, 341.463776f, 3.746361f },
	{ 764.988098f, 238.561462f, 353.646484f, 3.324606f },
	{ 749.337463f, 236.288681f, 352.997223f, 1.633631f },
	{ 751.941528f, 304.626221f, 352.996124f, 3.128243f },
	{ 747.236511f, 304.922913f, 352.996155f, 6.278462f },
	{ 747.834106f, 362.513977f, 352.996155f, 1.604554f }
};

class AkamaAI : public CreatureAIScript
{
public:
	friend class AkamaGossip;

    ADD_CREATURE_FACTORY_FUNCTION(AkamaAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    AkamaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		for (int i = 1; i < 20; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, RUN));
		}

		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(BLESSING_OF_KINGS);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 600;	// 10 min
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEALING_POTION);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(AKAMA_DESPAWN);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->m_moveRun = true;
		RegisterAIUpdateEvent(1000);

		if (!DeathEvent[_unit->GetInstanceID()])
			_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
		else _unit->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		MaievDialog[_unit->GetInstanceID()] = 0;
		WayStart[_unit->GetInstanceID()] = 2;
		m_phase[_unit->GetInstanceID()] = 0;
		AkamaDialog = 0;
		CourtSay = 0;
		AkamaRun = 0;
	}
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		SoundLimiter = 0;

		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(Unit *mTarget)
    {
		if (m_phase[_unit->GetInstanceID()] > 0)
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

        //_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);

		//RemoveAIUpdateEvent();

		WayStart[_unit->GetInstanceID()] = 2;
		AkamaDialog = 0;
		//AkamaRun = 0;
		CourtSay = 0;

		//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Illidan will pay!");
				_unit->PlaySoundToSet(11381);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "One step closer!");
				_unit->PlaySoundToSet(11382);
				break;
			}
		}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fight on friends! Kill him and end the curse on my people!");	// not sure
		_unit->PlaySoundToSet(11391);

		RemoveAIUpdateEvent();

		WayStart[_unit->GetInstanceID()] = 2;
		AkamaDialog = 0;
		AkamaRun = 0;
		CourtSay = 0;
    }

    void AIUpdate()
    {
		SoundLimiter++;

		if (_unit->GetHealthPct() <= 15)
		{
			int RandomSpeach = rand()%2;

			if (SoundLimiter >= 15)
			{
				switch (RandomSpeach)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not last much longer!");
					_unit->PlaySoundToSet(11385);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No! Not yet!");
					_unit->PlaySoundToSet(11386);
					break;
				}

				SoundLimiter = 0;
			}

			if (m_phase[_unit->GetInstanceID()] == 1)
				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

		}

		if (!m_phase[_unit->GetInstanceID()] && !WayStart[_unit->GetInstanceID()] && MaievDialog[_unit->GetInstanceID()] < 30)
		{
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			
			WayStart[_unit->GetInstanceID()] = 2;
			DoorEvent = 0;
		}

		if (WayStart[_unit->GetInstanceID()] == 1)
		{
			GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(774.7f, 304.6f, 314.85f, 185905);

			Unit* DoorTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(771.5f, 304.7f, 319.0f, CN_DOOR_EVENT_TRIGGER);

			if (!Gate)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It's strange that Illidan doesn't protect himself against intruders." );
				_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
				_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(7);
				_unit->SetFacing(6.248631f);

				if (Udalo)
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				if (Olum)
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				WayStart[_unit->GetInstanceID()] = 2;
				return;
			}

			switch (DoorEvent)
			{
				case 1:
				{
					_unit->SetFacing(6.248631f);
				}break;

				case 5:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The door is all that stands between us and the Betrayer. Stand aside, friends." );
					_unit->Emote(EMOTE_ONESHOT_TALK);
				}break;

				case 9:		// here will be first try to break door (Akama himself only)
				{
					if (DoorTrigger)
					{
						_unit->CastSpell(DoorTrigger, dbcSpell.LookupEntry(AKAMA_DOOR_FAIL), false);
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_DOOR_FAIL);
					}
				}break;

				case 18:	// here we will stop casting spell on door
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}break;

				case 19:
				{
					_unit->Emote(EMOTE_ONESHOT_NO);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I cannot do this alone..." );
				}break;

				case 25:	// summoning two spirits to help Akama with breaking doors
				{
					Udalo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23410, 751.884705f, 311.270050f, 312.121185f, 0.047113f, true, false, 0, 0);
					Olum  = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23411, 751.687744f, 297.408600f, 312.124817f, 0.054958f, true, false, 0, 0);

					if (Udalo)
					{
						Udalo->GetAIInterface()->SetAllowedToEnterCombat(false);
						Udalo->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
					}
					
					if (Olum)
					{
						Olum->GetAIInterface()->SetAllowedToEnterCombat(false);
						Olum->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
					}
				}break;

				case 26:
				{
					if (Udalo)
						Udalo->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You are not alone, Akama." );
				}break;

				case 32:
				{
					if (Olum)
						Olum->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Your people will always be with you." );
				}break;

				case 37:	// Akama starts casting his spell again with both spirits
				{
					if (DoorTrigger)
					{
						_unit->CastSpell(DoorTrigger, dbcSpell.LookupEntry(AKAMA_DOOR_OPEN), false);
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_DOOR_OPEN);
					}
				}break;

				case 38:
				{
					if (DoorTrigger)
					{
						if (Udalo)
						{
							Udalo->CastSpell(DoorTrigger, dbcSpell.LookupEntry(DEATHSWORN_DOOR_OPEN), false);
							Udalo->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
							Udalo->SetUInt32Value(UNIT_CHANNEL_SPELL, DEATHSWORN_DOOR_OPEN);
						}
	
						if (Olum)
						{
							Olum->CastSpell(DoorTrigger, dbcSpell.LookupEntry(DEATHSWORN_DOOR_OPEN), false);
							Olum->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
							Olum->SetUInt32Value(UNIT_CHANNEL_SPELL, DEATHSWORN_DOOR_OPEN);
						}
					}
				}break;

				case 47:	// Door has been broken and channel stoped
				{
					Gate->SetUInt32Value(GAMEOBJECT_STATE, 0);

					if (DoorTrigger)
					{
						DoorTrigger->CastSpell(_unit, dbcSpell.LookupEntry(GATE_FAILURE), true);
					}
				}break;

				case 48:
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

					if (Udalo)
					{
						Udalo->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						Udalo->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					}

					if (Olum)
					{
						Olum->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						Olum->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					}
				}break;

				case 51:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I thank you for your aid, my brothers. Our people will be redeemed!" );
					_unit->Emote(EMOTE_ONESHOT_SALUTE);
				}break;

				case 54:
				{
					if (Udalo)
						Udalo->Emote(EMOTE_ONESHOT_SALUTE);
					if (Olum)
						Olum->Emote(EMOTE_ONESHOT_SALUTE);
				}break;

				case 57:
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(7);

					WayStart[_unit->GetInstanceID()] = 2;
				}break;
			}
			DoorEvent++;
		}

		if (CourtSay)
		{
			CourtSay++;

			switch (CourtSay)
			{
			case 2:
				{
					_unit->PlaySoundToSet(11388);
					_unit->SetFacing(2.113512f);
				}break;
			case 17:
				{
					_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);

					CourtSay = 0;
				}break;
			}
		}

		if (WayStart[_unit->GetInstanceID()] == 4)
		{
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(17);
			_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);

			WayStart[_unit->GetInstanceID()] = 2;
		}

		if (AkamaDialog)
		{
			Illidan = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(704.539001f, 305.282013f, 353.919006f, 22917);

			if (!Illidan || !Illidan->isAlive() && m_phase[_unit->GetInstanceID()] == 0 || !DeathEvent[_unit->GetInstanceID()] && !Illidan->isAlive())	// added check of Illidan's life state, coz some stupid GMs can kill Illidan by command before real part of event starts
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Not this time my friends." );
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				_unit->Despawn(0,0);

				AkamaDialog = 0;
				return;
			}

			switch(AkamaDialog)
			{
			case 1:
				{
					_unit->GetAIInterface()->SetNextTarget(Illidan);
					_unit->SetFacing(3.126680f);
				}break;
			case 2:
				{
					Illidan->RemoveAura(39656);
				}break;
			case 3:
			    {
			        Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago.");
					Illidan->PlaySoundToSet(11463);
			    }break;
			case 7:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 12:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 15:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We've come to end your reign, Illidan. My people and all of Outland shall be free!");
					_unit->PlaySoundToSet(11389);

					_unit->Emote(EMOTE_ONESHOT_POINT);
			    }break;
			case 18:
				{
					_unit->Emote(EMOTE_ONESHOT_TALK);
				}break;
			case 22:
				{
					_unit->Emote(EMOTE_ONESHOT_SALUTE);
				}break;
			case 25:
			    {
			        Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Boldly said. But I remain unconvinced.");
					Illidan->PlaySoundToSet(11464);

					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
			    }break;
			case 28:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 32:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The time has come! The moment is at hand!");
					_unit->PlaySoundToSet(11380);

					_unit->Emote(EMOTE_ONESHOT_SHOUT);
			    }break;
			case 34:
				{
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
					_unit->Emote(EMOTE_ONESHOT_ROAR);
				}break;
			case 36:
				{
					Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are not prepared!");
					Illidan->PlaySoundToSet(11466);

					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					Illidan->Emote(EMOTE_ONESHOT_CUSTOMSPELL05);
					
				}break;
			case 38:
				{
					Illidan->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
				}break;
			case 40:
				{
					GameObject* RightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
					GameObject* LeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
					
					if (RightGate)
						RightGate->SetUInt32Value(GAMEOBJECT_STATE, 1);

					if (LeftGate)
						LeftGate->SetUInt32Value(GAMEOBJECT_STATE, 1);
					
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					_unit->GetAIInterface()->SetAIState(STATE_IDLE);
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					Illidan->GetAIInterface()->SetAllowedToEnterCombat(true);
					Illidan->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Illidan->GetAIInterface()->m_canMove = true;

					_unit->GetAIInterface()->AttackReaction(Illidan, 1, 0);

					CastTriggering = 2;
					AkamaRun = 0;
				}break;
			}

			AkamaDialog++;

			if (AkamaDialog == 45)
				AkamaDialog = 0;
		}

		if (m_phase[_unit->GetInstanceID()] == 0 && DeathEvent[_unit->GetInstanceID()] >= 41)
		{	
			if (DeathEvent[_unit->GetInstanceID()] == 41)
			{
				Maiev = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(700.009216f, 305.660065f, 354.405609f, 23197);
				if (Maiev && Maiev->isAlive())
				{
					MposX = Maiev->GetPositionX();
					MposY = Maiev->GetPositionY();
					MposZ = Maiev->GetPositionZ();
					MposO = Maiev->GetOrientation();
				}
			}

			if (_unit->GetPositionX() == MposX && _unit->GetPositionY() == MposY && _unit->GetPositionZ() == MposZ && DeathEvent[_unit->GetInstanceID()] < 44)
				DeathEvent[_unit->GetInstanceID()] = 44;

			if (DeathEvent[_unit->GetInstanceID()] == 42)
			{
				_unit->GetAIInterface()->MoveTo(MposX, MposY, MposZ, MposO);
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
				_unit->GetAIInterface()->m_moveRun = true;

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(1000);

				DeathEvent[_unit->GetInstanceID()] = 43;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	// dunno why this emote sometimes bugs facing
				//_unit->GetAIInterface()->SetNextTarget(Illidan);
			}

			if (DeathEvent[_unit->GetInstanceID()] >= 45)
			{
				switch (DeathEvent[_unit->GetInstanceID()])
				{
				case 45:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Light will fill these dismal halls once again. I swear it.");
						_unit->PlaySoundToSet(11387);

						if (Illidan)
							_unit->GetAIInterface()->SetNextTarget(Illidan);

						_unit->Emote(EMOTE_ONESHOT_TALK);
					}break;
				case 49:
					{
						_unit->Emote(EMOTE_ONESHOT_SALUTE);
					}break;
				case 53:
					{
						DeathEvent[_unit->GetInstanceID()] = 0;

						_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
						_unit->Despawn(0,0);
					}break;
				}
			}

			if (DeathEvent[_unit->GetInstanceID()] >= 44)
				DeathEvent[_unit->GetInstanceID()]++;
		}

		if (m_phase[_unit->GetInstanceID()])
		{
			CastTriggering--;
			if (!CastTriggering && AkamaRun < 7 && Illidan && Illidan->GetHealthPct() > 85)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				float val = (float)RandomFloat(100.0f);
				SpellCast(val);

				CastTriggering = 2;
			}
/*
			if (Illidan->GetAIInterface()->getAITargetsCount() == 1 && AkamaRun == 7 && Illidan->GetAIInterface()->GetNextTarget() == _unit)
				return;

			if (m_phase[_unit->GetInstanceID()] > 1 && AkamaRun < 7)				// those cases are needed, as if we won't disallow Akama
				_unit->GetAIInterface()->m_canMove = false;	// to move when ex. Illidan starts flying Akama will come
			if (m_phase[_unit->GetInstanceID()] > 1 && AkamaRun >= 7)				// with Illidan and will stay on main platform ----------
				_unit->GetAIInterface()->m_canMove = true;	// (battlefield) ----------------------------------------
*/
			if ((!Illidan || !Illidan->isAlive()) && AkamaRun <= 7 || !DeathEvent[_unit->GetInstanceID()] && !Illidan->isAlive())	// added check for Illidan life state, coz some stupid GMs can kill Illidan by command before real part of event starts
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Not this time my friends." );
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				AkamaRun = 0;

				_unit->Despawn(0,0);
				return;
			}

			if (Illidan && Illidan->isAlive() && Illidan->GetHealthPct() <= 85 && AkamaRun <= 7 || AkamaRun > 7)	// we don't need to check Illidan if AkamaRun > 7 coz we don't use him in that time (only _unit)
			{
				switch (AkamaRun)
				{
					case 0:
					{
						Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, my minions. Deal with this traitor as he deserves!");
						Illidan->PlaySoundToSet(11465);
					}break;

					case 7:
					{	
						_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
						_unit->GetAIInterface()->SetAIState(STATE_IDLE);
						/*_unit->GetAIInterface()->WipeTargetList();	// it's strange that if Akama goes out of combat he affects Illidan so he won't cast or update ai correctly
						_unit->GetAIInterface()->WipeHateList();*/

						_unit->GetAIInterface()->disable_melee = true;
						_unit->GetAIInterface()->m_canMove = false;

						Illidan->GetAIInterface()->RemoveThreatByPtr(_unit);	// Akama will stuck in place anyway if alone in combat with Illidan

						//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					}break;

					case 8:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will deal with these mongrels! Strike now, friends! Strike at the Betrayer!");
						_unit->PlaySoundToSet(11390);

						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					}break;

					case 9:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					}break;

					case 10:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					}break;

					case 11:
					{
						_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
						_unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
					}break;
		
					case 13:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
						_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
					}break;

					case 15:
					{
						if (_unit->GetAIInterface()->getMoveType() != MOVEMENTTYPE_WANTEDWP) 
						{
							// maybe something to stop Illidan from fight Akama, or switch target to PC something!
							_unit->GetAIInterface()->m_canMove = true;

							_unit->GetAIInterface()->StopMovement(0);
							_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
							_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
							_unit->GetAIInterface()->setWaypointToMove(18);

							//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
						}
					}break;
				}
				
				AkamaRun++;
			}
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
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch (iWaypointId)
		{
		case 6:
			{
				WayStart[_unit->GetInstanceID()] = 1;
			}break;
		case 16:
			{
				WayStart[_unit->GetInstanceID()] = 3;
				CourtSay = 1;
			}break;
		case 17:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				AkamaDialog = 1;
			}break;
		case 19:
			{
				//_unit->Despawn(0, 0);	// do not let Illidan's AI being updated
			}break;
		case 20:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				DeathEvent[_unit->GetInstanceID()] = 49;
			}break;
		default:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId+1);
			}
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = ToIllidan[id].x;
        wp->y = ToIllidan[id].y;
        wp->z = ToIllidan[id].z;
        wp->o = ToIllidan[id].o;
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

	float MposX, MposY, MposZ, MposO;
	uint32 CastTriggering;
	uint32 SoundLimiter;
	uint32 m_currentWP;
	uint32 AkamaDialog;
	uint32 DoorEvent;
	uint32 AkamaRun;
	uint32 CourtSay;
	Unit* Illidan;
	int nrspells;
	Unit* Maiev;
	Unit* Udalo;
	Unit* Olum;
};

/* Blade of Azzinoth AI - Creature AI Class */

#define CN_BLADE_OF_AZZINOTH 22996

class BladeOfAzzinothAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BladeOfAzzinothAI);

    BladeOfAzzinothAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->m_noRespawn = true;
		
		RegisterAIUpdateEvent(500);
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       //RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		switch (BladesEvent[_unit->GetInstanceID()])
		{
			case 29:	// Despawn of Blades -> Return to Illidan's hands
			{
				_unit->Despawn(0,0);
			}break;
		}
    }
};

/* Maiev AI - Creature AI Class */

#define CN_MAIEV 23197

//#define TELEPORT_MAIEV 41221
#define TELEPORT 34673	// 34673, 41992

class MaievAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MaievAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    MaievAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(TELEPORT);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 0;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 0;

		Illidan = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(704.539001f, 305.282013f, 353.919006f, 22917);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	
		RegisterAIUpdateEvent(250);

		CastTriggering = 0;
		SoundTimer = 50;

		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
    }

    void OnCombatStop(Unit *mTarget)
    {
        //_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        //_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
	   SoundTimer = 50;
    }

	void OnDamageTaken(Unit *mAttacker, float fAmount)
	{
		_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	}

    void AIUpdate()
    {
		_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		if (MaievDialog[_unit->GetInstanceID()] >= 29 && !DeathEvent[_unit->GetInstanceID()])
			SoundTimer++;

		float SayNaisha = (float)RandomFloat(100.0f);
		if (!DeathEvent[_unit->GetInstanceID()] && ((SayNaisha < 2.5f && SoundTimer >= 100 && SoundTimer < 1330) || SoundTimer == 1361 || SoundTimer > 1400))
		{
			if (SoundTimer != 1361)
				RandText = rand()%3;
				//RandText = sRand.rand(3);
			if (SoundTimer == 1361)
				RandText = rand()%2;
				//RandText = sRand.rand(2);
			switch (RandText)	// Not sure if those both should be here, but couldn't find any relation when those are said on blizz
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bleed as I have bled!");	// not sure
					_unit->PlaySoundToSet(11494);

					SoundTimer = 0;
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That is for Naisha!");
					_unit->PlaySoundToSet(11493);

					SoundTimer = 0;
					break;
				case 2:
					if (Illidan)
					{
						Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Feel the hatred of ten thousand years!");
						Illidan->PlaySoundToSet(11470);

						SoundTimer = 1337;
				}break;
			}
		}

		if (DeathEvent[_unit->GetInstanceID()] && Illidan)
		{
			switch (DeathEvent[_unit->GetInstanceID()])
			{
			case 1:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, it is finished. You are beaten.");	// not sure
					_unit->PlaySoundToSet(11496);

					//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
					//_unit->WipeTargetList();
					//_unit->WipeHateList();

					Illidan->GetAIInterface()->SetNextTarget(NULL);
					Illidan->Emote(EMOTE_ONESHOT_CUSTOMSPELL06);
					if(Illidan->GetCurrentSpell() != NULL)
						Illidan->GetCurrentSpell()->cancel();

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(1000);
				}break;
			case 2:
				{
					Illidan->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CUSTOMSPELL07);
					_unit->GetAIInterface()->SetNextTarget(Illidan);
				}break;
			case 5:
				{
					Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have won... Maiev. But the huntress... is nothing without the hunt. You... are nothing... without me.");
					Illidan->PlaySoundToSet(11478);

					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
					//MaievWeaponOff();
				}break;
			case 25:
				{
					GameObject* RightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
					GameObject* LeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
					
					if (RightGate)
						RightGate->SetUInt32Value(GAMEOBJECT_STATE, 0);

					if (LeftGate)
						LeftGate->SetUInt32Value(GAMEOBJECT_STATE, 0);

					Illidan->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					Illidan->SetUInt32Value(UNIT_FIELD_HEALTH,0);
					Illidan->setDeathState(JUST_DIED);
				}break;
			case 30:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "He's right. I feel nothing. I am nothing.");
					_unit->PlaySoundToSet(11497);
				}break;
			case 39:
				{
					//_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_AKAMA, 739.545410f, 323.023743f, 352.996094f, 4.578086f, true, false, 0, 0);	// we have one Akama already spawned so we don't need another one =/ (still it's freakin' workaround -.-')
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Farewell, champions.");
					_unit->PlaySoundToSet(11498);
				}break;
			case 40:
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(TELEPORT), true);
				}break;
			case 41:
				{
					RemoveAIUpdateEvent();

					_unit->Despawn(0,0);
				}break;
			}

			if (DeathEvent[_unit->GetInstanceID()] <= 41)
				DeathEvent[_unit->GetInstanceID()]++;
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
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }


protected:

	uint32 CastTriggering;
	uint32 SoundTimer;
	uint32 RandText;
	Unit* Illidan;
	int nrspells;
	Unit* Maiev;
};


/* Illidan Stormrage AI - Creature AI Class */

#define CN_ILLIDAN_STORMRAGE 22917

// Normal Form Spells
#define SHEAR								41032  // +
#define DRAW_SOUL							40904  // +
#define FLAME_CRASH							40832  // +
#define MORTAL_STRIKE						37335  // +

// Demon Forms
#define DEMON_FORM1							40511  // +
#define DEMON_FORM2							40398  // +
#define DEMON_FORM3							40510  // +

// Demon Form Spells
#define SHADOW_BLAST						41078  // +
#define SUMMON_DEMON						41117  // - Core Support
#define FLAME_BURST							41131  // ? Haven't Tried	// 41126 is the right one, but it's spell that needs to be scripted

// Illidan's Glaive spells
#define SUMMON_GLAIVE						41466
#define GLAIVE_RETURNS						39873
#define THROW_GLAIVE1						39635
#define THROW_GLAIVE2						39849

// Phase 2 Spells
#define AURA_OF_DREAD						41142
#define FIREBALL							40598  // +
#define DARK_BARRAGE						40585  // ? Haven't Tried
#define SUMMON_TEAR_OF_AZZINOTH				39855  // +

#define EYE_BLAST1 39908
#define EYE_BLAST2 40017
#define EYE_BLAST3 40017

#define TEAR_OF_AZZINOTH_CHANNEL 39857

// Phase 3 Spells
#define AGONIZING_FLAMES					40932  // ? Haven't Tried (Before Demon Form!)

// Phase 4 Spells

// Phase 5 Spells
#define ENRAGE								34971  // +

// Other spells
#define SHADOW_PRISON						40647  // +
#define SKULL_INTRO							39656  // + Works with RemoveAura
#define SUMMON_PARASITIC_SHADOWFIENDS		41915  // ? Haven't Tried
#define PARASITIC_SHADOWFIEND1				41914  // ? Haven't Tried
#define PARASITIC_SHADOWFIEND2				41913  // ? Haven't Tried

static Coords ForIllidan[] = 
{
	{  },
	{ 709.783203f, 305.073883f, 362.988770f, 1.546307f },
	{ 680.798157f, 305.648743f, 353.192200f, 3.196716f }
};

class IllidanStormrageAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IllidanStormrageAI);
	SP_AI_Spell spells[15];
	bool m_spellcheck[15];

    IllidanStormrageAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, RUN));

		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

        spells[0].info = dbcSpell.LookupEntry(SHEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(DRAW_SOUL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 0.0f;	// 6.0f	attacks himself too so it can cause crashes if he dies because of his own hit
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(FLAME_CRASH);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 20;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(AGONIZING_FLAMES);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = 25;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 30.0f;
		spells[4].minhp2cast = 0;
		spells[4].maxhp2cast = 100;

        spells[5].info = dbcSpell.LookupEntry(ENRAGE);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = 0;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].speech = "You've wasted too much time mortals, now you shall fall!";
		spells[5].soundid = 11474;

        spells[6].info = dbcSpell.LookupEntry(SHADOW_BLAST);
		spells[6].targettype = TARGET_DESTINATION;
		spells[6].instant = false;
		spells[6].cooldown = 0;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

        spells[7].info = dbcSpell.LookupEntry(SUMMON_DEMON);
		spells[7].targettype = TARGET_SELF;	// ?
		spells[7].instant = false;
		spells[7].cooldown = 0;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;

        spells[8].info = dbcSpell.LookupEntry(SHADOW_PRISON);
		spells[8].targettype = TARGET_VARIOUS;
		spells[8].instant = true;
		spells[8].cooldown = 0;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;

		/* Spells after this point are not counted when using the SpellCast() 
		   funct in AI and need to be manually triggered! :-D */
		spells[9].info = dbcSpell.LookupEntry(DEMON_FORM1);
		spells[9].instant = true;
		spells[9].cooldown = 0;
		spells[9].perctrigger = 0.0f;
		spells[9].attackstoptimer = 1000;

		spells[10].info = dbcSpell.LookupEntry(DEMON_FORM2);
		spells[10].instant = true;
		spells[10].cooldown = 0;
		spells[10].perctrigger = 0.0f;

		spells[11].info = dbcSpell.LookupEntry(DEMON_FORM3);
		spells[11].instant = true;
		spells[11].cooldown = 0;
		spells[11].perctrigger = 0.0f;

		spells[12].info = dbcSpell.LookupEntry(FIREBALL);
		spells[12].targettype = TARGET_RANDOM_DESTINATION;
		spells[12].instant = false;
		spells[12].cooldown = 0;
		spells[12].perctrigger = 0.0f;

		spells[13].info = dbcSpell.LookupEntry(DARK_BARRAGE);
		spells[13].targettype = TARGET_RANDOM_SINGLE;
		spells[13].instant = false;
		spells[13].cooldown = 0;
		spells[13].perctrigger = 0.0f;

		spells[14].info = dbcSpell.LookupEntry(EYE_BLAST1);
		spells[14].instant = false;
		spells[14].cooldown = 0;
		spells[14].perctrigger = 0.0f;

		/* Setup Illidan */

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(SKULL_INTRO), true);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
		_unit->GetAIInterface()->m_canMove = false;

		/* Initialize variables and stuff, I Guess */
		FoA1 = FoA2 = Maiev = Blade1 = Blade2 = NULL;
		BladesEvent[_unit->GetInstanceID()] = 0;
		DeathEvent[_unit->GetInstanceID()] = 0;
		m_phase[_unit->GetInstanceID()] = 0;	// must be 0 to use it with some other stuff.
		DemonSoundRand = false;
		DemonPhaseTimer = 5;
		DemonPhase = 0;		
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
		_unit->GetAIInterface()->m_canMove = true;

		m_phase[_unit->GetInstanceID()] = 1;
		DemonPhaseTimer = 5;
		DemonPhase = 0;

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who shall be next to taste my blades?!");
				_unit->PlaySoundToSet(11473);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is too easy!");
				_unit->PlaySoundToSet(11472);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->disable_melee = false;
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;

		_unit->WipeTargetList();
		_unit->WipeHateList();

		RemoveAIUpdateEvent();

		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		
		BladesEvent[_unit->GetInstanceID()] = 0;
		DeathEvent[_unit->GetInstanceID()] = 0;
		m_phase[_unit->GetInstanceID()] = 0;
		DemonSoundRand = false;
		DemonPhaseTimer = 5;
		m_currentWP = 0;
		DemonPhase = 0;

		if (m_phase[_unit->GetInstanceID()] == 2)
		{
			_unit->WipeTargetList();
			_unit->WipeHateList();

			_unit->Emote(EMOTE_ONESHOT_LAND);

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			float x = _unit->m_spawn->x;
			float y = _unit->m_spawn->y;
			float z = _unit->m_spawn->z;
			float o = _unit->m_spawn->o;
			
			_unit->GetAIInterface()->MoveTo(x, y, z, o);
		}

		if (m_phase[_unit->GetInstanceID()] == 4)
		{
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		if (FoA1) delete FoA1;
		if (FoA2) delete FoA2;
		if (Blade1) delete Blade1;
		if (Blade2) delete Blade2;

		if (_unit->isAlive()) delete Maiev;
    }

    void OnDied(Unit * mKiller)
    {
		if (m_phase[_unit->GetInstanceID()] == 2)
		{
			_unit->GetAIInterface()->m_moveFly = false;
			_unit->Emote(EMOTE_ONESHOT_LAND);
		}

		m_phase[_unit->GetInstanceID()] = 0;
		DemonSoundRand = false;
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		
		RemoveAIUpdateEvent();

		if (!DeathEvent[_unit->GetInstanceID()] && MaievDialog[_unit->GetInstanceID()] >= 28 && Maiev && Maiev->isAlive())	// it's workaround (added pointer check, coz if he dies when Maiev is not on battlefield then it will crash server)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->SetNextTarget(NULL);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			_unit->GetAIInterface()->StopMovement(0);	// needed?
			if(_unit->GetCurrentSpell() != NULL)
			    _unit->GetCurrentSpell()->cancel();

			Maiev->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			Maiev->GetAIInterface()->SetAIState(STATE_IDLE);
			Maiev->WipeTargetList();
			Maiev->WipeHateList();

			DeathEvent[_unit->GetInstanceID()] = 1;
			m_phase[_unit->GetInstanceID()] = 0;
		}
    }

	void AIUpdate()
	{
		if (_unit->CombatStatus.IsInCombat())
		{
			if (DemonPhase > 0 && DemonPhase < 75)	// 0;9
			{
				DemonTrans();
			}
	
			if (DemonPhase > 75 && DemonPhase < 134)	// 9;17
			{
				TransRevert();
			}
	
			else
			{
				if (m_phase[_unit->GetInstanceID()] == 1)	// First Phase (without demon form)
				{
					PhaseOne();
				}
			
				if (m_phase[_unit->GetInstanceID()] == 2)	// Second Phase (Fly phase)
				{
					PhaseTwo();
				}
		
				if (m_phase[_unit->GetInstanceID()] == 3)	// Third Phase normal form with ability to change into Demon
				{
					PhaseThree();
				}
		
				if (m_phase[_unit->GetInstanceID()] == 4)	// Fourth Phase Demon Form
				{
					PhaseFour();
				}
	
				if (m_phase[_unit->GetInstanceID()] == 5)	// Fifth Phase with ability to change into Demon and Enrage
				{
					PhaseFive();
				}
			}
		}
	}

	void PhaseOne()
	{
		if (_unit->GetHealthPct() <= 65 && _unit->GetCurrentSpell() == NULL)	// <-
		{
			if (_unit->GetCurrentSpell() != NULL)	// safe check after adding		^-- this
				_unit->GetCurrentSpell()->cancel();

			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be touched by a rebels such as you!");	// not sure
			_unit->PlaySoundToSet(11479);

			_unit->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() + 10.0f, _unit->GetOrientation());
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_moveFly = true;
			_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

			WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
            data << _unit->GetNewGUID();
            data << uint32(0);
            _unit->SendMessageToSet(&data, false);

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(500);

			BladesEvent[_unit->GetInstanceID()] = 1;
			FoADeaths[_unit->GetInstanceID()] = 2;
			m_phase[_unit->GetInstanceID()] = 2;
			TriggerSpawned = false;
			FireWallTimer = 0;
			LastFireWall = 0;
			m_currentWP = 0;
		}

		else
		{
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	// in OnCombatStart or in Dialog part it doesn't work (in places I wanted to add it)
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void FoASummon()	// little hacky way to spawn them by pushing into world, but proper spell won't work now
	{					// we can eventually add them as pointers and trigger some events from here (like Blade's channeling)
		FoA1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 672.039246f, 326.748322f, 354.206390f, 0.207343f, true, false, 0, 0);
		FoA2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 673.008667f, 283.813660f, 354.267548f, 6.203853f, true, false, 0, 0);
	
		if (Blade1 && FoA1)
		{
			Blade1->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, FoA1->GetGUID());
			Blade1->SetUInt32Value(UNIT_CHANNEL_SPELL, TEAR_OF_AZZINOTH_CHANNEL);
		}

		if (Blade2 && FoA2)
		{
			Blade2->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, FoA2->GetGUID());
			Blade2->SetUInt32Value(UNIT_CHANNEL_SPELL, TEAR_OF_AZZINOTH_CHANNEL);
		}
	}


	void PhaseTwo()
	{
		if ((BladesEvent[_unit->GetInstanceID()] && BladesEvent[_unit->GetInstanceID()] < 20) || (BladesEvent[_unit->GetInstanceID()] > 24 && BladesEvent[_unit->GetInstanceID()] < 45))
		{
			switch (BladesEvent[_unit->GetInstanceID()])
			{
			case 2:
				{
					_unit->GetAIInterface()->StopMovement(0);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(1);
				}break;
			case 10:	// Blade1 fly
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(THROW_GLAIVE2), false);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the flames of Azzinoth!");
					_unit->PlaySoundToSet(11480);
				}break;
			case 12:
				{
					_unit->CastSpellAoF(676.717346f, 322.445251f, 354.153320f, dbcSpell.LookupEntry(THROW_GLAIVE1), false);
					_unit->CastSpellAoF(677.368286f, 285.374725f, 354.242157f, dbcSpell.LookupEntry(THROW_GLAIVE1), false);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
				}break;
			case 14:		// Blades spawn
				{
					Blade1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLADE_OF_AZZINOTH, 676.717346f, 322.445251f, 354.153320f, 5.732623f, false, false, 0, 0);
					Blade2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLADE_OF_AZZINOTH, 677.368286f, 285.374725f, 354.242157f, 5.645614f, false, false, 0, 0);
					
					if (Blade1)
					{
						Blade1->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						Blade1->GetAIInterface()->SetAllowedToEnterCombat(false);
					}
					
					if (Blade2)
					{
						Blade2->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						Blade2->GetAIInterface()->SetAllowedToEnterCombat(false);
					}
				}break;
			case 18:		// Flames of Azzinoth Spawn + channeling
				{
					FoASummon();
					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(1000);

					TriggerSpawned = false;
					FireWallTimer = 0;

					//_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);	//<-- flag blocks dmg of Fireball spell
					//_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				}break;
			case 30:		// Blades return
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(GLAIVE_RETURNS), false);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);	// he should also make something like jump emote when weapons are in his hands, but I can't find right one o_O
				}break;
			case 34:
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	
					_unit->GetAIInterface()->m_moveFly = false;
					_unit->Emote(EMOTE_ONESHOT_LAND);

					WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
					data << _unit->GetNewGUID();
					data << uint32(0);
					_unit->SendMessageToSet(&data, false);
				}break;
			case 40:	//36
				{
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->disable_melee = false;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

					MaievDialog[_unit->GetInstanceID()] = 0;
					m_phase[_unit->GetInstanceID()] = 3;
					TriggerSpawned = false;
					DemonPhaseTimer = 30;
					FireWallTimer = 0;
					m_currentWP = 0;
				}break;
			}
			if (BladesEvent[_unit->GetInstanceID()] != 3)
				BladesEvent[_unit->GetInstanceID()]++;
		}

		else
		{
			if (BladesEvent[_unit->GetInstanceID()] > 18 && BladesEvent[_unit->GetInstanceID()] < 25)
			{
				if (Blade1 && FoA1 && !FoA1->isAlive())
				{
					Blade1->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					Blade1->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				if (Blade2 && FoA2 && !FoA2->isAlive())
				{
					Blade2->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					Blade2->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}
			}

			if (!FoADeaths[_unit->GetInstanceID()] && FireWallTimer < 30 && _unit->GetCurrentSpell() != NULL)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
				_unit->GetAIInterface()->m_canMove = true;
				if(_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();

				_unit->GetAIInterface()->StopMovement(0);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(2);
				
				FoADeaths[_unit->GetInstanceID()] = 2;
			}

			if (m_currentWP != 2 && _unit->GetAIInterface()->getMoveType() != MOVEMENTTYPE_WANTEDWP && BladesEvent[_unit->GetInstanceID()] == 20)
			{
				FireWallTimer++;

				if (FireWallTimer >= 30)
				{
					if (TriggerSpawned == false)
					{
						float posX;
						float posY;
						float posZ;

						uint32 RandomWall = rand()%8+1;

						while ((RandomWall == LastFireWall) || (RandomWall == LastFireWall - 4) || (RandomWall == LastFireWall + 4))
						{
							RandomWall = rand()%8+1;
						}

						switch (RandomWall)
						{
						case 1:
							posX = 642.240601f;
							posY = 297.297180f;
							posZ = 353.423401f;
							break;
						case 2:
							posX = 641.197449f;
							posY = 314.330963f;
							posZ = 353.300262f;
							break;
						case 3:
							posX = 657.239807f;
							posY = 256.925568f;
							posZ = 352.996094f;
							break;
						case 4:
							posX = 657.913330f;
							posY = 353.562775f;
							posZ = 352.996185f;
							break;
						case 5:
							posX = 707.019043f;
							posY = 270.441772f;
							posZ = 352.996063f;
							break;
						case 6:
							posX = 706.592407f;
							posY = 343.425568f;
							posZ = 352.996124f;
							break;
						case 7:
							posX = 706.593262f;
							posY = 310.011475f;
							posZ = 353.693848f;
							break;
						case 8:
							posX = 706.751343f;
							posY = 298.312683f;
							posZ = 353.653809f;
							break;
						}

						Trigger = NULL;
						Trigger = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_EYE_BEAM_TRIGGER, posX, posY, posZ, 0.0f, false, false, 0, 0);
						
						if (Trigger && Trigger->isAlive() && Trigger->IsInWorld())
						{
							if(_unit->GetCurrentSpell() != NULL)
								_unit->GetCurrentSpell()->cancel();

							_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, Trigger->GetGUID());
							_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, EYE_BLAST1);
							_unit->CastSpell(Trigger, spells[14].info, true);
						}
						
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stare into the eyes of the Betrayer!");	// must check this
						_unit->PlaySoundToSet(11481);

						LastFireWall = RandomWall;
						TriggerSpawned = true;
						FireWallTimer = 30;
					}

					if (TriggerSpawned == true && FireWallTimer == 43)
					{
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
						_unit->RemoveAura(EYE_BLAST1);

						TriggerSpawned = false;
						FireWallTimer = 0;
					}
				}
				
				else if (_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
					Unit* target = NULL;
					target = _unit->GetAIInterface()->GetNextTarget();

					int Spell = rand()%100;
		
					if (Spell >= 0 && Spell <= 25) // dunno why it doesn't make dmg, but looks correctly (it does damage when target can get into combat mode)
					{
						CastSpellOnRandomTarget(12, 0.0f, 60.0f, 0, 100);
					} 
					if (Spell > 25 && Spell <= 35) 
					{
						CastSpellOnRandomTarget(13, 0.0f, 60.0f, 0, 100);
					}
				} 
			}
		}
	 }

	void PhaseThree()
	{
		if (_unit->GetHealthPct() <= 30 && !MaievDialog[_unit->GetInstanceID()] && (DemonPhase <= 0 || DemonPhase >= 135 && !_unit->GetCurrentSpell()))	// add more stuff with maiev in future!
		{
			#ifdef USE_SHADOW_PRISON
				_unit->CastSpell(_unit, spells[8].info, spells[8].instant);
			#endif

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->m_canMove = false;
			if(_unit->GetCurrentSpell() != NULL)		// keeping it for now, will delete it later anyway
				_unit->GetCurrentSpell()->cancel();

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(1000);

			MaievDialog[_unit->GetInstanceID()] = 1;
		}

		if (MaievDialog[_unit->GetInstanceID()] && MaievDialog[_unit->GetInstanceID()] < 32 && (DemonPhase <= 0 || DemonPhase >= 135))
		{	// add her weapon switching (from belt to hand, emote cases etc.)
			switch (MaievDialog[_unit->GetInstanceID()])
			{
			case 3:		// 3
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Is this it, mortals? Is this all the fury you can muster?");
					_unit->PlaySoundToSet(11476);
				}break;
			case 6:		// 6
				{
					_unit->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 11:	// 12
				{
					SpawnMaiev();
					Maiev->GetAIInterface()->SetNextTarget(_unit);

					Maiev->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Their fury pales before mine, Illidan. We have some unsettled business between us.");
					Maiev->PlaySoundToSet(11491);

					_unit->GetAIInterface()->SetNextTarget(Maiev);
					//_unit->SetFacing(5.287312f);					// I must think about good formula for both units
					//_unit->GetAIInterface()->setInFront(Maiev);	// dunno why it doesn't work -.-'
				}break;
			case 12:	// 13
				{
					Maiev->CastSpell(_unit, dbcSpell.LookupEntry(TELEPORT), true);

					MaievWeaponOff();
				}break;
			case 13:	// 14
				{
					Maiev->Emote(EMOTE_ONESHOT_EXCLAMATION);
				}break;
			case 18:	// 20
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Maiev... How is it even possible?");
					_unit->PlaySoundToSet(11477);
				}break;
			case 21:	// 23
				{
					//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				}break;
			case 24:	// 27
				{
					Maiev->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, my long hunt is finally over. Today, Justice will be done!");
					Maiev->PlaySoundToSet(11492);

					Maiev->Emote(EMOTE_ONESHOT_EXCLAMATION);
				}break;
			case 27:	// 30
				{
					Maiev->Emote(EMOTE_ONESHOT_YES);
				}break;
			case 28:	// 31
				{
					Maiev->Emote(EMOTE_ONESHOT_ROAR);
				}break;
			case 31:	//34
				{
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->disable_melee = false;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					Maiev->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					Maiev->GetAIInterface()->SetAllowedToEnterCombat(true);
					Maiev->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					Maiev->GetAIInterface()->SetAIState(STATE_IDLE);
					Maiev->GetAIInterface()->m_canMove = true;

					Maiev->GetAIInterface()->AttackReaction(_unit, 1, 0);

					MaievWeaponOn();
					
					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
					
					m_phase[_unit->GetInstanceID()] = 5;
					DemonPhaseTimer = 30;
					EnrageTimer = 10;
				}break;
			}

			MaievDialog[_unit->GetInstanceID()]++;
		}
		
		else
		{
			DemonPhaseTimer--;

			if (!DemonPhaseTimer)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				_unit->GetAIInterface()->disable_melee = true;
				if(_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();
				
				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(250);

				DemonPhase = 1;
			}
	
			else
			{
				if (DemonPhase < 1 || DemonPhase > 134)
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					float val = (float)RandomFloat(100.0f);
					SpellCast(val);
				}
			}
		}
	}

	void DemonTrans()	// check and change how long spell 9 is in use on official
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();

		switch (DemonPhase)
		{	// let's give it 4 free triggers to not use melee attacks and spells
		case 5:
			{
				_unit->CastSpell(_unit, spells[9].info, spells[9].instant);
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);

				//_unit->GetAIInterface()->disable_melee = true;
			}break;

		case 13:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the power... of the demon within!");
				_unit->PlaySoundToSet(11475);

				_unit->CastSpell(_unit, spells[10].info, spells[10].instant);
			}break;

		case 33:
			{
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21322);
				_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 5.0f);
			}break;

		case 43:
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04);
				//_unit->RemoveAura(40398);	
			}break;

		case 59:
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->RemoveAura(40510);
			}break;

		case 61:	//63	//65	// he should also drop aggro and take new target (warlock should take it by dots casted before final change)
			{
				_unit->RemoveAura(40510);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

				DemonSoundRand = false;
				DemonPhaseTimer = 0;
				DemonPhase = 74;
				m_phase[_unit->GetInstanceID()] = 4;
			}break;
		}

		DemonPhase++;
	}

	void TransRevert()	// same here to spell 9 (but not that much I think); also I am thinking about changing spells on emotes
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();

		switch (DemonPhase)
		{
		case 81:	// 1 diff is too long?
			{
				_unit->CastSpell(_unit, spells[9].info, spells[9].instant);
				_unit->GetAIInterface()->m_canMove = false;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 403);
				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02);
			}break;

		case 84:	//85
			{
				_unit->CastSpell(_unit, spells[10].info, spells[10].instant);

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 404);
			}break;

		case 108:	//108	//109
			{
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21137);
				_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.5f);
			}break;

		case 114:	//116	//117
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 405);
				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04);
			}break;

		case 125:	//122	//125	//123 - hand show	//125 - on rage showing lol	//126
			{
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
			}break;

		case 129:	//133	//129	//130
			{
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->RemoveAura(40510);

				if (!MaievDialog[_unit->GetInstanceID()])
					m_phase[_unit->GetInstanceID()] = 3;
				else
				{
					EnrageTimer = rand()%10+11;
					m_phase[_unit->GetInstanceID()] = 5;
				}
				
				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

				DemonPhaseTimer = 30;
				DemonPhase = 135;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			}break;
		}

		DemonPhase++;
	}

	void PhaseFour()
	{
		DemonPhaseTimer++;

		if (DemonPhaseTimer == 15)
		{
			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		if (DemonPhaseTimer < 2 && MaievDialog[_unit->GetInstanceID()] && !DemonSoundRand)
		{
			int Sound = rand()%3;	// 66% chance to say something, should work fine

			switch (Sound)
			{
			case 0:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You know nothing of power!");
					_unit->PlaySoundToSet(11469);

					DemonSoundRand = true;
				}break;
			case 1:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Such arrogance!");
					_unit->PlaySoundToSet(11471);

					DemonSoundRand = true;
				}break;
			}
		}

		if (_unit->GetHealthPct() <= 30 && !MaievDialog[_unit->GetInstanceID()])
		{
			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		else
		{
			if (_unit->GetAIInterface()->GetNextTarget() && DemonPhase == 75)
			{
				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();

				if (_unit->GetDistance2dSq(target) >= 0.0f && _unit->GetDistance2dSq(target) <= 10000.0f)
				{
					_unit->GetAIInterface()->m_canMove = false;

					if (_unit->GetCurrentSpell()) return;

					int Spell = rand()%100;
					if (Spell >= 0 && Spell <= 80)
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[6].info, spells[6].instant);
					if (Spell > 80 && Spell <= 90) return;
						//_unit->CastSpell(_unit, spells[7].info, spells[7].instant);	// disabled for now as it can cause some unneeded animations			// both should have _unit as target
					if (Spell > 90 && Spell <= 100)
						_unit->CastSpell(_unit, spells[1].info, spells[1].instant);	// this one needs to be scripted
				}

				else
					_unit->GetAIInterface()->m_canMove = true;
			}
		}
	}

	void PhaseFive()
	{
		DemonPhaseTimer--;
		EnrageTimer--;

		if (!DemonPhaseTimer)
		{
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->disable_melee = true;
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);
			
			DemonPhase = 1;
		}

		if (!EnrageTimer)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You've wasted too much time mortals, now you shall fall!");
			_unit->PlaySoundToSet(11474);

			_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
		}

		else
		{
			if (DemonPhase < 1 || DemonPhase > 134)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);

				float val = (float)RandomFloat(100.0f);
				SpellCast(val);
			}
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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					if ((i == 4 && m_phase[_unit->GetInstanceID()] == 3) || i != 4)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void SpawnMaiev()	// this doesn't have collision checks! so keep in mind that Maiev can be spawned behind walls!
	{
		float xchange  = (float)RandomFloat(15.0f);
		float distance = 15.0f;

		float ychange = sqrt(distance*distance - xchange*xchange);

		if (rand()%2 == 1)
			xchange *= -1;
		if (rand()%2 == 1)
			ychange *= -1;

		float newposx = _unit->GetPositionX() + xchange;
		float newposy = _unit->GetPositionY() + ychange;

		Maiev = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MAIEV, newposx, newposy, _unit->GetPositionZ() + 0.5f, 2.177125f, true, false, 0, 0);
	}

	void MaievWeaponOff()
	{
		/*Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 218169346);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 4);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);*/
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 33488898);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 781);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
	}

	void MaievWeaponOn()
	{
		/*Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 218169346);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 4);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);*/	
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 33488898);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 781);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch(iWaypointId)
		{
		case 1:
			{
				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				BladesEvent[_unit->GetInstanceID()] = 4;

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(500);
			}break;
		case 2:
			{
				_unit->GetAIInterface()->m_canMove = false;

				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(500);

				BladesEvent[_unit->GetInstanceID()] = 25;
				m_currentWP = 2;
			}break;
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = ForIllidan[id].x;
        wp->y = ForIllidan[id].y;
        wp->z = ForIllidan[id].z;
        wp->o = ForIllidan[id].o;
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

	uint32 FireWallTimer;
	bool DemonSoundRand;
	uint32 LastFireWall;
	bool TriggerSpawned;
	int DemonPhaseTimer;
	uint32 m_currentWP;
	int EnrageTimer;
	int DemonPhase;
	Unit* Trigger;
	int nrspells;
	Unit* Blade1;
	Unit* Blade2;
	Unit* Maiev;
	Unit* FoA1;
	Unit* FoA2;
};

void SetupBlackTemple(ScriptMgr * mgr)
{
	//Bosses
	mgr->register_creature_script(CN_SUPREMUS, &SupremusAI::Create);
	mgr->register_creature_script(CN_NAJENTUS, &NajentusAI::Create);
	mgr->register_creature_script(CN_GURTOGG_BLOODBOIL, &GurtoggAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFSUFFERING, &EssenceOfSufferingAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFDESIRE, &EssenceOfDesireAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFANGER, &EssenceOfAngerAI::Create);
	mgr->register_creature_script(CN_MOTHER_SHAHRAZ, &ShahrazAI::Create);
	mgr->register_creature_script(CN_LADY_MALANDE, &MalandeAI::Create);
	mgr->register_creature_script(CN_GATHIOS_THE_SHATTERER, &GathiosAI::Create);
	mgr->register_creature_script(CN_HIGH_NETHERMANCER_ZEREVOR, &ZerevorAI::Create);
	mgr->register_creature_script(CN_VERAS_DARKSHADOW, &VerasAI::Create);
	mgr->register_creature_script(CN_TERON_GOREFIEND, &TerongorefiendAI::Create);
	//mgr->register_creature_script(CN_SHADE_OF_AKAMA, &ShadeofakamaAI::Create); //test
	//Mobs  
	mgr->register_creature_script(CN_SHADOWY_CONSTRUCTS, &ShadowyconstructsAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_DEFENDER, &AshtonguedefenderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ELEMENTALIST, &AshtongueelementalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ROGUE, &AshtonguerogueAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_SPIRITBINDER, &AshtonguespiritbinderAI::Create);

	//Illidan Stormrage related
	/*GossipScript * AG = (GossipScript*) new AkamaGossip();
	mgr->register_gossip_script(CN_AKAMA, AG);

	mgr->register_creature_script(CN_DOOR_EVENT_TRIGGER, &DoorEventTriggerAI::Create);
	mgr->register_creature_script(CN_DEMON_FIRE, &DemonFireAI::Create);
	mgr->register_creature_script(CN_EYE_BEAM_TRIGGER, &EyeBeamTriggerAI::Create);
	mgr->register_creature_script(CN_FLAME_CRASH, &FlameCrashAI::Create);
	mgr->register_creature_script(CN_BLAZE, &BlazeAI::Create);
	mgr->register_creature_script(CN_FLAME_OF_AZZINOTH, &FlameOfAzzinothAI::Create);
	mgr->register_creature_script(CN_AKAMA, &AkamaAI::Create);
	mgr->register_creature_script(CN_MAIEV, &MaievAI::Create);
	mgr->register_creature_script(CN_BLADE_OF_AZZINOTH, &BladeOfAzzinothAI::Create);
	mgr->register_creature_script(CN_ILLIDAN_STORMRAGE, &IllidanStormrageAI::Create);*/
}