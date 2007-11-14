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

//
// MapMgr.h
//

#ifndef __MAPMGR_H
#define __MAPMGR_H

class MapCell;
class Map;
class Object;
class MapScriptInterface;
class WorldSession;
class GameObject;
class Creature;
class Player;
class Pet;
class Transporter;
class Corpse;
class CBattleground;
class Instance;


enum MapMgrTimers
{
	MMUPDATE_OBJECTS = 0,
	MMUPDATE_SESSIONS = 1,
	MMUPDATE_FIELDS = 2,
	MMUPDATE_IDLE_OBJECTS = 3,
	MMUPDATE_ACTIVE_OBJECTS = 4,
	MMUPDATE_COUNT = 5
};

enum ObjectActiveState
{
	OBJECT_STATE_NONE	 = 0,
	OBJECT_STATE_INACTIVE = 1,
	OBJECT_STATE_ACTIVE   = 2,
};

typedef std::set<Object*> ObjectSet;
typedef std::set<Object*> UpdateQueue;
typedef std::set<Player*> PUpdateQueue;
typedef std::set<Player*> PlayerSet;
typedef HM_NAMESPACE::hash_map<uint32, Object*> StorageMap;
typedef set<uint64> CombatProgressMap;
typedef set<Creature*> CreatureSet;
typedef set<GameObject*> GameObjectSet;
typedef HM_NAMESPACE::hash_map<uint32, Creature*> CreatureSqlIdMap;
typedef HM_NAMESPACE::hash_map<uint32, GameObject*> GameObjectSqlIdMap;

#define MAX_TRANSPORTERS_PER_MAP 25

class Transporter;
#define RESERVE_EXPAND_SIZE 1024
class LuaEngine;

class SERVER_DECL MapMgr : public CellHandler <MapCell>, public EventableObject,public CThread
{
	friend class UpdateObjectThread;
	friend class ObjectUpdaterThread;
	friend class MapCell;
	friend class MapScriptInterface;
public:
		
	//This will be done in regular way soon

	Mutex m_objectinsertlock;
	ObjectSet m_objectinsertpool;
	void AddObject(Object *);

////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of GameObjects
/////////////////////////////////////////////
	uint32 m_GOArraySize;
	uint32 m_GOHighGuid;
	GameObject ** m_GOStorage;
	__inline GameObject * CreateGameObject()
	{
		if(_reusable_guids_gameobject.size())
		{
			uint32 guid = _reusable_guids_gameobject.front();
			_reusable_guids_gameobject.pop_front();
			return new GameObject(HIGHGUID_GAMEOBJECT, guid);
		}

		if(++m_GOHighGuid  >= m_GOArraySize)
		{
			// Reallocate array with larger size.
			m_GOArraySize += RESERVE_EXPAND_SIZE;
			m_GOStorage = (GameObject**)realloc(m_GOStorage, sizeof(GameObject*) * m_GOArraySize);
			memset(&m_GOStorage[m_GOHighGuid],0,(m_GOArraySize-m_GOHighGuid)*sizeof(GameObject*));
		}
		return new GameObject(HIGHGUID_GAMEOBJECT, m_GOHighGuid);
	}

	__inline uint32 GenerateGameobjectGuid()
	{
		return ++m_GOHighGuid;
	}

	__inline GameObject * GetGameObject(uint32 guid)
	{
		if(guid > m_GOHighGuid)
			return 0;
		return m_GOStorage[guid];
	}

/////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of Creatures
/////////////////////////////////////////////
	uint32 m_CreatureArraySize;
	uint32 m_CreatureHighGuid;
	Creature ** m_CreatureStorage;
	__inline Creature * CreateCreature()
	{
		if(_reusable_guids_creature.size())
		{
			uint32 guid = _reusable_guids_creature.front();
			_reusable_guids_creature.pop_front();
			return new Creature(HIGHGUID_UNIT, guid);
		}

		if(++m_CreatureHighGuid  >= m_CreatureArraySize)
		{
			// Reallocate array with larger size.
			m_CreatureArraySize += RESERVE_EXPAND_SIZE;
			m_CreatureStorage = (Creature**)realloc(m_CreatureStorage, sizeof(Creature*) * m_CreatureArraySize);
			memset(&m_CreatureStorage[m_CreatureHighGuid],0,(m_CreatureArraySize-m_CreatureHighGuid)*sizeof(Creature*));
		}
		
		return new Creature(HIGHGUID_UNIT, m_CreatureHighGuid);
	}

	__inline Creature * GetCreature(uint32 guid)
	{
		if(guid > m_CreatureHighGuid)
			return 0;
		return m_CreatureStorage[guid];
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of DynamicObjects
////////////////////////////////////////////
	uint32 m_DynamicObjectHighGuid;
	typedef HM_NAMESPACE::hash_map<uint32, DynamicObject*> DynamicObjectStorageMap;
	DynamicObjectStorageMap m_DynamicObjectStorage;
	__inline DynamicObject * CreateDynamicObject()
	{
		return new DynamicObject(HIGHGUID_DYNAMICOBJECT, ++m_DynamicObjectHighGuid);
	}

	__inline DynamicObject * GetDynamicObject(uint32 guid)
	{
		DynamicObjectStorageMap::iterator itr = m_DynamicObjectStorage.find(guid);
		return (itr != m_DynamicObjectStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of pets
///////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<uint32, Pet*> PetStorageMap;
	PetStorageMap m_PetStorage;
	__inline Pet * GetPet(uint32 guid)
	{
		PetStorageMap::iterator itr = m_PetStorage.find(guid);
		return (itr != m_PetStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of players for faster lookup
////////////////////////////////
    
    // double typedef lolz// a compile breaker..
	typedef HM_NAMESPACE::hash_map<uint32, Player*>                     PlayerStorageMap;
	PlayerStorageMap m_PlayerStorage;
	__inline Player * GetPlayer(uint32 guid)
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.find(guid);
		return (itr != m_PlayerStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of combats in progress
////////////////////////////////
	CombatProgressMap _combatProgress;
	void AddCombatInProgress(uint64 guid)
	{
		_combatProgress.insert(guid);
	}
	void RemoveCombatInProgress(uint64 guid)
	{
		_combatProgress.erase(guid);
	}

//////////////////////////////////////////////////////////
// Lookup Wrappers
///////////////////////////////////
	Unit * GetUnit(const uint64 & guid);
	Object * _GetObject(const uint64 & guid);

	bool run();
	bool Do();

	MapMgr(Map *map, uint32 mapid, uint32 instanceid);
	~MapMgr();

	void PushObject(Object *obj);
	void PushStaticObject(Object * obj);
	void RemoveObject(Object *obj, bool free_guid);
	void ChangeObjectLocation(Object *obj); // update inrange lists
	void ChangeFarsightLocation(Player *plr, Creature *farsight);

	//! Mark object as updated
	void ObjectUpdated(Object *obj);
	void UpdateCellActivity(uint32 x, uint32 y, int radius);

	// Terrain Functions
	inline float  GetLandHeight(float x, float y) { return GetBaseMap()->GetLandHeight(x, y); }
	inline float  GetWaterHeight(float x, float y) { return GetBaseMap()->GetWaterHeight(x, y); }
	inline uint8  GetWaterType(float x, float y) { return GetBaseMap()->GetWaterType(x, y); }
	inline uint8  GetWalkableState(float x, float y) { return GetBaseMap()->GetWalkableState(x, y); }
	inline uint16 GetAreaID(float x, float y) { return GetBaseMap()->GetAreaID(x, y); }

	inline uint32 GetMapId() { return _mapId; }

	void PushToProcessed(Player* plr);

	inline bool HasPlayers() { return (m_PlayerStorage.size() > 0); }
	inline bool IsCombatInProgress() { return (_combatProgress.size() > 0); }
	void TeleportPlayers();

	inline uint32 GetInstanceID() { return m_instanceID; }
	inline MapInfo *GetMapInfo() { return pMapInfo; }

	bool _shutdown;

	inline MapScriptInterface * GetInterface() { return ScriptInterface; }
	virtual int32 event_GetInstanceID() { return m_instanceID; }

	void LoadAllCells();
	inline size_t GetPlayerCount() { return m_PlayerStorage.size(); }

	void _PerformObjectDuties();
	uint32 mLoopCounter;
	uint32 lastGameobjectUpdate;
	uint32 lastUnitUpdate;
	void EventCorpseDespawn(uint64 guid);

	time_t InactiveMoveTime;
    uint32 iInstanceMode;

	void UnloadCell(uint32 x,uint32 y);
	void EventRespawnCreature(Creature * c, MapCell * p);
	void EventRespawnGameObject(GameObject * o, MapCell * c);
	void SendMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius = 2);
	void SendChatMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator);

	Instance * pInstance;
	void BeginInstanceExpireCountdown();
	
	// better hope to clear any references to us when calling this :P
	void InstanceShutdown()
	{
		pInstance = NULL;
		SetThreadState(THREADSTATE_TERMINATE);
	}

	// kill the worker thread only
	void KillThread()
	{
		pInstance=NULL;
		thread_kill_only = true;
		SetThreadState(THREADSTATE_TERMINATE);
		while(thread_running)
		{
			Sleep(100);
		}
	}

protected:

	//! Collect and send updates to clients
	void _UpdateObjects();

private:
	//! Objects that exist on map
 
	uint32 _mapId;
	set<Object*> _mapWideStaticObjects;

	bool _CellActive(uint32 x, uint32 y);
	void UpdateInRangeSet(Object *obj, Player *plObj, MapCell* cell, ByteBuffer ** buf);

public:
	// Distance a Player can "see" other objects and receive updates from them (!! ALREADY dist*dist !!)
	float m_UpdateDistance;

private:
	/* Update System */
	FastMutex m_updateMutex;		// use a user-mode mutex for extra speed
	UpdateQueue _updates;
	PUpdateQueue _processQueue;

	/* Sessions */
	
	SessionSet Sessions;

	/* Map Information */
	MapInfo *pMapInfo;
	uint32 m_instanceID;

	MapScriptInterface * ScriptInterface;

public:
#ifdef WIN32
	DWORD threadid;
#endif

	GameObjectSet activeGameObjects;
	CreatureSet activeCreatures;
	EventableObjectHolder eventHolder;
	CBattleground * m_battleground;
	set<Corpse*> m_corpses;
	CreatureSqlIdMap _sqlids_creatures;
	GameObjectSqlIdMap _sqlids_gameobjects;

	Creature * GetSqlIdCreature(uint32 sqlid);
	GameObject * GetSqlIdGameObject(uint32 sqlid);
	deque<uint32> _reusable_guids_gameobject;
	deque<uint32> _reusable_guids_creature;
	inline LuaEngine* GetScriptEngine() { return m_engine; }

	bool forced_expire;
	bool thread_kill_only;
	bool thread_running;
	LuaEngine * m_engine;
};

#endif
