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
  PRIMARY KEY  (`account_id`)
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
