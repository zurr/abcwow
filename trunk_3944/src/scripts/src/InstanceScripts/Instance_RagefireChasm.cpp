
//Instance_Ragefire_Chasm.cpp created by HamtaroFreak

#include "StdAfx.h"
#include "Setup.h"

//Jergosh The Invoker - Curse Of Weakness & Immolate

#define JERGOSH_CURSE 8552
#define JERGOSH_IMMOLATE 707

class JergoshAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(JergoshAI);
JergoshAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
}

void AIUpdate()
{
float JerCast = (float)RandomFloat(100.0f);
CastSpell(JerCast);
}

void CastSpell(float JerCast)
{
//15% chance to Immolate
if (JerCast <= 15.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_IMMOLATE, false);
}
//10% chance to Curse Of Weakness
else if (JerCast >= 90.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_CURSE, false);
}
}

void Destroy()
{
delete (JergoshAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Oggleflint - Cleave

#define CLEAVE 15496

class OggleflintAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(OggleflintAI);
OggleflintAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float OggCast = (float)RandomFloat(100.0f);
CastSpell(OggCast);
}

void CastSpell(float OggCast)
{
//10% chance to Cleave
if (OggCast <= 10.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), CLEAVE, true);
}

}

void Destroy()
{
delete (OggleflintAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Taragaman the Hungerer - Fire Nova & Uppercut

#define TAR_NOVA 11969
#define TAR_UPPERCUT 10966

class TaragamanAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(TaragamanAI);
TaragamanAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float TarCast = (float)RandomFloat(100.0f);
CastSpell(TarCast);
}

void CastSpell(float TarCast)
{
//10% chance to Fire Nova
if (TarCast <= 10.0f) {
_unit->CastSpell(_unit, TAR_NOVA, false);
}
//10% chance to Uppercut
else if (TarCast >= 90.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), TAR_UPPERCUT, false);
}
}

void Destroy()
{
delete (TaragamanAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Bazzalan - Poison and Sinister Strike

#define POISON 24583
#define SSTRIKE 1758

class BazzalanAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(BazzalanAI);
BazzalanAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float BazCast = (float)RandomFloat(100.0f);
CastSpell(BazCast);
}

void CastSpell(float BazCast)
{
//5% chance to cast "Scorpid Poison 2"
if (BazCast <= 5.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), POISON, true);
}
//5% chance to cast "Sinister Strike (Rank 3)"
else if (BazCast >= 95.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), SSTRIKE, true);
}
}

void Destroy()
{
delete (BazzalanAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};

};

void SetupRagefireChasm(ScriptMgr * mgr)
{
mgr->register_creature_script(11519, &BazzalanAI::Create);
mgr->register_creature_script(11518, &JergoshAI::Create);
mgr->register_creature_script(11517, &OggleflintAI::Create);
mgr->register_creature_script(11520, &TaragamanAI::Create);
}