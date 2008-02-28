#include "StdAfx.h"
#include "Setup.h"
// Fel Orc Scavengers Quest
class KilsorrowAgentsQAI : public CreatureAIScript
{
public:
        ADD_CREATURE_FACTORY_FUNCTION(KilsorrowAgentsQAI);
    KilsorrowAgentsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}
void OnDied(Unit * mKiller, Player* plr)
    {
       QuestLogEntry *en = plr->GetQuestLogForEntry(21276);
        if(!en)
                en->SendUpdateAddKill(1);
    }
};
void SetupKilsorrowAgents(ScriptMgr * mgr)
{
mgr->register_creature_script(17147, &KilsorrowAgentsQAI::Create);
mgr->register_creature_script(17148, &KilsorrowAgentsQAI::Create);
mgr->register_creature_script(18397, &KilsorrowAgentsQAI::Create);
mgr->register_creature_script(18658, &KilsorrowAgentsQAI::Create);
mgr->register_creature_script(17146, &KilsorrowAgentsQAI::Create);
}

