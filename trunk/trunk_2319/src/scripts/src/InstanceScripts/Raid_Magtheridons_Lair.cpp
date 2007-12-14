#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_Magtheridons_Lair.cpp Script											*/
/************************************************************************/
// Hellfire Warder
#define CN_HELLFIRE_WARDER 18829

#define SHADOW_BOLT_VOLLEY	39175
#define DEATH_COIL			41070
#define RAIN_OF_FIRE		37465

class HellfireWarderAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HellfireWarderAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HellfireWarderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DEATH_COIL);
		spells[1].targettype = TARGET_VARIOUS;	
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[2].targettype = TARGET_VARIOUS; // Needs checking
		spells[2].instant = false;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		_unit->SetStandState(8);

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
						target = _unit->GetAIInterface()->GetNextTarget();
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

// Hellfire Channeler
#define CN_HELLFIRE_CHANNELER 17256

#define SHADOW_BOLT_VOLLEY	39175
#define DARK_MENDING		30528
#define SOUL_TRANSFER       30531

class HellfireChannelerAI : public CreatureAIScript
	{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HellfireChannelerAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];
		
		HellfireChannelerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
				
			} 
			
			spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 9.0f;
			spells[0].attackstoptimer = 1000;
			
			spells[1].info = dbcSpell.LookupEntry(DARK_MENDING);
			spells[1].targettype = TARGET_VARIOUS;	
			spells[1].instant = true;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 1000;

			magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.657900f, 2.159050f, -0.345542f, 17257);
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, magtheridon->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 30207);
		}
		void OnCombatStart(Unit* mTarget)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
			RegisterAIUpdateEvent(1000);
		}
		
		void OnCombatStop(Unit *mTarget)
		{
			if (_unit->isAlive() && magtheridon->isAlive() && !magtheridon->CombatStatus.IsInCombat())
			{
				_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, magtheridon->GetGUID());
				_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 30207);
			}
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
								target = _unit->GetAIInterface()->GetNextTarget();
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
		Creature *magtheridon;
	};


// Magtheridon
#define CN_MAGTHERIDON 17257

#define QUAKE		30571
#define BLAST_NOVA	30613 // its only one tick but you cant cancel the full spell (30616)
#define CLEAVE		37476
#define BANISH		31797
#define CAVE_IN		36240 // wrong... cant fins something

class MagtheridonAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MagtheridonAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    MagtheridonAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = true;
		} 
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 4000;

		channeler1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER, -55.638000f, 1.869050f, 0.630946f, 0.000000f, true, false, 0, 0);
		channeler2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER, -32.171600f, 39.926800f, 0.630921f, 4.940120f, true, false, 0, 0);
		channeler3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER, 10.477100f, 24.445499f, 0.630891f, 3.894760f, true, false, 0, 0);
		channeler4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER, 10.469200f, -19.894800f, 0.630910f, 2.555650f, true, false, 0, 0);
		channeler5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HELLFIRE_CHANNELER,  -31.861300f, -35.919399f, 0.630945f, 1.314720f, true, false, 0, 0);

		_unit->CastSpell(_unit, BANISH, true);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		m_phase = 1;
		RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thank you for releasing me. Now... die!");
		_unit->PlaySoundToSet(10254);
		timer_blastNova = 0;
		timer_quake = 0;
		timer_enrage = 1200;
		enrage = 0;
    }

    void OnCombatStop(Unit *mTarget)
    {
		if (_unit->isAlive())
		{
			channeler1->Despawn(100, 0);
			channeler2->Despawn(100, 0);
			channeler3->Despawn(100, 0);
			channeler4->Despawn(100, 0);
			channeler5->Despawn(100, 0);
			_unit->Despawn(100, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MAGTHERIDON, -22.657900f, 2.159050f, -0.345542f, 3.159940f, true, false, 0, 0);
		}
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion... will consume you... all....");
		_unit->PlaySoundToSet(10258);
       RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Did you think me weak? Soft? Who is the weak one now?!");
			_unit->PlaySoundToSet(10255);
		}
	}
	
	void AIUpdate()
    {
		if(_unit->GetHealthPct() <= 30 && m_phase == 2)
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
		if (!channeler1->isAlive() && !channeler2->isAlive() && !channeler3->isAlive() && !channeler4->isAlive() && !channeler5->isAlive())
		{
			m_phase = 2;
			_unit->RemoveAura(BANISH);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		}
	}
	void PhaseTwo()
	{
		if (!timer_enrage && !enrage)
		{
			_unit->DamageDoneModPCT[0] = 2;
			enrage = 1;
		}
		else
			timer_enrage--;
		if (!quake)
		{
			timer_quake++;
			timer_blastNova++;
			float val = (float)sRand.rand(100.0f);
			SpellCast(val);

			if(timer_blastNova > 54)
			{
				_unit->CastSpell(_unit, BLAST_NOVA, true);
				timer_blastNova = 0;

			}
			else if (timer_quake > 40)
			{
				_unit->setAttackTimer(9000, false);
				_unit->GetAIInterface()->StopMovement(9000);
				_unit->CastSpell(_unit, QUAKE, true);
				timer_quake = 0;
				quake = 1;
			}
		}
		else
		{
			if (quake <= 6)
			{
				_unit->CastSpell(_unit, QUAKE, true);
				quake++;
			}
			else
			{
				quake = 0;
			}
		}
	}
	void PhaseThree()
	{
		if (!timer_enrage && !enrage)
		{
			_unit->DamageDoneModPCT[0] = 2;
			enrage = 1;
		}
		else
			timer_enrage--;
		if (!quake)
		{
			timer_quake++;
			timer_blastNova++;
			float val = (float)sRand.rand(100.0f);
			SpellCast(val);

			if(timer_blastNova > 54)
			{
				_unit->CastSpell(_unit, BLAST_NOVA, true);
				timer_blastNova = 0;

			}
			else if (timer_quake > 40)
			{
				_unit->setAttackTimer(8000, false);
				_unit->GetAIInterface()->StopMovement(8000);
				_unit->CastSpell(_unit, QUAKE, true);
				timer_quake = 0;
				quake = 1;
			}
		}
		else
		{
			if (quake <= 6)
			{
				_unit->CastSpell(_unit, QUAKE, true);
				quake++;
			}
			else
			{
				quake = 0;
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
	int m_phase;
	int timer_blastNova;
	//int timer_RemoveBanish;
	int timer_enrage;
	int timer_quake;
	int quake;
	int enrage;

	Creature *channeler1;
	Creature *channeler2;
	Creature *channeler3;
	Creature *channeler4;
	Creature *channeler5;
};

void SetupMagtheridonsLair(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_HELLFIRE_WARDER, &HellfireWarderAI::Create);
	mgr->register_creature_script(CN_MAGTHERIDON, &MagtheridonAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_CHANNELER, &HellfireChannelerAI::Create);
}
