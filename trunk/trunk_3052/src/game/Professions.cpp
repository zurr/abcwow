/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

uint32 GetGOReqSkill(GameObject * gameObjTarget)  
{
	if(gameObjTarget->GetEntry()==180215)return 300;
	GameObjectInfo*gi= GameObjectNameStorage.LookupEntry(gameObjTarget->GetEntry());
	if(!gi)
	{
		sLog.outError("Unknown go entry=%u",gameObjTarget->GetEntry());
		return 0;
	}

	Lock *lock = dbcLock.LookupEntry( gi->SpellFocus );
	if(!lock) return 0;
	for(uint32 i=0;i<5;i++)
		if(lock->locktype[i] == 2 && lock->minlockskill[i])
		{
			return lock->minlockskill[i];
		}
	return 0;
}

void AddItemFromDisenchant(uint32 loot_id, Player *owner)
{
	LootStore::iterator tab =lootmgr.DisenchantingLoot.find(loot_id);
	if( lootmgr.DisenchantingLoot.end()==tab)
		return;

	StoreLootList *list = &(tab->second);

	uint32 count;
	for( uint32 x = 0; x < list->count; x++ )
	{
		if( list->items[x].item.itemproto )
		{
			float chance = list->items[x].chance;
			if(chance == 0.0f) continue;
			
			ItemPrototype *itemproto = list->items[x].item.itemproto;
			if(Rand(chance))
			{
				if( list->items[x].mincount == list->items[x].maxcount )
					count = list->items[x].maxcount;
				else
					count = RandomUInt(list->items[x].maxcount - list->items[x].mincount) + list->items[x].mincount;

				Item *add;
				SlotResult slotresult;
				add = owner->GetItemInterface()->FindItemLessMax(list->items[x].item.itemproto->ItemId, count, false);
				if (!add)
				{
					slotresult = owner->GetItemInterface()->FindFreeInventorySlot(itemproto);
					if(!slotresult.Result)
					{
						owner->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						return;
					}
					Item * it=objmgr.CreateItem(list->items[x].item.itemproto->ItemId,owner);  
					it->SetUInt32Value( ITEM_FIELD_STACK_COUNT, count);
					owner->GetItemInterface()->SafeAddItem(it,slotresult.ContainerSlot, slotresult.Slot);
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + count);
					add->m_isDirty = true;
				}
			}
		}
	}
}


void AddItemFromProspecting(uint32 loot_id, Player *owner)
{
	LootStore::iterator tab =lootmgr.ProspectingLoot.find(loot_id);
	if( lootmgr.ProspectingLoot.end()==tab)
		return;

	StoreLootList *list = &(tab->second);

	uint32 count;
	for( uint32 x = 0; x < list->count; x++ )
	{
		if( list->items[x].item.itemproto )
		{
			float chance = list->items[x].chance;
			if(chance == 0.0f) continue;
			
			ItemPrototype *itemproto = list->items[x].item.itemproto;
			if(Rand(chance))
			{
				if( list->items[x].mincount == list->items[x].maxcount )
					count = list->items[x].maxcount;
				else
					count = RandomUInt(list->items[x].maxcount - list->items[x].mincount) + list->items[x].mincount;

				Item *add;
				SlotResult slotresult;
				add = owner->GetItemInterface()->FindItemLessMax(list->items[x].item.itemproto->ItemId, count, false);
				if (!add)
				{
					slotresult = owner->GetItemInterface()->FindFreeInventorySlot(itemproto);
					if(!slotresult.Result)
					{
						owner->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						return;
					}
					Item * it=objmgr.CreateItem(list->items[x].item.itemproto->ItemId,owner);  
					it->SetUInt32Value( ITEM_FIELD_STACK_COUNT, count);
					owner->GetItemInterface()->SafeAddItem(it,slotresult.ContainerSlot, slotresult.Slot);
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + count);
					add->m_isDirty = true;
				}
			}
		}
	}
}
