alter table items add column ArenaRankRequirement int(30) not null default 0 after ItemExtendedCost;

alter table creature_names add column mouseFlags int(30) not null default 0 after subname;

DROP TABLE IF EXISTS `map_checkpoint`;
CREATE TABLE `map_checkpoint` (
  `entry` int(30) NOT NULL,
  `prereq_checkpoint_id` int(30) NOT NULL,
  `creature_id` int(30) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

alter table worldmap_info add column required_checkpoint int(30) not null default 0;

ALTER TABLE creature_names CHANGE mouseFlags info_str VARCHAR(500) NOT NULL;
UPDATE creature_names set info_str=""

CREATE TABLE `transport_creatures` (
  `transport_entry` int(10) unsigned NOT NULL,
  `creature_entry` int(10) unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

alter table vendors add column `max_amount` INTEGER(11) not null default 0;
alter table vendors add column `inctime` BIGINT(20) not null default 0;

DROP TABLE IF EXISTS `wordfilter_character_names`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `wordfilter_character_names` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `wordfilter_chat`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `wordfilter_chat` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

LOCK TABLES `creature_names_localized` WRITE;
ALTER TABLE `creature_names_localized` collate utf8_unicode_ci;
UNLOCK TABLES;

LOCK TABLES `gameobject_names_localized` WRITE;
ALTER TABLE `gameobject_names_localized` collate utf8_unicode_ci;
UNLOCK TABLES;

LOCK TABLES `itempages_localized` WRITE;
ALTER TABLE `itempages_localized` collate utf8_unicode_ci;
UNLOCK TABLES;

LOCK TABLES `npc_text_localized` WRITE;
ALTER TABLE `npc_text_localized` collate utf8_unicode_ci;
UNLOCK TABLES;

LOCK TABLES `quests_localized` WRITE;
ALTER TABLE `quests_localized` collate utf8_unicode_ci;
UNLOCK TABLES;