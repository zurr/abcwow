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

#include "Base.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class SpellDesc
SpellDesc::SpellDesc(SpellEntry* pInfo, SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	mInfo = pInfo;
	mSpellFunc = pFnc;
	mTargetType = pTargetType;
	mChance = max(min(pChance, 100.0f), 0.0f);
	mCastTime = pCastTime;
	mCooldown = pCooldown;
	mMinRange = pMinRange;
	mMaxRange = pMaxRange;
	mStrictRange = pStrictRange;
	mEnabled = true;
	mPredefinedTarget = NULL;
	mLastCastTime = 0;
	AddEmote(pText, pTextType, pSoundId);
}

SpellDesc::~SpellDesc()
{
	DeleteArray(mEmotes);
}

EmoteDesc* SpellDesc::AddEmote(const char* pText, TextType pType, uint32 pSoundId)
{
	EmoteDesc* NewEmote = NULL;
	if( pText || pSoundId )
	{
		NewEmote = new EmoteDesc(pText, pType, pSoundId);
		mEmotes.push_back(NewEmote);
	}
	return NewEmote;
}

void SpellDesc::TriggerCooldown(uint32 pCurrentTime)
{
	uint32 CurrentTime = ( pCurrentTime == 0 ) ? (uint32)time(NULL) : pCurrentTime;
	mLastCastTime = CurrentTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class MoonScriptCreatureAI
MoonScriptCreatureAI::MoonScriptCreatureAI(Creature* pCreature) : CreatureAIScript(pCreature)
{
	mRunToTargetCache = NULL;
	mRunToTargetSpellCache = NULL;
	mTimerIdCounter = 0;
	mAIUpdateFrequency = DEFAULT_UPDATE_FREQUENCY;
	mBaseAttackTime = _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
	mDespawnWhenInactive = false;
}

MoonScriptCreatureAI::~MoonScriptCreatureAI()
{
	DeleteArray(mOnDiedEmotes);
	DeleteArray(mOnTargetDiedEmotes);
	DeleteArray(mOnCombatStartEmotes);
	DeleteArray(mOnTauntEmotes);
	DeleteArray(mSpells);
}

bool MoonScriptCreatureAI::GetCanMove()
{
	return _unit->GetAIInterface()->m_canMove;
}

void MoonScriptCreatureAI::SetCanMove(bool pCanMove)
{
	if( pCanMove ) _unit->Unroot();
	else _unit->Root();
}

void MoonScriptCreatureAI::MoveTo(MoonScriptCreatureAI* pCreature)
{
	MoveTo(pCreature->_unit);
}

void MoonScriptCreatureAI::MoveTo(Unit* pUnit)
{
	MoveTo(pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ());
}

void MoonScriptCreatureAI::MoveTo(float pX, float pY, float pZ, bool pRun)
{
	_unit->GetAIInterface()->MoveTo(pX, pY, pZ, 0);
	if (pRun)
		_unit->GetAIInterface()->m_moveRun = true;
}

void MoonScriptCreatureAI::MoveToSpawnOrigin()
{
	MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ());
}

void MoonScriptCreatureAI::StopMovement()
{
	_unit->GetAIInterface()->StopMovement(0);
}

void MoonScriptCreatureAI::SetFlyMode(bool pValue)
{
	if( pValue && !_unit->GetAIInterface()->m_moveFly )
	{
		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = true;
	}
	else if( !pValue && _unit->GetAIInterface()->m_moveFly )
	{
		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = false;
	}
}

bool MoonScriptCreatureAI::GetCanEnterCombat()
{
	return _unit->GetAIInterface()->GetAllowedToEnterCombat();
}

void MoonScriptCreatureAI::SetCanEnterCombat(bool pCanEnterCombat)
{
	_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( pCanEnterCombat ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
	_unit->GetAIInterface()->SetAllowedToEnterCombat(pCanEnterCombat);
}

bool MoonScriptCreatureAI::IsInCombat()
{
	return _unit->CombatStatus.IsInCombat();
}

void MoonScriptCreatureAI::DelayNextAttack(int32 pMilliseconds)
{
	_unit->setAttackTimer(pMilliseconds, false);
}

void MoonScriptCreatureAI::SetDespawnWhenInactive(bool pValue)
{
	mDespawnWhenInactive = pValue;
}

void MoonScriptCreatureAI::SetBehavior(BehaviorType pBehavior)
{
	switch( pBehavior )
	{
		case Behavior_Default:		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL); break;
		case Behavior_Melee:		_unit->GetAIInterface()->setCurrentAgent(AGENT_MELEE); break;
		case Behavior_Ranged:		_unit->GetAIInterface()->setCurrentAgent(AGENT_RANGED); break;
		case Behavior_Spell:		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL); break;
		case Behavior_Flee:			_unit->GetAIInterface()->setCurrentAgent(AGENT_FLEE); break;
		case Behavior_CallForHelp:	_unit->GetAIInterface()->setCurrentAgent(AGENT_CALLFORHELP); break;
		default:					sLog.outDebug("MoonScriptCreatureAI::SetBehavior() : Invalid behavior type!\n"); break;
	}
}

BehaviorType MoonScriptCreatureAI::GetBehavior()
{
	switch( _unit->GetAIInterface()->getCurrentAgent() )
	{
		case AGENT_NULL:		return Behavior_Default;
		case AGENT_MELEE:		return Behavior_Melee;
		case AGENT_RANGED:		return Behavior_Ranged;
		case AGENT_FLEE:		return Behavior_Flee;
		case AGENT_SPELL:		return Behavior_Spell;
		case AGENT_CALLFORHELP:	return Behavior_CallForHelp;
		default:				sLog.outDebug("MoonScriptCreatureAI::SetBehavior() : Invalid behavior type!\n"); return Behavior_Default;
	}
}

void MoonScriptCreatureAI::SetAllowMelee(bool pAllow)
{
	_unit->GetAIInterface()->disable_melee = !pAllow;
}

bool MoonScriptCreatureAI::GetAllowMelee()
{
	return !_unit->GetAIInterface()->disable_melee;
}

void MoonScriptCreatureAI::SetAllowRanged(bool pAllow)
{
	_unit->GetAIInterface()->disable_ranged = !pAllow;
}

bool MoonScriptCreatureAI::GetAllowRanged()
{
	return !_unit->GetAIInterface()->disable_ranged;
}

void MoonScriptCreatureAI::SetAllowSpell(bool pAllow)
{
	_unit->GetAIInterface()->disable_spell = !pAllow;
}

bool MoonScriptCreatureAI::GetAllowSpell()
{
	return !_unit->GetAIInterface()->disable_spell;
}

void MoonScriptCreatureAI::SetAllowTargeting(bool pAllow)
{
	_unit->GetAIInterface()->disable_targeting = !pAllow;
}

bool MoonScriptCreatureAI::GetAllowTargeting()
{
	return !_unit->GetAIInterface()->disable_targeting;
}

void MoonScriptCreatureAI::AggroNearestUnit(int pInitialThreat)
{
	Unit* NearestRandomTarget = GetBestUnitTarget(TargetFilter_Closest);
	if( NearestRandomTarget )
	{
		_unit->GetAIInterface()->AttackReaction(NearestRandomTarget, pInitialThreat);
		OnCombatStart(NearestRandomTarget);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void MoonScriptCreatureAI::AggroRandomUnit(int pInitialThreat)
{
	Unit* RandomTarget = GetBestUnitTarget();
	if( RandomTarget )
	{
		_unit->GetAIInterface()->AttackReaction(RandomTarget, pInitialThreat);
		OnCombatStart(RandomTarget);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void MoonScriptCreatureAI::AggroNearestPlayer(int pInitialThreat)
{
	Unit* NearestRandomPlayer = GetBestPlayerTarget(TargetFilter_Closest);
	if( NearestRandomPlayer )
	{
		_unit->GetAIInterface()->AttackReaction(NearestRandomPlayer, pInitialThreat);
		OnCombatStart(NearestRandomPlayer);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void MoonScriptCreatureAI::AggroRandomPlayer(int pInitialThreat)
{
	Unit* RandomPlayer = GetBestPlayerTarget();
	if( RandomPlayer )
	{
		_unit->GetAIInterface()->AttackReaction(RandomPlayer, pInitialThreat);
		OnCombatStart(RandomPlayer);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void MoonScriptCreatureAI::ClearHateList()
{
	_unit->ClearHateList();	//without leaving combat
}

void MoonScriptCreatureAI::WipeHateList()
{
	_unit->WipeHateList();
}

int32 MoonScriptCreatureAI::GetHealthPercent()
{
	return _unit->GetHealthPct();
}

int32 MoonScriptCreatureAI::GetManaPercent()
{
	return _unit->GetManaPct();
}

void MoonScriptCreatureAI::Regenerate()
{
	_unit->RegenerateHealth();
	_unit->RegeneratePower(false);
}

bool MoonScriptCreatureAI::IsAlive()
{
	return _unit->isAlive();
}

void MoonScriptCreatureAI::SetScale(float pScale)
{
	_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, pScale);
}

float MoonScriptCreatureAI::GetScale()
{
	return _unit->GetFloatValue(OBJECT_FIELD_SCALE_X);
}

void MoonScriptCreatureAI::SetDisplayId(uint32 pDisplayId)
{
	_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, pDisplayId);
}

void MoonScriptCreatureAI::SetWieldWeapon(bool pValue)
{
	if( pValue && _unit->GetUInt32Value(UNIT_FIELD_BYTES_2) != 1 )
	{
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
	}
	else if( !pValue && _unit->GetUInt32Value(UNIT_FIELD_BYTES_2) != 0 )
	{
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
	}
}

void MoonScriptCreatureAI::SetDisplayWeapon(bool pMainHand, bool pOffHand)
{
	SetDisplayWeaponIds(pMainHand ? _unit->GetProto()->Item1SlotDisplay : 0, pMainHand ? _unit->GetProto()->Item1Info1 : 0, pMainHand ? _unit->GetProto()->Item1Info2 : 0,
		pOffHand ? _unit->GetProto()->Item2SlotDisplay : 0, pOffHand ? _unit->GetProto()->Item2Info1 : 0, pOffHand ? _unit->GetProto()->Item2Info2 : 0);
}

void MoonScriptCreatureAI::SetDisplayWeaponIds(uint32 pItem1Id, uint32 pItem1Info, uint32 pItem1Slot, uint32 pItem2Id, uint32 pItem2Info, uint32 pItem2Slot)
{
	//Main Hand
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, pItem1Id);
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, pItem1Info);
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, pItem1Slot);

	//Off Hand
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, pItem2Id);
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, pItem2Info);
	_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, pItem2Slot);
}

float MoonScriptCreatureAI::GetRange(MoonScriptCreatureAI* pCreature)
{
	return ( _unit != pCreature->_unit ) ? _unit->CalcDistance(pCreature->_unit) : 0;
}

float MoonScriptCreatureAI::GetRangeToUnit(Unit* pUnit)
{
	return _unit->CalcDistance(pUnit);
}

GameObject* MoonScriptCreatureAI::GetNearestGameObject(uint32 pGameObjectId)
{
	return _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), pGameObjectId);
}

MoonScriptCreatureAI* MoonScriptCreatureAI::GetNearestCreature(uint32 pCreatureId)
{
	Creature* NearestCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), pCreatureId);
	return ( NearestCreature ) ? static_cast<MoonScriptCreatureAI*>(NearestCreature->GetScript()) : NULL;
}

MoonScriptCreatureAI* MoonScriptCreatureAI::SpawnCreature(uint32 pCreatureId, bool pForceSameFaction)
{
	return SpawnCreature(pCreatureId, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), pForceSameFaction);
}

MoonScriptCreatureAI* MoonScriptCreatureAI::SpawnCreature(uint32 pCreatureId, float pX, float pY, float pZ, float pO, bool pForceSameFaction)
{
	Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(pCreatureId, pX, pY, pZ, pO, false, false, 0, 0);
	MoonScriptCreatureAI* CreatureScriptAI = ( NewCreature ) ? static_cast<MoonScriptCreatureAI*>(NewCreature->GetScript()) : NULL;
	if( pForceSameFaction && NewCreature )
	{
		uint32 FactionTemplate = _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE);
		NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, FactionTemplate);
	}
	return CreatureScriptAI;
}

Unit* MoonScriptCreatureAI::ForceCreatureFind(uint32 pCreatureId, float pX, float pY, float pZ, bool pCurrentPosition)
{
	Unit *pUnit = NULL;
	MapMgr *pMapMgr = _unit->GetMapMgr();
	if (pMapMgr == NULL)
		return pUnit;

	if (pCurrentPosition)
		pUnit = pMapMgr->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), pCreatureId);
	else
		pUnit = pMapMgr->GetInterface()->GetCreatureNearestCoords(pX, pY, pZ, pCreatureId);

	if (pUnit == NULL)
	{
		for (uint32 i = 0; i != pMapMgr->m_CreatureHighGuid; ++i)
		{
			if (pMapMgr->m_CreatureStorage[i] != NULL)
			{ 
				pUnit = pMapMgr->m_CreatureStorage[i];
				if (pUnit->GetEntry() == pCreatureId)
					return pUnit;

				pUnit = NULL;
			}
		}
	}

	return pUnit;
}

void MoonScriptCreatureAI::Despawn(uint32 pDelay, uint32 pRespawnTime)
{
	_unit->Despawn(pDelay, pRespawnTime);
}

SpellDesc* MoonScriptCreatureAI::AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	//Cannot add twice same spell id	- M4ksiu: Disabled, until I rewrite SetPhase(...) function to not disable same spells that are in different phases
	//SpellDesc* NewSpell = FindSpellById(pSpellId);
	//if( NewSpell ) return NewSpell;
	SpellDesc* NewSpell = NULL;

	//Find spell info from spell id
	SpellEntry* Info = dbcSpell.LookupEntry(pSpellId);

#ifdef USE_DBC_SPELL_INFO
	float CastTime = ( Info->CastingTimeIndex ) ? GetCastTime(dbcSpellCastTime.LookupEntry(Info->CastingTimeIndex)) : pCastTime;
	int32 Cooldown = Info->RecoveryTime;
	float MinRange = ( Info->rangeIndex ) ? GetMinRange(dbcSpellRange.LookupEntry(Info->rangeIndex)) : pMinRange;
	float MaxRange = ( Info->rangeIndex ) ? GetMaxRange(dbcSpellRange.LookupEntry(Info->rangeIndex)) : pMaxRange;
	sLog.outColor(TYELLOW, "MoonScriptCreatureAI::AddSpell(%u) : casttime=%.1f cooldown=%d minrange=%.1f maxrange=%.1f\n", pSpellId, CastTime, Cooldown, MinRange, MaxRange);
#else
	float CastTime = pCastTime;
	int32 Cooldown = pCooldown;
	float MinRange = pMinRange;
	float MaxRange = pMaxRange;
#endif

	//Create new spell
	NewSpell = new SpellDesc(Info, NULL, pTargetType, pChance, CastTime, Cooldown, MinRange, MaxRange, pStrictRange, pText, pTextType, pSoundId);
	mSpells.push_back(NewSpell);
	return NewSpell;
}

SpellDesc* MoonScriptCreatureAI::AddSpellFunc(SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	if( !pFnc ) return NULL;

	//Create new spell
	SpellDesc* NewSpell = new SpellDesc(NULL, pFnc, pTargetType, pChance, pCastTime, pCooldown, pMinRange, pMaxRange, pStrictRange, pText, pTextType, pSoundId);
	mSpells.push_back(NewSpell);
	return NewSpell;
}

void MoonScriptCreatureAI::CastSpell(SpellDesc* pSpell)
{
	if( !IsSpellScheduled(pSpell) ) mQueuedSpells.push_back(pSpell);
}

void MoonScriptCreatureAI::CastSpellNowNoScheduling(SpellDesc* pSpell)
{
	if( CastSpellInternal(pSpell) )
	{
		DelayNextAttack(CalcSpellAttackTime(pSpell));
	}
}

SpellDesc* MoonScriptCreatureAI::FindSpellById(uint32 pSpellId)
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		if( (*SpellIter)->mInfo && (*SpellIter)->mInfo->Id == pSpellId ) return (*SpellIter);
	}
	return NULL;
}

SpellDesc* MoonScriptCreatureAI::FindSpellByFunc(SpellFunc pFnc)
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		if( (*SpellIter)->mSpellFunc == pFnc ) return (*SpellIter);
	}
	return NULL;
}

bool MoonScriptCreatureAI::IsCasting()
{
	return _unit->isCasting();
}

void MoonScriptCreatureAI::ApplyAura(uint32 pSpellId)
{
	_unit->CastSpell(_unit, dbcSpell.LookupEntry(pSpellId), true);
}

void MoonScriptCreatureAI::RemoveAura(uint32 pSpellId)
{
	_unit->RemoveAura(pSpellId);
}

void MoonScriptCreatureAI::RemoveAuraOnPlayers(uint32 pSpellId)
{
	for( set<Player*>::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{
		(*PlayerIter)->RemoveAura(pSpellId);
	}
}

void MoonScriptCreatureAI::RemoveAllAuras()
{
	_unit->RemoveAllAuras();
}

void MoonScriptCreatureAI::TriggerCooldownOnAllSpells()
{
	uint32 CurrentTime = (uint32)time(NULL);
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		(*SpellIter)->TriggerCooldown(CurrentTime);
	}
}

void MoonScriptCreatureAI::CancelAllCooldowns()
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		(*SpellIter)->mLastCastTime = 0;
	}
}

EmoteDesc* MoonScriptCreatureAI::AddEmote(EventType pEventType, const char* pText, TextType pType, uint32 pSoundId)
{
	EmoteDesc* NewEmote = NULL;
	if( pText || pSoundId )
	{
		NewEmote = new EmoteDesc(pText, pType, pSoundId);
		switch( pEventType )
		{
			case Event_OnCombatStart:	mOnCombatStartEmotes.push_back(NewEmote); break;
			case Event_OnTargetDied:	mOnTargetDiedEmotes.push_back(NewEmote); break;
			case Event_OnDied:			mOnDiedEmotes.push_back(NewEmote); break;
			case Event_OnTaunt:			mOnTauntEmotes.push_back(NewEmote); break;
			default:					sLog.outDebug("MoonScriptCreatureAI::AddEmote() : Invalid event type!\n"); break;
		}
	}
	return NewEmote;
}

void MoonScriptCreatureAI::RemoveEmote(EventType pEventType, EmoteDesc* pEmote)
{
	switch( pEventType )
	{
		case Event_OnCombatStart:	DeleteItem(mOnCombatStartEmotes, pEmote); break;
		case Event_OnTargetDied:	DeleteItem(mOnTargetDiedEmotes, pEmote); break;
		case Event_OnDied:			DeleteItem(mOnDiedEmotes, pEmote); break;
		case Event_OnTaunt:			DeleteItem(mOnTauntEmotes, pEmote); break;
		default:					sLog.outDebug("MoonScriptCreatureAI::RemoveEmote() : Invalid event type!\n"); break;
	}
}

void MoonScriptCreatureAI::RemoveAllEmotes(EventType pEventType)
{
	switch( pEventType )
	{
		case Event_OnCombatStart:	DeleteArray(mOnCombatStartEmotes); break;
		case Event_OnTargetDied:	DeleteArray(mOnTargetDiedEmotes); break;
		case Event_OnDied:			DeleteArray(mOnDiedEmotes); break;
		case Event_OnTaunt:			DeleteArray(mOnTauntEmotes); break;
		default:					sLog.outDebug("MoonScriptCreatureAI::RemoveAllEmotes() : Invalid event type!\n"); break;
	}
}

void MoonScriptCreatureAI::Emote(EmoteDesc* pEmote)
{
	if( pEmote ) Emote(pEmote->mText.c_str(), pEmote->mType, pEmote->mSoundId);
}

void MoonScriptCreatureAI::Emote(const char* pText, TextType pType, uint32 pSoundId)
{
	if( pText && strlen(pText) > 0 )
	{
		switch( pType )
		{
			case Text_Say:		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, pText); break;
			case Text_Yell:		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, pText); break;
			case Text_Emote:	_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, pText); break;
			default:			sLog.outDebug("MoonScriptCreatureAI::Emote() : Invalid text type!\n"); break;
		}
	}
	if( pSoundId > 0 ) _unit->PlaySoundToSet(pSoundId);
}

uint32 MoonScriptCreatureAI::AddTimer(int32 pDurationMillisec)
{
	int32 Index = mTimerIdCounter++;
	mTimers.push_back(std::make_pair(Index, pDurationMillisec));
	return Index;
}

int32 MoonScriptCreatureAI::GetTimer(int32 pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			return TimerIter->second;
		}
	}

	return 0;
}

void MoonScriptCreatureAI::RemoveTimer(int32& pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			mTimers.erase(TimerIter);
			pTimerId = INVALIDATE_TIMER;
			break;
		}
	}
}

void MoonScriptCreatureAI::ResetTimer(int32 pTimerId, int32 pDurationMillisec)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			TimerIter->second = pDurationMillisec;
			break;
		}
	}
}

bool MoonScriptCreatureAI::IsTimerFinished(int32 pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId ) return ( TimerIter->second <= 0 ) ? true : false;
	}
	return false;
}

void MoonScriptCreatureAI::CancelAllTimers()
{
	mTimers.clear();
	mTimerIdCounter = 0;
}

void MoonScriptCreatureAI::SetTargetToChannel(Unit* pTarget, uint32 pSpellId)
{
	if (pTarget == NULL)
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
	else
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, pTarget->GetGUID());

	_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, pSpellId);
}

Unit* MoonScriptCreatureAI::GetTargetToChannel()
{

	return _unit->GetMapMgr()->GetUnit(_unit->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
}

void MoonScriptCreatureAI::SetAIUpdateFreq(uint32 pUpdateFreq)
{
	if( mAIUpdateFrequency != pUpdateFreq )
	{
		mAIUpdateFrequency = pUpdateFreq;
		ModifyAIUpdateEvent(mAIUpdateFrequency);
	}
}

uint32 MoonScriptCreatureAI::GetAIUpdateFreq()
{
	return mAIUpdateFrequency;
}

void MoonScriptCreatureAI::AddLootToTable(LootTable* pTable, uint32 pItemID, uint32 pChance, uint32 pMinCount, uint32 pMaxCount, uint32 pFFA)
{
	LootDesc loot;
	loot.mItemID = pItemID;
	loot.mChance = pChance;
	loot.mMinCount = pMinCount;
	loot.mMaxCount = pMaxCount;
	loot.mFFA = pFFA;
	pTable->push_back(loot);
}

void MoonScriptCreatureAI::ClearLoot(Unit* pTarget)
{
	pTarget->loot.items.clear();
	pTarget->loot.gold = 0;
}

void MoonScriptCreatureAI::AddLootFromTable(Unit* pTarget, LootTable* pTable, uint32 pCount)
{
	uint32 total = 0;
	for (LootTable::iterator it = pTable->begin(); it != pTable->end(); ++it) total += (*it).mChance;
	for (uint32 count = 0; count < pCount; ++count)
	{
		uint32 result = RandomUInt(total);
		uint32 sum = 0;
		for (LootTable::iterator it = pTable->begin(); it != pTable->end(); ++it)
		{
			sum += (*it).mChance;
			if (result <= sum)
			{
				LootMgr::getSingleton().AddLoot(&pTarget->loot, (*it).mItemID, (*it).mMinCount, (*it).mMaxCount, (*it).mFFA);
				break;
			}
		}
	}
}

void MoonScriptCreatureAI::SetGoldLoot(Unit* pTarget, uint32 pMinGold, uint32 pMaxGold)
{
	pTarget->loot.gold = RandomUInt(pMaxGold - pMinGold) + pMinGold;
}

void MoonScriptCreatureAI::AddLoot(Unit* pTarget, uint32 pItemID, uint32 pMinCount, uint32 pMaxCount, uint32 pFFA)
{
	LootMgr::getSingleton().AddLoot(&pTarget->loot, pItemID, pMinCount, pMaxCount, pFFA);
}

void MoonScriptCreatureAI::AddRareLoot(Unit* pTarget, uint32 pItemID, float pPercentChance)
{
	float result = RandomFloat(100.0f);
	if (result <= pPercentChance)
		LootMgr::getSingleton().AddLoot(&pTarget->loot, pItemID, 1, 1, 0);
}

WayPoint* MoonScriptCreatureAI::CreateWaypoint(int pId, uint32 pWaittime, uint32 pMoveFlag, Coords pCoords)
{
	WayPoint * wp = _unit->CreateWaypointStruct();
	wp->id = pId;
	wp->x = pCoords.mX;
	wp->y = pCoords.mY;
	wp->z = pCoords.mZ;
	wp->o = pCoords.mO;
	wp->waittime = pWaittime;
	wp->flags = pMoveFlag;
	wp->forwardemoteoneshot = 0;
	wp->forwardemoteid = 0;
	wp->backwardemoteoneshot = 0;
	wp->backwardemoteid = 0;
	wp->forwardskinid = 0;
	wp->backwardskinid = 0;
	return wp;
}

void MoonScriptCreatureAI::AddWaypoint(WayPoint* pWayPoint)
{
	_unit->GetAIInterface()->addWayPoint(pWayPoint);
}

void MoonScriptCreatureAI::ForceWaypointMove(uint32 pWaypointId)
{
	if (GetCanEnterCombat())
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	if (!GetCanMove())
		SetCanMove(true);

	_unit->GetAIInterface()->StopMovement(0);
	_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
	_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
	_unit->GetAIInterface()->setWaypointToMove(pWaypointId);
}

void MoonScriptCreatureAI::SetWaypointToMove(uint32 pWaypointId)
{
	_unit->GetAIInterface()->setWaypointToMove(pWaypointId);
}

void MoonScriptCreatureAI::StopWaypointMovement()
{
	_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
	_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
	_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
	_unit->GetAIInterface()->setWaypointToMove(0);
}

void MoonScriptCreatureAI::SetMoveType(MoveType pMoveType)
{
	switch( pMoveType )
	{
		case Move_None:				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_NONE); break;
		case Move_RandomWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_RANDOMWP); break;
		case Move_CircleWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_CIRCLEWP); break;
		case Move_WantedWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP); break;
		case Move_DontMoveWP:		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP); break;
		case Move_Quest:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_QUEST); break;
		case Move_ForwardThenStop:	_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP); break;
		default:					sLog.outDebug("MoonScriptCreatureAI::SetMoveType() : Invalid move type!\n"); break;
	}
}

MoveType MoonScriptCreatureAI::GetMoveType()
{
	switch( _unit->GetAIInterface()->getMoveType() )
	{
		case MOVEMENTTYPE_NONE:				return Move_None;
		case MOVEMENTTYPE_RANDOMWP:			return Move_RandomWP;
		case MOVEMENTTYPE_CIRCLEWP:			return Move_CircleWP;
		case MOVEMENTTYPE_WANTEDWP:			return Move_WantedWP;
		case MOVEMENTTYPE_DONTMOVEWP:		return Move_DontMoveWP;
		case MOVEMENTTYPE_QUEST:			return Move_Quest;
		case MOVEMENTTYPE_FORWARDTHANSTOP:	return Move_ForwardThenStop;
		default:							sLog.outDebug("MoonScriptCreatureAI::GetMoveType() : Invalid move type!\n"); return Move_None;
	}
}

uint32 MoonScriptCreatureAI::GetCurrentWaypoint()
{
	return _unit->GetAIInterface()->getCurrentWaypoint();
}

size_t MoonScriptCreatureAI::GetWaypointCount()
{
	return _unit->GetAIInterface()->GetWayPointsCount();
}

void MoonScriptCreatureAI::OnCombatStart(Unit* pTarget)
{
	RandomEmote(mOnCombatStartEmotes);
	SetBehavior(Behavior_Melee);
    RegisterAIUpdateEvent(mAIUpdateFrequency);
}

void MoonScriptCreatureAI::OnCombatStop(Unit* pTarget)
{
	CancelAllSpells();
	CancelAllTimers();
	RemoveAllAuras();
	SetCanMove(true);
	SetBehavior(Behavior_Default);
	//_unit->GetAIInterface()->SetAIState(STATE_IDLE);				// Fix for stucking mobs that don't regen
	RemoveAIUpdateEvent();
	if( mDespawnWhenInactive ) Despawn(DEFAULT_DESPAWN_TIMER);
}

void MoonScriptCreatureAI::OnTargetDied(Unit* pTarget)
{
	if( GetHealthPercent() > 0 )	//Prevent double yelling (OnDied and OnTargetDied)
	{
		RandomEmote(mOnTargetDiedEmotes);
	}
}

void MoonScriptCreatureAI::OnDied(Unit* pKiller)
{
	RandomEmote(mOnDiedEmotes);
	CancelAllSpells();
	CancelAllTimers();
	RemoveAllAuras();
	RemoveAIUpdateEvent();
	if( mDespawnWhenInactive ) Despawn(DEFAULT_DESPAWN_TIMER);
}

void MoonScriptCreatureAI::AIUpdate()
{
	SpellDesc*	Spell;
	uint32		CurrentTime = (uint32)time(NULL);

	//Elapse timers
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		TimerIter->second -= mAIUpdateFrequency;
	}

	//Check if we have a spell scheduled to be cast
	for( SpellDescList::iterator SpellIter = mScheduledSpells.begin(); SpellIter != mScheduledSpells.end(); ++SpellIter )
	{
		Spell = (*SpellIter);
		if( CastSpellInternal(Spell, CurrentTime) )	//Can fail if we are already casting a spell, or if the spell is on cooldown
		{
			mScheduledSpells.erase(SpellIter);
			break;
		}
	}

	//Do not schedule spell if we are *currently* casting a non-instant cast spell
	if( !IsCasting() && !mRunToTargetCache )
	{
		//Check if have queued spells that needs to be scheduled before we go back to random casting
		if( !mQueuedSpells.empty() )
		{
			Spell = mQueuedSpells.front();
			mScheduledSpells.push_back(Spell);
			mQueuedSpells.pop_front();

			//Stop melee attack for a short while for scheduled spell cast
			if( Spell->mCastTime >= 0 )
			{
				DelayNextAttack(mAIUpdateFrequency + CalcSpellAttackTime(Spell));
				if( Spell->mCastTime > 0 )
				{
					SetCanMove(false);
					SetBehavior(Behavior_Spell);
				}
			}
			return;	//Scheduling one spell at a time, exit now
		}

		//Try our chance at casting a spell (Will actually be cast on next ai update, so we just
		//schedule it. This is needed to avoid next dealt melee damage while we cast the spell.)
		float ChanceRoll = RandomFloat(100), ChanceTotal = 0;
		for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
		{
			Spell = (*SpellIter);
			if( Spell->mEnabled == false ) continue;
			if( Spell->mChance == 0 ) continue;

			//Check if spell won the roll
			if( (Spell->mChance == 100 || (ChanceRoll >= ChanceTotal && ChanceRoll < ChanceTotal + Spell->mChance)) &&
				(Spell->mLastCastTime + Spell->mCooldown <= CurrentTime) &&
				!IsSpellScheduled(Spell) )
			{
				mScheduledSpells.push_back(Spell);

				//Stop melee attack for a short while for scheduled spell cast
				if( Spell->mCastTime >= 0 )
				{
					DelayNextAttack(mAIUpdateFrequency + CalcSpellAttackTime(Spell));
					if( Spell->mCastTime > 0 )
					{
						SetCanMove(false);
						SetBehavior(Behavior_Spell);
					}
				}
				return;	//Scheduling one spell at a time, exit now
			}
			else if( Spell->mChance != 100 ) ChanceTotal += Spell->mChance;	//Only add spells that aren't 100% chance of casting
		}

		//Go back to default behavior since we didn't decide anything
		SetCanMove(true);
		SetBehavior(Behavior_Melee);

		//Random taunts
		if( ChanceRoll >= 95 ) RandomEmote(mOnTauntEmotes);
	}
}

bool MoonScriptCreatureAI::IsSpellScheduled(SpellDesc* pSpell)
{
	return ( std::find(mScheduledSpells.begin(), mScheduledSpells.end(), pSpell) == mScheduledSpells.end() ) ? false : true;
}

void MoonScriptCreatureAI::CancelAllSpells()
{
	mQueuedSpells.clear();
	mScheduledSpells.clear();
	PopRunToTargetCache();
}

bool MoonScriptCreatureAI::CastSpellInternal(SpellDesc* pSpell, uint32 pCurrentTime)
{
	//Do not cast if we are already casting
	if( IsCasting() ) return false;

	//Do not cast if we are in cooldown
	uint32 CurrentTime = ( pCurrentTime == 0 ) ? (uint32)time(NULL) : pCurrentTime;
	if( pSpell->mLastCastTime + pSpell->mCooldown > CurrentTime ) return false;

	//Check range before casting
	Unit* Target = GetTargetForSpell(pSpell);
	if( Target )
	{
		if( IsUnitInRange(Target, pSpell) )
		{
			//If we were running to a target, stop because we're in range now
			PopRunToTargetCache();

			//Do emote associated with this spell
			RandomEmote(pSpell->mEmotes);

			//Cast spell now
			if( pSpell->mInfo ) CastSpellOnTarget(Target, pSpell->mTargetType, pSpell->mInfo, ( pSpell->mCastTime == 0 ) ? true : false);
			else if( pSpell->mSpellFunc ) pSpell->mSpellFunc(pSpell, this, Target, pSpell->mTargetType);
			else sLog.outDebug("MoonScriptCreatureAI::CastSpellInternal() : Invalid spell!\n");

			//Store cast time for cooldown
			pSpell->mLastCastTime = CurrentTime;
			return true;
		}
		else if( !pSpell->mStrictRange ) //Target is out of range, run to it
		{
			PushRunToTargetCache(Target, pSpell);
			return false;
		}
	}

	//If we get here, its because the RunToTarget changed type, so its no longer valid, clear it
	PopRunToTargetCache();
	DelayNextAttack(0);		//Cancel attack delay
	return true;			//No targets possible? Consider spell casted nonetheless
}

void MoonScriptCreatureAI::CastSpellOnTarget(Unit* pTarget, TargetType pType, SpellEntry* pEntry, bool pInstant)
{
	switch( pType )
	{
		case Target_Self:
		case Target_Current:
		case Target_SecondMostHated:
		case Target_Predefined:
		case Target_RandomPlayer:
		case Target_RandomPlayerNotCurrent:
		case Target_RandomUnit:
		case Target_RandomUnitNotCurrent:
		case Target_RandomFriendly:
		case Target_WoundedPlayer:
		case Target_WoundedUnit:
		case Target_WoundedFriendly:
		case Target_ClosestPlayer:
		case Target_ClosestPlayerNotCurrent:
		case Target_ClosestUnit:
		case Target_ClosestUnitNotCurrent:
		case Target_ClosestFriendly:
			_unit->CastSpell(pTarget, pEntry, pInstant);
			break;

		case Target_RandomPlayerApplyAura:
			pTarget->CastSpell(pTarget, pEntry, pInstant);
			break;

		case Target_Destination:
		case Target_RandomDestination:
		case Target_RandomPlayerDestination:
			_unit->CastSpellAoF(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pEntry, pInstant);
			break;

		case Target_ClosestCorpse:
		case Target_RandomCorpse:
			break;

		default:
			sLog.outDebug("MoonScriptCreatureAI::CastSpellOnTarget() : Invalid target type!\n");
			break;
	}
}

int32 MoonScriptCreatureAI::CalcSpellAttackTime(SpellDesc* pSpell)
{
#ifdef USE_DBC_SPELL_INFO
	return mBaseAttackTime + int32(pSpell->mCastTime);
#else
	return mBaseAttackTime + int32(pSpell->mCastTime * 1000);
#endif
}

bool MoonScriptCreatureAI::IsUnitInRange(Unit* pTarget, SpellDesc* pSpell)
{
	if( pSpell->mTargetType != Target_Self && pTarget != _unit && (pSpell->mMinRange > 0 || pSpell->mMaxRange > 0) )
	{
		float Range = GetRangeToUnit(pTarget);
		if( pSpell->mMinRange > 0 && (Range < pSpell->mMinRange) ) return false;
		if( pSpell->mMaxRange > 0 && (Range > pSpell->mMaxRange) ) return false;
	}
	return true;
}

Unit* MoonScriptCreatureAI::GetTargetForSpell(SpellDesc* pSpell)
{
	//Check if run-to-target cache and return it if its valid
	if( mRunToTargetCache && mRunToTargetSpellCache == pSpell && IsValidUnitTarget(mRunToTargetCache, TargetFilter_None) )
	{
		return mRunToTargetCache;
	}

	//Find a suitable target for the described situation :)
	switch( pSpell->mTargetType )
	{
		case Target_Self:
			return _unit;

		case Target_SecondMostHated:
			return GetBestPlayerTarget(TargetFilter_SecondMostHated);

		case Target_Current:
		case Target_Destination:
			return _unit->GetAIInterface()->GetNextTarget();

		case Target_Predefined:
			return pSpell->mPredefinedTarget;

		case Target_RandomPlayer:
		case Target_RandomPlayerApplyAura:
		case Target_RandomPlayerDestination:
			return GetBestPlayerTarget();

		case Target_RandomPlayerNotCurrent:
			return GetBestPlayerTarget(TargetFilter_NotCurrent);

		case Target_RandomUnit:
		case Target_RandomDestination:
			return GetBestUnitTarget();

		case Target_RandomUnitNotCurrent:
			return GetBestUnitTarget(TargetFilter_NotCurrent);

		case Target_RandomFriendly:
			return GetBestUnitTarget(TargetFilter_Friendly);

		case Target_WoundedPlayer:
			return GetBestPlayerTarget(TargetFilter_Wounded);

		case Target_WoundedUnit:
			return GetBestUnitTarget(TargetFilter_Wounded);

		case Target_WoundedFriendly:
			return GetBestUnitTarget(TargetFilter(TargetFilter_Wounded | TargetFilter_Friendly));

		case Target_ClosestPlayer:
			return GetBestPlayerTarget(TargetFilter_Closest);

		case Target_ClosestPlayerNotCurrent:
			return GetBestPlayerTarget(TargetFilter(TargetFilter_Closest | TargetFilter_NotCurrent));

		case Target_ClosestUnit:
			return GetBestUnitTarget(TargetFilter_Closest);

		case Target_ClosestUnitNotCurrent:
			return GetBestUnitTarget(TargetFilter(TargetFilter_Closest | TargetFilter_NotCurrent));

		case Target_ClosestFriendly:
			return GetBestUnitTarget(TargetFilter(TargetFilter_Closest | TargetFilter_Friendly));

		case Target_ClosestCorpse:
			return GetBestUnitTarget(TargetFilter(TargetFilter_Closest | TargetFilter_Corpse | TargetFilter_Friendly));

		case Target_RandomCorpse:
			return GetBestUnitTarget(TargetFilter(TargetFilter_Corpse | TargetFilter_Friendly));

		default:
			sLog.outDebug("MoonScriptCreatureAI::GetTargetForSpell() : Invalid target type!\n");
			return NULL;
	}
}

Unit* MoonScriptCreatureAI::GetBestPlayerTarget(TargetFilter pFilter)
{
	//Build potential target list
	UnitArray TargetArray;
	for( set<Player*>::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{ 
		if( IsValidUnitTarget(*PlayerIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*PlayerIter));
	}

	return ChooseBestTargetInArray(TargetArray, pFilter);
}

Unit* MoonScriptCreatureAI::GetBestUnitTarget(TargetFilter pFilter)
{
	//Build potential target list
	UnitArray TargetArray;
	if( pFilter & TargetFilter_Friendly )
	{
		for( std::set<Object*>::iterator ObjectIter = _unit->GetInRangeSetBegin(); ObjectIter != _unit->GetInRangeSetEnd(); ++ObjectIter )
		{
			if( IsValidUnitTarget(*ObjectIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*ObjectIter));
		}
		if( IsValidUnitTarget(_unit, pFilter) ) TargetArray.push_back(_unit);	//Also add self as possible friendly target
	}
	else
	{
		for( std::set<Object*>::iterator ObjectIter = _unit->GetInRangeOppFactsSetBegin(); ObjectIter != _unit->GetInRangeOppFactsSetEnd(); ++ObjectIter )
		{
			if( IsValidUnitTarget(*ObjectIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*ObjectIter));
		}
	}

	return ChooseBestTargetInArray(TargetArray, pFilter);
}

Unit* MoonScriptCreatureAI::ChooseBestTargetInArray(UnitArray& pTargetArray, TargetFilter pFilter)
{
	//If only one possible target, return it
	if( pTargetArray.size() == 1 ) return pTargetArray[0];

	//Find closest unit if requested
	if( pFilter & TargetFilter_Closest ) return GetNearestTargetInArray(pTargetArray);

	//Find second most hated if requested
	if( pFilter & TargetFilter_SecondMostHated ) return GetSecondMostHatedTargetInArray(pTargetArray);

	//Choose random unit in array
	return ( pTargetArray.size() > 1 ) ? pTargetArray[RandomUInt((uint32)pTargetArray.size()-1)] : NULL;
}

Unit* MoonScriptCreatureAI::GetNearestTargetInArray(UnitArray& pTargetArray)
{
	Unit* NearestUnit = NULL;
	float Distance, NearestDistance = 99999;
	for( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		Distance = GetRangeToUnit(*UnitIter);
		if( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestUnit = (*UnitIter);
		}
	}
	return NearestUnit;
}

Unit* MoonScriptCreatureAI::GetSecondMostHatedTargetInArray(UnitArray& pTargetArray)
{
	Unit*	TargetUnit = NULL;
	Unit*	MostHatedUnit = NULL;
	Unit*	CurrentTarget = _unit->GetAIInterface()->GetNextTarget();
	uint32	Threat = 0, HighestThreat = 0;
	for( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		TargetUnit = (*UnitIter);
		if( TargetUnit != CurrentTarget )
		{
			Threat = _unit->GetAIInterface()->getThreatByPtr(TargetUnit);
			if( Threat > HighestThreat )
			{
				MostHatedUnit = TargetUnit;
				HighestThreat = Threat;
			}
		}
	}
	return MostHatedUnit;
}

bool MoonScriptCreatureAI::IsValidUnitTarget(Object* pObject, TargetFilter pFilter)
{
	//Make sure its a valid unit
	if( !pObject->IsUnit() ) return false;
	if( pObject->GetInstanceID() != _unit->GetInstanceID() ) return false;
	Unit* UnitTarget = static_cast<Unit*>(pObject);

	//Skip dead (if not required), feign death or invisible targets
	if( !UnitTarget->isAlive() )
	{
		if( (pFilter & TargetFilter_Corpse) && UnitTarget->IsCreature() && static_cast<Creature*>(UnitTarget)->GetCreatureInfo()->Rank != ELITE_WORLDBOSS )
			return true;
		else
			return false;
	}
	if( UnitTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH) ) return false;
	if( UnitTarget->m_invisible ) return false;
	if( UnitTarget->isAlive() && (pFilter & TargetFilter_Corpse) ) return false;

	//Check if we apply target filtering
	if( pFilter != TargetFilter_None )
	{
		//Skip units not on threat list
		if( (pFilter & TargetFilter_Aggroed) && _unit->GetAIInterface()->getThreatByPtr(UnitTarget) == 0 ) return false;

		//Skip current attacking target if requested
		if( (pFilter & TargetFilter_NotCurrent) && UnitTarget == _unit->GetAIInterface()->GetNextTarget() ) return false;

		//Keep only wounded targets if requested
		if( (pFilter & TargetFilter_Wounded) && UnitTarget->GetHealthPct() >= 99 ) return false;

		//Handle hostile/friendly
		if( pFilter & TargetFilter_Friendly ) 
		{
			if( !UnitTarget->CombatStatus.IsInCombat() ) return false; //Skip not-in-combat targets if friendly
			if( isHostile(_unit, UnitTarget) || _unit->GetAIInterface()->getThreatByPtr(UnitTarget) > 0 ) return false;
		}
	}

	return true; //This is a valid unit target
}

void MoonScriptCreatureAI::PushRunToTargetCache(Unit* pTarget, SpellDesc* pSpell)
{
	if( mRunToTargetCache != pTarget )
	{
		mRunToTargetCache = pTarget;
		mRunToTargetSpellCache = pSpell;
		SetCanMove(true);
		SetAllowMelee(false);
		SetAllowRanged(false);
		SetAllowSpell(false);
	}
	if( mRunToTargetCache ) MoveTo(mRunToTargetCache);
}

void MoonScriptCreatureAI::PopRunToTargetCache()
{
	if( mRunToTargetCache )
	{
		mRunToTargetCache = NULL;
		mRunToTargetSpellCache = NULL;
		SetAllowMelee(true);
		SetAllowRanged(true);
		SetAllowSpell(true);
		StopMovement();
	}
}

void MoonScriptCreatureAI::RandomEmote(EmoteArray& pEmoteArray)
{
	int32 ArraySize = (int32)pEmoteArray.size();
	if( ArraySize > 0 )
	{
		uint32 Roll = ( ArraySize > 1 ) ? RandomUInt(ArraySize - 1) : 0;
		Emote(pEmoteArray[Roll]->mText.c_str(), pEmoteArray[Roll]->mType, pEmoteArray[Roll]->mSoundId);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class MoonScriptBossAI
MoonScriptBossAI::MoonScriptBossAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
{
	mPhaseIndex = -1;
	mEnrageSpell = NULL;
	mEnrageTimerDuration = -1;
	mEnrageTimer = INVALIDATE_TIMER;
}

MoonScriptBossAI::~MoonScriptBossAI()
{
	mPhaseSpells.clear();
}

SpellDesc* MoonScriptBossAI::AddPhaseSpell(int32 pPhase, SpellDesc* pSpell)
{
	mPhaseSpells.push_back(std::make_pair(pPhase, pSpell));
	return pSpell;
}

int32 MoonScriptBossAI::GetPhase()
{
	return mPhaseIndex;
}

void MoonScriptBossAI::SetPhase(int32 pPhase, SpellDesc* pPhaseChangeSpell)
{
	if( mPhaseIndex != pPhase )
	{
		//Cancel all spells
		CancelAllSpells();

		//Enable spells related to that phase
		for( PhaseSpellArray::iterator SpellIter = mPhaseSpells.begin(); SpellIter != mPhaseSpells.end(); ++SpellIter )
		{
			if( SpellIter->first == pPhase ) SpellIter->second->mEnabled = true;
			else SpellIter->second->mEnabled = false;
		}

		//Remember phase index
		mPhaseIndex = pPhase;

		//Cast phase change spell now if available
		if( pPhaseChangeSpell ) CastSpellNowNoScheduling(pPhaseChangeSpell);
	}
}

void MoonScriptBossAI::SetEnrageInfo(SpellDesc* pSpell, int32 pTriggerMilliseconds)
{
	mEnrageSpell = pSpell;
	mEnrageTimerDuration = pTriggerMilliseconds;
}

void MoonScriptBossAI::OnCombatStart(Unit* pTarget)
{
	SetPhase(1);
	if( mEnrageSpell && mEnrageTimerDuration > 0 )
	{
		mEnrageTimer = AddTimer(mEnrageTimerDuration);
	}
	TriggerCooldownOnAllSpells();
	MoonScriptCreatureAI::OnCombatStart(pTarget);
}

void MoonScriptBossAI::OnCombatStop(Unit* pTarget)
{
	SetPhase(1);
	RemoveTimer(mEnrageTimer);
	MoonScriptCreatureAI::OnCombatStop(pTarget);
}

void MoonScriptBossAI::AIUpdate()
{
	if( mEnrageSpell && mEnrageTimerDuration > 0 && IsTimerFinished(mEnrageTimer) )
	{
		CastSpell(mEnrageSpell);
		RemoveTimer(mEnrageTimer);
	}
	MoonScriptCreatureAI::AIUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Premade Spell Functions
#define SPELLFUNC_VANISH 24699

void SpellFunc_ClearHateList(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->ClearHateList();
}

void SpellFunc_Disappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->ClearHateList();
	pCreatureAI->SetCanMove(false);
	pCreatureAI->SetCanEnterCombat(false);
	pCreatureAI->ApplyAura(SPELLFUNC_VANISH);
}

void SpellFunc_Reappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->SetCanMove(true);
	pCreatureAI->SetCanEnterCombat(true);
	pCreatureAI->RemoveAura(SPELLFUNC_VANISH);
}
