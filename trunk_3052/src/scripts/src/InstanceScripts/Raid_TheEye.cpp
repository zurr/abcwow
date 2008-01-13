/************************************************************************/
/* Raid_TheEye.cpp Script												*/
/************************************************************************/

/****************/
/*    Trashes   */
/****************/

// APPRENTICE STAR SCRYER
#define CN_A_STAR_SCRYER	20043

#define ARCANE_VOLLEY		40424
#define A_STAR_ARCANE_EXPLOSION	27082

class ASTARTSCRYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ASTARTSCRYERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    ASTARTSCRYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ARCANE_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(A_STAR_ARCANE_EXPLOSION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// STAR SCRYER
#define CN_STAR_SCRYER 20034

#define STARFALL 37124

class STARTSCRYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(STARTSCRYERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    STARTSCRYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(STARFALL);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// ASTROMANCER
#define CN_ASTROMANCER			20033

#define ASTROMANCER_FIREBALL_VOLLEY	 29922
#define ASTROMANCER_BLAST_WAVE 33933

class ASTROMANCERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ASTROMANCERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    ASTROMANCERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ASTROMANCER_FIREBALL_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ASTROMANCER_BLAST_WAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 7;
		spells[1].perctrigger = 60.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// ASTROMANCER LORD
#define CN_ASTROMANCER_LORD 20046

#define A_LORD_FIREBALL_VOLLEY 29922
#define BLAST_WEAVE	33933
#define DRAGONS_BREATH 33043

class ASTROMANCERLORD : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ASTROMANCERLORD);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    ASTROMANCERLORD(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(A_LORD_FIREBALL_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLAST_WEAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 7;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;
			
		spells[2].info = dbcSpell.LookupEntry(DRAGONS_BREATH);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// BLOODWARDER VINDICATOR
#define CN_BLOOD_VINDICATOR	20032

#define HAMMER_OF_JUSTICE 32416
#define FLASH_HEAL 38588

class SBLOODVINDICATORAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SBLOODVINDICATORAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SBLOODVINDICATORAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLASH_HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// BLOODWARDER LEGIONNARE
#define CN_BLOOD_LEGIONNARE 20031

#define LEGION_WHIRLWIND 36981
#define CLEAVE	26350

class BLOODLEGIONNAREAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLOODLEGIONNAREAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLOODLEGIONNAREAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(LEGION_WHIRLWIND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 40.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// BLOODWARDER MARSHAL
#define CN_BLOOD_MARSHAL 20035

#define WHIRLWIND		36981
#define UPPERCUT	26007

class BLOODMARSHALAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLOODMARSHALAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLOODMARSHALAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(WHIRLWIND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(UPPERCUT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// PHOENIX HAWK
#define CN_PHOENIX_HAWK 20039

#define MANA_BURN 25380
#define HAWK_CHARGE	16636

class PHOENIXHAWKAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PHOENIXHAWKAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    PHOENIXHAWKAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(MANA_BURN);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HAWK_CHARGE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 12;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 15;
		spells[1].maxdist2cast = 45;

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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// CRYSTALCORE SENTINEL
#define CN_CRYSTAL_SENTINEL 20041

#define TRAMPLE 40340
#define CHARGET_ARCANE_EXPLOSION 37106

class CRYSTALSENTINELAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CRYSTALSENTINELAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CRYSTALSENTINELAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(TRAMPLE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGET_ARCANE_EXPLOSION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 50.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};

// CRYSTALCORE MECHANIC
#define CN_CRYSTAL_MECHANIC 20052

#define SAW_BLADE 37123

class CRYSTALMECHANICAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CRYSTALMECHANICAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CRYSTALMECHANICAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SAW_BLADE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 6;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
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
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
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
	
protected:

	int nrspells;
};


/****************/
/*    Bosses    */
/****************/

// Void Reaver AI

#define CN_VOID_REAVER 19516

#define POUNDING 34164
#define ARCANE_ORB 34190
#define KNOCK_AWAY 36512 // maybe wrong id (maybe should be: 21737 ? ), but works
#define ENRAGE 36992 // Needs checking (as it can be wrong [or maybe IS wrong])

//TO DO: Add rest of the sounds (no idea to which action they should be used)

class VOIDREAVERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VOIDREAVERAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    VOIDREAVERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = dbcSpell.LookupEntry(POUNDING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 16.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_ORB);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(KNOCK_AWAY);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Alert, you are marked for extermination!");
		_unit->PlaySoundToSet(11213);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Extermination, successful.");
				_unit->PlaySoundToSet(11215);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Imbecile life form, no longer functional.");
				_unit->PlaySoundToSet(11216);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Threat neutralized.");
				_unit->PlaySoundToSet(11217);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Systems... shutting... down...");
        _unit->PlaySoundToSet(11214);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		Timer = Timer + 1;

		if (Timer == 200) // Need correct attackspeed in DB to change it to correct value
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
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
	int Timer;
};


// High Astromancer Solarian AI
// First try to make it in phase system. For now not enough luck (as I want to update spell list
// when next phase has just started.

#define CN_HIGH_ASTROMANCER_SOLARIAN 18805
#define CN_SOLARIUM_AGENT 18925
#define CN_SOLARIUM_PRIEST 18806
//Phase 1 spells
#define ARCANE_MISSILES 39414 // Should have random targeting
#define WRATH_OF_THE_ASTROMANCER 33045 // Needs random function
#define MARK_OF_SOLARIAN 33023 // not sure... but should be used on random target
//Phase 2 spells
// Just to define portal summoning + summoning creatures + creatures AI
// Add sounds to creature summoning events
//Phase 3 spells
#define VOID_BOLT 39329 // RANDOM target, but because of lack of feature ATTACKING
#define FEAR 10890 // probably wrong id; maybe one of these are correct: 31970, 31358 (?)

// TO DO: Rewrite it to phase style.
/*
Quotes

Phase 1 (Astromancer Phase):
Tal anu'men no Sin'dorei!
Phase 2 (Solarium Agents):
I will crush your delusions of grandeur!
Ha ha ha! You are hopelessly outmatched!
Phase 3 (Voidwalker Form):
Enough of this! Now I call upon the fury of the cosmos itself.
I become ONE... with the VOID!
Slaying:
Your soul belongs to the Abyss!
By the blood of the Highborne!
For the Sunwell!
Dying:
The warmth of the sun... awaits.
*/
class HIGHASTROMANCERSOLARIANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HIGHASTROMANCERSOLARIANAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    HIGHASTROMANCERSOLARIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 2;
		spells[0].perctrigger = 80.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WRATH_OF_THE_ASTROMANCER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(MARK_OF_SOLARIAN);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(VOID_BOLT);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 40.0f;
		spells[3].attackstoptimer = 2000;

		spells[4].info = dbcSpell.LookupEntry(FEAR);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 20.0f;
		spells[4].attackstoptimer = 1000;

		priestTimer = 0;
		spawnTimer = 0;
		spmin = 0;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Tal anu'men no Sin'dorei!");
		_unit->PlaySoundToSet(11134);

        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		spmin = 0;
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul belongs to the Abyss!");
				_unit->PlaySoundToSet(11136);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the blood of the Highborne!");
				_unit->PlaySoundToSet(11137);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For the Sunwell!");
				_unit->PlaySoundToSet(11138);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 18239); //Human Form
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The warmth of the sun... awaits.");
        _unit->PlaySoundToSet(11135);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		spawnTimer++;
		priestTimer++;

		if(spawnTimer > 50 && !setVoidForm)
		{
			
			_unit->ClearHateList();
			_unit->GetAIInterface()->m_canMove = false;
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(24699), true);
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(35182), true);

			for(int i = 1;i <= 4;i++)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_SOLARIUM_AGENT, 421.254822f, -352.720978f, 17.001482f, 0.0f, false, false, 0, 0);
			}
			for(int i = 1;i <= 4;i++)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_SOLARIUM_AGENT, 410.456543f, -367.723816f, 16.994316f, 0.0f, false, false, 0, 0);
			}
			for(int i = 1;i <= 4;i++)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_SOLARIUM_AGENT, 439.210846f, -351.484894f, 17.009836f, 0.0f, false, false, 0, 0);
			}
			priestTimer = 100;
			spawnTimer = 0;
		}
		if(priestTimer > 115 && !setVoidForm)
		{
			_unit->GetAIInterface()->m_canMove = true;
			_unit->RemoveAllAuras();
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOLARIUM_PRIEST, _unit->GetPositionX(), _unit->GetPositionY() + 5.0f, _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOLARIUM_PRIEST, _unit->GetPositionX(), _unit->GetPositionY() + -5.0f, _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
			priestTimer = 0;
		}
		if(_unit->GetHealthPct() > 20)
		{
			nrspells = 3;
		}
		else
		{
			if(!setVoidForm)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I become ONE... with the VOID!");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 18953); //VoidWalker Form
				setVoidForm = true;
			}
			spmin = 4;
			nrspells = 5;
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
	bool setVoidForm;
	int spawnTimer;
	int priestTimer;
};


// Al'ar AI

#define WALK 0
#define RUN 256
#define FLY 768

#define CN_ALAR 19514

// Phase1 spells
#define FLAME_BUFFET 34121	// casted only when there is no player in melee range!
#define FLAME_QUILLS 34316	// it flies to a high point in a center of room and use this on players located on platform	// also can be many others (Idk even if this spell works =S)
#define SUMMON_PHOENIX_ADDS 18814	// Summons 3 phoenixs

// Phase2 spells
#define FLAME_PATCH 35383	// 35383, 35380;
#define METEOR 35181		// but shouldn't be instant imho
#define EMBER_BLAST 34133	// used when one of adds has low health
/*#define MELT_ARMOR		// maybe they are used, but not sure
#define RANDOM_CHARGE*/

// Other spells
#define REBIRTH 34342

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords fly[] = 
{
	{  },
	{ 337.864868f,  65.702301f, 33.171944f, 1.329919f },	// fly 1 to ...
	{ 391.245148f,  35.472462f, 36.886353f, 0.822089f },
	{ 392.959869f, -34.031734f, 33.630096f, 3.342431f },
	{ 321.277008f, -70.101540f, 43.650482f, 3.844443f },
    { 256.202911f,  -1.549352f, 44.718426f, 0.069512f },	// ... fly 5
	{ 342.090088f,  55.661064f, 27.638260f, 0.739368f },	// attack/tank positions from phase 1 from here to ...
	{ 392.815369f,  31.636963f, 25.414761f, 0.551340f },
	{ 388.397308f, -38.834595f, 22.335297f, 5.702067f },
	{ 333.922229f, -60.645069f, 24.484278f, 1.454599f },	// ... to here
	{ 328.103455f,  -0.192393f, 52.216309f, 4.188326f },	// fire quills cast position	// EMOTE_STATE_WHIRLWIND = 382,
	{ 326.225647f,   2.381837f, -2.389485f, 4.877070f }	// center of the room which is used in phase 2
};



class AlarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AlarAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    AlarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		/************************ Waypoint Place ************************/

		for (int i = 1; i < 12; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, RUN));	// FLY stucks my client
		}

		/************************** Spells ******************************/

		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(FLAME_BUFFET);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(FLAME_QUILLS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SUMMON_PHOENIX_ADDS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FLAME_PATCH);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(METEOR);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;	// =(
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(EMBER_BLAST);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(REBIRTH);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		/******************* Additional Settings *******************/

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->m_moveFly = true;

		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		FlameQuills = false;
		Meteor = false;
		Phase = 0;
        //RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
		_unit->GetAIInterface()->setWaypointToMove(0);

		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		CastTime();

		FlyWay = rand()%2;
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
		
		PositionChange=rand()%8+15;	// 30-45sec /*** if attack time 1000 (%15+31) ***/
		PhoenixSummon=rand()%6+17;	// 34-44sec /*** if attack time 1000 (%11+34) ***/
		FlameQuills = false;
		Meteor = false;
		Phase = 1;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
		_unit->GetAIInterface()->setWaypointToMove(0);

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->m_moveFly = true;
		CastTime();

		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		FlameQuills = false;
		Meteor = false;
		Phase = 0;

        //_unit->GetAIInterface()->StopMovement(0);
        //_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		//_unit->GetAIInterface()->setWaypointToMove(1);
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
		//float val = (float)RandomFloat(100.0f);
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
			uint32 val = RandomUInt(100);

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
			case 5:
			    {
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
					_unit->GetAIInterface()->setWaypointToMove(0);
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			    }break;

			case 6:
			    {
					//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
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
					//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
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
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
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
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
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
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
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

			default:
				{
				}
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

//------------------------------------ 
//    -= Kael'thas SunstriderEvent =- 
//------------------------------------ 

//Thaladred the Darkener AI(1st advisor)
#define CN_DARKENER 20064

#define PHYSBLOW 36966
#define SILENCE 29943

class DarkenerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DarkenerAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	DarkenerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(PHYSBLOW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		//spells[0].speech = "Physic blow";
		
		spells[1].info = dbcSpell.LookupEntry(SILENCE); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		//spells[1].speech = "Silence";

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	}
    void OnCombatStart(Unit* mTarget)
    {
		//door close
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Combat start");
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
	{
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "OnTargetDied1");
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
		Timer = Timer++;
		
		if (Timer == 30)
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
		}
	
		else
		{
			float val = RandomFloat(100.0f);
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
	int Timer;
};

//Lord Sanguinar AI(2nd advisor)
#define CN_SANGUINAR 20060

#define BELLOWING 22686

class SanguinarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SanguinarAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 
	
	SanguinarAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BELLOWING);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].speech = "Fear";

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

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
			float val = RandomFloat(100.0f);
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

//Grand Astromancer Capernian AI(3rd advisor)
#define CN_CAPERNIAN 20062

#define FIREBALL 36971
#define CONFLAGRATION 37018
#define ARCANE_EXPLOSION 40425 // SpellID is not correct, should be 4-5k damage + knockback. Can't find right one

class CapernianAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CapernianAI);
	SP_AI_Spell spells[3]; 
	bool m_spellcheck[3]; 
	
	CapernianAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 20.0f;
		spells[1].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(CONFLAGRATION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 20.0f;

		spells[2].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 20.0f;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

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
			float val = RandomFloat(100.0f);
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
//Master Engineer Telonicus AI(4th advisor)
#define CN_TELONICUS 20063

#define BOMB 37036
#define REMOTE_TOY 37027

class TelonicusAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TelonicusAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	TelonicusAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BOMB);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 0;

		spells[1].info = dbcSpell.LookupEntry(REMOTE_TOY);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

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
			float val = RandomFloat(100.0f);
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
//Phoenix Egg AI
#define CN_FLAMESTRIKE 21369

#define FLAMESTRIKE_TRIGGER 36730

class FlameStrikeAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FlameStrikeAI);
	FlameStrikeAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		DespawnTimer = 5;
	}

	void OnSpawn(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
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
		_unit->Despawn(0,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		DespawnTimer--;

		if(DespawnTimer == 3)
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAMESTRIKE_TRIGGER), false);
		if(!DespawnTimer)
		{
			_unit->RemoveAura(36730);
			_unit->Despawn(0,0);
		}
	}
	
protected:	

	int nrspells;
	int DespawnTimer;
};

//Phoenix AI
#define CN_PHOENIX 21362

#define BURN 36721
//#define REBIRTH 35369 //Already used in Al'Ar AI

class PhoenixAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PhoenixAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	PhoenixAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BURN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(REBIRTH);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;

		_unit->CastSpell(_unit, dbcSpell.LookupEntry(REBIRTH), false);
	}

	void OnSpawn(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);

		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
		Timer = 0;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(21364, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
		_unit->Despawn(0,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		Timer = Timer++;
		
		if (_unit->GetUInt32Value(UNIT_FIELD_HEALTH) >= 2500)
		{
			if (Timer%3 == 0)
			{
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (_unit->GetUInt32Value(UNIT_FIELD_HEALTH))-2500);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(BURN), false);
			}
		}
		else
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(21364, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
			_unit->Despawn(0,0);
			RemoveAIUpdateEvent();
		}

		if (Timer == 20)
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
		}
	
		else
		{
			float val = RandomFloat(100.0f);
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
	int Timer;
};

//Phoenix Egg AI
#define CN_PHOENIX_EGG 21364

class PhoenixEggAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PhoenixEggAI);
	PhoenixEggAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;

		DespawnTimer = 30;
	}

	void OnSpawn(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
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
		_unit->Despawn(0,0);
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		DespawnTimer--;

		if(!DespawnTimer)
		{
			Unit* Phoenix;
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Spawn Phoenix");
			Phoenix = _unit->GetMapMgr()->GetInterface()->SpawnCreature(21362, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
			_unit->Despawn(0,0);
		}
	}
	
protected:	

	int nrspells;
	int DespawnTimer;
};

//Prince Kael'Thas
#define CN_KAELTHAS 19622

#define FLAME_STRIKE 36731 //TODO: Flamestrike trigger spawn
#define PYROBLAST 38535
#define ARCANE_DIST 36834 
#define SHIELD 36815
#define MIND_CONTROL 36797
//#define GRAVITY1 34480 //knockback + aura
//#define GRAVITY2 35941 // explosion effect
#define NETHERBEAM 35873
#define SUMMON_WEAPONS 36976 //casting effect

#define REMOVE_INFUSER 39498
#define REMOVE_DEVASTATION 39499
#define REMOVE_INFINITY 39500
#define REMOVE_LONGBOW 39501
#define REMOVE_BULWARK 39502
#define REMOVE_STAFF 39503
#define REMOVE_SLICER 39504


class KaelThasAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KaelThasAI);
	SP_AI_Spell spells[9]; 
	bool m_spellcheck[9]; 
	
	KaelThasAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(FLAME_STRIKE); 
		spells[0].targettype = TARGET_DESTINATION; 
		spells[0].instant = true; 
		spells[0].cooldown = -1;
		spells[0].perctrigger = 5.0f; 
		spells[0].attackstoptimer = 1000; 
		//spells[0].speech = "Flame Strike"; //TODO - should be cast after the "warning"
		
		spells[1].info = dbcSpell.LookupEntry(PYROBLAST); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;
		//spells[1].speech = "Pyroblast";
		
		spells[2].info = dbcSpell.LookupEntry(ARCANE_DIST);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		//spells[2].speech = "Arcane Disturption";

		spells[3].info = dbcSpell.LookupEntry(SHIELD);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
		//spells[3].speech = "Schock Barrier";

		spells[4].info = dbcSpell.LookupEntry(MIND_CONTROL);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
		//spells[4].speech = "Mind Control";

		spells[5].info = dbcSpell.LookupEntry(NETHERBEAM);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		//spells[5].speech = "Nether Beam";

		spells[6].info = dbcSpell.LookupEntry(SUMMON_WEAPONS);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;
		//spells[6].speech = "Summon Weapons";

		// Remove Weapons Spells

		spells[7].info = dbcSpell.LookupEntry(REMOVE_INFUSER);
		//spells[7].speech = "Started removing weapons";

		spells[8].info = dbcSpell.LookupEntry(REMOVE_SLICER);
		//spells[8].speech = "Removing weapons finished";

		Timer = 0;
		addPhase = 1;
		addActive = 1;
		//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		//_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		
		

	}
    void OnCombatStart(Unit* mTarget)
    {
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Combat start");
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);
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
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not prevail.");
				_unit->PlaySoundToSet(11270);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You gambled...and lost.");
				_unit->PlaySoundToSet(11271);
				break;
			}
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		reset();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For...Quel...thalas!");
		
		_unit->PlaySoundToSet(11274);
		
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		Timer++;
		
		switch(addPhase)
		{
			case 1:
				FirstAd();
				break;
			case 2:
				SecondAd();
				break;
			case 3:
				ThirdAd();
				break;
			case 4:
				FourthAd();
				break;
			case 5:
				Phase2();
			default:
				float val = RandomFloat(100.0f);
				SpellCast(val);
				if (Timer >= 120)
				{
					SummonPhoenix();
					Timer = 0;
				}
				break;
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


	void Phase2()
	{
		if(addActive == 1)
		{
			Darkener->setDeathState(ALIVE);
			Darkener->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			Sanguinar->setDeathState(ALIVE);
			Sanguinar->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			Capernian->setDeathState(ALIVE);
			Capernian->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			Telonicus->setDeathState(ALIVE);
			Telonicus->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			addActive = 2;

			return;
		}

		if(!Darkener->isAlive() && !Sanguinar->isAlive() && !Capernian->isAlive() && !Telonicus->isAlive())
		{
			_unit->GetAIInterface()->m_canMove = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			addPhase = 6;
			addActive = 1;
		}
	}

	void reset()
	{
		if(!Darkener->isAlive())
		{
			Darkener->setDeathState(ALIVE);
			Darkener->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}
			Darkener->GetAIInterface()->MoveTo(Darkener->GetSpawnX(),Darkener->GetSpawnY(),Darkener->GetSpawnZ(),Darkener->GetSpawnO());
			Darkener->GetAIInterface()->SetAllowedToEnterCombat(false);
			Darkener->GetAIInterface()->m_canMove = false;
			Darkener->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);


		if(!Sanguinar->isAlive())
		{
			Sanguinar->setDeathState(ALIVE);
			Sanguinar->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}
			Sanguinar->GetAIInterface()->MoveTo(Sanguinar->GetSpawnX(),Sanguinar->GetSpawnY(),Sanguinar->GetSpawnZ(),Sanguinar->GetSpawnO());
			Sanguinar->GetAIInterface()->SetAllowedToEnterCombat(false);
			Sanguinar->GetAIInterface()->m_canMove = false;
			Sanguinar->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		


		if(!Capernian->isAlive())
		{
			Capernian->setDeathState(ALIVE);
			Capernian->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}
			Capernian->GetAIInterface()->MoveTo(Capernian->GetSpawnX(),Capernian->GetSpawnY(),Capernian->GetSpawnZ(),Capernian->GetSpawnO());
			Capernian->GetAIInterface()->SetAllowedToEnterCombat(false);
			Capernian->GetAIInterface()->m_canMove = false;
			Capernian->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		

		if(!Telonicus->isAlive())
		{
			Telonicus->setDeathState(ALIVE);
			Telonicus->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}
			Telonicus->GetAIInterface()->MoveTo(Telonicus->GetSpawnX(),Telonicus->GetSpawnY(),Telonicus->GetSpawnZ(),Telonicus->GetSpawnO());
			Telonicus->GetAIInterface()->SetAllowedToEnterCombat(false);
			Telonicus->GetAIInterface()->m_canMove = false;
			Telonicus->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

		Timer = 0;
		addPhase = 1;
		addActive = 1;
	}

	void FirstAd()
	{
		if(addActive == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let us see how your nerves hold up against the Darkener, Thaladred."); 
			_unit->PlaySoundToSet(11259);

			Darkener->GetAIInterface()->SetAllowedToEnterCombat(true);
			Darkener->GetAIInterface()->m_canMove = true;
			Darkener->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = NULL;
			target = Darkener->GetAIInterface()->GetNextTarget();
			Darkener->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
			addActive = 2;
		}

		if(!Darkener->isAlive())
		{
			addPhase = 2;
			addActive = 1;
		}
	}

	void SecondAd()
	{
		if(addActive == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have persevered against some of my best advisors. But none can withstand the might of the Bloodhammer. Behold, Lord Sanguinar."); 
			_unit->PlaySoundToSet(11260);

			Sanguinar->GetAIInterface()->SetAllowedToEnterCombat(true);
			Sanguinar->GetAIInterface()->m_canMove = true;
			Sanguinar->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = NULL;
			target = Sanguinar->GetAIInterface()->GetNextTarget();
			Sanguinar->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
			addActive = 2;
		}

		if(!Sanguinar->isAlive())
		{
			addPhase = 3;
			addActive = 1;
		}
	}

	void ThirdAd()
	{
		if(addActive == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Capernian will see to it that your stay here is a short one."); 
			_unit->PlaySoundToSet(11257);

			Capernian->GetAIInterface()->SetAllowedToEnterCombat(true);
			Capernian->GetAIInterface()->m_canMove = true;
			Capernian->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = NULL;
			target = Capernian->GetAIInterface()->GetNextTarget();
			Capernian->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
			addActive = 2;
		}

		if(!Capernian->isAlive())
		{
			addPhase = 4;
			addActive = 1;
		}
	}

	void FourthAd()
	{
		if(addActive == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Well done. You have proven worthy to test your skills against my Master Engineer, Telonicus."); 
			_unit->PlaySoundToSet(11258);

			Telonicus->GetAIInterface()->SetAllowedToEnterCombat(true);
			Telonicus->GetAIInterface()->m_canMove = true;
			Telonicus->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = NULL;
			target = Telonicus->GetAIInterface()->GetNextTarget();
			Telonicus->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
			addActive = 2;
		}

		if(!Telonicus->isAlive())
		{
			addPhase = 5;
			addActive = 1;
		}
	}

	void SummonPhoenix()
	{
		SummonTimer++;
		switch (SummonTimer)
			{
				case 1:
				{
					int RandomSpeach;
					RandomUInt(1000);
					RandomSpeach=rand()%2;
					switch (RandomSpeach)
					{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Anara'nel belore!");
						_unit->PlaySoundToSet(11265);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the power of the sun!");
						_unit->PlaySoundToSet(11267);
						break;
					}
					Unit *target = NULL;
					target = _unit->GetAIInterface()->GetMostHated(); //TODO
					SummonX = target->GetPositionX();
					SummonY = target->GetPositionY();
					SummonZ = target->GetPositionZ();
					SummonO = target->GetOrientation();
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAMESTRIKE, SummonX, SummonY, SummonZ, SummonO, false, false, 0, 0);
				}break;

				case 5:
				{
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PHOENIX, SummonX, SummonY, SummonZ, SummonO, false, false, 0, 0);
					Timer = 0;
				}break;
		}
	}


protected:	

	//Pointers
	Unit* Darkener;
	Unit* Sanguinar;
	Unit* Capernian;
	Unit* Telonicus;

	int nrspells;
	int Timer;
	int SummonTimer;
	float SummonX;
	float SummonY;
	float SummonZ;
	float SummonO;

	int addPhase;
	int addActive;
	};


void SetupTheEye(ScriptMgr * mgr)
{

	// Trashes
	mgr->register_creature_script(CN_A_STAR_SCRYER, &ASTARTSCRYERAI::Create);
	mgr->register_creature_script(CN_ASTROMANCER, &ASTROMANCERAI::Create);
	mgr->register_creature_script(CN_STAR_SCRYER, &STARTSCRYERAI::Create);
	mgr->register_creature_script(CN_ASTROMANCER_LORD, &ASTROMANCERLORD::Create);
	mgr->register_creature_script(CN_BLOOD_VINDICATOR, &SBLOODVINDICATORAI::Create);
	mgr->register_creature_script(CN_BLOOD_LEGIONNARE, &BLOODLEGIONNAREAI::Create);
	mgr->register_creature_script(CN_BLOOD_MARSHAL, &BLOODMARSHALAI::Create);
	mgr->register_creature_script(CN_PHOENIX_HAWK, &PHOENIXHAWKAI::Create);
	mgr->register_creature_script(CN_CRYSTAL_SENTINEL, &CRYSTALSENTINELAI::Create);
	mgr->register_creature_script(CN_CRYSTAL_MECHANIC, &CRYSTALMECHANICAI::Create);
	// Bosses
	mgr->register_creature_script(CN_VOID_REAVER, &VOIDREAVERAI::Create);
	mgr->register_creature_script(CN_HIGH_ASTROMANCER_SOLARIAN, &HIGHASTROMANCERSOLARIANAI::Create);
	mgr->register_creature_script(CN_ALAR, &AlarAI::Create);

	//Kael'Thas Encounter
	mgr->register_creature_script(CN_PHOENIX, &PhoenixAI::Create);
	mgr->register_creature_script(CN_PHOENIX_EGG, &PhoenixEggAI::Create);
	mgr->register_creature_script(CN_FLAMESTRIKE, &FlameStrikeAI::Create);
	mgr->register_creature_script(CN_DARKENER, &DarkenerAI::Create);
	mgr->register_creature_script(CN_SANGUINAR, &SanguinarAI::Create);
	mgr->register_creature_script(CN_CAPERNIAN, &CapernianAI::Create);
	mgr->register_creature_script(CN_TELONICUS, &TelonicusAI::Create);
	mgr->register_creature_script(CN_KAELTHAS, &KaelThasAI::Create);

}
