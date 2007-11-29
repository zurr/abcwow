#include <string>
#ifndef INSTANCE_SCRIPTS_SETUP_H
#define INSTANCE_SCRIPTS_SETUP_H

//Instances
void SetupArcatraz(ScriptMgr * mgr);
void SetupAuchenaiCrypts(ScriptMgr * mgr);
void SetupBlackrockDepths(ScriptMgr * mgr);
void SetupBloodFurnace(ScriptMgr * mgr);
void SetupBotanica(ScriptMgr * mgr);
void SetupDeadmines(ScriptMgr * mgr);
void SetupDireMaul(ScriptMgr * mgr);
void SetupHellfireRamparts(ScriptMgr * mgr);
void SetupManaTombs(ScriptMgr * mgr);
void SetupMaraudon(ScriptMgr * mgr);
void SetupRazorfenDowns(ScriptMgr * mgr);
void SetupScarletMonastery(ScriptMgr * mgr);
void SetupScholomance(ScriptMgr * mgr);
void SetupSethekkHalls(ScriptMgr * mgr);
void SetupShadowfangKeep(ScriptMgr * mgr);
void SetupShadowLabyrinth(ScriptMgr * mgr);
void SetupTheMechanar(ScriptMgr * mgr);
void SetupTheShatteredHalls(ScriptMgr * mgr);
void SetupTheSlavePens(ScriptMgr * mgr);
void SetupTheSteamvault(ScriptMgr * mgr);
void SetupTheUnderbog(ScriptMgr * mgr);
void SetupUldaman(ScriptMgr * mgr);
//Raids
void SetupBlackTemple(ScriptMgr * mgr);
void SetupBlackwingLair(ScriptMgr * mgr);
void SetupBattleOfMountHyjal(ScriptMgr * mgr);
void SetupGruulsLair(ScriptMgr * mgr);
void SetupKarazhan(ScriptMgr * mgr);
void SetupMoltenCore(ScriptMgr * mgr);
void SetupNaxxramas(ScriptMgr * mgr);
void SetupOnyxiasLair(ScriptMgr * mgr);
void SetupTheEye(ScriptMgr * mgr);
void SetupZulGurub(ScriptMgr * mgr);
void SetupSerpentshrineCavern(ScriptMgr * mgr);
void SetupMagtheridonsLair(ScriptMgr * mgr);
//other
//void SetupGenericAI(ScriptMgr * mgr);

struct SP_AI_Spell{
	SpellEntry *info; // spell info
	char targettype; // 0-self , 1-attaking target, ....
	bool instant; // does it is instant or not?
	float perctrigger; // % of the cast of this spell in a total of 100% of the attacks
	int attackstoptimer; // stop the creature from attacking
	int soundid; // sound id in DBC
	std::string speech; // text displaied when spell was casted
	uint32 cooldown;	// spell cooldown
	uint32 casttime;	// "time" left to cast spell
	uint32 reqlvl; //required level
	float hpreqtocast;
};

enum
{
	TARGET_SELF,
	TARGET_VARIOUS,
	TARGET_ATTACKING,
	TARGET_DESTINATION,
	TARGET_SOURCE,
	TARGET_RANDOMDESTINATION,

	//.....add
};

#endif
