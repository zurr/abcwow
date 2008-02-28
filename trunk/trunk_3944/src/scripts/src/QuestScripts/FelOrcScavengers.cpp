#include "StdAfx.h"
#include "Setup.h"
// Fel Orc Scavengers Quest
class FelOrcScavengersQAI : public CreatureAIScript
{
public:
        ADD_CREATURE_FACTORY_FUNCTION(FelOrcScavengersQAI);
    FelOrcScavengersQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}
void OnDied(Unit * mKiller, Player* plr)
    {
       QuestLogEntry *en = plr->GetQuestLogForEntry(10482);
        if(!en)
                en->SendUpdateAddKill(1);
    }
};
void SetupFelOrcScavengers(ScriptMgr * mgr)
{
mgr->register_creature_script(16772, &FelOrcScavengersQAI::Create);
mgr->register_creature_script(19701, &FelOrcScavengersQAI::Create);
mgr->register_creature_script(16876, &FelOrcScavengersQAI::Create);
mgr->register_creature_script(16925, &FelOrcScavengersQAI::Create);
mgr->register_creature_script(18952, &FelOrcScavengersQAI::Create);
}

