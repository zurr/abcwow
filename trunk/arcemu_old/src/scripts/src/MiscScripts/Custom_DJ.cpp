#include "StdAfx.h"
#include "Setup.h"
#ifdef WIN32
#pragma warning(disable:4305)        // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

class SCRIPT_DECL DJ : public GossipScript
{
public:
void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
    {

            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Power of the Horde", 1);
            Menu->AddItem(5, "Brewfest Songs", 2);
            Menu->AddItem(0, "Menu Music", 3);
            Menu->AddItem(0, "Lament of the Highborne", 4);
            Menu->AddItem(0, "Darkmoon Faire", 5);
            Menu->AddItem(0, "Darnassus Day", 6);
            Menu->AddItem(0, "Exodar City", 7);
            Menu->AddItem(0, "IronForge", 8);
            Menu->AddItem(0, "Tavern Dwarf", 18);
            Menu->AddItem(5, "Page 2", 9);
            if(AutoSend)
            Menu->SendTo(Plr);
        
    }
    
void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
		return;

        GossipMenu * Menu;
        switch(IntId)
        {
            case 1:
                Plr->PlaySoundToSet(11803);
                break;
            case 2:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(0, "Drunken Gnomes", 31);
                Menu->AddItem(0, "Party Dwarfs", 32);
                Menu->AddItem(0, "Party Gnomes", 34);
                Menu->AddItem(0, "Drunken Goblin", 35);
                Menu->AddItem(0, "Party Goblin", 36);
                Menu->AddItem(0, "Drunk Fight", 37);
                Menu->AddItem(5, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
            
            case 33:
            Plr->PlaySoundToSet(11810);
            break;
            
            case 31:
            Plr->PlaySoundToSet(11811);
            break;
            
            case 32:
            Plr->PlaySoundToSet(11812);
            break;
            
            case 34:
            Plr->PlaySoundToSet(11813);
            break;
            
            case 35:
            Plr->PlaySoundToSet(11814);
            break;
            
            case 36:
            Plr->PlaySoundToSet(11815);
            break;
            
            case 37:
            Plr->PlaySoundToSet(11832);
            break;
            
            case 3:
            Plr->PlaySoundToSet(10925);
            break;
            
            case 4:
            Plr->PlaySoundToSet(10896);
            break;
            
            case 5:
            Plr->PlaySoundToSet(8440);
            break;
            
            case 6:
            Plr->PlaySoundToSet(4210);
            break;
            
            case 7:
            Plr->PlaySoundToSet(9133);
            break;
            
            case 8:
            Plr->PlaySoundToSet(4162);
            break;
            
            case 9:
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Silvermoon General Day", 10);
            Menu->AddItem(0, "Orgrimmar Day", 11);
            Menu->AddItem(0, "Ship Interior", 12);
            Menu->AddItem(0, "Ship Exterior", 13);
            Menu->AddItem(0, "Stormwind Day", 14);
            Menu->AddItem(0, "Thunder Bluff Day", 15);
            Menu->AddItem(0, "Undercity", 16);
            Menu->AddItem(0, "Tavern Pirate", 17);
            Menu->AddItem(5, "Page 1", 99);
            Menu->SendTo(Plr);
            break;
            
            case 10:
            Plr->PlaySoundToSet(9131);
            break;
            
            case 11:
            Plr->PlaySoundToSet(4185);
            break;
            
            case 12:
            Plr->PlaySoundToSet(4216);
            break;
            
            case 13:
            Plr->PlaySoundToSet(4215);
            break;
            
            case 14:
            Plr->PlaySoundToSet(4198);
            break;
            
            case 15:
            Plr->PlaySoundToSet(4202);
            break;
            
            case 16:
            Plr->PlaySoundToSet(4163);
            break;
            
            case 17:
            Plr->PlaySoundToSet(11805);
            break;
            
            case 18:
            Plr->PlaySoundToSet(11806);
            break;
            
            
            
            case 99:
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Power of the Horde", 1);
            Menu->AddItem(5, "Brewfest Songs", 2);
            Menu->AddItem(0, "Menu Music", 3);
            Menu->AddItem(0, "Lament of the Highborne", 4);
            Menu->AddItem(0, "Darkmoon Faire", 5);
            Menu->AddItem(0, "Darnassus Day", 6);
            Menu->AddItem(0, "Exodar City", 7);
            Menu->AddItem(0, "IronForge", 8);
            Menu->AddItem(0, "Tavern Dwarf", 18);
            Menu->AddItem(5, "Page 2", 9);
            Menu->SendTo(Plr);
            break;
            
            case 0:
                GossipHello(pObject, Plr, true);
            break;
        }
}
    
void GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

protected:
};

void SetupDJ(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new DJ();
    mgr->register_gossip_script(30011, gs);
}