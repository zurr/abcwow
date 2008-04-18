#include "StdAfx.h"
#include "Setup.h"


#define TOKEN_COST				3
#define FACTION_TOKEN_COST		50
#define HONOR_COST				3000
#define ARENA_COST				200
/*--------------------------------*/
#define HALAA_TOKEN				26045
#define HONOR_HOLD_TOKEN		24579
#define THRALLMAR_TOKEN			24581
#define EOTS_MARK				29024
#define WARSONG_MARK			20558
#define AV_MARK					20560
#define AB_MARK					20559


class SCRIPT_DECL Token_Vendor: public GossipScript
{
	public:
		void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
		void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
		void GossipEnd(Object * pObject, Player* Plr);
		void TradeItem(Object * pObject, Player* Plr, uint32 sItem, uint32 sItemCount, uint32 dItem, uint32 dItemCount);
		void TradePoints(Object * pObject, Player* Plr, uint32 Index, uint32 count, uint32 dItem, uint32 dItemCount);
		GossipMenu *Menu;
		void Destroy()
		{
			delete this;
		}
};
void Token_Vendor::GossipHello(Object* pObject, Player * Plr, bool AutoSend)
{
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
    Menu->AddItem(5, "Hello, Would you like to trade some of your earthly possessions for Alterac Valley Mark of Honor?", 0);
	Menu->AddItem(0, " - I would like to exchange other tokens for AV.", 1);
	Menu->AddItem(0, " - I would like exchange Honor points for AV tokens.", 2);
	Menu->AddItem(0, " - I would like exchange Arena points for AV tokens.", 4);

	if (AutoSend)
       Menu->SendTo(Plr);
}

void Token_Vendor::GossipSelectOption(Object *pObject, Player *Plr, uint32 Id, uint32 IntId, const char *Code)
{
	uint32 Team = Plr->GetTeam();
    if(Team > 1) Team = 1;

   switch(IntId)
   {
	case 0:
	{
		GossipHello(pObject, Plr, true);
	}
	break;
	case 1:
	{
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		char msg[200];
		if ( Team == 0 )
		{
			snprintf( msg,200," %d Honor Hold Battle Tokens for one AV mark.", FACTION_TOKEN_COST );
			Menu->AddItem(0, msg, 6);
		}
		else
		{
			snprintf( msg,200," %d Thrallmar Battle Tokens for one AV mark.", FACTION_TOKEN_COST );
			Menu->AddItem(0, msg, 7);
		}

		snprintf( msg,200," %d Halaa Hold Battle Tokens for one AV mark.", FACTION_TOKEN_COST );
		Menu->AddItem(0, msg, 8);
		snprintf( msg,200," %d EotS marks for one AV mark.", TOKEN_COST );
		Menu->AddItem(0, msg, 9);
		snprintf( msg,200," %d WSG marks for one AV mark.", TOKEN_COST );
		Menu->AddItem(0, msg, 10);
		snprintf( msg,200," %d AB marks for one AV mark.", TOKEN_COST );
		Menu->AddItem(0, msg, 11);

        Menu->SendTo(Plr);
	}
	break;
	case 2:
	{
	   char msg[200];
	   snprintf( msg,200,"The price for single token is: %d honor points.", HONOR_COST );
	   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
	   Menu->AddItem(5, msg, 2);
	   Menu->AddItem(0, "Buy one.", 3);
	   //Menu->AddItem(0, "No, Thank you.", 0);
	   Menu->SendTo(Plr);
	}
    break;
	case 3:
	{
		TradePoints(pObject, Plr, PLAYER_FIELD_HONOR_CURRENCY, HONOR_COST, AV_MARK, 1);
	}
	break;
	case 4:
	{
	   char msg[200];
	   snprintf( msg,200,"The price for single token is: %d arena points.", ARENA_COST );
	   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
	   Menu->AddItem(5, msg, 4);
	   Menu->AddItem(0, "Buy one.", 5);
	   //Menu->AddItem(0, "No, Thank you.", 0);
	   Menu->SendTo(Plr);
	}
    break;
	case 5:
	{
		TradePoints(pObject, Plr, PLAYER_FIELD_ARENA_CURRENCY, ARENA_COST, AV_MARK, 1);
	}
	break;
	case 6: //HONOR_HOLD
	{
		TradeItem(pObject, Plr, HONOR_HOLD_TOKEN, FACTION_TOKEN_COST, AV_MARK, 1);
	}
	break;
	case 7: //THARLLMAR
	{
		TradeItem(pObject, Plr, THRALLMAR_TOKEN, FACTION_TOKEN_COST, AV_MARK, 1);
	}
	break;
	case 8: //HALAA
	{
		TradeItem(pObject, Plr, HALAA_TOKEN, FACTION_TOKEN_COST, AV_MARK, 1);
	}
	break;
	case 9: //EOTS
	{
		TradeItem(pObject, Plr, EOTS_MARK, TOKEN_COST, AV_MARK, 1);
	}
	break;
	case 10: //WSG
	{
		TradeItem(pObject, Plr, WARSONG_MARK, TOKEN_COST, AV_MARK, 1);
	}
	break;
	case 11: //AB
	{
		TradeItem(pObject, Plr, AB_MARK, TOKEN_COST, AV_MARK, 1);
	}
	break;
	
  }
}

void  Token_Vendor::TradeItem(Object * pObject, Player* Plr, uint32 sItem, uint32 sItemCount, uint32 dItem, uint32 dItemCount)
{
	if( Plr->GetItemInterface()->GetItemCount( sItem ) >= sItemCount )
	{
		if (Plr->GetItemInterface()->RemoveItemAmt(sItem, sItemCount) )
		{
			Item *item = objmgr.CreateItem( dItem, Plr );
			item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, dItemCount );
			item->SoulBind();

			if( Plr->GetItemInterface()->AddItemToFreeSlot(item) )
			{
				SlotResult *lr = Plr->GetItemInterface()->LastSearchResult();
				Plr->GetSession()->SendItemPushResult(item, false, true, false, true, lr->ContainerSlot, lr->Slot, dItemCount);
			}
			else
			{
				delete item;
			}

			Plr->Gossip_Complete();
		}
	}
	else
	{
		Plr->BroadcastMessage(" You cannot afford this item(s) yet.");
		GossipHello(pObject, Plr, true);
	}
}

void Token_Vendor::TradePoints(Object * pObject, Player* Plr, uint32 Index, uint32 count, uint32 dItem, uint32 dItemCount)
{
	if( Plr->GetUInt32Value(Index) >= count )
	{
		Item *item = objmgr.CreateItem( dItem, Plr );
		item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, dItemCount );
		item->SoulBind();

		if( Plr->GetItemInterface()->AddItemToFreeSlot(item) )
		{
			Plr->ModUInt32Value(Index, -int32(count));
			Plr->m_honorPoints -=int32(count);

			SlotResult *lr = Plr->GetItemInterface()->LastSearchResult();
			Plr->GetSession()->SendItemPushResult(item, false, true, false, true, lr->ContainerSlot, lr->Slot, dItemCount);
		}
		else
		{
			delete item;
		}

		Plr->Gossip_Complete();
	}
	else
	{
		Plr->BroadcastMessage(" You cannot afford this item(s) yet.");
		GossipHello(pObject, Plr, true);
	}
}

void  Token_Vendor::GossipEnd(Object * pObject, Player* Plr)
{
	GossipScript::GossipEnd(pObject, Plr);
}

void SetupTokenVendor(ScriptMgr * mgr)
{
	GossipScript * vendor = (GossipScript*) new Token_Vendor();
	mgr->register_gossip_script(66667, vendor);
}