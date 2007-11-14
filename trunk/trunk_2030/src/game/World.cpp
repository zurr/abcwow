/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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
 *
 */

#include "StdAfx.h"

initialiseSingleton( World );
DayWatcherThread * dw = NULL;

World::World()
{
	m_playerLimit = 0;
	m_allowMovement = true;
	m_gmTicketSystem = true;

	reqGmClient = false;
	GmClientChannel = "";

	m_UpdateDistance = 0.0f;
	m_StartTime = 0;
	eventholder = new EventableObjectHolder(-1);
	m_holder = eventholder;
	m_event_Instanceid = eventholder->GetInstanceID();

	mQueueUpdateInterval = 10000;
	PeakSessionCount = 0;
	mInWorldPlayerCount = 0;
	mAcceptedConnections = 0;
	HordePlayers = 0;
	AlliancePlayers = 0;
	gm_skip_attunement = false;
	show_gm_in_who_list = true;
	map_unload_time=0;
#ifndef CLUSTERING
	SocketSendBufSize = WORLDSOCKET_SENDBUF_SIZE;
	SocketRecvBufSize = WORLDSOCKET_RECVBUF_SIZE;
#endif
}

World::~World()
{
	sLog.outString("  Saving players to DB...");
	for(SessionMap::iterator i=m_sessions.begin();i!=m_sessions.end();i++)
	{
		(i->second)->LogoutPlayer(true);
	
	}

	sLog.outString("  Deleting world packet logger...");
	delete WorldLog::getSingletonPtr();

	sLog.outString("  Deleting Social Manager...");
	delete SocialMgr::getSingletonPtr();

	sLog.outString("  Deleting Object Manager...");
	delete ObjectMgr::getSingletonPtr();
	
	sLog.outString("  Deleting Loot Manager...");		
	delete LootMgr::getSingletonPtr();
	
	sLog.outString("  Deleting LFG Manager...");	
	delete LfgMgr::getSingletonPtr();

	sLog.outString("  Deleting Channel Manager...");
	delete ChannelMgr::getSingletonPtr();

	sLog.outString("  Deleting Quest Manager...");
	delete QuestMgr::getSingletonPtr();
  
	sLog.outString("  Deleting Weather Manager...");
	delete WeatherMgr::getSingletonPtr();

	sLog.outString("  Deleting Taxi Manager...");
	delete TaxiMgr::getSingletonPtr();
	
	sLog.outString("  Deleting Battleground Manager...");	
	delete CBattlegroundManager::getSingletonPtr();

	sLog.outString("Removing all objects and deleting WorldCreator...\n");
	delete WorldCreator::getSingletonPtr();

	//sLog.outString("Deleting Thread Manager..");
	//delete ThreadMgr::getSingletonPtr();
	delete WordFilter::getSingletonPtr();

	sLog.outString("Deleting Instance Saving Management...");
	delete InstanceSavingManagement::getSingletonPtr();

	sLog.outString("Deleting Random Number Generator...");
	delete MTRand::getSingletonPtr();

	for( AreaTriggerMap::iterator i = m_AreaTrigger.begin( ); i != m_AreaTrigger.end( ); ++ i ) 
	{
		delete i->second;
	}

	//eventholder = 0;
	delete eventholder;

	Storage_Cleanup();
	/*
	for(list<SpellEntry*>::iterator itr = dummyspells.begin(); itr != dummyspells.end(); ++itr)
		delete *itr;
	*/
}


WorldSession* World::FindSession(uint32 id)
{
	m_sessionlock.AcquireReadLock();
	WorldSession * ret = 0;
	SessionMap::const_iterator itr = m_sessions.find(id);

	if(itr != m_sessions.end())
		ret = itr->second;
	
	m_sessionlock.ReleaseReadLock();

	return ret;
}

void World::RemoveSession(uint32 id)
{
	m_sessionlock.AcquireWriteLock();

	SessionMap::iterator itr = m_sessions.find(id);

	if(itr != m_sessions.end())
	{
		delete itr->second;
		m_sessions.erase(itr);
	}

	m_sessionlock.ReleaseWriteLock();
}

void World::AddSession(WorldSession* s)
{
	m_sessionlock.AcquireWriteLock();

	ASSERT(s);
	m_sessions[s->GetAccountId()] = s;

	if(m_sessions.size() >  PeakSessionCount)
		PeakSessionCount = m_sessions.size();

	m_sessionlock.ReleaseWriteLock();
}

void World::AddGlobalSession(WorldSession *session)
{
	SessionsMutex.Acquire();
	Sessions.insert(session);
	SessionsMutex.Release();
}

void World::RemoveGlobalSession(WorldSession *session)
{
	SessionsMutex.Acquire();
	Sessions.erase(session);
	SessionsMutex.Release();
}

bool BasicTaskExecutor::run()
{
	/* Set thread priority, this is a bitch for multiplatform :P */
#ifdef WIN32
	switch(priority)
	{
		case BTE_PRIORITY_LOW:
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_LOWEST );
			break;

		case BTW_PRIORITY_HIGH:
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
			break;

		default:		// BTW_PRIORITY_MED
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_NORMAL );
			break;
	}
#else
	struct sched_param param;
	switch(priority)
	{
	case BTE_PRIORITY_LOW:
		param.sched_priority = 0;
		break;

	case BTW_PRIORITY_HIGH:
		param.sched_priority = 10;
		break;

	default:		// BTW_PRIORITY_MED
		param.sched_priority = 5;
		break;
	}
	pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
#endif

	// Execute the task in our new context.
	cb->execute();
#ifdef WIN32
	::SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
#else
	param.sched_priority = 5;
	pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
#endif

	return true;
}

bool World::SetInitialWorldSettings()
{
	CharacterDatabase.WaitExecute("UPDATE characters SET online = 0 WHERE online = 1");
	CharacterDatabase.WaitExecute("UPDATE characters SET level = 70 WHERE level > 70");
   
	m_lastTick = UNIXTIME;

	// TODO: clean this
	time_t tiempo;
	char hour[3];
	char minute[3];
	char second[3];
	struct tm *tmPtr;
	tiempo = UNIXTIME;
	tmPtr = localtime(&tiempo);
	strftime( hour, 3, "%H", tmPtr );
	strftime( minute, 3, "%M", tmPtr );
	strftime( second, 3, "%S", tmPtr );
	m_gameTime = (3600*atoi(hour))+(atoi(minute)*60)+(atoi(second)); // server starts at noon

	// TODO: clean this
	// fill in emotes table
	// it appears not every emote has an animation
	mPrices[1] = 10;
	mPrices[4] = 80;
	mPrices[6] = 150;
	mPrices[8] = 200;
	mPrices[10] = 300;
	mPrices[12] = 800;
	mPrices[14] = 900;
	mPrices[16] = 1800;
	mPrices[18] = 2200;
	mPrices[20] = 2300;
	mPrices[22] = 3600;
	mPrices[24] = 4200;
	mPrices[26] = 6700;
	mPrices[28] = 7200;
	mPrices[30] = 8000;
	mPrices[32] = 11000;
	mPrices[34] = 14000;
	mPrices[36] = 16000;
	mPrices[38] = 18000;
	mPrices[40] = 20000;
	mPrices[42] = 27000;
	mPrices[44] = 32000;
	mPrices[46] = 37000;
	mPrices[48] = 42000;
	mPrices[50] = 47000;
	mPrices[52] = 52000;
	mPrices[54] = 57000;
	mPrices[56] = 62000;
	mPrices[58] = 67000;
	mPrices[60] = 72000;

	uint32 start_time = getMSTime();

	Log.Notice("World", "Loading DBC files...");
	if(!LoadDBCs())
		return false;

	/*{
		DBCFile moo;
		moo.open("DBC/FactionTemplate.dbc");
		set<pair<uint32, uint32> > fs;
		set<uint32> known;
		for(uint32 i = 0; i < moo.getRecordCount(); ++i)
		{
			fs.insert(make_pair(moo.getRecord(i).getUInt(0), moo.getRecord(i).getUInt(1)));
			known.insert(moo.getRecord(i).getUInt(0));
		}

		QueryResult * result = sDatabase.Query("SELECT entry, faction FROM creature_proto");
		uint32 e, f;
		do 
		{
			e = result->Fetch()[0].GetUInt32();
			f = result->Fetch()[1].GetUInt32();
			FactionDBC * d;
			if(known.find(f) == known.end())
			{
				printf("Bad faction %u!", f);
				//d = sFactionStore.LookupEntryForced(f);
				d = ((FastIndexedDataStore<FactionDBC>*)FactionStore::getSingletonPtr())->LookupEntryForced(f);
				if(!d)
					printf(".. REALLY bad faction\n");
				else
				{
					// find a faction template that matches.. meh ;p
					set<pair<uint32, uint32> >::iterator itr = fs.begin();
					for(; itr != fs.end(); ++itr)
					{
						if(itr->second == f)
						{
							printf(" replaced with %u (%s)\n", itr->first, sFactionStore.LookupString(d->Name));
							sDatabase.WaitExecute("UPDATE creature_proto SET faction = %u WHERE entry = %u", e, itr->second);
							sDatabase.WaitExecute("UPDATE creature_spawns SET factionid = %u WHERE entry = %u", e, itr->second);
							break;
						}
					}
				}
			}
		} while(result->NextRow());
	}*/

	/* Convert area table ids/flags */
	DBCFile area;
	area.open("DBC/AreaTable.dbc");
	uint32 flag_, area_, zone_;
	for(uint32 i = 0; i < area.getRecordCount(); ++i)
	{
		area_ = area.getRecord(i).getUInt(0);
		flag_ = area.getRecord(i).getUInt(3);
		zone_ = area.getRecord(i).getUInt(2);

		mAreaIDToTable[flag_] = dbcArea.LookupEntry(area_);
		if(mZoneIDToTable.find(zone_) != mZoneIDToTable.end())
		{
			if(mZoneIDToTable[zone_]->AreaFlags != 312 &&
				mAreaIDToTable[flag_]->AreaFlags == 312)
			{
				// over ride.
				mZoneIDToTable[zone_] = mAreaIDToTable[flag_];
			}
		}
		else
		{
			mZoneIDToTable[zone_] = mAreaIDToTable[flag_];
		}
	}

	new ObjectMgr;
	new QuestMgr;
	new LootMgr;
	new LfgMgr;
	new WeatherMgr;
	new TaxiMgr;
	new AddonMgr;
	new SocialMgr;
	new WorldLog;

	// grep: this only has to be done once between version updates
	// to re-fill the table.

	/*sLog.outString("Filling spell replacements table...");
	FillSpellReplacementsTable();
	sLog.outString("");*/

#define MAKE_TASK(sp, ptr) tl.AddTask(new Task(new CallbackP0<sp>(sp::getSingletonPtr(), &sp::ptr)))
	// Fill the task list with jobs to do.
	TaskList tl;
	MAKE_TASK(ObjectMgr, LoadPlayerCreateInfo);
	MAKE_TASK(ObjectMgr, LoadPlayersInfo);
	Storage_FillTaskList(tl);

	// spawn worker threads (2 * number of cpus)
	tl.spawn();

	/* storage stuff has to be loaded first */
	tl.wait();

	MAKE_TASK(ObjectMgr, LoadSpellDataExtra);
	MAKE_TASK(ObjectMgr, LoadCreatureWaypoints);
	MAKE_TASK(ObjectMgr, LoadTrainers);
	MAKE_TASK(ObjectMgr, LoadTotemSpells);
	MAKE_TASK(ObjectMgr, LoadSpellSkills);
	MAKE_TASK(ObjectMgr, LoadSpellOverride);
	MAKE_TASK(ObjectMgr, LoadVendors);
	MAKE_TASK(ObjectMgr, LoadAIThreatToSpellId);
	MAKE_TASK(ObjectMgr, LoadDefaultPetSpells);
	MAKE_TASK(ObjectMgr, LoadPetSpellCooldowns);
	MAKE_TASK(ObjectMgr, LoadGuildCharters);
	MAKE_TASK(ObjectMgr, LoadGMTickets);
	MAKE_TASK(SocialMgr, LoadFromDB);
	MAKE_TASK(AddonMgr, LoadFromDB);
	MAKE_TASK(ObjectMgr, SetHighestGuids);
	MAKE_TASK(ObjectMgr, LoadReputationModifiers);
	MAKE_TASK(ObjectMgr, LoadMonsterSay);
	MAKE_TASK(WeatherMgr, LoadFromDB);
	MAKE_TASK(ObjectMgr,LoadGroups);
	MAKE_TASK(ObjectMgr, LoadCreatureFamilyNames);
	MAKE_TASK(ObjectMgr, LoadExtraCreatureProtoStuff);
	MAKE_TASK(ObjectMgr, LoadExtraItemStuff);
	MAKE_TASK(QuestMgr, LoadExtraQuestStuff);
	MAKE_TASK(ObjectMgr, LoadArenaTeams);

#undef MAKE_TASK

	// wait for all loading to complete.
	tl.wait();

	CommandTableStorage::getSingleton().Load();
	new WordFilter;
	sWordFilter.Load();

	sLog.outString("");
	Log.Notice("World", "Database loaded in %ums.", getMSTime() - start_time);
	sLog.outString("");

#ifdef COLLISION
	Log.Notice("Collision", "Initializing...");
	Collision::InitTileManagers();
#endif

	// calling this puts all maps into our task list.
	new WorldCreator(&tl);

	// wait for the events to complete.
	tl.wait();

	// wait for them to exit, now.
	tl.kill();
	tl.waitForThreadsToExit();
	sLog.outString("");
	LoadNameGenData();

	new InstanceSavingManagement;
	sInstanceSavingManager.LoadSavedInstances();

/*
	for(uint32 x=0; x < cnt; x++)  //for every spell in dbc
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// proc intervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		if(strstr(nametext, "Lightning Shield") && sp->EffectTriggerSpell[0])
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "for $");
			if(startofid)
			{
				startofid += strlen("for $");
				sp->EffectTriggerSpell[0]=atoi(startofid); //get new lightning shield trigger id
			}
			sp->proc_interval = 3000; //few seconds
		}
		//mage ignite talent should proc only on some chances
		else if(strstr(nametext, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0]==4)
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
			}
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
			sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
			sp->procFlags = PROC_SPELL_CRIT; //add procflag here since this was not processed with the others !
		}
		// Winter's Chill handled by frost school
		else if(strstr(nametext, "Winter's Chill"))
		{
			sp->School = 4;
		}
		// Blackout handled by Shadow school
		else if(strstr(nametext, "Blackout"))
		{
			sp->School = 5;
		}
		// Shadow Weaving
		else if(strstr(nametext, "Shadow Weaving"))
		{
			sp->School = 5;
			sp->EffectApplyAuraName[0] = 42;
			sp->procChance = sp->EffectBasePoints[0] + 1;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
		//Improved Aspect of the Hawk
		else if(strstr(nametext, "Improved Aspect of the Hawk"))
			sp->EffectSpellGroupRelation[1] = 0x100000;
		//more triggered spell ids are wrong. I think blizz is trying to outsmart us :S
		else if( strstr(nametext, "Nature's Guardian"))
		{
			sp->EffectTriggerSpell[0]=31616;
			sp->proc_interval = 5000;
		}
		//this starts to be an issue for trigger spell id : Deep Wounds
		else if(strstr(nametext, "Deep Wounds") && sp->EffectTriggerSpell[0])
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "over $");
			if(startofid)
			{
				startofid += strlen("over $");
				sp->EffectTriggerSpell[0]=atoi(startofid);
			}
		}
		else if(strstr(nametext, "Holy Shock"))
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "causing $");
			if(startofid)
			{
				startofid += strlen("causing $");
				sp->EffectTriggerSpell[0]=atoi(startofid);
			}
			//check if we can find in the desription
			startofid=strstr(desc, " or $");
			if(startofid)
			{
				startofid += strlen(" or $");
				sp->EffectTriggerSpell[1]=atoi(startofid);
			}
		}
		else if(strstr(nametext, "Touch of Weakness"))
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "cause $");
			if(startofid)
			{
				startofid += strlen("cause $");
				sp->EffectTriggerSpell[0]=atoi(startofid);
				sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
				sp->procFlags = uint32(PROC_STRUCK_MELEE | PROC_TAGRGET_SELF);
			}
		}
		else if(strstr(nametext, "Firestone Passive"))
		{
			//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
			//check if we can find in the desription
			char * startofid=strstr(desc, "to deal $");
			if(startofid)
			{
				startofid += strlen("to deal $");
				sp->EffectTriggerSpell[0]=atoi(startofid);
				sp->EffectApplyAuraName[0]=42;
				sp->procFlags = PROC_ATTACK_MELEE;
				sp->procChance = 50;
			}
		}
		//some procs trigger at intervals
		else if(strstr(nametext, "Water Shield"))
		{
			sp->proc_interval = 3000; //few seconds
			sp->procFlags |= PROC_TAGRGET_SELF;
		}
		else if(strstr(nametext, "Earth Shield"))
			sp->proc_interval = 3000; //few seconds
		else if(strstr(nametext, "Shadowguard"))
			sp->proc_interval = 3000; //few seconds
		else if(strstr(nametext, "Poison Shield"))
			sp->proc_interval = 3000; //few seconds
		else if(strstr(nametext, "Infused Mushroom"))
			sp->proc_interval = 10000; //10 seconds
		else if(strstr(nametext, "Aviana's Purpose"))
			sp->proc_interval = 10000; //10 seconds
//		else if(strstr(nametext, "Illumination"))
//			sp->EffectTriggerSpell[0]=20272;
		if(sp->proc_interval!=0)
			sp->procFlags |= PROC_REMOVEONUSE;

		// Seal of Command - Proc Chance 
		if(sp->NameHash == 0xC5C30B39)
			sp->procChance = 25;
		
		// Seal of Jusice - Proc Chance 
		if(sp->NameHash == 0xCC6D4182)
			sp->procChance = 25;

		// Decapitate
		if(sp->NameHash == 0xB6C3243C)
			sp->procChance = 30;

		if(namehash == 0x56392512)			// seal of light
			sp->procChance=45;

		if(namehash==3238263755UL)
			sp->procFlags=0;

		if(namehash==0x8D4A2E9F)		// warlock - intensity
		{
			sp->EffectSpellGroupRelation[0] |= 1;		// shadow bolt
			sp->EffectSpellGroupRelation[0] |= 4;		// immolatate
		}

		if(namehash==0x2bc0ae00)		// warlock - incinerate
			sp->SpellGroupType=1;

	}







	//this is so lame : shamanistic rage triggers a new spell which borrows it's stats from parent spell :S
	SpellEntry * parentsp = SpellDataStorage.LookupEntry(30823);
	SpellEntry * triggersp = SpellDataStorage.LookupEntry(30824);
	if(parentsp && triggersp) 
		triggersp->EffectBasePoints[0] = parentsp->EffectBasePoints[0];

	SpellEntry * sp = SpellDataStorage.LookupEntry(16164);
	if(sp && sp->Id==16164)
		sp->procFlags=PROC_STRUCK_SPELL_CRIT;

	sp = SpellDataStorage.LookupEntry(17364);
	//remove stormstrike effect 0
	if(sp && sp->Id==17364)
		sp->Effect[0]=0;

	//Druid: Frenzied Regeneration
	sp = SpellDataStorage.LookupEntry(22842);
	if(sp)
	{
		sp->Effect[0] = 6;
		sp->EffectApplyAuraName[0] = 23;
		sp->EffectTriggerSpell[0] = 22845;
	}
	sp = SpellDataStorage.LookupEntry(22895);
	if(sp)
	{
		sp->Effect[0] = 6;
		sp->EffectApplyAuraName[0] = 23;
		sp->EffectTriggerSpell[0] = 22845;
	}
	sp = SpellDataStorage.LookupEntry(22896);
	if(sp)
	{
		sp->Effect[0] = 6;
		sp->EffectApplyAuraName[0] = 23;
		sp->EffectTriggerSpell[0] = 22845;
	}
	sp = SpellDataStorage.LookupEntry(26999);
	if(sp)
	{
		sp->Effect[0] = 6;
		sp->EffectApplyAuraName[0] = 23;
		sp->EffectTriggerSpell[0] = 22845;
	}

	//paladin - seal of blood
	sp = SpellDataStorage.LookupEntry(31892);
	if(sp)
	{
		sp->procFlags = PROC_ATTACK_MELEE;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 31893;
	}
	sp = SpellDataStorage.LookupEntry(38008);
	if(sp)
	{
		sp->procFlags = PROC_ATTACK_MELEE;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 31893;
	}
	//paladin - Spiritual Attunement 
	sp = SpellDataStorage.LookupEntry(31785);
	if(sp)
	{
		sp->procFlags = PROC_STRUCK_SPELL ;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 31786;
	}
	sp = SpellDataStorage.LookupEntry(33776);
	if(sp)
	{
		sp->procFlags = PROC_STRUCK_SPELL ;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 31786;
	}
	//Druid: Leader of the Pack
	sp = SpellDataStorage.LookupEntry(24932);
	if (sp)
	{
		sp->Effect[1] = 0;
		sp->Effect[2] = 0; //removing strange effects.
	}
	//Druid: Improved Leader of the Pack
	sp = SpellDataStorage.LookupEntry(34299); if (sp) sp->proc_interval = 6000;//6 secs

	//fix for the right Enchant ID for Enchant Cloak - Major Resistance
	sp = SpellDataStorage.LookupEntry(27962);
	if(sp)
		sp->EffectMiscValue[0] = 2998;
	sp = SpellDataStorage.LookupEntry(36285);
	if(sp)
		sp->EffectMiscValue[0] = 2998;

	//muhaha, rewriting Retaliation spell as old one :D
	sp = SpellDataStorage.LookupEntry(20230);
	if(sp)
	{
		sp->Effect[0] = 6; //aura
		sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
		sp->EffectTriggerSpell[0] = 22858; //evil , but this is good for us :D
		sp->procFlags = PROC_STRUCK_MELEE; //add procflag here since this was not processed with the others !
	}

	//"bloodthirst" new version is ok but old version is wrong from now on :(
	sp = SpellDataStorage.LookupEntry(23881);
	if(sp)
	{
		sp->Effect[1] = 64; //cast on us, it is good
		sp->EffectTriggerSpell[1] = 23885; //evil , but this is good for us :D
	}
	sp = SpellDataStorage.LookupEntry(23892);
	if(sp)
	{
		sp->Effect[1] = 64;
		sp->EffectTriggerSpell[1] = 23886; //evil , but this is good for us :D
	}
	sp = SpellDataStorage.LookupEntry(23893);
	if(sp)
	{
		sp->Effect[1] = 64; //
		sp->EffectTriggerSpell[1] = 23887; //evil , but this is good for us :D
	}
	sp = SpellDataStorage.LookupEntry(23894);
	if(sp)
	{
		sp->Effect[1] = 64; //
		sp->EffectTriggerSpell[1] = 23888; //evil , but this is good for us :D
	}
	sp = SpellDataStorage.LookupEntry(25251);
	if(sp)
	{
		sp->Effect[1] = 64; //aura
		sp->EffectTriggerSpell[1] = 25252; //evil , but this is good for us :D
	}
	sp = SpellDataStorage.LookupEntry(30335);
	if(sp)
	{
		sp->Effect[1] = 64; //aura
		sp->EffectTriggerSpell[1] = 30339; //evil , but this is good for us :D
	}
	//Rogue: Seal Fate
	sp = SpellDataStorage.LookupEntry(14186);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 20;
	}
	sp = SpellDataStorage.LookupEntry(14190);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 40;
	}
	sp = SpellDataStorage.LookupEntry(14193);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 60;
	}
	sp = SpellDataStorage.LookupEntry(14194);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 80;
	}
	sp = SpellDataStorage.LookupEntry(14195);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 100;
	}
	//Druid: Primal Fury (2 parts)
	//Blood Frenzy
	sp = SpellDataStorage.LookupEntry(16954);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 100;
	}
	sp = SpellDataStorage.LookupEntry(16952);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 50;
	}
	//Primal Fury
	sp = SpellDataStorage.LookupEntry(16961);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 100;
	}
	sp = SpellDataStorage.LookupEntry(16958);
	if(sp) 
	{
		sp->procFlags = PROC_ATTACK_MELEE_CRIT;
		sp->procChance = 50;
	}
	//Druid:Intensity
	sp = SpellDataStorage.LookupEntry(17106);
	if(sp)
	{
	   sp->EffectApplyAuraName[1] = 42;
	   sp->procFlags=PROC_CAST_SPELL;
	}
	sp = SpellDataStorage.LookupEntry(17107);
	if(sp)
	{
		sp->EffectApplyAuraName[1] = 42;
		 sp->procFlags=PROC_CAST_SPELL;
	}
	sp = SpellDataStorage.LookupEntry(17108);
	if(sp)
	{
		sp->EffectApplyAuraName[1] = 42;
		sp->procFlags=PROC_CAST_SPELL;
	}
    //Improved Sprint
	sp = SpellDataStorage.LookupEntry(13743);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42;
		sp->procFlags=PROC_CAST_SPELL;
		sp->procChance = 50;
	}
	sp = SpellDataStorage.LookupEntry(13875);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42;
		sp->procFlags=PROC_CAST_SPELL;
	}
	//warlock: Shadow Mastery
	for (uint32 i=0;i<5;i++)
	{
		sp  = SpellDataStorage.LookupEntry(18271+i);
		if (sp)
		{
			sp->EffectSpellGroupRelation[0]=33562624;
			sp->EffectSpellGroupRelation[1]=8421376;
		}
	}
	//mage: Arcane Power
	sp  = SpellDataStorage.LookupEntry(12042);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=552210455;
		sp->EffectSpellGroupRelation[1]=10518528;
	}
	//mage: Fire Power
	sp  = SpellDataStorage.LookupEntry(11124);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=868;
		sp->EffectSpellGroupRelation[1]=868;
	}
	sp  = SpellDataStorage.LookupEntry(12398);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=868;
		sp->EffectSpellGroupRelation[1]=868;
	}
	sp  = SpellDataStorage.LookupEntry(12399);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=868;
		sp->EffectSpellGroupRelation[1]=868;
	}
	sp  = SpellDataStorage.LookupEntry(12400);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=868;
		sp->EffectSpellGroupRelation[1]=868;
	}
	sp  = SpellDataStorage.LookupEntry(12378);
	if (sp)
	{
		sp->EffectSpellGroupRelation[0]=868;
		sp->EffectSpellGroupRelation[1]=868;
	}
	////mage: Spell Power
	//sp = sSpellStore.LookupEntry(35581);
	//if(sp)	
	//{
	//	sp->EffectSpellGroupRelation[0]=5775504;
	//}
	//sp = sSpellStore.LookupEntry(35578);
	//if(sp)	
	//{
	//	sp->EffectSpellGroupRelation[0]=5775504;
	//}
	//mage: Blazing Speed
	sp = SpellDataStorage.LookupEntry(31641); 
	if(sp)	sp->EffectTriggerSpell[0]=31643;
	sp = SpellDataStorage.LookupEntry(31642);
	if(sp)	sp->EffectTriggerSpell[0]=31643;

	//mage talent "frostbyte". we make it to be dummy
	sp = SpellDataStorage.LookupEntry(11071);
	if(sp)	sp->EffectApplyAuraName[0]=4;
	sp = SpellDataStorage.LookupEntry(12496);
	if(sp)	sp->EffectApplyAuraName[0]=4;
	sp = SpellDataStorage.LookupEntry(12497);
	if(sp)	sp->EffectApplyAuraName[0]=4;
	//rogue-shiv -> add 1 combo point
	sp = SpellDataStorage.LookupEntry(5938);
	if(sp)	sp->Effect[1]=80;

	//warlock - soul leech
	sp = SpellDataStorage.LookupEntry(30293);
	if(sp)
	{
		sp->Effect[0] = 6; //aura
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 30294;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}
	sp = SpellDataStorage.LookupEntry(30295);
	if(sp)
	{
		sp->Effect[0] = 6; //aura
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 30294;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}
	sp = SpellDataStorage.LookupEntry(30296);
	if(sp)
	{
		sp->Effect[0] = 6; //aura
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 30294;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}

	//warlock - Pyroclasm
	sp = SpellDataStorage.LookupEntry(18073);
	if(sp)
	{
		sp->Effect[0] = 0; //delete this owerride effect :P
		sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
		sp->procFlags=PROC_CAST_SPELL;
		sp->procChance = 13; //god, save us from fixed values !
	}
	sp = SpellDataStorage.LookupEntry(18096);
	if(sp)
	{
		sp->Effect[0] = 0; //delete this owerride effect :P
		sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
		sp->procFlags=PROC_CAST_SPELL;
		sp->procChance = 26; //god, save us from fixed values !
	}

	//improved scorch
	sp = SpellDataStorage.LookupEntry(11095);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42; 
		sp->procFlags=PROC_CAST_SPELL;
	}
	sp = SpellDataStorage.LookupEntry(12872);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42; 
		sp->procFlags=PROC_CAST_SPELL;
	}
	sp = SpellDataStorage.LookupEntry(12873);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42; 
		sp->procFlags=PROC_CAST_SPELL;
	}

	//Nature's Grasp
	sp = SpellDataStorage.LookupEntry(16689);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 339; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(16810);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 1062; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(16811);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 5195; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(16812);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 5196; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(16813);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 9852; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(17329);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 9853; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	sp = SpellDataStorage.LookupEntry(27009);
	if(sp)
	{
		sp->Effect[0] = 6; 
		sp->EffectApplyAuraName[0] = 42; 
		sp->EffectTriggerSpell[0] = 26989; 
		sp->Effect[1] = 0; 
		sp->procFlags = PROC_STRUCK_MELEE | PROC_REMOVEONUSE;
		sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
	}
	//wrath of air totem targets sorounding creatures instead of us
	sp = SpellDataStorage.LookupEntry(2895);
	if(sp)
	{
		sp->EffectImplicitTargetA[0]=1;
		sp->EffectImplicitTargetA[1]=1;
		sp->EffectImplicitTargetA[2]=0;
		sp->EffectImplicitTargetB[0]=0;
		sp->EffectImplicitTargetB[1]=0;
		sp->EffectImplicitTargetB[2]=0;
	}
//-----BLEED effects----- (setting mech to BLEED)
	//garrot
	sp = SpellDataStorage.LookupEntry(14179);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(8631);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(8632);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(8633);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11289);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11290);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(26839);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(26884);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//pounce
	sp = SpellDataStorage.LookupEntry(9007);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9824);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9826);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(27007);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(14179);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//rend
	sp = SpellDataStorage.LookupEntry(772);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(6546);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(6547);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(6548);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11572);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11573);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11574);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(25208);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//rip
	sp = SpellDataStorage.LookupEntry(1079);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9492);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9493);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9752);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9894);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9896);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(27008);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//rupture
	sp = SpellDataStorage.LookupEntry(1943);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(8639);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(8640);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11273);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11274);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(11275);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(26867);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//rake
	sp = SpellDataStorage.LookupEntry(1822);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(1823);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(1824);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(9904);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	sp = SpellDataStorage.LookupEntry(27003);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
	//lacerate
	sp = SpellDataStorage.LookupEntry(33745);if (sp) sp->MechanicsType = MECHANIC_BLEEDING;
//-----------------------
	//Priest: Shadow Power
	sp = SpellDataStorage.LookupEntry(15310);
	if(sp)
	{
		uint32 group = sp->EffectSpellGroupRelation[0];
		sp = SpellDataStorage.LookupEntry(33221);
		if(sp)
			sp->EffectSpellGroupRelation[0]=group;
		sp = SpellDataStorage.LookupEntry(33222);
		if(sp)
			sp->EffectSpellGroupRelation[0]=group;
		sp = SpellDataStorage.LookupEntry(33223);
		if(sp)
			sp->EffectSpellGroupRelation[0]=group;
		sp = SpellDataStorage.LookupEntry(33224);
		if(sp)
			sp->EffectSpellGroupRelation[0]=group;
		sp = SpellDataStorage.LookupEntry(33225);
		if(sp)
			sp->EffectSpellGroupRelation[0]=group;
	}
	//Relentless Strikes
	sp = SpellDataStorage.LookupEntry(14179);
	if(sp)
	{
		sp->EffectApplyAuraName[0]=42;//proc spell
		sp->procFlags = PROC_CAST_SPELL;
		sp->EffectBasePoints[1] = 20; //client showes 20% chance but whe do not have it ? :O
	}
	//priest - surge of light
	sp = SpellDataStorage.LookupEntry(33150);
	if(sp)
		sp->procFlags = uint32(PROC_STRUCK_SPELL_CRIT | PROC_TAGRGET_SELF);
	sp = SpellDataStorage.LookupEntry(33154);
	if(sp)
		sp->procFlags = uint32(PROC_STRUCK_SPELL_CRIT | PROC_TAGRGET_SELF);
	sp = SpellDataStorage.LookupEntry(33151);
	if(sp)
	{
		sp->EffectSpellGroupRelation[0]=128;
		sp->EffectSpellGroupRelation[1]=128;
		sp->EffectSpellGroupRelation[2]=128;
		sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
	}
	//Seal of Justice -lowered proc chance (experimental values !)
	sp = SpellDataStorage.LookupEntry(20164);
	if(sp)
		sp->procChance = 20;
	sp = SpellDataStorage.LookupEntry(31895);
	if(sp)
		sp->procChance = 20;
	//make Berserking a simple spell 
	sp = SpellDataStorage.LookupEntry(20554);
	if(sp)
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = SpellDataStorage.LookupEntry(26296);
	if(sp)
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = SpellDataStorage.LookupEntry(26297);
	if(sp)
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	//rogue - intiative
	sp = SpellDataStorage.LookupEntry(13976);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}
	sp = SpellDataStorage.LookupEntry(13979);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}
	sp = SpellDataStorage.LookupEntry(13980);
	if(sp)
	{
		sp->EffectApplyAuraName[0] = 42;
		sp->procFlags=uint32(PROC_CAST_SPELL|PROC_TAGRGET_SELF);
	}
	//this an on equip item spell(2824) :  ice arrow(29501)
	sp = SpellDataStorage.LookupEntry(29501);
	if(sp)
	{
		sp->procChance = 30;//some say it is triggered every now and then
		sp->procFlags = PROC_ATTACK_RANGED;
	}
	//warrior - berserker rage is missing 1 effect = regenerate rage
	sp = SpellDataStorage.LookupEntry(18499);
	if(sp)
	{
		sp->Effect[2] = 6;
		sp->EffectApplyAuraName[2] = 42;
		sp->EffectTriggerSpell[2] = 9174; //not sure if this is the one. In my time this adds 30 rage
		sp->procFlags = PROC_ANY_DAMAGE_TAKE | PROC_TAGRGET_SELF;
	}
	//warrior - improved berserker rage
	sp = SpellDataStorage.LookupEntry(20500);
	if(sp)
		sp->procFlags = PROC_ANY_DAMAGE_TAKE | PROC_TAGRGET_SELF;
	sp = SpellDataStorage.LookupEntry(20501);
	if(sp)
		sp->procFlags = PROC_ANY_DAMAGE_TAKE | PROC_TAGRGET_SELF;
	//warrior - Rampage
	sp = SpellDataStorage.LookupEntry(29801);
	if(sp)
	{
		sp->procFlags = PROC_ATTACK_MELEE | PROC_TAGRGET_SELF;
		sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
	}
	sp = SpellDataStorage.LookupEntry(30030);
	if(sp)
	{
		sp->procFlags = PROC_ATTACK_MELEE | PROC_TAGRGET_SELF;
		sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
	}
	sp = SpellDataStorage.LookupEntry(30033);
	if(sp)
	{
		sp->procFlags = PROC_ATTACK_MELEE | PROC_TAGRGET_SELF;
		sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
	}
	// priest - Reflective Shield
	sp = SpellDataStorage.LookupEntry(33201);
	if(sp)
	{
		sp->procFlags = PROC_ABSORB;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
	}
	sp = SpellDataStorage.LookupEntry(33202);
	if(sp)
	{
		sp->procFlags = PROC_ABSORB;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
	}
	sp = SpellDataStorage.LookupEntry(33203);
	if(sp)
	{
		sp->procFlags = PROC_ABSORB;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
	}
	sp = SpellDataStorage.LookupEntry(33204);
	if(sp)
	{
		sp->procFlags = PROC_ABSORB;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
	}
	sp = SpellDataStorage.LookupEntry(33205);
	if(sp)
	{
		sp->procFlags = PROC_ABSORB;
		sp->EffectApplyAuraName[0] = 42;
		sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
	}
	// mage - Frost Warding
	sp = SpellDataStorage.LookupEntry(11189);
	if(sp)
	{
		SpellEntry * tsp = SpellDataStorage.LookupEntry(168); //frost armor
		if(tsp)
			sp->EffectSpellGroupRelation[0] = tsp->SpellGroupType;
		tsp = SpellDataStorage.LookupEntry(7302); //ice armor
		if(tsp)
			sp->EffectSpellGroupRelation[0] |= tsp->SpellGroupType;
	}
	sp = SpellDataStorage.LookupEntry(28332);
	if(sp)
	{
		SpellEntry * tsp = SpellDataStorage.LookupEntry(168); //frost armor
		if(tsp)
			sp->EffectSpellGroupRelation[0] = tsp->SpellGroupType;
		tsp = SpellDataStorage.LookupEntry(7302); //ice armor
		if(tsp)
			sp->EffectSpellGroupRelation[0] |= tsp->SpellGroupType;
//		sp->procChance = 100;
	}
	//Gnomish Poultryizer trinket - Poultryizer
	sp = SpellDataStorage.LookupEntry(30507);
	if(sp)
	{
		sp->Effect[0] = 6;
		sp->EffectApplyAuraName[0] = 56;
	}
	// paladin - benediction
	uint32 judgement_group=0;
	SpellEntry * tsp = SpellDataStorage.LookupEntry(20271); //judgement
	if(tsp)
		judgement_group = tsp->SpellGroupType;
	sp = SpellDataStorage.LookupEntry(20101);
	if(sp)
		sp->EffectSpellGroupRelation[0] = (sp->SpellGroupType) | judgement_group;
	sp = SpellDataStorage.LookupEntry(20102);
	if(sp)
		sp->EffectSpellGroupRelation[0] = (sp->SpellGroupType) | judgement_group;
	sp = SpellDataStorage.LookupEntry(20103);
	if(sp)
		sp->EffectSpellGroupRelation[0] = (sp->SpellGroupType) | judgement_group;
	sp = SpellDataStorage.LookupEntry(20104);
	if(sp)
		sp->EffectSpellGroupRelation[0] = (sp->SpellGroupType) | judgement_group;
	sp = SpellDataStorage.LookupEntry(20105);
	if(sp)
		sp->EffectSpellGroupRelation[0] = (sp->SpellGroupType) | judgement_group;
	// paladin - Improved Sanctity Aura
	uint32 Sanctity_group=0;
	tsp = SpellDataStorage.LookupEntry(20218); //Sanctity Aura
	if(tsp)
		Sanctity_group = tsp->SpellGroupType;
	sp = SpellDataStorage.LookupEntry(31869);
	if(sp)
	{
		sp->EffectSpellGroupRelation[0] = Sanctity_group;
		sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
	}
	sp = SpellDataStorage.LookupEntry(31870);
	if(sp)
	{
		sp->EffectSpellGroupRelation[0] = Sanctity_group;
		sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
	}
	sp = SpellDataStorage.LookupEntry(20608); //Reincarnation
	if(sp)
	{
		for(uint32 i=0;i<8;i++)
		{
			if(sp->Reagent[i])
			{
				sp->Reagent[i] = 0;
				sp->ReagentCount[i] = 0;
			}
		}
	}
	
//---------ITEMS-----------------
	sp = SpellDataStorage.LookupEntry(33758);
	if(sp)
		sp->proc_interval = 10000; //10 seconds.
	sp = SpellDataStorage.LookupEntry(33759);
	if(sp)
		sp->proc_interval = 10000; //10 seconds.

	//windfury weapon changes. Start to hate these day by day
	EnchantEntry * Enchantment = dbcEnchant.LookupEntry(283);
	if(Enchantment)
	{
		Enchantment->spell[0] = 33757; //this is actually good
		sp = SpellDataStorage.LookupEntry(33757);
		if(sp)
		{
			sp->EffectApplyAuraName[0] = 42; //who needs dummys anyway ?
			sp->procFlags = PROC_ATTACK_MELEE; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8232; //for the logs and rest
		}
	}
	Enchantment = dbcEnchant.LookupEntry(284);
	if(Enchantment)
	{
		Enchantment->spell[0] = 33756; 
		sp = SpellDataStorage.LookupEntry(33756);
		if(sp)
		{
			sp->EffectApplyAuraName[0] = 42; //who needs dummys anyway ?
			sp->procFlags = PROC_ATTACK_MELEE; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8235; //for the logs and rest
		}
	}
	Enchantment = dbcEnchant.LookupEntry(525);
	if(Enchantment)
	{
		Enchantment->spell[0] = 33755; 
		sp = SpellDataStorage.LookupEntry(33755);
		if(sp)
		{
			sp->EffectApplyAuraName[0] = 42; //who needs dummys anyway ?
			sp->procFlags = PROC_ATTACK_MELEE; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 10486; //for the logs and rest
		}
	}
	Enchantment = dbcEnchant.LookupEntry(1669);
	if(Enchantment)
	{
		Enchantment->spell[0] = 33754; 
		sp = SpellDataStorage.LookupEntry(33754);
		if(sp)
		{
			sp->EffectApplyAuraName[0] = 42; //who needs dummys anyway ?
			sp->procFlags = PROC_ATTACK_MELEE; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 16362; //for the logs and rest
		}
	}
	Enchantment = dbcEnchant.LookupEntry(2636);
	if(Enchantment)
	{
		Enchantment->spell[0] = 33727; 
		sp = SpellDataStorage.LookupEntry(33727);
		if(sp)
		{
			sp->EffectApplyAuraName[0] = 42; //who needs dummys anyway ?
			sp->procFlags = PROC_ATTACK_MELEE; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 25505; //for the logs and rest
		}
	}

	//fix for Predatory Strikes
	uint32 mm=(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1))|(1<<(FORM_MOONKIN-1))|(1<<(FORM_CAT-1));
	SpellDataStorage.LookupEntry(16972)->RequiredShapeShift = mm;
	SpellDataStorage.LookupEntry(16974)->RequiredShapeShift = mm;
	SpellDataStorage.LookupEntry(16975)->RequiredShapeShift = mm;
	SpellDataStorage.LookupEntry(20134)->procChance = 50;

	// aspect of the pack - change to AA 
	SpellDataStorage.LookupEntry(13159)->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
	SpellDataStorage.LookupEntry(13159)->Effect[1] = SPELL_EFFECT_APPLY_AREA_AURA;
	
	// shadowstep - change proc flags 
	SpellDataStorage.LookupEntry(36563)->procFlags = 0;

	// thrown - add a 1.6 second cooldown 
	const static uint32 thrown_spells[] = {SPELL_RANGED_GENERAL,SPELL_RANGED_THROW,SPELL_RANGED_WAND, 26679, 27084, 29436, 37074, 41182, 41346, 0};
	for(i = 0; thrown_spells[i] != 0; ++i)
	{
		sp = SpellDataStorage.LookupEntry(thrown_spells[i]);
		if(sp->RecoveryTime==0 && sp->StartRecoveryTime == 0)
			sp->RecoveryTime = 1600;
	}
*/

	Log.Notice("World","Starting Transport System...");
	objmgr.LoadTransporters();

	// start mail system
	MailSystem::getSingleton().StartMailSystem();

	Log.Notice("World", "Starting Auction System...");
	new AuctionMgr;
	sAuctionMgr.LoadAuctionHouses();

	m_queueUpdateTimer = mQueueUpdateInterval;
	if(Config.MainConfig.GetBoolDefault("Startup", "Preloading", false))
	{
		// Load all data on each map.

		sWorldCreator.GetInstance(0, uint32(0))->LoadAllCells();
		sWorldCreator.GetInstance(1, uint32(0))->LoadAllCells();
		sWorldCreator.GetInstance(530, uint32(0))->LoadAllCells();
	}

	ThreadPool.ExecuteTask(new WorldRunnable);
	if(Config.MainConfig.GetBoolDefault("Startup", "BackgroundLootLoading", true))
	{
		Log.Notice("World", "Backgrounding loot loading...");

		// loot background loading in a lower priority thread.
		ThreadPool.ExecuteTask(new BasicTaskExecutor(new CallbackP0<LootMgr>(LootMgr::getSingletonPtr(), &LootMgr::LoadLoot), 
			BTE_PRIORITY_LOW));
	}
	else
	{
		Log.Notice("World", "Loading loot in foreground...");
		lootmgr.LoadLoot();
	}

	Log.Notice("BattlegroundManager", "Starting...");
	new CBattlegroundManager;

	dw = new DayWatcherThread();
	ThreadPool.ExecuteTask(dw);
	return true;
}


void World::Update(time_t diff)
{
	eventholder->Update(diff);
	sAuctionMgr.Update();
	_UpdateGameTime();

	/* since time() is an expensive system call, we only update it once per server loop */
	UNIXTIME = time(NULL);
}


void World::SendGlobalMessage(WorldPacket *packet, WorldSession *self)
{
	m_sessionlock.AcquireReadLock();

	SessionMap::iterator itr;
	for (itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		if (itr->second->GetPlayer() &&
			itr->second->GetPlayer()->IsInWorld()
			&& itr->second != self)  // dont send to self!
		{
			itr->second->SendPacket(packet);
		}
	}

	m_sessionlock.ReleaseReadLock();
}
void World::SendFactionMessage(WorldPacket *packet, uint8 teamId)
{
	m_sessionlock.AcquireReadLock();
	SessionMap::iterator itr;
	Player * plr;
	for(itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		plr = itr->second->GetPlayer();
		if(!plr || !plr->IsInWorld())
			continue;

		if(plr->GetTeam() == teamId)
			itr->second->SendPacket(packet);
	}
	m_sessionlock.ReleaseReadLock();
}

void World::SendZoneMessage(WorldPacket *packet, uint32 zoneid, WorldSession *self)
{
	m_sessionlock.AcquireReadLock();

	SessionMap::iterator itr;
	for (itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		if (itr->second->GetPlayer() &&
			itr->second->GetPlayer()->IsInWorld()
			&& itr->second != self)  // dont send to self!
		{
			if (itr->second->GetPlayer()->GetZoneId() == zoneid)
				itr->second->SendPacket(packet);
		}
	}

	m_sessionlock.ReleaseReadLock();
}

void World::SendWorldText(const char* text, WorldSession *self)
{
    uint32 textLen = strlen((char*)text) + 1;

    WorldPacket data(textLen + 40);

	data.Initialize(SMSG_MESSAGECHAT);
	data << uint8(CHAT_MSG_SYSTEM);
	data << uint32(LANG_UNIVERSAL);
	
	data << (uint64)0; // Who cares about guid when there's no nickname displayed heh ?
	data << (uint32)0;
	data << (uint64)0;

	data << textLen;
	data << text;
	data << uint8(0);

	SendGlobalMessage(&data, self);

	sLog.outString("> %s", text);
}

void World::SendWorldWideScreenText(const char *text, WorldSession *self)
{
	WorldPacket data(256);
	data.Initialize(SMSG_AREA_TRIGGER_MESSAGE);
	data << (uint32)0 << text << (uint8)0x00;
	SendGlobalMessage(&data, self);
}

void World::UpdateSessions(uint32 diff)
{
	SessionSet::iterator itr, it2;
	WorldSession *session;
	int result;
	for(itr = Sessions.begin(); itr != Sessions.end();)
	{
		session = (*itr);
		it2 = itr;
		++itr;
		if(!session || session->GetInstance() != 0)
		{
			Sessions.erase(it2);
			continue;
		}

		if((result = session->Update(0)))
		{
			if(result == 1)
			{
				// complete deletion
				DeleteSession(session);
			}
			Sessions.erase(it2);
		}
	}
}

std::string World::GenerateName(uint32 type)
{
	if(_namegendata[type].size() == 0)
		return "ERR";

	uint32 ent = sRand.randInt(_namegendata[type].size()-1);
	return _namegendata[type].at(ent).name;
}

void World::DeleteSession(WorldSession *session)
{
	m_sessionlock.AcquireWriteLock();
	// remove from big map
	m_sessions.erase(session->GetAccountId());

	m_sessionlock.ReleaseWriteLock();

	// delete us
	delete session;
}

uint32 World::GetNonGmSessionCount()
{
	m_sessionlock.AcquireReadLock();

	uint32 total = m_sessions.size();

	SessionMap::const_iterator itr = m_sessions.begin();
	for( ; itr != m_sessions.end(); itr++ )
	{
		if( (itr->second)->HasGMPermissions() )
			total--;
	}

	m_sessionlock.ReleaseReadLock();

	return total;
}

uint32 World::AddQueuedSocket(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Add socket to list
	mQueuedSessions.push_back(Socket);
	queueMutex.Release();
	// Return queue position
	return mQueuedSessions.size();
}

void World::RemoveQueuedSocket(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Find socket in list
	QueueSet::iterator iter = mQueuedSessions.begin();
	for(; iter != mQueuedSessions.end(); ++iter)
	{
		if((*iter) == Socket)
		{
			// Remove from the queue and abort.
			// This will be slow (Removing from middle of a vector!) but it won't
			// get called very much, so it's not really a big deal.

			mQueuedSessions.erase(iter);
			queueMutex.Release();
			return;
		}
	}
	queueMutex.Release();
}

uint32 World::GetQueuePos(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Find socket in list
	QueueSet::iterator iter = mQueuedSessions.begin();
	uint32 QueuePos = 1;
	for(; iter != mQueuedSessions.end(); ++iter, ++QueuePos)
	{
		if((*iter) == Socket)
		{
			queueMutex.Release();
			// Return our queue position.
			return QueuePos;
		}
	}
	queueMutex.Release();
	// We shouldn't get here..
	return 1;
}

void World::UpdateQueuedSessions(uint32 diff)
{
#ifndef CLUSTERING
	if(diff >= m_queueUpdateTimer) 
	{
		m_queueUpdateTimer = mQueueUpdateInterval;
		queueMutex.Acquire();

		if(mQueuedSessions.size() == 0)
		{
			queueMutex.Release();
			return;
		}
		
		if(m_sessions.size() < m_playerLimit)
		{
			// Yay. We can let another player in now.
			// Grab the first fucker from the queue, but guard of course, since
			// this is in a different thread again.

			QueueSet::iterator iter = mQueuedSessions.begin();
			WorldSocket * QueuedSocket = *iter;
			mQueuedSessions.erase(iter);

			// Welcome, sucker.
			if(QueuedSocket->GetSession())
			{
				QueuedSocket->GetSession()->deleteMutex.Acquire();
				QueuedSocket->Authenticate();
			}
		}

		if(mQueuedSessions.size() == 0)
		{
			queueMutex.Release();
			return;
		}

		// Update the remaining queue members.
		QueueSet::iterator iter = mQueuedSessions.begin();
		uint32 Position = 1;
		for(; iter != mQueuedSessions.end(); ++iter, ++Position)
		{
			(*iter)->UpdateQueuePosition(Position);
		}
		queueMutex.Release();
	} 
	else 
	{
		m_queueUpdateTimer -= diff;
	}
#endif
}

void World::SaveAllPlayers()
{
	if(!(ObjectMgr::getSingletonPtr() && WorldCreator::getSingletonPtr()))
		return;

	sLog.outString("Saving all players to database...");
	uint32 count = 0;
	PlayerStorageMap::const_iterator itr;
		// Servers started and obviously runing. lets save all players.
	uint32 mt;
	objmgr._playerslock.AcquireReadLock();   
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
		{
			if(itr->second->GetSession())
			{
				mt = getMSTime();
				itr->second->SaveToDB(false);
				sLog.outString("Saved player `%s` (level %u) in %ums.", itr->second->GetName(), itr->second->GetUInt32Value(UNIT_FIELD_LEVEL), getMSTime() - mt);
				++count;
			}
		}
	objmgr._playerslock.ReleaseReadLock();
	sLog.outString("Saved %u players.", count);
}


WorldSession* World::FindSessionByName(const char * Name)//case insensetive
{
	m_sessionlock.AcquireReadLock();

	// loop sessions, see if we can find him
	SessionMap::iterator itr = m_sessions.begin();
	for(; itr != m_sessions.end(); ++itr)
	{
	  if(!stricmp(itr->second->GetAccountName().c_str(),Name))
	  {
		  m_sessionlock.ReleaseReadLock();
			return itr->second;
	  }
	}
	m_sessionlock.ReleaseReadLock();
	return 0;
}

void World::BroadcastExtendedMessage(WorldSession * self, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	char msg[1024];
	vsnprintf(msg, 1024, str, ap);
	va_end(ap);
	SessionSet::iterator itr = mExtendedSessions.begin();
	WorldSession * s;
	for(; itr != mExtendedSessions.end(); )
	{
		s = *itr;
		++itr;

		if(s->GetPlayer() /*&& s != this*/)
			s->GetPlayer()->BroadcastMessage(msg);
	}
}

void World::ShutdownClasses()
{
	sLog.outString("Deleting Addon Manager...");
	sAddonMgr.SaveToDB();
	delete AddonMgr::getSingletonPtr();

	sLog.outString("\nDeleting Auction Manager...");
	delete AuctionMgr::getSingletonPtr();
	sLog.outString("Deleting Loot Manager...");
	delete LootMgr::getSingletonPtr();

	delete MailSystem::getSingletonPtr();
	delete WorldCreator::getSingletonPtr();
}

void World::EventDeleteBattleground(Battleground * BG)
{
	// remove the instance
	//sWorldCreator.DestroyBattlegroundInstance(BG);
	//sBattlegroundMgr.RemoveBattleground(BG->GetID());
}

void World::GetStats(uint32 * GMCount, float * AverageLatency)
{
	int gm = 0;
	int count = 0;
	int avg = 0;
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetSession())
		{
			count++;
			avg += itr->second->GetSession()->GetLatency();
			if(itr->second->GetSession()->GetPermissionCount())
				gm++;
		}			
	}
	objmgr._playerslock.ReleaseReadLock();

	*AverageLatency = count ? (float)((float)avg / (float)count) : 0;
	*GMCount = gm;
}

void TaskList::AddTask(Task * task)
{
	queueLock.Acquire();
	tasks.insert(task);
	queueLock.Release();
}

Task * TaskList::GetTask()
{
	queueLock.Acquire();

	Task* t = 0;
	for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); ++itr)
	{
		if(!(*itr)->in_progress)
		{
			t = (*itr);
			t->in_progress = true;
			break;
		}
	}
	queueLock.Release();
	return t;
}

void TaskList::spawn()
{
	running = true;
	thread_count = 0;

	uint32 threadcount;
	if(Config.MainConfig.GetBoolDefault("Startup", "EnableMultithreadedLoading", true))
	{
		// get processor count
#ifndef WIN32
#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX
		long affmask;
		sched_getaffinity(0, 4, (cpu_set_t*)&affmask);
		threadcount = (BitCount8(affmask)) * 2;
		if(threadcount > 8) threadcount = 8;
		else if(threadcount <= 0) threadcount = 1;
#else
		threadcount = 2;
#endif
#else
		SYSTEM_INFO s;
		GetSystemInfo(&s);
		threadcount = s.dwNumberOfProcessors * 2;
#endif
	}
	else
		threadcount = 1;

	Log.Line();
	Log.Notice("World", "Beginning %s server startup with %u threads.", (threadcount == 1) ? "progressive" : "parallel", threadcount);
	Log.Line();

	for(uint32 x = 0; x < threadcount; ++x)
		ThreadPool.ExecuteTask(new TaskExecutor(this));
}

void TaskList::wait()
{
	bool has_tasks = true;
	while(has_tasks)
	{
		queueLock.Acquire();
		has_tasks = false;
		for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); ++itr)
		{
			if(!(*itr)->completed)
			{
				has_tasks = true;
				break;
			}
		}
		queueLock.Release();
		Sleep(20);
	}
}

void TaskList::kill()
{
	running = false;
}

void Task::execute()
{
	_cb->execute();
}

bool TaskExecutor::run()
{
	Task * t;
	THREAD_TRY_EXECUTION
	{
		while(starter->running)
		{
			t = starter->GetTask();
			if(t)
			{
				t->execute();
				t->completed = true;
				starter->RemoveTask(t);
				delete t;
			}
			else
				Sleep(20);
		}
	}
	THREAD_HANDLE_CRASH
	return true;
}

void TaskList::waitForThreadsToExit()
{
	while(thread_count)
	{
		Sleep(20);
	}
}

void World::DeleteObject(Object * obj)
{
	delete obj;
}

void World::Rehash(bool load)
{
	if(load)
	{
		#ifdef WIN32
		Config.MainConfig.SetSource("ascent.conf", true);
		#else
		Config.MainConfig.SetSource((char*)CONFDIR "/ascent.conf", true);
		#endif
	}

	if(!ChannelMgr::getSingletonPtr())
		new ChannelMgr;

	if(!MailSystem::getSingletonPtr())
		new MailSystem;

	channelmgr.seperatechannels = Config.MainConfig.GetBoolDefault("Server", "SeperateChatChannels", false);
	PvpFactionLimitation = Config.MainConfig.GetBoolDefault("Server", "PvpFactionLimitation", true);
	FriendFactionLimitation = Config.MainConfig.GetBoolDefault("Server", "FriendFactionLimitation", true);
	sendRevisionOnJoin = Config.MainConfig.GetBoolDefault("Server", "SendBuildOnJoin", false);
	MapPath = Config.MainConfig.GetStringDefault("Terrain", "MapPath", "maps");
	UnloadMapFiles = Config.MainConfig.GetBoolDefault("Terrain", "UnloadMapFiles", true);
	BreathingEnabled = Config.MainConfig.GetBoolDefault("Server", "EnableBreathing", true);
	SendStatsOnJoin = Config.MainConfig.GetBoolDefault("Server", "SendStatsOnJoin", true);
	compression_threshold = Config.MainConfig.GetIntDefault("Server", "CompressionThreshold", 1000);

	// load regeneration rates.
	setRate(RATE_HEALTH,Config.MainConfig.GetFloatDefault("Rates", "Health",1));
	setRate(RATE_POWER1,Config.MainConfig.GetFloatDefault("Rates", "Power1",1));
	setRate(RATE_POWER2,Config.MainConfig.GetFloatDefault("Rates", "Power2",1));
	setRate(RATE_POWER3,Config.MainConfig.GetFloatDefault("Rates", "Power4",1));
	setRate(RATE_DROP0,Config.MainConfig.GetFloatDefault("Rates", "DropGrey",1));
  setRate(RATE_DROP1,Config.MainConfig.GetFloatDefault("Rates", "DropWhite",1));
  setRate(RATE_DROP2,Config.MainConfig.GetFloatDefault("Rates", "DropGreen",1));
  setRate(RATE_DROP3,Config.MainConfig.GetFloatDefault("Rates", "DropBlue",1));
  setRate(RATE_DROP4,Config.MainConfig.GetFloatDefault("Rates", "DropPurple",1));
  setRate(RATE_DROP5,Config.MainConfig.GetFloatDefault("Rates", "DropOrange",1));
  setRate(RATE_DROP6,Config.MainConfig.GetFloatDefault("Rates", "DropArtifact",1));
	setRate(RATE_XP,Config.MainConfig.GetFloatDefault("Rates", "XP",1));
	setRate(RATE_RESTXP,Config.MainConfig.GetFloatDefault("Rates", "RestXP", 1));
	setRate(RATE_QUESTXP,Config.MainConfig.GetFloatDefault("Rates", "QuestXP", 1));
	setIntRate(INTRATE_SAVE, Config.MainConfig.GetIntDefault("Rates", "Save", 1));
	setRate(RATE_MONEY, Config.MainConfig.GetFloatDefault("Rates", "DropMoney", 1.0f));
	setRate(RATE_QUESTREPUTATION, Config.MainConfig.GetFloatDefault("Rates", "QuestReputation", 1.0f));
	setRate(RATE_KILLREPUTATION, Config.MainConfig.GetFloatDefault("Rates", "KillReputation", 1.0f));
	setRate(RATE_HONOR, Config.MainConfig.GetFloatDefault("Rates", "Honor", 1.0f));
	setRate(RATE_SKILLCHANCE, Config.MainConfig.GetFloatDefault("Rates", "SkillChance", 1.0f));
	setRate(RATE_SKILLRATE, Config.MainConfig.GetFloatDefault("Rates", "SkillRate", 1.0f));
	setIntRate(INTRATE_COMPRESSION, Config.MainConfig.GetIntDefault("Rates", "Compression", 1));
	setIntRate(INTRATE_PVPTIMER, Config.MainConfig.GetIntDefault("Rates", "PvPTimer", 300000));
	setRate(RATE_ARENAPOINTMULTIPLIER2X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier2x", 1.0f));
	setRate(RATE_ARENAPOINTMULTIPLIER3X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier3x", 1.0f));
	setRate(RATE_ARENAPOINTMULTIPLIER5X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier5x", 1.0f));
	SetPlayerLimit(Config.MainConfig.GetIntDefault("Server", "PlayerLimit", 1000));
	SetMotd(Config.MainConfig.GetStringDefault("Server", "Motd", "Ascent Default MOTD").c_str());
	SetUpdateDistance( Config.MainConfig.GetFloatDefault("Server", "PlrUpdateDistance", 79.1f) );
	mQueueUpdateInterval = Config.MainConfig.GetIntDefault("Server", "QueueUpdateInterval", 5000);
	SetKickAFKPlayerTime(Config.MainConfig.GetIntDefault("Server", "KickAFKPlayers", 0));
	sLog.SetScreenLoggingLevel(Config.MainConfig.GetIntDefault("LogLevel", "Screen", 1));
	sLog.SetFileLoggingLevel(Config.MainConfig.GetIntDefault("LogLevel", "File", -1));
	Log.log_level = Config.MainConfig.GetIntDefault("LogLevel", "Screen", 1);
	gm_skip_attunement = Config.MainConfig.GetBoolDefault("Server", "SkipAttunementsForGM", true);
#ifndef CLUSTERING
	SocketRecvBufSize = Config.MainConfig.GetIntDefault("WorldSocket", "RecvBufSize", WORLDSOCKET_RECVBUF_SIZE);
	SocketSendBufSize = Config.MainConfig.GetIntDefault("WorldSocket", "SendBufSize", WORLDSOCKET_SENDBUF_SIZE);
#endif

	bool log_enabled = Config.MainConfig.GetBoolDefault("Log", "Cheaters", false);
	if(Anticheat_Log->IsOpen())
	{
		if(!log_enabled)
			Anticheat_Log->Close();
	}
	else
		if(log_enabled)
			Anticheat_Log->Open();

	log_enabled = Config.MainConfig.GetBoolDefault("Log", "GMCommands", false);
	if(GMCommand_Log->IsOpen())
	{
		if(!log_enabled)
			GMCommand_Log->Close();
	}
	else
		if(log_enabled)
			GMCommand_Log->Open();

#ifdef WIN32
	DWORD current_priority_class = GetPriorityClass(GetCurrentProcess());
	bool high = Config.MainConfig.GetBoolDefault("Server", "AdjustPriority", false);

	if(current_priority_class == HIGH_PRIORITY_CLASS && !high)
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	else if(current_priority_class != HIGH_PRIORITY_CLASS && high)
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	reqGmClient = Config.MainConfig.GetBoolDefault("GMClient", "ReqGmClient", false);
	if(!Config.MainConfig.GetString("GMClient", "GmClientChannel", &GmClientChannel))
	{
		GmClientChannel = "";
	}

	realmtype = Config.MainConfig.GetBoolDefault("Server", "RealmType", false);
	TimeOut= uint32(1000* Config.MainConfig.GetIntDefault("Server", "ConnectionTimeout", 180) );

	uint32 config_flags = 0;
	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageCostsForGM", true))
		config_flags |= MAIL_FLAG_NO_COST_FOR_GM;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageCosts", false))
		config_flags |= MAIL_FLAG_DISABLE_POSTAGE_COSTS;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageDelayItems", true))
		config_flags |= MAIL_FLAG_DISABLE_HOUR_DELAY_FOR_ITEMS;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisableMessageExpiry", false))
		config_flags |= MAIL_FLAG_NO_EXPIRY;

	if(Config.MainConfig.GetBoolDefault("Mail", "EnableInterfactionMail", true))
		config_flags |= MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION;

	if(Config.MainConfig.GetBoolDefault("Mail", "EnableInterfactionForGM", true))
		config_flags |= MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION_GM;

	sMailSystem.config_flags = config_flags;
	flood_lines = Config.MainConfig.GetIntDefault("FloodProtection", "Lines", 0);
	flood_seconds = Config.MainConfig.GetIntDefault("FloodProtection", "Seconds", 0);
	flood_message = Config.MainConfig.GetBoolDefault("FloodProtection", "SendMessage", false);
	show_gm_in_who_list = Config.MainConfig.GetBoolDefault("Server", "ShowGMInWhoList", true);
	if(!flood_lines || !flood_seconds)
		flood_lines = flood_seconds = 0;

	map_unload_time=Config.MainConfig.GetIntDefault("Server", "MapUnloadTime", 0);

	antihack_teleport = Config.MainConfig.GetBoolDefault("AntiHack", "Teleport", true);
	antihack_speed = Config.MainConfig.GetBoolDefault("AntiHack", "Speed", true);
	antihack_falldmg = Config.MainConfig.GetBoolDefault("AntiHack", "FallDamage", true);
	antihack_flight = Config.MainConfig.GetBoolDefault("AntiHack", "Flight", true);
	no_antihack_on_gm = Config.MainConfig.GetBoolDefault("AntiHack", "DisableOnGM", false);
	SpeedhackProtection = antihack_speed;
	Channel::LoadConfSettings();
}

void World::LoadNameGenData()
{
	DBCFile dbc;
	dbc.open("DBC/NameGen.dbc");
	for(uint32 i = 0; i < dbc.getRecordCount(); ++i)
	{
		NameGenData d;
		d.name = string(dbc.getRecord(i).getString(1));
		d.type = dbc.getRecord(i).getUInt(3);
		_namegendata[d.type].push_back(d);
	}
}

void World::CharacterEnumProc(QueryResultVector& results, uint32 AccountId)
{
	WorldSession * s = FindSession(AccountId);
	if(s == NULL)
		return;

	s->CharacterEnumProc(results[0].result);
}

void World::LoadAccountDataProc(QueryResultVector& results, uint32 AccountId)
{
	WorldSession * s = FindSession(AccountId);
	if(s == NULL)
		return;

	s->LoadAccountDataProc(results[0].result);
}

void World::CleanupCheaters()
{
	/*uint32 guid;
	string name;
	uint32 cl;
	uint32 level;
	uint32 talentpts;
	char * start, *end;
	Field * f;
	uint32 should_talents;
	uint32 used_talents;
	SpellEntry * sp;

	QueryResult * result = CharacterDatabase.Query("SELECT guid, name, class, level, available_talent_points, spells FROM characters");
	if(result == NULL)
		return;

	do 
	{
		f = result->Fetch();
		guid = f[0].GetUInt32();
		name = string(f[1].GetString());
		cl = f[2].GetUInt32();
		level = f[3].GetUInt32();
		talentpts = f[4].GetUInt32();
		start = f[5].GetString();
		should_talents = (level<10 ? 0 : level - 9);
		used_talents -= 
        		

		start = (char*)get_next_field.GetString();//buff;
		while(true) 
		{
			end = strchr(start,',');
			if(!end)break;
			*end=0;
			sp = SpellDataStorage.LookupEntry(atol(start));
			start = end +1;

			if(sp->talent_tree)

		}

	} while(result->NextRow());*/

}
