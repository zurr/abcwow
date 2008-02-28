#include "StdAfx.h"
#include "Setup.h"
// Fel Orc Scavengers Quest
class GordunniOgresQAI : public CreatureAIScript
{
public:
        ADD_CREATURE_FACTORY_FUNCTION(GordunniOgresQAI);
    GordunniOgresQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}
void OnDied(Unit * mKiller, Player* plr)
    {
       QuestLogEntry *en = plr->GetQuestLogForEntry(23450);
        if(!en)
                en->SendUpdateAddKill(1);
    }
};
void SetupGordunniOgres(ScriptMgr * mgr)
{
mgr->register_creature_script(22143, &GordunniOgresQAI::Create);
mgr->register_creature_script(22144, &GordunniOgresQAI::Create);
mgr->register_creature_script(22148, &GordunniOgresQAI::Create);
mgr->register_creature_script(23022, &GordunniOgresQAI::Create);
}

