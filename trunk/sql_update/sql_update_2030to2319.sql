-- non existing crap
DELETE FROM trainer_spells where cast_spell IN (3983, 4020, 39786);

ALTER TABLE `spell_data_extra` ADD COLUMN `proc_interval` INTEGER UNSIGNED AFTER `dmg_bonus`
, COMMENT = '';
ALTER TABLE `spell_data_extra` MODIFY COLUMN `dmg_bonus` INTEGER UNSIGNED,
 MODIFY COLUMN `proc_interval` INTEGER UNSIGNED;

ALTER TABLE `worldmap_info` CHANGE COLUMN `mapid` `entry` INT(10) NOT NULL DEFAULT 0;
update worldmap_info set cooldown = 43200 where cooldown = 0 and type = 4;


ALTER TABLE `characters` CHANGE COLUMN `bannedReason` `banReason` VARCHAR(255);

CREATE TABLE `banned_phrases` (
  `phrase` varchar(250) NOT NULL,
  PRIMARY KEY  (`phrase`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `spell_proc_data` (
  `name_hash` int(30) NOT NULL,
  `proc_chance` int(30) NOT NULL,
  `forced_proc_flags` int(30) NOT NULL default '-1',
  PRIMARY KEY  (`name_hash`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `account_data` (
  `acct` int(30) NOT NULL,
  `uiconfig0` longtext,
  `uiconfig1` longtext,
  `uiconfig2` longtext,
  `uiconfig3` longtext,
  `uiconfig4` longtext,
  `uiconfig5` longtext,
  `uiconfig6` longtext,
  `uiconfig7` longtext,
  `uiconfig8` longtext,
  PRIMARY KEY  (`acct`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

alter table mailbox add column external_attached_item_guid bigint(20) not null default 0 after attached_item_guid;

CREATE TABLE `playeritems_external` (
  `ownerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `creator` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  `charges` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `randomprop` int(10) unsigned NOT NULL DEFAULT '0',
  `randomsuffix` int(30) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL DEFAULT '0',
  `durability` int(10) unsigned NOT NULL DEFAULT '0',
  `containerslot` int(11) DEFAULT '-1' COMMENT '',
  `slot` tinyint(4) NOT NULL DEFAULT '0',
  `enchantments` longtext COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`guid`),
  UNIQUE KEY `guid` (`guid`),
  KEY `ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

DROP TABLE IF EXISTS `creature_names_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `creature_names_localized` (
  `id` int(30) unsigned NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `name` varchar(100) NOT NULL,
  `subname` varchar(100) NOT NULL,
  PRIMARY KEY (`id`,`language_code`),
  KEY `lol` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `gameobject_names_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `gameobject_names_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `itempages_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `itempages_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `text` text NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `a` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `items_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `items_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `name` varchar(100) NOT NULL,
  `description` varchar(100) NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `npc_text_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `npc_text_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `text0` varchar(200) NOT NULL,
  `text0_1` varchar(200) NOT NULL,
  `text1` varchar(200) NOT NULL,
  `text1_1` varchar(200) NOT NULL,
  `text2` varchar(200) NOT NULL,
  `text2_1` varchar(200) NOT NULL,
  `text3` varchar(200) NOT NULL,
  `text3_1` varchar(200) NOT NULL,
  `text4` varchar(200) NOT NULL,
  `text4_1` varchar(200) NOT NULL,
  `text5` varchar(200) NOT NULL,
  `text5_1` varchar(200) NOT NULL,
  `text6` varchar(200) NOT NULL,
  `text6_1` varchar(200) NOT NULL,
  `text7` varchar(200) NOT NULL,
  `text7_1` varchar(200) NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `quests_localized`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `quests_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `Title` text NOT NULL,
  `Details` text NOT NULL,
  `Objectives` text NOT NULL,
  `CompletionText` text NOT NULL,
  `IncompleteText` text NOT NULL,
  `EndText` text NOT NULL,
  `ObjectiveText1` text NOT NULL,
  `ObjectiveText2` text NOT NULL,
  `ObjectiveText3` text NOT NULL,
  `ObjectiveText4` text NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

alter table accounts add column forceLanguage varchar(5) not null default "enUS";

DROP TABLE IF EXISTS `instances`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `instances` (
  `id` int(30) NOT NULL,
  `mapid` int(30) NOT NULL,
  `creation` int(30) NOT NULL,
  `expiration` int(30) NOT NULL,
  `killed_npc_guids` text NOT NULL,
  `difficulty` int(30) NOT NULL,
  `creator_group` int(30) NOT NULL,
  `creator_guid` int(30) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

ALTER TABLE `quests` ADD COLUMN `ReqCastSpellId1` INTEGER(11) NOT NULL DEFAULT 0 AFTER `ReqKillMobOrGOCount4`;
ALTER TABLE `quests` ADD COLUMN `ReqCastSpellId2` INTEGER(11) NOT NULL DEFAULT 0 AFTER `ReqCastSpellId1`;
ALTER TABLE `quests` ADD COLUMN `ReqCastSpellId3` INTEGER(11) NOT NULL DEFAULT 0 AFTER `ReqCastSpellId2`;
ALTER TABLE `quests` ADD COLUMN `ReqCastSpellId4` INTEGER(11) NOT NULL DEFAULT 0 AFTER `ReqCastSpellId3`;

alter table characters modify column banned int(40) not null;

alter table playeritems add column wrapped_item_id int(30) not null default 0 after entry;
alter table playeritems_external add column wrapped_item_id int(30) not null default 0 after entry;

alter table playeritems add column wrapped_creator int(30) not null default 0 after wrapped_item_id;
alter table playeritems_external add column wrapped_creator int(30) not null default 0 after wrapped_item_id;


DROP TABLE IF EXISTS `characters_insert_queue`;
CREATE TABLE `characters_insert_queue` (
  `insert_temp_guid` int(10) unsigned NOT NULL default '0',
  `acct` int(10) unsigned NOT NULL default '0',
  `name` varchar(21) collate utf8_unicode_ci NOT NULL,
  `race` tinyint(3) unsigned NOT NULL default '0',
  `class` tinyint(3) unsigned NOT NULL default '0',
  `gender` tinyint(3) unsigned NOT NULL default '0',
  `custom_faction` int(30) NOT NULL default '0',
  `level` int(10) unsigned NOT NULL default '0',
  `xp` int(10) unsigned NOT NULL default '0',
  `exploration_data` longtext collate utf8_unicode_ci NOT NULL,
  `skills` longtext collate utf8_unicode_ci NOT NULL,
  `watched_faction_index` int(10) unsigned NOT NULL default '0',
  `selected_pvp_title` int(10) unsigned NOT NULL default '0',
  `available_pvp_titles` int(10) unsigned NOT NULL default '0',
  `gold` int(10) unsigned NOT NULL default '0',
  `ammo_id` int(10) unsigned NOT NULL default '0',
  `available_prof_points` int(10) unsigned NOT NULL default '0',
  `available_talent_points` int(10) unsigned NOT NULL default '0',
  `current_hp` int(10) unsigned NOT NULL default '0',
  `current_power` int(10) unsigned NOT NULL default '0',
  `pvprank` tinyint(3) unsigned NOT NULL default '0',
  `bytes` int(10) unsigned NOT NULL default '0',
  `bytes2` int(10) unsigned NOT NULL default '0',
  `player_flags` int(10) unsigned NOT NULL default '0',
  `player_bytes` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `mapId` int(10) unsigned NOT NULL default '0',
  `zoneId` int(10) unsigned NOT NULL default '0',
  `taximask` longtext collate utf8_unicode_ci NOT NULL,
  `banned` int(40) NOT NULL,
  `banReason` varchar(50) collate utf8_unicode_ci NOT NULL,
  `timestamp` int(11) NOT NULL,
  `online` int(11) default NULL,
  `bindpositionX` float NOT NULL default '0',
  `bindpositionY` float NOT NULL default '0',
  `bindpositionZ` float NOT NULL default '0',
  `bindmapId` int(10) unsigned NOT NULL default '0',
  `bindzoneId` int(10) unsigned NOT NULL default '0',
  `isResting` tinyint(3) unsigned NOT NULL default '0',
  `restState` tinyint(3) unsigned NOT NULL default '0',
  `restTime` int(10) unsigned NOT NULL default '0',
  `playedtime` longtext collate utf8_unicode_ci NOT NULL,
  `deathstate` int(10) unsigned NOT NULL default '0',
  `TalentResetTimes` int(10) unsigned NOT NULL default '0',
  `first_login` tinyint(3) unsigned NOT NULL default '0',
  `forced_rename_pending` tinyint(3) unsigned NOT NULL default '0',
  `publicNote` varchar(32) collate utf8_unicode_ci NOT NULL,
  `officerNote` varchar(32) collate utf8_unicode_ci NOT NULL,
  `guildid` int(10) unsigned NOT NULL default '0',
  `guildRank` int(10) unsigned NOT NULL default '0',
  `arenaPoints` int(10) NOT NULL,
  `totalstableslots` int(10) unsigned NOT NULL default '0',
  `instance_id` int(10) unsigned NOT NULL default '0',
  `entrypointmap` int(10) unsigned NOT NULL default '0',
  `entrypointx` float NOT NULL default '0',
  `entrypointy` float NOT NULL default '0',
  `entrypointz` float NOT NULL default '0',
  `entrypointo` float NOT NULL default '0',
  `entrypointinstance` int(10) unsigned NOT NULL default '0',
  `taxi_path` int(10) unsigned NOT NULL default '0',
  `taxi_lastnode` int(10) unsigned NOT NULL default '0',
  `taxi_mountid` int(10) unsigned NOT NULL default '0',
  `transporter` int(10) unsigned NOT NULL default '0',
  `transporter_xdiff` float NOT NULL default '0',
  `transporter_ydiff` float NOT NULL default '0',
  `transporter_zdiff` float NOT NULL default '0',
  `spells` longtext collate utf8_unicode_ci NOT NULL,
  `deleted_spells` longtext collate utf8_unicode_ci NOT NULL,
  `reputation` longtext collate utf8_unicode_ci NOT NULL,
  `actions` longtext collate utf8_unicode_ci NOT NULL,
  `auras` longtext collate utf8_unicode_ci NOT NULL,
  `finished_quests` longtext collate utf8_unicode_ci NOT NULL,
  `honorPointsToAdd` int(10) NOT NULL,
  `killsToday` int(10) unsigned NOT NULL default '0',
  `killsYesterday` int(10) unsigned NOT NULL default '0',
  `killsLifeTime` int(10) unsigned NOT NULL default '0',
  `honorToday` int(10) unsigned NOT NULL default '0',
  `honorYesterday` int(10) unsigned NOT NULL default '0',
  `honorPoints` int(10) unsigned NOT NULL default '0',
  `difficulty` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `guid` (`insert_temp_guid`),
  KEY `acct` (`acct`),
  KEY `guildid` (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

DROP TABLE IF EXISTS `playeritems_insert_queue`;
CREATE TABLE `playeritems_insert_queue` (
  `ownerguid` int(10) unsigned NOT NULL default '0',
  `entry` int(10) unsigned NOT NULL default '0',
  `wrapped_item_id` int(30) NOT NULL default '0',
  `wrapped_creator` int(30) NOT NULL default '0',
  `creator` int(10) unsigned NOT NULL default '0',
  `count` int(10) unsigned NOT NULL default '0',
  `charges` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `randomprop` int(10) unsigned NOT NULL default '0',
  `randomsuffix` int(30) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL default '0',
  `durability` int(10) unsigned NOT NULL default '0',
  `containerslot` int(11) NOT NULL default '-1' COMMENT 'couldnt find this being used in source',
  `slot` tinyint(4) NOT NULL default '0',
  `enchantments` longtext collate utf8_unicode_ci NOT NULL,
  KEY `ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

ALTER TABLE `ai_agents` CHANGE `entryId` `entry` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `AI_AGENT` `type` SmallINT(5) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `procEvent` `event` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `procChance` `chance` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `procCount` `maxcount` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `spellCooldown` `cooldown` INT(8) NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `spellId` `spell` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `spellType` `spelltype` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `ai_agents` CHANGE `spelltargetType` `targettype` INT(11) UNSIGNED NOT NULL DEFAULT '0';

-- ai_threattospellid
ALTER TABLE `ai_threattospellid` CHANGE `spellId` `spell` INT(11) UNSIGNED NOT NULL DEFAULT '0';

-- areatriggers
ALTER TABLE `areatriggers` CHANGE `AreaTriggerid` `entry` INT(11) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `Type` `type` TinyINT(3) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `Mapid` `map` INT(11) UNSIGNED NULL;
ALTER TABLE `areatriggers` CHANGE `Screen` `screen` INT(11) UNSIGNED NULL;
ALTER TABLE `areatriggers` CHANGE `Name` `name` varchar(100) NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `x` `position_x` float NOT NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `y` `position_y` float NOT NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `z` `position_z` float NOT NULL DEFAULT '0';
ALTER TABLE `areatriggers` CHANGE `o` `orientation` float NOT NULL DEFAULT '0';

-- auctionhouse
ALTER TABLE `auctionhouse` CHANGE `entryid` `id` INT(32) NOT NULL AUTO_INCREMENT;
ALTER TABLE `auctionhouse` CHANGE `auctioneer` `creature_entry` BigINT(64) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `auctionhouse` CHANGE `AHid` `group` INT(32) NOT NULL DEFAULT '0';

-- creature_formations
ALTER TABLE `creature_formations` CHANGE `creature_sqlid` `spawn_id` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_formations` CHANGE `followtarget_sqlid` `target_spawn_id` INT(10) UNSIGNED NOT NULL DEFAULT '0';

-- creature_names
ALTER TABLE `creature_names` CHANGE `creature_name` `name` varchar(100) NOT NULL;
ALTER TABLE `creature_names` CHANGE `Subname` `subname` varchar(100) NOT NULL;
ALTER TABLE `creature_names` CHANGE `Family` `family` INT(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `creature_names` CHANGE `Rank` `rank` INT(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `creature_names` CHANGE `SpellDataID` `spelldataid` INT(10) UNSIGNED NULL;
ALTER TABLE `creature_names` CHANGE `unknown_int1` `male_displayid2` INT(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_names` CHANGE `unknown_int2` `female_displayid2` INT(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_names` CHANGE `Civilian` `civilian` INT(4) UNSIGNED NULL;
ALTER TABLE `creature_names` CHANGE `Leader` `leader` TinyINT(3) UNSIGNED NULL;

-- creature_proto
ALTER TABLE `creature_proto` CHANGE `resistance0_armor` `armor` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item1slotdisplay` `equipmodel1` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item2slotdisplay` `equipmodel2` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item3slotdisplay` `equipmodel3` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item1info1` `equipinfo1` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item2info1` `equipinfo2` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item3info1` `equipinfo3` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item1info2` `equipslot1` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item2info2` `equipslot2` INT(30) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto` CHANGE `item3info2` `equipslot3` INT(30) UNSIGNED NOT NULL DEFAULT '0';

-- creature_spawns
ALTER TABLE `creature_spawns` CHANGE `x` `position_x` float NOT NULL;
ALTER TABLE `creature_spawns` CHANGE `y` `position_y` float NOT NULL;
ALTER TABLE `creature_spawns` CHANGE `z` `position_z` float NOT NULL;
ALTER TABLE `creature_spawns` CHANGE `o` `orientation` float NOT NULL;
ALTER TABLE `creature_spawns` CHANGE `factionid` `faction` INT(30) NOT NULL DEFAULT '14';
ALTER TABLE `creature_spawns` CHANGE `Map` `map` INT(30) NOT NULL;

-- creature_waypoints
ALTER TABLE `creature_waypoints` CHANGE `creatureid` `spawnid` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_waypoints` CHANGE `x` `position_x` float NOT NULL DEFAULT '0';
ALTER TABLE `creature_waypoints` CHANGE `y` `position_y` float NOT NULL DEFAULT '0';
ALTER TABLE `creature_waypoints` CHANGE `z` `position_z` float NOT NULL DEFAULT '0';

-- gameobject_quest_item_binding
ALTER TABLE `gameobject_quest_item_binding` CHANGE `gameobject_entry` `entry` INT(11) DEFAULT '0' NOT NULL;
ALTER TABLE `gameobject_quest_item_binding` CHANGE `quest_id` `quest` INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_quest_item_binding` CHANGE `item_id` `item` INT(11) NOT NULL DEFAULT '0';

-- gameobject_quest_pickup_binding
ALTER TABLE `gameobject_quest_pickup_binding` CHANGE `gameobject_entry` `entry` INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_quest_pickup_binding` CHANGE `quest_id` `quest` INT(11) NOT NULL DEFAULT '0';

-- gameobject_spawns
ALTER TABLE `gameobject_spawns` CHANGE `x` `position_x` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `y` `position_y` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `z` `position_z` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `o1` `orientation1` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `o2` `orientation2` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `o3` `orientation3` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `o4` `orientation4` float NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_spawns` CHANGE `Map` `map` int(10) unsigned NOT NULL DEFAULT '0';

-- graveyards
ALTER TABLE `graveyards` CHANGE `o` `orientation` float NOT NULL DEFAULT '0';
ALTER TABLE `graveyards` CHANGE `z` `position_z` float NOT NULL DEFAULT '0';
ALTER TABLE `graveyards` CHANGE `y` `position_y` float NOT NULL DEFAULT '0';
ALTER TABLE `graveyards` CHANGE `x` `position_x` float NOT NULL DEFAULT '0';
ALTER TABLE `graveyards` CHANGE `factionid` `faction` INT(10) UNSIGNED NOT NULL DEFAULT '0';

-- npc_gossip_textid
ALTER TABLE `npc_gossip_textid` CHANGE `entryid` `creatureid` INT(10) UNSIGNED NOT NULL DEFAULT '0';

-- quests
ALTER TABLE `quests` CHANGE `QuestSort` `sort` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quests` CHANGE `QuestFlags` `flags` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quests` CHANGE `MaxLevel` `questlevel` INT(10) UNSIGNED DEFAULT '0' NOT NULL;

-- recall
ALTER TABLE `recall` CHANGE `locname` `name` varchar(100) NOT NULL;

-- teleport_coords
ALTER TABLE `teleport_coords` CHANGE `x` `position_x` float NOT NULL DEFAULT '0';
ALTER TABLE `teleport_coords` CHANGE `y` `position_y` float NOT NULL DEFAULT '0';
ALTER TABLE `teleport_coords` CHANGE `z` `position_z` float NOT NULL DEFAULT '0';

-- totemspells
ALTER TABLE `totemspells` CHANGE `spellId` `spell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `totemspells` CHANGE `spellToCast1` `castspell1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `totemspells` CHANGE `spellToCast2` `castspell2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `totemspells` CHANGE `spellToCast3` `castspell3` int(10) UNSIGNED NOT NULL DEFAULT '0';

-- vendors
ALTER TABLE `vendors` CHANGE `vendorGuid` `entry` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vendors` CHANGE `itemGuid` `item` INT(10) UNSIGNED NOT NULL DEFAULT '0';

-- zoneguards
ALTER TABLE `zoneguards` CHANGE `zoneId` `zone` INT(10) UNSIGNED NOT NULL;
ALTER TABLE `zoneguards` CHANGE `hordeEntry` `horde_entry` INT(10) UNSIGNED NULL;
ALTER TABLE `zoneguards` CHANGE `allianceEntry` `alliance_entry` INT(10) UNSIGNED NULL;


alter table worldmap_info add column viewingDistance float(0) not null default 80.0;

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for itempetfood
-- ----------------------------
CREATE TABLE `itempetfood` (
  `entry` int(11) NOT NULL,
  `food_type` int(11) NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `itempetfood` VALUES ('117', '1');
INSERT INTO `itempetfood` VALUES ('414', '3');
INSERT INTO `itempetfood` VALUES ('422', '3');
INSERT INTO `itempetfood` VALUES ('729', '1');
INSERT INTO `itempetfood` VALUES ('769', '1');
INSERT INTO `itempetfood` VALUES ('787', '2');
INSERT INTO `itempetfood` VALUES ('1015', '1');
INSERT INTO `itempetfood` VALUES ('1017', '1');
INSERT INTO `itempetfood` VALUES ('1080', '1');
INSERT INTO `itempetfood` VALUES ('1081', '1');
INSERT INTO `itempetfood` VALUES ('1113', '4');
INSERT INTO `itempetfood` VALUES ('1114', '4');
INSERT INTO `itempetfood` VALUES ('1487', '4');
INSERT INTO `itempetfood` VALUES ('1707', '3');
INSERT INTO `itempetfood` VALUES ('2070', '3');
INSERT INTO `itempetfood` VALUES ('2287', '1');
INSERT INTO `itempetfood` VALUES ('2672', '1');
INSERT INTO `itempetfood` VALUES ('2673', '1');
INSERT INTO `itempetfood` VALUES ('2675', '2');
INSERT INTO `itempetfood` VALUES ('2677', '1');
INSERT INTO `itempetfood` VALUES ('2679', '1');
INSERT INTO `itempetfood` VALUES ('2680', '1');
INSERT INTO `itempetfood` VALUES ('2681', '1');
INSERT INTO `itempetfood` VALUES ('2682', '2');
INSERT INTO `itempetfood` VALUES ('2683', '4');
INSERT INTO `itempetfood` VALUES ('2684', '1');
INSERT INTO `itempetfood` VALUES ('2685', '1');
INSERT INTO `itempetfood` VALUES ('2687', '1');
INSERT INTO `itempetfood` VALUES ('2886', '1');
INSERT INTO `itempetfood` VALUES ('2888', '1');
INSERT INTO `itempetfood` VALUES ('2924', '1');
INSERT INTO `itempetfood` VALUES ('3173', '1');
INSERT INTO `itempetfood` VALUES ('3220', '1');
INSERT INTO `itempetfood` VALUES ('3404', '1');
INSERT INTO `itempetfood` VALUES ('3448', '5');
INSERT INTO `itempetfood` VALUES ('3662', '1');
INSERT INTO `itempetfood` VALUES ('3665', '3');
INSERT INTO `itempetfood` VALUES ('3666', '4');
INSERT INTO `itempetfood` VALUES ('3667', '1');
INSERT INTO `itempetfood` VALUES ('3712', '1');
INSERT INTO `itempetfood` VALUES ('3726', '1');
INSERT INTO `itempetfood` VALUES ('3727', '1');
INSERT INTO `itempetfood` VALUES ('3728', '1');
INSERT INTO `itempetfood` VALUES ('3729', '1');
INSERT INTO `itempetfood` VALUES ('3730', '1');
INSERT INTO `itempetfood` VALUES ('3731', '1');
INSERT INTO `itempetfood` VALUES ('3770', '1');
INSERT INTO `itempetfood` VALUES ('3771', '1');
INSERT INTO `itempetfood` VALUES ('3927', '3');
INSERT INTO `itempetfood` VALUES ('4457', '1');
INSERT INTO `itempetfood` VALUES ('4536', '6');
INSERT INTO `itempetfood` VALUES ('4537', '6');
INSERT INTO `itempetfood` VALUES ('4538', '6');
INSERT INTO `itempetfood` VALUES ('4539', '6');
INSERT INTO `itempetfood` VALUES ('4540', '4');
INSERT INTO `itempetfood` VALUES ('4541', '4');
INSERT INTO `itempetfood` VALUES ('4542', '4');
INSERT INTO `itempetfood` VALUES ('4544', '4');
INSERT INTO `itempetfood` VALUES ('4592', '2');
INSERT INTO `itempetfood` VALUES ('4593', '2');
INSERT INTO `itempetfood` VALUES ('4594', '2');
INSERT INTO `itempetfood` VALUES ('4599', '1');
INSERT INTO `itempetfood` VALUES ('4601', '4');
INSERT INTO `itempetfood` VALUES ('4602', '6');
INSERT INTO `itempetfood` VALUES ('4603', '2');
INSERT INTO `itempetfood` VALUES ('4604', '5');
INSERT INTO `itempetfood` VALUES ('4605', '5');
INSERT INTO `itempetfood` VALUES ('4606', '5');
INSERT INTO `itempetfood` VALUES ('4607', '5');
INSERT INTO `itempetfood` VALUES ('4608', '5');
INSERT INTO `itempetfood` VALUES ('4655', '2');
INSERT INTO `itempetfood` VALUES ('4656', '6');
INSERT INTO `itempetfood` VALUES ('4739', '1');
INSERT INTO `itempetfood` VALUES ('5051', '1');
INSERT INTO `itempetfood` VALUES ('5095', '2');
INSERT INTO `itempetfood` VALUES ('5349', '4');
INSERT INTO `itempetfood` VALUES ('5465', '1');
INSERT INTO `itempetfood` VALUES ('5467', '1');
INSERT INTO `itempetfood` VALUES ('5468', '2');
INSERT INTO `itempetfood` VALUES ('5469', '1');
INSERT INTO `itempetfood` VALUES ('5470', '1');
INSERT INTO `itempetfood` VALUES ('5471', '1');
INSERT INTO `itempetfood` VALUES ('5472', '1');
INSERT INTO `itempetfood` VALUES ('5474', '1');
INSERT INTO `itempetfood` VALUES ('5476', '2');
INSERT INTO `itempetfood` VALUES ('5477', '1');
INSERT INTO `itempetfood` VALUES ('5478', '1');
INSERT INTO `itempetfood` VALUES ('5479', '1');
INSERT INTO `itempetfood` VALUES ('5480', '1');
INSERT INTO `itempetfood` VALUES ('5503', '2');
INSERT INTO `itempetfood` VALUES ('5504', '2');
INSERT INTO `itempetfood` VALUES ('5527', '2');
INSERT INTO `itempetfood` VALUES ('6038', '2');
INSERT INTO `itempetfood` VALUES ('6289', '2');
INSERT INTO `itempetfood` VALUES ('6290', '2');
INSERT INTO `itempetfood` VALUES ('6291', '2');
INSERT INTO `itempetfood` VALUES ('6303', '2');
INSERT INTO `itempetfood` VALUES ('6308', '2');
INSERT INTO `itempetfood` VALUES ('6316', '2');
INSERT INTO `itempetfood` VALUES ('6317', '2');
INSERT INTO `itempetfood` VALUES ('6361', '2');
INSERT INTO `itempetfood` VALUES ('6362', '2');
INSERT INTO `itempetfood` VALUES ('6887', '2');
INSERT INTO `itempetfood` VALUES ('6890', '1');
INSERT INTO `itempetfood` VALUES ('7097', '1');
INSERT INTO `itempetfood` VALUES ('7974', '2');
INSERT INTO `itempetfood` VALUES ('8075', '4');
INSERT INTO `itempetfood` VALUES ('8076', '4');
INSERT INTO `itempetfood` VALUES ('8364', '2');
INSERT INTO `itempetfood` VALUES ('8365', '2');
INSERT INTO `itempetfood` VALUES ('8932', '3');
INSERT INTO `itempetfood` VALUES ('8948', '5');
INSERT INTO `itempetfood` VALUES ('8950', '4');
INSERT INTO `itempetfood` VALUES ('8952', '1');
INSERT INTO `itempetfood` VALUES ('8953', '6');
INSERT INTO `itempetfood` VALUES ('8957', '2');
INSERT INTO `itempetfood` VALUES ('8959', '2');
INSERT INTO `itempetfood` VALUES ('9681', '1');
INSERT INTO `itempetfood` VALUES ('11950', '6');
INSERT INTO `itempetfood` VALUES ('12037', '1');
INSERT INTO `itempetfood` VALUES ('12184', '1');
INSERT INTO `itempetfood` VALUES ('12202', '1');
INSERT INTO `itempetfood` VALUES ('12203', '1');
INSERT INTO `itempetfood` VALUES ('12204', '1');
INSERT INTO `itempetfood` VALUES ('12205', '1');
INSERT INTO `itempetfood` VALUES ('12206', '2');
INSERT INTO `itempetfood` VALUES ('12208', '1');
INSERT INTO `itempetfood` VALUES ('12209', '1');
INSERT INTO `itempetfood` VALUES ('12210', '1');
INSERT INTO `itempetfood` VALUES ('12212', '1');
INSERT INTO `itempetfood` VALUES ('12213', '1');
INSERT INTO `itempetfood` VALUES ('12216', '2');
INSERT INTO `itempetfood` VALUES ('12217', '1');
INSERT INTO `itempetfood` VALUES ('12218', '3');
INSERT INTO `itempetfood` VALUES ('12223', '1');
INSERT INTO `itempetfood` VALUES ('12224', '1');
INSERT INTO `itempetfood` VALUES ('12238', '2');
INSERT INTO `itempetfood` VALUES ('13546', '2');
INSERT INTO `itempetfood` VALUES ('13754', '2');
INSERT INTO `itempetfood` VALUES ('13755', '2');
INSERT INTO `itempetfood` VALUES ('13756', '2');
INSERT INTO `itempetfood` VALUES ('13758', '2');
INSERT INTO `itempetfood` VALUES ('13759', '2');
INSERT INTO `itempetfood` VALUES ('13760', '2');
INSERT INTO `itempetfood` VALUES ('13851', '1');
INSERT INTO `itempetfood` VALUES ('13888', '2');
INSERT INTO `itempetfood` VALUES ('13889', '2');
INSERT INTO `itempetfood` VALUES ('13893', '2');
INSERT INTO `itempetfood` VALUES ('13927', '2');
INSERT INTO `itempetfood` VALUES ('13928', '2');
INSERT INTO `itempetfood` VALUES ('13929', '2');
INSERT INTO `itempetfood` VALUES ('13930', '2');
INSERT INTO `itempetfood` VALUES ('13932', '2');
INSERT INTO `itempetfood` VALUES ('13933', '2');
INSERT INTO `itempetfood` VALUES ('13935', '2');
INSERT INTO `itempetfood` VALUES ('15924', '2');
INSERT INTO `itempetfood` VALUES ('16168', '6');
INSERT INTO `itempetfood` VALUES ('16169', '4');
INSERT INTO `itempetfood` VALUES ('16766', '2');
INSERT INTO `itempetfood` VALUES ('16971', '2');
INSERT INTO `itempetfood` VALUES ('17119', '1');
INSERT INTO `itempetfood` VALUES ('17197', '4');
INSERT INTO `itempetfood` VALUES ('17222', '1');
INSERT INTO `itempetfood` VALUES ('17406', '3');
INSERT INTO `itempetfood` VALUES ('18045', '1');
INSERT INTO `itempetfood` VALUES ('19223', '1');
INSERT INTO `itempetfood` VALUES ('19304', '1');
INSERT INTO `itempetfood` VALUES ('19305', '1');
INSERT INTO `itempetfood` VALUES ('19306', '1');
INSERT INTO `itempetfood` VALUES ('19696', '4');
INSERT INTO `itempetfood` VALUES ('19994', '6');
INSERT INTO `itempetfood` VALUES ('19995', '1');
INSERT INTO `itempetfood` VALUES ('19996', '2');
INSERT INTO `itempetfood` VALUES ('20074', '1');
INSERT INTO `itempetfood` VALUES ('20424', '1');
INSERT INTO `itempetfood` VALUES ('21023', '1');
INSERT INTO `itempetfood` VALUES ('21030', '6');
INSERT INTO `itempetfood` VALUES ('21031', '6');
INSERT INTO `itempetfood` VALUES ('21033', '6');
INSERT INTO `itempetfood` VALUES ('21071', '2');
INSERT INTO `itempetfood` VALUES ('21153', '2');
INSERT INTO `itempetfood` VALUES ('21235', '1');
INSERT INTO `itempetfood` VALUES ('21254', '4');
INSERT INTO `itempetfood` VALUES ('21552', '2');
INSERT INTO `itempetfood` VALUES ('22019', '4');
INSERT INTO `itempetfood` VALUES ('22644', '1');
INSERT INTO `itempetfood` VALUES ('22645', '1');
INSERT INTO `itempetfood` VALUES ('22895', '4');
INSERT INTO `itempetfood` VALUES ('23160', '4');
INSERT INTO `itempetfood` VALUES ('23495', '1');
INSERT INTO `itempetfood` VALUES ('23676', '1');
INSERT INTO `itempetfood` VALUES ('24105', '1');
INSERT INTO `itempetfood` VALUES ('24477', '2');
INSERT INTO `itempetfood` VALUES ('24539', '5');
INSERT INTO `itempetfood` VALUES ('27422', '2');
INSERT INTO `itempetfood` VALUES ('27425', '2');
INSERT INTO `itempetfood` VALUES ('27429', '2');
INSERT INTO `itempetfood` VALUES ('27435', '2');
INSERT INTO `itempetfood` VALUES ('27437', '2');
INSERT INTO `itempetfood` VALUES ('27438', '2');
INSERT INTO `itempetfood` VALUES ('27439', '2');
INSERT INTO `itempetfood` VALUES ('27635', '1');
INSERT INTO `itempetfood` VALUES ('27636', '1');
INSERT INTO `itempetfood` VALUES ('27651', '1');
INSERT INTO `itempetfood` VALUES ('27657', '1');
INSERT INTO `itempetfood` VALUES ('27658', '1');
INSERT INTO `itempetfood` VALUES ('27659', '1');
INSERT INTO `itempetfood` VALUES ('27660', '1');
INSERT INTO `itempetfood` VALUES ('27661', '2');
INSERT INTO `itempetfood` VALUES ('27662', '2');
INSERT INTO `itempetfood` VALUES ('27663', '2');
INSERT INTO `itempetfood` VALUES ('27664', '2');
INSERT INTO `itempetfood` VALUES ('27665', '2');
INSERT INTO `itempetfood` VALUES ('27667', '2');
INSERT INTO `itempetfood` VALUES ('27668', '1');
INSERT INTO `itempetfood` VALUES ('27669', '1');
INSERT INTO `itempetfood` VALUES ('27671', '1');
INSERT INTO `itempetfood` VALUES ('27674', '1');
INSERT INTO `itempetfood` VALUES ('27677', '1');
INSERT INTO `itempetfood` VALUES ('27678', '1');
INSERT INTO `itempetfood` VALUES ('27681', '1');
INSERT INTO `itempetfood` VALUES ('27682', '1');
INSERT INTO `itempetfood` VALUES ('27854', '1');
INSERT INTO `itempetfood` VALUES ('27855', '4');
INSERT INTO `itempetfood` VALUES ('27856', '6');
INSERT INTO `itempetfood` VALUES ('27857', '3');
INSERT INTO `itempetfood` VALUES ('27858', '2');
INSERT INTO `itempetfood` VALUES ('27859', '5');
INSERT INTO `itempetfood` VALUES ('28112', '5');
INSERT INTO `itempetfood` VALUES ('28486', '4');
INSERT INTO `itempetfood` VALUES ('29292', '1');
INSERT INTO `itempetfood` VALUES ('29393', '6');
INSERT INTO `itempetfood` VALUES ('29394', '4');
INSERT INTO `itempetfood` VALUES ('29448', '3');
INSERT INTO `itempetfood` VALUES ('29449', '4');
INSERT INTO `itempetfood` VALUES ('29450', '6');
INSERT INTO `itempetfood` VALUES ('29451', '1');
INSERT INTO `itempetfood` VALUES ('29452', '2');
INSERT INTO `itempetfood` VALUES ('30155', '2');
INSERT INTO `itempetfood` VALUES ('30458', '3');
INSERT INTO `itempetfood` VALUES ('30817', '4');