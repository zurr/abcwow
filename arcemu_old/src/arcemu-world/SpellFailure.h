/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef _SPELLFAILURE_H
#define _SPELLFAILURE_H

enum SpellCastError
{
	SPELL_FAILED_AFFECTING_COMBAT                                = 0,
	SPELL_FAILED_ALREADY_AT_FULL_HEALTH                          = 1,
	SPELL_FAILED_ALREADY_AT_FULL_MANA                            = 2,
	SPELL_FAILED_ALREADY_AT_FULL_POWER                           = 3,
	SPELL_FAILED_ALREADY_BEING_TAMED                             = 4,
	SPELL_FAILED_ALREADY_HAVE_CHARM                              = 5,
	SPELL_FAILED_ALREADY_HAVE_SUMMON                             = 6,
	SPELL_FAILED_ALREADY_OPEN                                    = 7,
	SPELL_FAILED_AURA_BOUNCED                                    = 8,
	SPELL_FAILED_AUTOTRACK_INTERRUPTED                           = 9,
	SPELL_FAILED_BAD_IMPLICIT_TARGETS                            = 10,
	SPELL_FAILED_BAD_TARGETS                                     = 11,
	SPELL_FAILED_CANT_BE_CHARMED                                 = 12,
	SPELL_FAILED_CANT_BE_DISENCHANTED                            = 13,
	SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL                      = 14,
	SPELL_FAILED_CANT_BE_PROSPECTED                              = 15,
	SPELL_FAILED_CANT_CAST_ON_TAPPED                             = 16,
	SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE                       = 17,
	SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED                       = 18,
	SPELL_FAILED_CANT_STEALTH                                    = 19,
	SPELL_FAILED_CASTER_AURASTATE                                = 20,
	SPELL_FAILED_CASTER_DEAD                                     = 21,
	SPELL_FAILED_CHARMED                                         = 22,
	SPELL_FAILED_CHEST_IN_USE                                    = 23,
	SPELL_FAILED_CONFUSED                                        = 24,
	SPELL_FAILED_DONT_REPORT                                     = 25,
	SPELL_FAILED_EQUIPPED_ITEM                                   = 26,
	SPELL_FAILED_EQUIPPED_ITEM_CLASS                             = 27,
	SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND                    = 28,
	SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND                     = 29,
	SPELL_FAILED_ERROR                                           = 30,
	SPELL_FAILED_FIZZLE                                          = 31,
	SPELL_FAILED_FLEEING                                         = 32,
	SPELL_FAILED_FOOD_LOWLEVEL                                   = 33,
	SPELL_FAILED_HIGHLEVEL                                       = 34,
	SPELL_FAILED_HUNGER_SATIATED                                 = 35,
	SPELL_FAILED_IMMUNE                                          = 36,
	SPELL_FAILED_INTERRUPTED                                     = 37,
	SPELL_FAILED_INTERRUPTED_COMBAT                              = 38,
	SPELL_FAILED_ITEM_ALREADY_ENCHANTED                          = 39,
	SPELL_FAILED_ITEM_GONE                                       = 40,
	SPELL_FAILED_ITEM_NOT_FOUND                                  = 41,
	SPELL_FAILED_ITEM_NOT_READY                                  = 42,
	SPELL_FAILED_LEVEL_REQUIREMENT                               = 43,
	SPELL_FAILED_LINE_OF_SIGHT                                   = 44,
	SPELL_FAILED_LOWLEVEL                                        = 45,
	SPELL_FAILED_LOW_CASTLEVEL                                   = 46,
	SPELL_FAILED_MAINHAND_EMPTY                                  = 47,
	SPELL_FAILED_MOVING                                          = 48,
	SPELL_FAILED_NEED_AMMO                                       = 49,
	SPELL_FAILED_NEED_AMMO_POUCH                                 = 50,
	SPELL_FAILED_NEED_EXOTIC_AMMO                                = 51,
	SPELL_FAILED_NOPATH                                          = 52,
	SPELL_FAILED_NOT_BEHIND                                      = 53,
	SPELL_FAILED_NOT_FISHABLE                                    = 54,
	SPELL_FAILED_NOT_FLYING                                      = 55,
	SPELL_FAILED_NOT_HERE                                        = 56,
	SPELL_FAILED_NOT_INFRONT                                     = 57,
	SPELL_FAILED_NOT_IN_CONTROL                                  = 58,
	SPELL_FAILED_NOT_KNOWN                                       = 59,
	SPELL_FAILED_NOT_MOUNTED                                     = 60,
	SPELL_FAILED_NOT_ON_TAXI                                     = 61,
	SPELL_FAILED_NOT_ON_TRANSPORT                                = 62,
	SPELL_FAILED_NOT_READY                                       = 63,
	SPELL_FAILED_NOT_SHAPESHIFT                                  = 64,
	SPELL_FAILED_NOT_STANDING                                    = 65,
	SPELL_FAILED_NOT_TRADEABLE                                   = 66,
	SPELL_FAILED_NOT_TRADING                                     = 67,
	SPELL_FAILED_NOT_UNSHEATHED                                  = 68,
	SPELL_FAILED_NOT_WHILE_GHOST                                 = 69,
	SPELL_FAILED_NO_AMMO                                         = 70,
	SPELL_FAILED_NO_CHARGES_REMAIN                               = 71,
	SPELL_FAILED_NO_CHAMPION                                     = 72,
	SPELL_FAILED_NO_COMBO_POINTS                                 = 73,
	SPELL_FAILED_NO_DUELING                                      = 74,
	SPELL_FAILED_NO_ENDURANCE                                    = 75,
	SPELL_FAILED_NO_FISH                                         = 76,
	SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED                     = 77,
	SPELL_FAILED_NO_MOUNTS_ALLOWED                               = 78,
	SPELL_FAILED_NO_PET                                          = 79,
	SPELL_FAILED_NO_POWER                                        = 80,
	SPELL_FAILED_NOTHING_TO_DISPEL                               = 81,
	SPELL_FAILED_NOTHING_TO_STEAL                                = 82,
	SPELL_FAILED_ONLY_ABOVEWATER                                 = 83,
	SPELL_FAILED_ONLY_DAYTIME                                    = 84,
	SPELL_FAILED_ONLY_INDOORS                                    = 85,
	SPELL_FAILED_ONLY_MOUNTED                                    = 86,
	SPELL_FAILED_ONLY_NIGHTTIME                                  = 87,
	SPELL_FAILED_ONLY_OUTDOORS                                   = 88,
	SPELL_FAILED_ONLY_SHAPESHIFT                                 = 89,
	SPELL_FAILED_ONLY_STEALTHED                                  = 90,
	SPELL_FAILED_ONLY_UNDERWATER                                 = 91,
	SPELL_FAILED_OUT_OF_RANGE                                    = 92,
	SPELL_FAILED_PACIFIED                                        = 93,
	SPELL_FAILED_POSSESSED                                       = 94,
	SPELL_FAILED_REAGENTS                                        = 95,
	SPELL_FAILED_REQUIRES_AREA                                   = 96,
	SPELL_FAILED_REQUIRES_SPELL_FOCUS                            = 97,
	SPELL_FAILED_ROOTED                                          = 98,
	SPELL_FAILED_SILENCED                                        = 99,
	SPELL_FAILED_SPELL_IN_PROGRESS                               = 100,
	SPELL_FAILED_SPELL_LEARNED                                   = 101,
	SPELL_FAILED_SPELL_UNAVAILABLE                               = 102,
	SPELL_FAILED_STUNNED                                         = 103,
	SPELL_FAILED_TARGETS_DEAD                                    = 104,
	SPELL_FAILED_TARGET_AFFECTING_COMBAT                         = 105,
	SPELL_FAILED_TARGET_AURASTATE                                = 106,
	SPELL_FAILED_TARGET_DUELING                                  = 107,
	SPELL_FAILED_TARGET_ENEMY                                    = 108,
	SPELL_FAILED_TARGET_ENRAGED                                  = 109,
	SPELL_FAILED_TARGET_FRIENDLY                                 = 110,
	SPELL_FAILED_TARGET_IN_COMBAT                                = 111,
	SPELL_FAILED_TARGET_IS_PLAYER                                = 112,
	SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED                     = 113,
	SPELL_FAILED_TARGET_NOT_DEAD                                 = 114,
	SPELL_FAILED_TARGET_NOT_IN_PARTY                             = 115,
	SPELL_FAILED_TARGET_NOT_LOOTED                               = 116,
	SPELL_FAILED_TARGET_NOT_PLAYER                               = 117,
	SPELL_FAILED_TARGET_NO_POCKETS                               = 118,
	SPELL_FAILED_TARGET_NO_WEAPONS                               = 119,
	SPELL_FAILED_TARGET_UNSKINNABLE                              = 120,
	SPELL_FAILED_THIRST_SATIATED                                 = 121,
	SPELL_FAILED_TOO_CLOSE                                       = 122,
	SPELL_FAILED_TOO_MANY_OF_ITEM                                = 123,
	SPELL_FAILED_TOTEM_CATEGORY                                  = 124,
	SPELL_FAILED_TOTEMS                                          = 125,
	SPELL_FAILED_TRAINING_POINTS                                 = 126,
	SPELL_FAILED_TRY_AGAIN                                       = 127,
	SPELL_FAILED_UNIT_NOT_BEHIND                                 = 128,
	SPELL_FAILED_UNIT_NOT_INFRONT                                = 129,
	SPELL_FAILED_WRONG_PET_FOOD                                  = 130,
	SPELL_FAILED_NOT_WHILE_FATIGUED                              = 131,
	SPELL_FAILED_TARGET_NOT_IN_INSTANCE                          = 132,
	SPELL_FAILED_NOT_WHILE_TRADING                               = 133,
	SPELL_FAILED_TARGET_NOT_IN_RAID                              = 134,
	SPELL_FAILED_DISENCHANT_WHILE_LOOTING                        = 135,
	SPELL_FAILED_PROSPECT_WHILE_LOOTING                          = 136,
	SPELL_FAILED_PROSPECT_NEED_MORE                              = 137,
	SPELL_FAILED_TARGET_FREEFORALL                               = 138,
	SPELL_FAILED_NO_EDIBLE_CORPSES                               = 139,
	SPELL_FAILED_ONLY_BATTLEGROUNDS                              = 140,
	SPELL_FAILED_TARGET_NOT_GHOST                                = 141,
	SPELL_FAILED_TOO_MANY_SKILLS                                 = 142,
	SPELL_FAILED_TRANSFORM_UNUSABLE                              = 143,
	SPELL_FAILED_WRONG_WEATHER                                   = 144,
	SPELL_FAILED_DAMAGE_IMMUNE                                   = 145,
	SPELL_FAILED_PREVENTED_BY_MECHANIC                           = 146,
	SPELL_FAILED_PLAY_TIME                                       = 147,
	SPELL_FAILED_REPUTATION                                      = 148,
	SPELL_FAILED_MIN_SKILL                                       = 149,
	SPELL_FAILED_NOT_IN_ARENA                                    = 150,
	SPELL_FAILED_NOT_ON_SHAPESHIFT                               = 151,
	SPELL_FAILED_NOT_ON_STEALTHED                                = 152,
	SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE                            = 153,
	SPELL_FAILED_NOT_ON_MOUNTED                                  = 154,
	SPELL_FAILED_TOO_SHALLOW                                     = 155,
	SPELL_FAILED_TARGET_NOT_IN_SANCTUARY                         = 156,
	SPELL_FAILED_TARGET_IS_TRIVIAL                               = 157,
	SPELL_FAILED_BM_OR_INVISGOD                                  = 158,
	SPELL_FAILED_EXPERT_RIDING_REQUIREMENT                       = 159,
	SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT                      = 160,
	SPELL_FAILED_NOT_IDLE                                        = 161,
	SPELL_FAILED_NOT_INACTIVE                                    = 162,
	SPELL_FAILED_PARTIAL_PLAYTIME                                = 163,
	SPELL_FAILED_NO_PLAYTIME                                     = 164,
	SPELL_FAILED_NOT_IN_BATTLEGROUND                             = 165,
	SPELL_FAILED_NOT_IN_RAID_INSTANCE							 = 166,
	SPELL_FAILED_ONLY_IN_ARENA									 = 167,
	SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE                  = 168,
	SPELL_FAILED_UNKNOWN                                         = 169,
	SPELL_CANCAST_OK                                             = 255,
};
enum PetTameFailure
{
	PETTAME_INVALIDCREATURE			= 1,
	PETTAME_TOOMANY					= 2,
	PETTAME_CREATUREALREADYOWNED	= 3,
	PETTAME_NOTTAMEABLE				= 4,
	PETTAME_ANOTHERSUMMONACTIVE		= 5,
	PETTAME_UNITSCANTTAME			= 6,
	PETTAME_NOPETAVAILABLE			= 7,
	PETTAME_INTERNALERROR			= 8,
	PETTAME_TOOHIGHLEVEL			= 9,
	PETTAME_DEAD					= 10,
	PETTAME_NOTDEAD					= 11,
	PETTAME_UNKNOWNERROR			= 12,
};
#endif

