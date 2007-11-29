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
#define SOUL_TRANSFER       30531

Unit* Channeler;
Unit* ChannelerTO;
Unit* Magtheridon;
int dieCount;

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
#define BEAM_BLUNDER        1120

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
			dieCount++;

			sLog.outString("[Moon++][Magtheridon]Phase I Channeler dieCount = %u/5" , dieCount );

			if(dieCount >=5)
			{
				Magtheridon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-32.171f, 39.926f, 0.630f, 17257);
				Magtheridon->GetAIInterface()->SetAllowedToEnterCombat(true);
				Magtheridon->GetAIInterface()->m_canMove = true;
				Magtheridon->RemoveAllAuras();
			}
			
			//todo fix the way channelers give eachother the buff
			/*Channeler = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-32.171f, 39.926f, 0.630f, 17256);
			ChannelerTO = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10.171f, 23.926f, 0.630f, 17256);
			Channeler->CastSpell(ChannelerTO, SOUL_TRANSFER, true);*/

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


// Magtheridon
#define CN_MAGTHERIDON 17257

#define QUAKE      30571
#define BLAST_NOVA 30616
#define CLEAVE     37476
#define BANISH     31797
#define CAVE_IN    36240

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
/*
 Draining: "Not again... NOT AGAIN!" //sound id: 10256
 Freed from the draining: (laughs evilly) "I... am... UNLEASHED!!!" //soundid 10253*/
//		
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 5000;
		
		sLog.outString("[Moon++][Magtheridon]Started");
		_unit->CastSpell(_unit, BANISH, true);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
	}
	void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thank you for releasing me. Now... die!");
		_unit->PlaySoundToSet(10254);
		timer_blastNova = 0;
		timer_caveIn = 0;
		
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion... will consume you... all....");
		_unit->PlaySoundToSet(10258);
		sLog.outString("[Moon++][Magtheridon]Is now dead.");
       RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Did you think me weak? Soft? Who is the weak one now?!");
			_unit->PlaySoundToSet(10255);
			sLog.outString("[Moon++][Magtheridon]Killed player");
		}
	}
	
	void AIUpdate()
    {
		
		m_phase = 1;

		if(_unit->GetHealthPct() <= 99 && _unit->GetHealthPct() >= 31)
		{
			m_phase = 2;
		}
		if(_unit->GetHealthPct() <= 30)
		{
			m_phase = 3;
		}

		sLog.outString("[Moon++][Magtheridon] AI Update Tick m_phase = %u", m_phase);

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
		float val = (float)sRand.rand(100.0f);
        SpellCast(val);
    }
	
	void PhaseOne()
	{
		//Phase one is handled in the channeler code
		/*timer_RemoveBanish++;
		if(timer_RemoveBanish > 100 && dieCount > 1)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->m_canMove = true;
			_unit->RemoveAllAuras();
			float val = (float)sRand.rand(100.0f);
			SpellCast(val);
		}
		else
		{
			sLog.outString("[Moon++][Magtheridon]Banish Countdown = %u & dieCount = %u" , timer_RemoveBanish, dieCount);
		}*/
	}
	void PhaseTwo()
	{
		timer_blastNova++;
		float val = (float)sRand.rand(100.0f);
		SpellCast(val);
		Unit *target = _unit->GetAIInterface()->GetNextTarget();
		sLog.outString("[Moon++][Magtheridon] Phase II Blast nova = %u / 54", timer_blastNova);
		if(timer_blastNova > 54)
		{
			_unit->CastSpell(target, dbcSpell.LookupEntry(BLAST_NOVA), true);
			timer_blastNova = 0;
		}
	}
	void PhaseThree()
	{
		timer_caveIn++;
		timer_blastNova++;

		sLog.outString("[Moon++][Magtheridon] Phase III Blast nova = %u / 54   Cave In = %u / 60", timer_blastNova, timer_caveIn);
		float val = (float)sRand.rand(100.0f);
		SpellCast(val);
		Unit *target = _unit->GetAIInterface()->GetNextTarget();
		
		if(timer_blastNova > 54)
		{
			_unit->CastSpell(target, dbcSpell.LookupEntry(BLAST_NOVA), true);
			timer_blastNova = 0;
		}
		if(timer_caveIn > 60)
		{
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), dbcSpell.LookupEntry(CAVE_IN), true);

			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be taken so easily. Let the walls of this prison tremble... and FALL!!!");
			_unit->PlaySoundToSet(10257);
			timer_caveIn = 0;
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
	int timer_caveIn;
	int timer_blastNova;
	int timer_RemoveBanish;
};

void SetupMagtheridonsLair(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_HELLFIRE_WARDER, &HellfireWarderAI::Create);
	mgr->register_creature_script(CN_MAGTHERIDON, &MagtheridonAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_CHANNELER, &HellfireChannelerAI::Create);
}
