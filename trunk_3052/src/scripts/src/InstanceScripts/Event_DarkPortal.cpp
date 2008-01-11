/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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
 */

#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Event_DarkPortal.cpp Script - Created by M4ksiu						*/
/************************************************************************/

//------------------------------- TO-DO --------------------------------//

/* 1. Add to ranged units ranged combat style (like for undercity or
 *    stormwind mages, darnassian archer or darkspear axe thrower.	DONE
 */

/* 2. Add combat AI for Pit Commander and include "special cases" for him
 *    like Infernal spawning and so on.								DONE
 */

/* 3. Add additional stuff to Trigger.								DONE
 */

/* 4. Add Waypoints for Infernals.									DELETED
 */

/* 5. Add special NPC spawning - no clear informations.
 */

/* 6. Additional fights (I saw some on youtube movies - like Silvermoon
 *    magisters fighting with infernals and so, but I don't have infos
 *	  about those.
 */

/* 7. Npc special emotes/yells/texts - no clear informations about
 *    situations
 */

/* 8. Rest of waypoints - I have NO informations about that for		DONE
 *    Alliance and Horde
 */

/* 9. Fix unexpected fights between ex. SW Soldiers and Org Gunts
 */

// ----------------------------- Defines ------------------------------ //

// Movement types
#define WALK 0
#define RUN 256
#define FLY 768

// Event settings

#define MIN_TIME_TO_SUMMON		3	// it says the smallest diff between one and another portal spawning
#define SPECIAL_UNIT_CHECK_TIME 5
#define SUMMON_INFERNAL_TIME	40
#define DEMON_RESPAWN_TIME		180
#define TEN_DEMON_WAVE_DELAY	50
#define SPAWN_NEW_UNIT_PORTAL	5

// Evil side
#define PIT_COMMANDER				18945
#define FEL_SOLDIER					18944	// +
#define WRATH_MASTER				19005	// + no spells
#define INFERNAL_SIEGEBREAKER		18946	// + no spells

// Horde Side
#define LIEUTENANT_GENERAL_ORION	19253	// + no spells
#define MELGROMM_HIGHMOUNTAIN		18969	// +
#define ORGRIMMAR_GRUNT				18950	// +
#define ORGRIMMAR_SHAMAN			18972	// +
#define DARKSPEAR_AXE_THROWER		18970	// +
#define UNDERCITY_MAGE				18971	// +
#define SILVERMOON_MAGISTER			19006	// + no spells
#define	THUNDER_BLUFF_HUNTER		00000	// --- Thunder Bluff Hunters spawned by Melgromm Highmountain - can't find those

// Alliance Side
#define COMMANDER_DURON				19229	// + no spells
#define JUSTINIUS_THE_HARBINGER		18966	// +
#define LORD_MARSHAL_RAYNOR			19385	// + no spells
#define STORMWIND_SOLDIER			18948	// +
#define DARNASSIAN_ARCHER			18965	// +
#define IRONFORGE_PALADIN			18986	// +
#define STORMWIND_MAGE				18949	// +
#define AZUREMYST_VINDICATOR		19407	// + no spells ---- Spawned by Justinius the Harbinger
#define STORMWIND_MARSHAL			19386	// + no spells ---- Spawned by Lord Marshal Raynor

struct SCoords
{
	float x;
	float y;
	float z;
	float o;
};

//==============================================================================================//
//======================================= Demon Stuff ==========================================//
//==============================================================================================//

struct SCoords Infernals[]=
{
	{ -261.202820f, 1097.126099f, 41.666908f, 4.543878f },
	{ -255.785507f, 1097.852295f, 41.666908f, 4.680013f },
	{ -247.238403f, 1098.785767f, 41.666908f, 4.697028f },
	{ -241.768570f, 1097.750366f, 41.666908f, 4.580745f }
};

struct SCoords DSPoints[]=
{
	{ -259.384735f, 1207.090210f, 37.783020f, 3.815325f },	// <- 1
	{ -255.924347f, 1214.213379f, 35.689049f, 3.397272f },
	{ -252.891357f, 1209.680542f, 36.874710f, 4.853663f },	// <-- 2
	{ -244.423294f, 1207.830566f, 38.780174f, 4.997303f },
	{ -249.017365f, 1212.447998f, 36.301140f, 1.153214f },	// <--- 3
	{ -243.170227f, 1212.201172f, 36.496788f, 5.213856f },
	{ -237.440628f, 1207.207397f, 37.915062f, 4.767401f },	// <---- 4
	{ -235.196548f, 1211.153564f, 36.487946f, 2.763414f },
	{ -232.128296f, 1202.827637f, 40.078438f, 0.422579f },	// <----- 5
	{ -231.589218f, 1209.054321f, 37.514587f, 4.626029f }
};

struct SCoords PitDemonPaths[]=
{
	{},
	{ -259.824768f, 1193.424561f, 41.702896f, 4.637918f },	// <- 1S
	{ -264.705597f, 1176.797485f, 41.786797f, 4.065013f },
	{ -297.176361f, 1155.448364f, 41.229752f, 4.708117f },
	{ -266.808868f, 1091.026733f, 43.115368f, 4.761819f },
	{ -268.872498f, 1073.627075f, 54.306843f, 4.695586f },
	{ -261.097046f, 1014.932922f, 54.326466f, 4.722487f },
	{ -261.324615f,  969.228638f, 84.328705f, 4.714633f },
	{ -261.324615f,  896.572571f, 84.380699f, 4.738189f },	// <- 1F
	{ -257.960693f, 1188.684570f, 41.988678f, 4.680151f },
	{ -280.667053f, 1156.924194f, 41.421211f, 4.066449f },
	{ -292.243652f, 1159.064819f, 41.168304f, 4.718767f },
	{ -263.203674f, 1090.460327f, 43.528366f, 4.659718f },
	{ -263.701752f, 1074.675659f, 54.010033f, 4.717880f },
	{ -258.547886f, 1014.932922f, 54.326466f, 4.722487f },
	{ -258.380275f,  969.228638f, 84.328705f, 4.714633f },
	{ -258.380275f,  896.572571f, 84.380699f, 4.738189f },
	{ -258.138306f, 1190.368774f, 41.925529f, 4.405354f },	// <-- 2S
	{ -261.418549f, 1173.848755f, 41.693317f, 4.586868f },
	{ -271.486603f, 1155.338623f, 41.648853f, 4.708117f },
	{ -258.496887f, 1092.138062f, 42.462128f, 4.778837f },
	{ -260.949005f, 1074.773193f, 53.971958f, 4.700864f },
	{ -255.998726f, 1014.932922f, 54.326466f, 4.722487f },
	{ -255.663504f,  969.228638f, 84.328705f, 4.714633f },
	{ -255.663504f,  896.572571f, 84.380699f, 4.738189f },	// <-- 2F
	{ -254.217911f, 1191.097290f, 42.226845f, 4.165057f },
	{ -263.735260f, 1174.469849f, 41.721027f, 4.570626f },
	{ -267.121826f, 1159.238770f, 41.640697f, 4.809984f },
	{ -255.944672f, 1090.757080f, 43.405441f, 4.776000f },
	{ -258.346588f, 1073.799194f, 54.306805f, 4.684153f },
	{ -253.449566f, 1014.932922f, 54.326466f, 4.722487f },
	{ -252.946733f,  969.228638f, 84.328705f, 4.714633f },
	{ -252.946733f,  896.572571f, 84.380699f, 4.738189f },
	{ -248.362381f, 1187.077271f, 42.552185f, 4.510992f },	// <--- 3S
	{ -248.353027f, 1172.465576f, 41.577877f, 4.624440f },
	{ -249.298492f, 1153.972900f, 41.650188f, 4.700939f },
	{ -253.146927f, 1089.656372f, 44.164940f, 4.773167f },
	{ -255.567947f, 1073.581177f, 54.307045f, 4.714306f },
	{ -250.900406f, 1014.932922f, 54.326466f, 4.722487f },
	{ -250.229962f,  969.228638f, 84.328705f, 4.714633f },
	{ -250.229962f,  896.572571f, 84.380699f, 4.738189f },	// <--- 3F
	{ -244.554230f, 1194.357300f, 42.886631f, 4.500755f },
	{ -248.103027f, 1180.660645f, 41.850349f, 4.704960f },
	{ -248.586914f, 1158.138184f, 41.619884f, 4.685106f },
	{ -250.886108f, 1089.793945f, 44.096958f, 4.773167f },
	{ -250.981888f, 1074.039429f, 54.307217f, 4.717141f },
	{ -248.351246f, 1014.932922f, 54.326466f, 4.722487f },
	{ -247.513191f,  969.228638f, 84.328705f, 4.714633f },
	{ -247.513191f,  896.572571f, 84.380699f, 4.738189f },
	{ -235.939011f, 1187.490479f, 42.089699f, 4.754903f },	// <---- 4S
	{ -231.975601f, 1167.140015f, 41.504807f, 5.202579f },
	{ -221.789551f, 1155.126343f, 41.600170f, 4.708117f },
	{ -244.984070f, 1091.562134f, 42.983116f, 4.708852f },
	{ -247.069885f, 1073.525757f, 54.307018f, 4.748340f },
	{ -245.802086f, 1014.932922f, 54.326466f, 4.722487f },
	{ -244.796420f,  969.228638f, 84.328705f, 4.714633f },
	{ -244.796420f,  896.572571f, 84.380699f, 4.738189f },	// <---- 4F
	{ -233.501907f, 1192.402588f, 42.064373f, 4.805360f },
	{ -219.749039f, 1167.461060f, 41.554111f, 4.666389f },
	{ -220.225464f, 1158.493164f, 41.573982f, 4.621010f },
	{ -242.115265f, 1089.942505f, 44.088120f, 4.708852f },
	{ -242.313522f, 1074.069946f, 54.306789f, 4.722817f },
	{ -243.252926f, 1014.932922f, 54.326466f, 4.722487f },
	{ -242.079649f,  969.228638f, 84.328705f, 4.714633f },
	{ -242.079649f,  896.572571f, 84.380699f, 4.738189f },
	{ -229.465118f, 1185.047363f, 42.061714f, 4.904214f },	// <----- 5S
	{ -221.545197f, 1175.243774f, 41.702663f, 5.599072f },
	{ -199.750961f, 1155.032104f, 41.520683f, 4.708117f },
	{ -238.349487f, 1092.800171f, 42.229187f, 4.708852f },
	{ -240.603683f, 1073.694580f, 54.308067f, 4.766930f },
	{ -240.073766f, 1014.932922f, 54.326466f, 4.722487f },
	{ -239.362878f,  969.228638f, 84.328705f, 4.714633f },
	{ -239.362878f,  896.572571f, 84.380699f, 4.738189f },	// <----- 5F
	{ -228.239334f, 1184.733032f, 42.055592f, 4.672062f },
	{ -204.227600f, 1169.488403f, 42.624237f, 4.638264f },
	{ -204.140060f, 1161.501465f, 42.566750f, 4.723349f },
	{ -234.986465f, 1092.102661f, 42.726883f, 4.708852f },
	{ -235.440399f, 1074.475586f, 54.306778f, 4.681848f },
	{ -238.154602f, 1014.932922f, 54.326466f, 4.722487f },
	{ -236.873672f,  969.283508f, 84.328705f, 4.714633f },
	{ -236.873672f,  896.572571f, 84.380699f, 4.738189f }
};

//==============================================================================================//
//====================================== Common Stuff ==========================================//
//==============================================================================================//

struct SCoords AHChecks[]=
{
	{ -265.506012f, 1028.310059f, 54.324100f, 5.455820f },	// ---- Commander Duron
	{ -232.919998f, 1028.270020f, 54.407799f, 3.892080f }	// ---- Lieutenant General Orion
};

struct SCoords SWP[]=	// Special Spawns WPs
{
	{ -269.433014f, 1072.020020f, 54.490700f, 1.505257f },	// ---- Justinius the Harbinger
	{ -230.486786f, 1072.293945f, 54.353722f, 1.658060f }	// ---- Melgromm Highmountain
};

//==============================================================================================//
//===================================== Alliance Stuff =========================================//
//==============================================================================================//

struct SCoords AMSPoints[]=	// Alliance melee units spawn pos
{
	{ -337.971008f, 963.656582f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656682f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656782f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656882f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656982f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657082f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657182f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657282f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657382f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657482f, 54.461300f, 1.675520f }
};

struct SCoords ARSPoints[]=	// Alliance ranged units spawn pos
{
	{ -337.971008f, 963.656782f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656882f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656982f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657082f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657182f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.657282f, 54.461300f, 1.675520f }
};

struct SCoords ACSPoints[]=	// Alliance casters spawn pos
{
	{ -337.971008f, 963.656882f, 54.461300f, 1.675520f },
	{ -337.971008f, 963.656982f, 54.461300f, 1.675520f }
};

struct SCoords AWP[]=	// Main Alliance way
{
	{},
	{ -329.585999f, 989.763977f, 54.327599f, 1.181110f },
	{ -318.148010f, 1017.619995f, 54.331100f, 1.181110f },
	{ -312.653015f, 1020.869995f, 54.353600f, 0.146198f},
	{ -309.266998f, 1021.359985f, 54.362099f, 0.146198f },
	{ -294.350006f, 1023.559998f, 54.399700f, 0.146198f },
	{ -280.778992f, 1025.560059f, 54.424801f, 0.146203f },
	{ -261.795013f, 1054.939941f, 54.310101f, 1.121552f },
	{ -261.808014f, 1055.189941f, 54.410099f, 1.621150f },
	{ -262.640015f, 1071.699951f, 54.308201f, 1.591097f }

	// ... to add
};

struct SCoords AMWP[]=
{
	{ -272.106995f, 1098.439941f, 42.094299f, 1.343900f },
	{ -268.051819f, 1098.389771f, 41.736443f, 1.637637f },	// custom
	{ -263.979004f, 1098.479980f, 41.854698f, 1.535890f },
	{ -258.976013f, 1098.489990f, 41.849998f, 1.640610f },
	{ -254.639008f, 1098.430054f, 41.849998f, 1.762780f },
	{ -271.437012f, 1093.290039f, 42.185299f, 1.378810f },
	{ -266.118988f, 1093.030029f, 41.976101f, 1.500980f },
	{ -261.527008f, 1093.329956f, 41.849998f, 1.588250f },
	{ -257.638000f, 1093.209961f, 41.941299f, 1.675520f },
	{ -253.451996f, 1092.979980f, 42.139801f, 1.762780f }
};

struct SCoords ARWP[]=
{
	{ -270.035004f, 1087.030029f, 45.921700f, 1.431170f },
	{ -265.329987f, 1087.189941f, 45.865299f, 1.518440f },
	{ -260.299011f, 1087.140015f, 45.944801f, 1.588250f },
	{ -270.035004f, 1084.189941f, 47.854801f, 1.518440f },	// custom
	{ -265.510010f, 1084.189941f, 47.854801f, 1.518440f },
	{ -260.299011f, 1084.189941f, 47.854801f, 1.518440f }	// custom
};

struct SCoords ACWP[]=
{
	{ -273.237000f, 1075.800049f, 54.494099f, 1.710420f },
	{ -276.255005f, 1084.689941f, 54.515301f, 1.466080f }
};

//==============================================================================================//
//======================================= Horde Stuff ==========================================//
//==============================================================================================//

struct SCoords HMSPoints[]=	// Horde melee units spawn pos
{
	{ -161.376007f, 966.457608f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.457708f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.457808f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.457908f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458008f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458108f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458208f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458308f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458408f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458508f, 54.463501f, 1.745330f }
};

struct SCoords HRSPoints[]=	// Horde ranged units spawn pos
{
	{ -161.376007f, 966.457808f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.457908f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458008f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458108f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458208f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458308f, 54.463501f, 1.745330f }
};

struct SCoords HCSPoints[]=	// Horde casters spawn pos
{
	{ -161.376007f, 966.457908f, 54.463501f, 1.745330f },
	{ -161.376007f, 966.458008f, 54.463501f, 1.745330f }
};

struct SCoords HWP[]=	// Main Horde way
{
	{},
	{ -173.104004f, 1002.599976f, 54.326099f, 1.867840f },
	{ -178.179993f, 1016.710022f, 54.323601f, 2.424430f },
	{ -186.614594f, 1024.242676f, 54.237057f, 3.075874f },
	{ -213.940994f, 1025.630005f, 54.322102f, 3.086890f },
	{ -219.587997f, 1029.859985f, 54.359200f, 2.372290f },	// not sure if this one should exist
	{ -224.069000f, 1034.199951f, 54.418598f, 2.372290f },
	{ -228.707001f, 1041.520020f, 54.433102f, 2.086830f },	// about this one too
	{ -233.727005f, 1050.369995f, 54.433300f, 2.086830f },
	{ -234.591003f, 1055.890015f, 54.433399f, 1.602190f },
	{ -235.057999f, 1070.739990f, 54.312000f, 2.351221f }
	
	// ... to add
};

struct SCoords HMWP[]=
{
	{ -229.492004f, 1098.260010f, 41.849998f, 1.413720f },
	{ -233.701004f, 1098.250000f, 41.849998f, 1.413720f },
	{ -237.951996f, 1098.130005f, 41.849998f, 1.570800f },
	{ -242.307999f, 1098.020020f, 41.849998f, 1.483530f },
	{ -246.106003f, 1097.790039f, 41.849998f, 1.413720f },
	{ -231.356003f, 1094.290039f, 41.849998f, 1.588250f },
	{ -236.117996f, 1094.290039f, 41.849998f, 1.518440f },
	{ -240.848007f, 1094.250000f, 41.849998f, 1.448620f },
	{ -245.259995f, 1094.140015f, 41.849998f, 1.378810f },
	{ -249.309998f, 1094.050049f, 41.849998f, 1.326450f }
};

struct SCoords HRWP[]=
{
	{ -232.173996f, 1087.959961f, 45.621899f, 1.570800f },	// custom
	{ -235.501999f, 1087.959961f, 45.621899f, 1.361360f },	// custom
	{ -238.819000f, 1087.959961f, 45.621899f, 1.361360f },
	{ -232.123993f, 1085.030029f, 47.635399f, 1.483530f },
	{ -235.501999f, 1084.930054f, 47.670101f, 1.431170f },
	{ -239.162003f, 1084.719971f, 47.769100f, 1.361360f }
};

struct SCoords HCWP[]=
{
	{ -227.608994f, 1074.969971f, 54.492901f, 1.570800f },
	{ -225.987000f, 1083.229980f, 54.437401f, 2.392000f }	// custom
};

//==============================================================================================//
//======================================= Other Stuff ==========================================//
//==============================================================================================//

struct SCoords AHSPoints[]=
{
	{ -337.971008f, 963.656982f, 54.461300f, 1.675520f },	// ---- Alliance Spawn Point
	{ -161.376007f, 966.458008f, 54.463501f, 1.745330f },	// ---- Horde Spawn Point
};

//==============================================================================================//
//======================================= AI Classes ===========================================//
//==============================================================================================//

// Dark Portal Event Trigger

#define CN_DARK_PORTAL_EVENT_TRIGGER 18625

#define SUMMON_INFERNALS	33393
#define SUMMON_INFERNALS2	33240

class DarkPortalEventTrigger : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DarkPortalEventTrigger);
	std::vector<Unit*> DemonTable, AllianceMelee, AllianceRanged, AllianceCaster, HordeMelee, HordeRanged, HordeCaster;
	std::vector<Creature*> InfernalTable;
	SP_AI_Spell spells[2];

    DarkPortalEventTrigger(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		spells[0].info = dbcSpell.LookupEntry(SUMMON_INFERNALS);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;

		spells[1].info = dbcSpell.LookupEntry(SUMMON_INFERNALS2);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = true;

		_unit->GetAIInterface()->MoveTo(_unit->GetSpawnX()+5.0f, _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO());
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_moveFly = true;

		PitCommanderRespawn = DEMON_RESPAWN_TIME;
		InfernalSpawn = SUMMON_INFERNAL_TIME;
		SpecialUnitCheckTimer = 0;
		pJusti = pMelgr = NULL;
		APortalRespawn = 0;
		HPortalRespawn = 0;
		SecondWave = false;
		_Infernals = false;
		PitComSpw = false;
		LastInfernal = 0;
		APortalMove = 0;
		HPortalMove = 0;
		Ressed = false;
		JustiMove = 0;
		MelgrMove = 0;
		Ress = false;

		RegisterAIUpdateEvent(1000);
    }

    void AIUpdate()
	{
		// it's very possible that trigger will fall down when cell is loaded for the first time after server start, so we will add hacky fix for that.
		if (_unit->GetPositionX() != _unit->GetSpawnX() || _unit->GetPositionY() != _unit->GetSpawnY() || _unit->GetPositionZ() != _unit->GetSpawnZ())
			_unit->GetAIInterface()->MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO());

		if (Ress && !Ressed)
		{
			if (pMelgr && !pMelgr->isAlive())
			{
				pMelgr->SetUInt32Value(UNIT_FIELD_HEALTH,_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));	// no idea if it should work like that
				pMelgr->setDeathState(ALIVE);

				pMelgr->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			}

			Ressed = true;
		}

		if (pJusti || pMelgr)
		{
			if (pJusti && (!pJusti->isAlive() || !pJusti->IsInWorld()))
				pJusti = NULL;

			if (pMelgr && !pMelgr->isAlive() && !Ress && !Ressed)
			{
				pMelgr->CastSpell(pMelgr, dbcSpell.LookupEntry(32343), true);

				Ress = true;
			}

			if (pMelgr && (!pMelgr->isAlive() || !pMelgr->IsInWorld()) && Ress && Ressed)
			{
				Ressed = false;
				Ress = false;

				pMelgr = NULL;
			}
		}

		SpecialUnitCheckTimer++;
		if (SpecialUnitCheckTimer >= SPECIAL_UNIT_CHECK_TIME)
			AHSpecialUnitChecks();

		JustiMove++;
		MelgrMove++;
		if (JustiMove == 5 || MelgrMove == 5)
		{
			if (JustiMove == 5 && pJusti && pJusti->isAlive())
			{
				pJusti->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				pJusti->GetAIInterface()->setWaypointToMove(1);
			}

			if (MelgrMove == 5 && pMelgr && pMelgr->isAlive())
			{
				pMelgr->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				pMelgr->GetAIInterface()->setWaypointToMove(1);
			}
		}

		if (APortalMove)
		{
			APortalMove++;
			if (APortalMove == 5 && pAlliance && pAlliance->isAlive())
			{
				pAlliance->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				pAlliance->GetAIInterface()->setWaypointToMove(1);

				APortalMove = 0;
			}
		}

		if (HPortalMove)
		{
			HPortalMove++;
			if (HPortalMove == 5 && pHorde && pHorde->isAlive())
			{
				pHorde->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				pHorde->GetAIInterface()->setWaypointToMove(1);

				HPortalMove = 0;
			}
		}

		APortalRespawn++;
		if (APortalRespawn >= SPAWN_NEW_UNIT_PORTAL)
		{
			// Spawn Alliance Unit
			AlliancePortal();

			APortalRespawn = 0;
		}

		HPortalRespawn++;
		if (HPortalRespawn >= SPAWN_NEW_UNIT_PORTAL)
		{
			// Spawn Horde Unit
			HordePortal();

			HPortalRespawn = 0;
		}

		InfernalSpawn--;
		if (!InfernalSpawn)
		{
			PitCommanderInfernalSummon();

			InfernalSpawn = SUMMON_INFERNAL_TIME;
		}

		if (InfernalSpawn == SUMMON_INFERNAL_TIME-3 && _Infernals)
		{
			Creature *pInfernal;
			pInfernal = _unit->GetMapMgr()->GetInterface()->SpawnCreature(INFERNAL_SIEGEBREAKER, Infernals[LastInfernal].x, Infernals[LastInfernal].y, Infernals[LastInfernal].z, Infernals[LastInfernal].o, true, false, 0, 0);
			InfernalTable.push_back(pInfernal);

			pInfernal = _unit->GetMapMgr()->GetInterface()->SpawnCreature(INFERNAL_SIEGEBREAKER, Infernals[LastInfernal+2].x, Infernals[LastInfernal+2].y, Infernals[LastInfernal+2].z, Infernals[LastInfernal+2].o, true, false, 0, 0);
			InfernalTable.push_back(pInfernal);

			_Infernals = false;
		}

		PitCommanderRespawn--;
		if (PitCommanderRespawn == DEMON_RESPAWN_TIME-20 && PitComSpw)
		{
			PitCommanderAttackCmd();

			PitComSpw = false;
		}

		if (PitCommanderRespawn == DEMON_RESPAWN_TIME-TEN_DEMON_WAVE_DELAY-20 && SecondWave)
		{
			SecondWaveAttackCmd();
			SecondWave = false;
		}

		if (!PitCommanderRespawn)
		{
			PitCommanderSpawn();

			PitComSpw = true;
		}
    }

	void AHSpecialUnitChecks()
	{
		Unit *pCommander = NULL;
		pCommander = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(AHChecks[0].x, AHChecks[0].y, AHChecks[0].z, COMMANDER_DURON);

		if ((!pCommander || pCommander && (pCommander->CombatStatus.IsInCombat() || !pCommander->isAlive())) && !pJusti)
		{
			pJusti = _unit->GetMapMgr()->GetInterface()->SpawnCreature(JUSTINIUS_THE_HARBINGER, AHSPoints[0].x, AHSPoints[0].y, AHSPoints[0].z, AHSPoints[0].o, true, false, 0, 0);
			
			JustiMove = 0;
		}

		Unit *pLieutenant = NULL;
		pLieutenant = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(AHChecks[1].x, AHChecks[1].y, AHChecks[1].z, LIEUTENANT_GENERAL_ORION);	
		
		if ((!pLieutenant || pLieutenant && (pLieutenant->CombatStatus.IsInCombat() || !pLieutenant->isAlive())) && !pMelgr)
		{
			pMelgr = _unit->GetMapMgr()->GetInterface()->SpawnCreature(MELGROMM_HIGHMOUNTAIN, AHSPoints[1].x, AHSPoints[1].y, AHSPoints[1].z, AHSPoints[1].o, true, false, 0, 0);
		
			MelgrMove = 0;
		}

		SpecialUnitCheckTimer = 0;
	}

	bool AMSize()
	{
		if (AllianceMelee.size() < 10)
			return true;

		else
		{
			for (uint32 i = 0; i < 10; i++)
			{
				Unit* SetUnit = NULL;
				if (AllianceMelee.size() >= i+1)
					SetUnit = AllianceMelee[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	bool ARSize()
	{
		if (AllianceRanged.size() < 6)
			return true;

		else
		{
			for (uint32 i = 0; i < 6; i++)
			{
				Unit* SetUnit = NULL;
				if (AllianceRanged.size() >= i+1)
					SetUnit = AllianceRanged[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	bool ACSize()
	{
		if (AllianceCaster.size() < 2)
			return true;

		else
		{
			for (uint32 i = 0; i < 2; i++)
			{
				Unit* SetUnit = NULL;
				if (AllianceCaster.size() >= i+1)
					SetUnit = AllianceCaster[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	void AlliancePortal()
	{
		uint32 Pos = 0;
		uint32 Entry = 0;
		uint32 Change = RandomUInt(6);
		std::vector<Unit*> BackupList;

		if (AMSize())						// default spawn
		{
			Entry = STORMWIND_SOLDIER;
			Pos = 10;
		}
		else if (ARSize())					// default spawn if melee regiment is full
		{
			Entry = DARNASSIAN_ARCHER;
			Pos = 6;
		}
		else if (ACSize())					// default spawn if ranged regiment is full
		{
			Entry = STORMWIND_MAGE;
			Pos = 2;
		}
		if (AMSize() && Change == 1)
		{
			Entry = IRONFORGE_PALADIN;
		}
		if (ARSize() && Change == 2)
		{
			Entry = DARNASSIAN_ARCHER;
			Pos = 6;
		}
		if (ACSize() && Change == 3)
		{
			Entry = STORMWIND_MAGE;
			Pos = 2;
		}

		bool Stop = false;
		for (uint32 i = 0; i < Pos; i++)
		{
			Unit* SetUnit = NULL;
			if (Pos == 10 && AllianceMelee.size() >= i+1)
				SetUnit = AllianceMelee[i];
			if (Pos == 6 && AllianceRanged.size() >= i+1)
				SetUnit = AllianceRanged[i];
			if (Pos == 2 && AllianceCaster.size() >= i+1)
				SetUnit = AllianceCaster[i];

			if ((!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld()))) && !Stop)
			{
				if (Pos == 10)
					SetUnit = pAlliance = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, AMSPoints[i].x, AMSPoints[i].y, AMSPoints[i].z, AMSPoints[i].o, true, false, 0, 0);
				if (Pos == 6)
					SetUnit = pAlliance = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, ARSPoints[i].x, ARSPoints[i].y, ARSPoints[i].z, ARSPoints[i].o, true, false, 0, 0);
				if (Pos == 2)
					SetUnit = pAlliance = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, ACSPoints[i].x, ACSPoints[i].y, ACSPoints[i].z, ACSPoints[i].o, true, false, 0, 0);
			
				APortalMove = 1;
				Stop = true;
			}

			BackupList.push_back(SetUnit);
		}

		if (Pos == 10)
		{
			AllianceMelee.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				AllianceMelee.push_back(BackupList[i]);
			}
		}

		if (Pos == 6)
		{
			AllianceRanged.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				AllianceRanged.push_back(BackupList[i]);
			}
		}

		if (Pos == 2)
		{
			AllianceCaster.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				AllianceCaster.push_back(BackupList[i]);
			}
		}

		BackupList.clear();
	}

	bool HMSize()
	{
		if (HordeMelee.size() < 10)
			return true;

		else
		{
			for (uint32 i = 0; i < 10; i++)
			{
				Unit* SetUnit = NULL;
				if (HordeMelee.size() >= i+1)
					SetUnit = HordeMelee[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	bool HRSize()
	{
		if (HordeRanged.size() < 6)
			return true;

		else
		{
			for (uint32 i = 0; i < 6; i++)
			{
				Unit* SetUnit = NULL;
				if (HordeRanged.size() >= i+1)
					SetUnit = HordeRanged[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	bool HCSize()
	{
		if (HordeCaster.size() < 2)
			return true;

		else
		{
			for (uint32 i = 0; i < 2; i++)
			{
				Unit* SetUnit = NULL;
				if (HordeCaster.size() >= i+1)
					SetUnit = HordeCaster[i];

				if (!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld())))
				{
					return true;
					break;
				}
			}

			return false;
		}
	}

	void HordePortal()
	{
		uint32 Pos = 0;
		uint32 Entry = 0;
		uint32 Change = RandomUInt(6);
		std::vector<Unit*> BackupList;

		if (HMSize())						// default spawn
		{
			Entry = ORGRIMMAR_GRUNT;
			Pos = 10;
		}
		else if (HRSize())					// default spawn if melee regiment is full
		{
			Entry = DARKSPEAR_AXE_THROWER;
			Pos = 6;
		}
		else if (HCSize())					// default spawn if ranged regiment is full
		{
			Entry = UNDERCITY_MAGE;
			Pos = 2;
		}
		if (HMSize() && Change == 1)
		{
			Entry = ORGRIMMAR_SHAMAN;
		}
		if (HRSize() && Change == 2)
		{
			Entry = DARKSPEAR_AXE_THROWER;
			Pos = 6;
		}
		if (HCSize() && Change == 3)
		{
			Entry = UNDERCITY_MAGE;
			Pos = 2;
		}

		bool Stop = false;
		for (uint32 i = 0; i < Pos; i++)
		{
			Unit* SetUnit = NULL;
			if (Pos == 10 && HordeMelee.size() >= i+1)
				SetUnit = HordeMelee[i];
			if (Pos == 6 && HordeRanged.size() >= i+1)
				SetUnit = HordeRanged[i];
			if (Pos == 2 && HordeCaster.size() >= i+1)
				SetUnit = HordeCaster[i];

			if ((!SetUnit || (SetUnit && (!SetUnit->isAlive() || !SetUnit->IsInWorld()))) && !Stop)
			{
				if (Pos == 10)
					SetUnit = pHorde = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, HMSPoints[i].x, HMSPoints[i].y, HMSPoints[i].z, HMSPoints[i].o, true, false, 0, 0);
				if (Pos == 6)
					SetUnit = pHorde = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, HRSPoints[i].x, HRSPoints[i].y, HRSPoints[i].z, HRSPoints[i].o, true, false, 0, 0);
				if (Pos == 2)
					SetUnit = pHorde = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, HCSPoints[i].x, HCSPoints[i].y, HCSPoints[i].z, HCSPoints[i].o, true, false, 0, 0);
			
				HPortalMove = 1;
				Stop = true;
			}

			BackupList.push_back(SetUnit);
		}

		if (Pos == 10)
		{
			HordeMelee.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				HordeMelee.push_back(BackupList[i]);
			}
		}

		if (Pos == 6)
		{
			HordeRanged.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				HordeRanged.push_back(BackupList[i]);
			}
		}

		if (Pos == 2)
		{
			HordeCaster.clear();

			for (uint32 i = 0; i < BackupList.size(); i++)
			{
				HordeCaster.push_back(BackupList[i]);
			}
		}

		BackupList.clear();
	}

	void PitCommanderInfernalSummon()
	{
		Unit* pPitCommander = NULL;
		pPitCommander = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-249.119995f, 1169.209961f, 41.552101f, PIT_COMMANDER);

		if (InfernalTable.size())
		{
			Creature* pInfernal;
			std::vector<Creature*> InfernalBackup;
			for (size_t i = 0; i < InfernalTable.size(); i++)
			{
				pInfernal = InfernalTable[i];
				if (pInfernal && !pInfernal->isAlive())
				{
					pInfernal->Despawn(0,0);
					continue;
				}

				InfernalBackup.push_back(pInfernal);
			}

			InfernalTable.clear();
			for (size_t i = 0; i < InfernalBackup.size(); i++)
			{
				InfernalTable.push_back(InfernalBackup[i]);
			}

			InfernalBackup.clear();
		}

		if (pPitCommander && pPitCommander->isAlive() && InfernalTable.size() <= 2)
		{
			pPitCommander->CastSpell(pPitCommander, spells[0].info, spells[0].instant);

			LastInfernal = rand()%2;

			_unit->CastSpellAoF(Infernals[LastInfernal].x, Infernals[LastInfernal].y, Infernals[LastInfernal].z, spells[1].info, spells[1].instant);
			_unit->CastSpellAoF(Infernals[LastInfernal+2].x, Infernals[LastInfernal+2].y, Infernals[LastInfernal+2].z, spells[1].info, spells[1].instant);

			_Infernals = true;
			return;							// will prevent adding infernals and letting demon army to move in the same time
		}

		InfernalSpawn = SUMMON_INFERNAL_TIME;
	}

	void PitCommanderAttackCmd()
	{
		Unit* pPitCommander = NULL;
		pPitCommander = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-249.119995f, 1169.209961f, 41.552101f, PIT_COMMANDER);
		
		if (pPitCommander && !pPitCommander->CombatStatus.IsInCombat() && RandomUInt(3) == 1)
				pPitCommander->Emote(EMOTE_ONESHOT_POINT);

		if (DemonTable.size() == 5)
		{
			for (uint32 i = 0; i < DemonTable.size(); i++)
			{
				Unit* pDemon = NULL;
				pDemon = DemonTable[i];

				if (pDemon)
				{
					pDemon->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					pDemon->GetAIInterface()->setWaypointToMove(3);
				}
			}
		}

		else
		{
			for (uint32 i = 0; i < DemonTable.size(); i+=2)
			{
				Unit* pDemon = NULL;
				pDemon = DemonTable[i];

				if (pDemon)
				{
					pDemon->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					pDemon->GetAIInterface()->setWaypointToMove(3);
				}
			}
		}

		if (DemonTable.size() == 5)
			DemonTable.clear();
		else SecondWave = true;
	}

	void SecondWaveAttackCmd()
	{
		Unit* pPitCommander = NULL;
		pPitCommander = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-249.119995f, 1169.209961f, 41.552101f, PIT_COMMANDER);
		
		if (pPitCommander && !pPitCommander->CombatStatus.IsInCombat() && RandomUInt(3) == 1)
				pPitCommander->Emote(EMOTE_ONESHOT_POINT);

		for (uint32 i = 1; i < DemonTable.size(); i+=2)
		{
			Unit* pDemon = NULL;
			pDemon = DemonTable[i];

			if (pDemon)
			{
				pDemon->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				pDemon->GetAIInterface()->setWaypointToMove(3);
			}
		}

		DemonTable.clear();
	}

	void PitCommanderSpawn()
	{
		PitCommanderRespawn = DEMON_RESPAWN_TIME;
		uint32 WaveSize = 0;
		uint32 Counter = 0;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
		{	
			if ((*itr)->GetTypeId()== TYPEID_UNIT)
			{
				Creature* CreatureToCheck = NULL;
				CreatureToCheck = (Creature*)(*itr);
				if (CreatureToCheck && CreatureToCheck->spawnid == 0 && CreatureToCheck->isAlive() && (CreatureToCheck->GetEntry() == 18944 || CreatureToCheck->GetEntry() == 19005))
					Counter++;
			}
		}

		if (Counter >= 0 && Counter <= 5)
			WaveSize = RandomUInt(3);
		if (Counter > 5 && Counter <= 10)
			WaveSize = 2;
		if (Counter > 10)
			WaveSize = 5;

		if (WaveSize == 1)
		{
			for (int i = 0; i < 10; i++)
			{
				uint32 Entry = FEL_SOLDIER;
				if (RandomUInt(3) == 1)
					Entry = WRATH_MASTER;

				DemonTable.push_back(_unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, DSPoints[i].x, DSPoints[i].y, DSPoints[i].z, DSPoints[i].o, true, false, 0, 0));
			}
		}

		else if (WaveSize == 5)
			PitCommanderRespawn = 60;

		else
		{
			for (int i = 0; i < 10; i+=2)
			{
				uint32 Entry = FEL_SOLDIER;
				if (RandomUInt(3) == 1)
					Entry = WRATH_MASTER;

				DemonTable.push_back(_unit->GetMapMgr()->GetInterface()->SpawnCreature(Entry, DSPoints[i].x, DSPoints[i].y, DSPoints[i].z, DSPoints[i].o, true, false, 0, 0));
			}
		}
	}

protected:

	uint32 SpecialUnitCheckTimer;
	uint32 PitCommanderRespawn;
	uint32 APortalRespawn;
	uint32 HPortalRespawn;
	uint32 InfernalSpawn;
	uint32 LastInfernal;
	uint32 APortalMove;
	uint32 HPortalMove;
	uint32 JustiMove;
	uint32 MelgrMove;
	bool SecondWave;
	bool _Infernals;
	bool PitComSpw;
	bool Ressed;
	bool Ress;

	Unit *pAlliance;
	Unit *pHorde;
	Unit *pJusti;
	Unit *pMelgr;
};


//=====================================================================================//
//===================================Horde Units=======================================//
//=====================================================================================//

// Melgromm Highmountain AI

#define EARTH_SHOCK 22885
#define CHAIN_LIGHTNING 33643
#define CHAIN_HEAL 33642
#define STRENGHT_OF_THE_STORM_TOTEM 33570
#define MAGMA_FLOW_TOTEM 33560

// Additional
#define PERMANENT_FEIGN_DEATH 31261
#define REVIVE_SELF 32343

class MelGrommHighmountainAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MelGrommHighmountainAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    MelGrommHighmountainAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(EARTH_SHOCK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CHAIN_HEAL);
		spells[2].targettype = TARGET_RANDOM_FRIEND;
		spells[2].instant = false;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].maxdist2cast = 30.0f;
		spells[2].minhp2cast = 60;

		spells[3].info = dbcSpell.LookupEntry(STRENGHT_OF_THE_STORM_TOTEM);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(MAGMA_FLOW_TOTEM);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(PERMANENT_FEIGN_DEATH);	// won't work anyway
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;

		spells[6].info = dbcSpell.LookupEntry(REVIVE_SELF);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		OCStop = 0;
		LastWP = 0;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		if (_unit->GetPositionX() == AHSPoints[1].x && _unit->GetPositionY() == AHSPoints[1].y &&
			_unit->GetPositionZ() == AHSPoints[1].z)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
			_unit->GetAIInterface()->m_moveBackward = true;
			_unit->GetAIInterface()->m_moveRun = true;
			_unit->m_noRespawn = true;

			for (uint32 x = 1; x < 12; x++)
			{
				_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		// Can't find his OnCombatStart sound id - _unit->PlaySoundToSet(?); 
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

        OCStop++;
		if (OCStop >= 2 && RandomUInt(2) == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The attackers shall not succeed!  Strength and honor to the Horde!");

			OCStop = 0;
		}

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 10)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].maxdist2cast, spells[i].minhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float distance, int health)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) <= distance*distance && ((RandomTarget->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 10:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 11:
			_unit->SetFacing(SWP[1].o);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;

		if (id == 11)
		{
			wp->x = SWP[1].x;
			wp->y = SWP[1].y;
			wp->z = SWP[1].z;
			wp->o = SWP[1].o;
		}

		else
		{
			wp->x = HWP[id].x;
			wp->y = HWP[id].y;
			wp->z = HWP[id].z;
			wp->o = HWP[id].o;
		}

		if (id == 10) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 OCStop;
	int nrspells;
};

// Orgrimmar Grunt AI

#define IRON_BITE 32962

class OrgrimmarGruntAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(OrgrimmarGruntAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    OrgrimmarGruntAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(IRON_BITE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == HMSPoints[i].x && _unit->GetPositionY() == HMSPoints[i].y &&
				_unit->GetPositionZ() == HMSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 12; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 10)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 64.0f )
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 10:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 11:
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 375);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 11)
		{
			wp->x = HMWP[FormationWP].x;
			wp->y = HMWP[FormationWP].y;
			wp->z = HMWP[FormationWP].z;
			wp->o = HMWP[FormationWP].o;
		}
		else
		{
			wp->x = HWP[id].x;
			wp->y = HWP[id].y;
			wp->z = HWP[id].z;
			wp->o = HWP[id].o;
		}

		if (id == 10) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Orgrimmar Shaman AI

#define OS_FLAME_SHOCK 15616
#define BLOODLUST 16170
#define LIGHTNING_SHIELD 20545
#define SLOW 18972

class OrgrimmarShamanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(OrgrimmarShamanAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    OrgrimmarShamanAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(OS_FLAME_SHOCK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLOODLUST);
		spells[1].targettype = TARGET_RANDOM_FRIEND;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].maxdist2cast = 30.0f;
		spells[1].minhp2cast = 100;

		spells[2].info = dbcSpell.LookupEntry(LIGHTNING_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SLOW);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 6.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].maxdist2cast = 30.0f;
		spells[3].minhp2cast = 100;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == HMSPoints[i].x && _unit->GetPositionY() == HMSPoints[i].y &&
				_unit->GetPositionZ() == HMSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1759);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 12; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 10)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].maxdist2cast, spells[i].minhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float distance, int health)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) <= distance*distance && ((RandomTarget->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 10:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 11:
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 45);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 11)
		{
			wp->x = HMWP[FormationWP].x;
			wp->y = HMWP[FormationWP].y;
			wp->z = HMWP[FormationWP].z;
			wp->o = HMWP[FormationWP].o;
		}
		else
		{
			wp->x = HWP[id].x;
			wp->y = HWP[id].y;
			wp->z = HWP[id].z;
			wp->o = HWP[id].o;
		}

		if (id == 10) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Darkspear Axe Thrower AI

#define THROW 10277

class DarkspearAxeThrowerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DarkspearAxeThrowerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DarkspearAxeThrowerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(THROW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;

		_unit->GetAIInterface()->m_canRangedAttack = true;
		_unit->GetAIInterface()->disable_ranged = true;
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 6; i++)
		{
			if (_unit->GetPositionX() == HRSPoints[i].x && _unit->GetPositionY() == HRSPoints[i].y &&
				_unit->GetPositionZ() == HRSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 12; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		if (_unit->GetDistance2dSq(mTarget) >= 64.0f && _unit->GetDistance2dSq(mTarget) <= 900.0f)
		{
			_unit->GetAIInterface()->disable_ranged = true;
			_unit->GetAIInterface()->m_canMove = false;

			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
		}

		if (_unit->GetDistance2dSq(mTarget) > 900.0f)
			_unit->GetAIInterface()->disable_ranged = true;

		RegisterAIUpdateEvent(1500);
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->disable_ranged = false;
		_unit->GetAIInterface()->m_canMove = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 10)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) >= 64.0f && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 900.0f)
			{
				_unit->GetAIInterface()->disable_ranged = true;
				_unit->GetAIInterface()->m_canMove = false;
				
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			else
			{
				_unit->GetAIInterface()->disable_ranged = false;
				_unit->GetAIInterface()->m_canMove = true;
			}
		}
    }
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 10:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 11)
		{
			wp->x = HRWP[FormationWP].x;
			wp->y = HRWP[FormationWP].y;
			wp->z = HRWP[FormationWP].z;
			wp->o = HRWP[FormationWP].o;
		}
		else
		{
			wp->x = HWP[id].x;
			wp->y = HWP[id].y;
			wp->z = HWP[id].z;
			wp->o = HWP[id].o;
		}

		if (id == 10) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Undercity Mage AI

#define ICEBOLT 33463
#define FIREBALL 33417
#define BLIZZARD 33624

class UndercityMageAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(UndercityMageAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    UndercityMageAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ICEBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 60.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FIREBALL);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 25.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(BLIZZARD);	// works differently o_O
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].perctrigger = 15.0f;
		spells[2].attackstoptimer = 1000;

		_unit->GetAIInterface()->m_canRangedAttack = true;
		_unit->GetAIInterface()->disable_ranged = true;
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 2; i++)
		{
			if (_unit->GetPositionX() == HCSPoints[i].x && _unit->GetPositionY() == HCSPoints[i].y &&
				_unit->GetPositionZ() == HCSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 12; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		if (_unit->GetDistance2dSq(mTarget) <= 1600.0f)
		{
			_unit->GetAIInterface()->disable_ranged = true;
			_unit->GetAIInterface()->m_canMove = false;

			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
		}

		else
			_unit->GetAIInterface()->disable_ranged = true;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->disable_ranged = false;
		_unit->GetAIInterface()->m_canMove = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 10)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) >= 0.0f && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 1600.0f)
			{
				_unit->GetAIInterface()->disable_ranged = true;
				_unit->GetAIInterface()->m_canMove = false;
				
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			else
			{
				_unit->GetAIInterface()->disable_ranged = false;
				_unit->GetAIInterface()->m_canMove = true;
			}
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 10:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 11:
			_unit->SetFacing(HCWP[FormationWP].o);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 11)
		{
			wp->x = HCWP[FormationWP].x;
			wp->y = HCWP[FormationWP].y;
			wp->z = HCWP[FormationWP].z;
			wp->o = HCWP[FormationWP].o;
		}
		else
		{
			wp->x = HWP[id].x;
			wp->y = HWP[id].y;
			wp->z = HWP[id].z;
			wp->o = HWP[id].o;
		}

		if (id == 10) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

//=====================================================================================//
//=================================Alliance Units======================================//
//=====================================================================================//

// Justinius The Harbinger AI

#define FLASH_OF_LIGHT 33641
#define CONSECRATION 33559
#define JUDGEMENT_OF_COMMAND 33554
#define DIVINE_SHIELD 33581
#define GREATER_BLESSING_OF_MIGHT 33564

class JustiniusTheHarbingerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JustiniusTheHarbingerAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    JustiniusTheHarbingerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FLASH_OF_LIGHT);
		spells[0].targettype = TARGET_RANDOM_FRIEND;
		spells[0].instant = false;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].maxdist2cast = 30.0f;
		spells[0].minhp2cast = 60;

		spells[1].info = dbcSpell.LookupEntry(CONSECRATION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(JUDGEMENT_OF_COMMAND);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(DIVINE_SHIELD);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 7.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(GREATER_BLESSING_OF_MIGHT);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = true;
		spells[4].perctrigger = 5.0f;
		spells[4].attackstoptimer = 1000;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		OCStop = 0;
		LastWP = 0;

		if (_unit->GetPositionX() == AHSPoints[0].x && _unit->GetPositionY() == AHSPoints[0].y &&
			_unit->GetPositionZ() == AHSPoints[0].z)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->m_moveBackward = true;
			_unit->GetAIInterface()->m_moveRun = true;
			_unit->m_noRespawn = true;

			for (uint32 x = 1; x < 11; x++)
			{
				_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 375);
		_unit->PlaySoundToSet(9705); 
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

        OCStop++;
		if (OCStop >= 2 && RandomUInt(2) == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the power of the Light! Grace and glory to the Alliance!");

			OCStop = 0;
		}

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 9)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].maxdist2cast, spells[i].minhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float distance, int health)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) <= distance*distance && ((RandomTarget->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;
		switch (iWaypointId)
		{
		case 9:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 10:
			_unit->SetFacing(SWP[0].o);	// bugs pos a bit
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;

		if (id == 10)
		{
			wp->x = SWP[0].x;
			wp->y = SWP[0].y;
			wp->z = SWP[0].z;
			wp->o = SWP[0].o;
		}

		else
		{
			wp->x = AWP[id].x;
			wp->y = AWP[id].y;
			wp->z = AWP[id].z;
			wp->o = AWP[id].o;
		}

		if (id == 9) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 OCStop;
	int nrspells;
};

// Stormwind Soldier AI

#define STRIKE 33626
#define DEMORALIZING_SHOUT 23511

class StormwindSoldierAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StormwindSoldierAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    StormwindSoldierAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(DEMORALIZING_SHOUT);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == AMSPoints[i].x && _unit->GetPositionY() == AMSPoints[i].y &&
				_unit->GetPositionZ() == AMSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 11; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 9)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 9:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 10:
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 45);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;

		if (id == 10)
		{
			wp->x = AMWP[FormationWP].x;
			wp->y = AMWP[FormationWP].y;
			wp->z = AMWP[FormationWP].z;
			wp->o = AMWP[FormationWP].o;
		}
		else
		{
			wp->x = AWP[id].x;
			wp->y = AWP[id].y;
			wp->z = AWP[id].z;
			wp->o = AWP[id].o;
		}

		if (id == 9) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Ironforge Paladin AI

#define SEAL_OF_SACRIFICE 13903
#define EXORCISM 33632
#define HAMMER_OF_JUSTICE 13005

class IronforgePaladinAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(IronforgePaladinAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    IronforgePaladinAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SEAL_OF_SACRIFICE);
		spells[0].targettype = TARGET_RANDOM_FRIEND;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].maxdist2cast = 30.0f;
		spells[0].minhp2cast = 100;

		spells[1].info = dbcSpell.LookupEntry(EXORCISM);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 7.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].maxdist2cast = 30.0f;
		spells[2].minhp2cast = 100;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == AMSPoints[i].x && _unit->GetPositionY() == AMSPoints[i].y &&
				_unit->GetPositionZ() == AMSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 11; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 9)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
    }
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].maxdist2cast, spells[i].minhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float distance, int health)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) <= distance*distance && ((RandomTarget->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() <= health && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 9:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 10:
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 45);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 10)
		{
			wp->x = AMWP[FormationWP].x;
			wp->y = AMWP[FormationWP].y;
			wp->z = AMWP[FormationWP].z;
			wp->o = AMWP[FormationWP].o;
		}
		else
		{
			wp->x = AWP[id].x;
			wp->y = AWP[id].y;
			wp->z = AWP[id].z;
			wp->o = AWP[id].o;
		}
		if (id == 9) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Darnassian Archer AI

#define SHOOT 15620

class DarnassianArcherAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DarnassianArcherAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    DarnassianArcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHOOT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;

		_unit->GetAIInterface()->m_canRangedAttack = true;
		_unit->GetAIInterface()->disable_ranged = true;
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 6; i++)
		{
			if (_unit->GetPositionX() == ARSPoints[i].x && _unit->GetPositionY() == ARSPoints[i].y &&
				_unit->GetPositionZ() == ARSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1755);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 11; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		if (_unit->GetDistance2dSq(mTarget) >= 64.0f && _unit->GetDistance2dSq(mTarget) <= 900.0f)
		{
			_unit->GetAIInterface()->disable_ranged = true;
			_unit->GetAIInterface()->m_canMove = false;

			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
		}

		if (_unit->GetDistance2dSq(mTarget) > 900.0f)
			_unit->GetAIInterface()->disable_ranged = true;

		RegisterAIUpdateEvent(1500);
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->disable_ranged = true;
		_unit->GetAIInterface()->m_canMove = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 9)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) >= 64.0f && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 900.0f)
			{
				_unit->GetAIInterface()->disable_ranged = true;
				_unit->GetAIInterface()->m_canMove = false;
				
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			else
			{
				_unit->GetAIInterface()->disable_ranged = false;
				_unit->GetAIInterface()->m_canMove = true;
			}
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 9:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;

		if (id == 10)
		{
			wp->x = ARWP[FormationWP].x;
			wp->y = ARWP[FormationWP].y;
			wp->z = ARWP[FormationWP].z;
			wp->o = ARWP[FormationWP].o;
		}
		else
		{
			wp->x = AWP[id].x;
			wp->y = AWP[id].y;
			wp->z = AWP[id].z;
			wp->o = AWP[id].o;
		}

		if (id == 9) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

// Stormwind Mage AI

#define SM_FIREBALL 33417
#define ARCANE_MISSILES 33419
#define SM_BLIZZARD 33624

class StormwindMageAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StormwindMageAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    StormwindMageAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SM_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 70.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SM_BLIZZARD);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].perctrigger = 15.0f;
		spells[2].attackstoptimer = 1000;

		_unit->GetAIInterface()->m_canRangedAttack = true;
		_unit->GetAIInterface()->disable_ranged = true;
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 2; i++)
		{
			if (_unit->GetPositionX() == ACSPoints[i].x && _unit->GetPositionY() == ACSPoints[i].y &&
				_unit->GetPositionZ() == ACSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1755);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->m_noRespawn = true;

				FormationWP = i;

				for (uint32 x = 1; x < 11; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		if (_unit->GetDistance2dSq(mTarget) <= 1600.0f)
		{
			_unit->GetAIInterface()->disable_ranged = true;
			_unit->GetAIInterface()->m_canMove = false;

			_unit->CastSpell(mTarget, spells[0].info, spells[0].instant);
		}

		else
			_unit->GetAIInterface()->disable_ranged = true;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->disable_ranged = false;
		_unit->GetAIInterface()->m_canMove = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		if (LastWP < 9)
			_unit->GetAIInterface()->setWaypointToMove(LastWP+1);
		else _unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) >= 0.0f && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 1600.0f)
			{
				_unit->GetAIInterface()->disable_ranged = true;
				_unit->GetAIInterface()->m_canMove = false;
				
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			else
			{
				_unit->GetAIInterface()->disable_ranged = false;
				_unit->GetAIInterface()->m_canMove = true;
			}
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		switch (iWaypointId)
		{
		case 9:
			_unit->Emote(EMOTE_ONESHOT_SALUTE);
			break;
		case 10:
			_unit->SetFacing(ACWP[FormationWP].o);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		if (id == 10)
		{
			wp->x = ACWP[FormationWP].x;
			wp->y = ACWP[FormationWP].y;
			wp->z = ACWP[FormationWP].z;
			wp->o = ACWP[FormationWP].o;
		}
		else
		{
			wp->x = AWP[id].x;
			wp->y = AWP[id].y;
			wp->z = AWP[id].z;
			wp->o = AWP[id].o;
		}
	
		if (id == 9) waittime = 3000;

        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 FormationWP;
	int nrspells;
};

//=====================================================================================//
//===================================Demon Units=======================================//
//=====================================================================================//

// Pit Commander AI

#define RAIN_OF_FIRE 33627
#define PC_CLEAVE 16044
#define SEED_OF_CORRUPTION 37826

class PitCommanderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PitCommanderAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    PitCommanderAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(PC_CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 9.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SEED_OF_CORRUPTION);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 64.0f )
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:

	int nrspells;
};

// Infernal Siegebreaker AI

class InfernalSiegebreakerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfernalSiegebreakerAI);

    InfernalSiegebreakerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		for (int i = 0; i < 4; i++)
		{
			if (_unit->GetPositionX() == Infernals[i].x && _unit->GetPositionY() == Infernals[i].y &&
			_unit->GetPositionZ() == Infernals[i].z)
			{
				_unit->m_noRespawn = true;
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		//float val = RandomFloat(100.0f);
		//SpellCast(val);
    }
};

// Fel Soldier AI

#define CUTDOWN 32009
#define CLEAVE 15496

class FelSoldierAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FelSoldierAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    FelSoldierAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CUTDOWN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 1000;

		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == DSPoints[i].x && _unit->GetPositionY() == DSPoints[i].y &&
			_unit->GetPositionZ() == DSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->m_noRespawn = true;

				WPStarter = 8*i;

				for (int x = 1; x < 9; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 64.0f )
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;

		if (iWaypointId == 3)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->SetFacing(4.708117f);
		}

		if (iWaypointId == 8)
			_unit->Despawn(0,0);
	}
	
	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
		WPStarter++;

        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		wp->x = PitDemonPaths[WPStarter].x;
		wp->y = PitDemonPaths[WPStarter].y;
		wp->z = PitDemonPaths[WPStarter].z;
		wp->o = PitDemonPaths[WPStarter].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 WPStarter;
	int nrspells;
};

// Wrath Master AI

class WrathMasterAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WrathMasterAI);

    WrathMasterAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		LastWP = 0;

		for (int i = 0; i < 10; i++)
		{
			if (_unit->GetPositionX() == DSPoints[i].x && _unit->GetPositionY() == DSPoints[i].y &&
			_unit->GetPositionZ() == DSPoints[i].z)
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1758);
				_unit->GetAIInterface()->m_moveBackward = true;
				_unit->m_noRespawn = true;

				WPStarter = 8*i;

				for (int x = 1; x < 9; x++)
				{
					_unit->GetAIInterface()->addWayPoint(CreateWaypoint(x, 0, RUN));
				}
			}
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->setWaypointToMove(LastWP);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		//float val = RandomFloat(100.0f);
		//SpellCast(val);
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		LastWP = iWaypointId;
		
		if (iWaypointId == 3)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->SetFacing(4.708117f);
		}

		if (iWaypointId == 8)
			_unit->Despawn(0,0);
	}
	
	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
		WPStarter++;

        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		wp->x = PitDemonPaths[WPStarter].x;
		wp->y = PitDemonPaths[WPStarter].y;
		wp->z = PitDemonPaths[WPStarter].z;
		wp->o = PitDemonPaths[WPStarter].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 LastWP;
	uint32 WPStarter;
};

void SetupEventDarkPortal(ScriptMgr * mgr)
{
	/* Event Trigger */
	mgr->register_creature_script(CN_DARK_PORTAL_EVENT_TRIGGER, &DarkPortalEventTrigger::Create);

	/* Horde Units */
	mgr->register_creature_script(MELGROMM_HIGHMOUNTAIN, &MelGrommHighmountainAI::Create);
	mgr->register_creature_script(ORGRIMMAR_GRUNT, &OrgrimmarGruntAI::Create);
	mgr->register_creature_script(ORGRIMMAR_SHAMAN, &OrgrimmarShamanAI::Create);
	mgr->register_creature_script(DARKSPEAR_AXE_THROWER, &DarkspearAxeThrowerAI::Create);
	mgr->register_creature_script(UNDERCITY_MAGE, &UndercityMageAI::Create);

	/* Alliance Units */
	mgr->register_creature_script(JUSTINIUS_THE_HARBINGER, &JustiniusTheHarbingerAI::Create);
	mgr->register_creature_script(STORMWIND_SOLDIER, &StormwindSoldierAI::Create);
	mgr->register_creature_script(IRONFORGE_PALADIN, &IronforgePaladinAI::Create);
	mgr->register_creature_script(DARNASSIAN_ARCHER, &DarnassianArcherAI::Create);
	mgr->register_creature_script(STORMWIND_MAGE, &StormwindMageAI::Create);

	/* Demon Units */
	mgr->register_creature_script(PIT_COMMANDER, &PitCommanderAI::Create);
	mgr->register_creature_script(INFERNAL_SIEGEBREAKER, &InfernalSiegebreakerAI::Create);
	mgr->register_creature_script(FEL_SOLDIER, &FelSoldierAI::Create);
	mgr->register_creature_script(WRATH_MASTER, &WrathMasterAI::Create);
}
