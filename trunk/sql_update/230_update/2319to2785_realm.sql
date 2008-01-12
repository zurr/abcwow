CREATE TABLE `characters_insert_queue` (
  `insert_temp_guid` int(10) unsigned NOT NULL default '0',
  `acct` int(10) unsigned NOT NULL,
  `name` varchar(21) collate utf8_unicode_ci NOT NULL,
  `race` tinyint(3) unsigned NOT NULL,
  `class` tinyint(3) unsigned NOT NULL,
  `gender` tinyint(3) unsigned NOT NULL,
  `custom_faction` int(10) unsigned NOT NULL default '0',
  `level` tinyint(3) unsigned NOT NULL,
  `xp` int(10) unsigned NOT NULL,
  `exploration_data` longtext collate utf8_unicode_ci NOT NULL,
  `skills` longtext collate utf8_unicode_ci NOT NULL,
  `watched_faction_index` int(10) unsigned NOT NULL default '0',
  `selected_pvp_title` int(10) unsigned NOT NULL default '0',
  `available_pvp_titles` int(10) unsigned NOT NULL default '0',
  `gold` int(10) unsigned NOT NULL default '0',
  `ammo_id` int(10) unsigned NOT NULL default '0',
  `available_prof_points` tinyint(3) unsigned NOT NULL default '0',
  `available_talent_points` tinyint(3) unsigned NOT NULL default '0',
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
  `banReason` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `timestamp` int(10) unsigned NOT NULL default '0',
  `online` tinyint(3) unsigned NOT NULL default '0',
  `bindpositionX` float NOT NULL default '0',
  `bindpositionY` float NOT NULL default '0',
  `bindpositionZ` float NOT NULL default '0',
  `bindmapId` int(10) unsigned NOT NULL default '0',
  `bindzoneId` int(10) unsigned NOT NULL default '0',
  `isResting` tinyint(3) unsigned NOT NULL default '0',
  `restState` tinyint(3) unsigned NOT NULL default '0',
  `restTime` int(10) unsigned NOT NULL default '0',
  `playedtime` longtext collate utf8_unicode_ci NOT NULL,
  `deathstate` tinyint(3) unsigned NOT NULL default '0',
  `TalentResetTimes` int(10) unsigned NOT NULL default '0',
  `first_login` tinyint(3) unsigned NOT NULL default '0',
  `forced_rename_pending` tinyint(3) unsigned NOT NULL default '0',
  `arenaPoints` int(10) unsigned NOT NULL default '0',
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
  `honorPointsToAdd` int(11) NOT NULL,
  `killsToday` int(10) unsigned NOT NULL default '0',
  `killsYesterday` int(10) unsigned NOT NULL default '0',
  `killsLifeTime` int(10) unsigned NOT NULL default '0',
  `honorToday` int(10) unsigned NOT NULL default '0',
  `honorYesterday` int(10) unsigned NOT NULL default '0',
  `honorPoints` int(10) unsigned NOT NULL default '0',
  `difficulty` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`insert_temp_guid`),
  KEY `acct` (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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


DROP TABLE IF EXISTS `guild_checkpoints`;
CREATE TABLE `guild_checkpoints` (
  `guildid` int(30) NOT NULL,
  `checkid` int(30) NOT NULL,
  PRIMARY KEY  (`guildid`,`checkid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

ALTER TABLE `playerpets` CHANGE `entryId` `entry` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `playerpets` CHANGE `data` `fields` longtext COLLATE utf8_unicode_ci NOT NULL;
ALTER TABLE `playerpets` DROP COLUMN `autocastspell`;

alter table groups add column difficulty int(30) not null default 0 after loot_threshold;
alter table groups add column assistant_leader int(30) not null default 0 after difficulty;
alter table groups add column main_tank int(30) not null default 0 after assistant_leader;
alter table groups add column main_assist int(30) not null default 0 after main_tank;

DROP TABLE IF EXISTS `account_data`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `account_data` (
  `acct` int(30) NOT NULL,
  `uiconfig0` blob,
  `uiconfig1` blob,
  `uiconfig2` blob,
  `uiconfig3` blob,
  `uiconfig4` blob,
  `uiconfig5` blob,
  `uiconfig6` blob,
  `uiconfig7` blob,
  `uiconfig8` blob,
  PRIMARY KEY (`acct`),
  UNIQUE KEY `a` (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `account_forced_permissions`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `account_forced_permissions` (
  `login` varchar(50) NOT NULL,
  `permissions` varchar(100) NOT NULL,
  PRIMARY KEY (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

alter table accounts drop INDEX `login`;
alter table accounts type=InnoDB;
alter table ipbans type=InnoDB;
ALTER TABLE `accounts` ADD UNIQUE `a`(`login`);
ALTER TABLE `ipbans` ADD UNIQUE `a`(`ip`);

alter table account_data drop index a;
alter table auctions drop index auctionHouse;
alter table characters drop index acct;
alter table characters_insert_queue drop index guid;
alter table characters_insert_queue drop index acct;
alter table characters_insert_queue drop index guildid;
alter table charters drop index leaderGuid;
alter table guild_bankitems drop index a;
alter table guild_bankitems drop index b;
alter table guild_bankitems drop index c;
alter table guild_banklogs drop index a;
alter table guild_banklogs drop index b;
alter table guild_banktabs drop index a;
alter table guild_banktabs drop index b;
alter table guilds drop index guildId;
alter table playeritems drop index ownerguid;
alter table playeritems_insert_queue drop index ownerguid;


alter table account_data type=InnoDB;
alter table account_forced_permissions type=InnoDB;
alter table arenateams type=InnoDB;
alter table auctions type=InnoDB;
alter table characters type=InnoDB;
alter table characters_insert_queue type=InnoDB;
alter table charters type=InnoDB;
alter table corpses type=InnoDB;
alter table gm_tickets type=InnoDB;
alter table groups type=InnoDB;
alter table guild_bankitems type=InnoDB;
alter table guild_banklogs type=InnoDB;
alter table guild_banktabs type=InnoDB;
alter table guild_checkpoints type=InnoDB;
alter table guild_data type=InnoDB;
alter table guild_logs type=InnoDB;
alter table guild_ranks type=InnoDB;
alter table guilds type=InnoDB;
alter table instances type=InnoDB;
alter table mailbox type=InnoDB;
alter table playercooldownitems type=InnoDB;
alter table playercooldownsecurity type=InnoDB;
alter table playeritems type=InnoDB;
alter table playeritems_insert_queue type=InnoDB;
alter table playerpets type=InnoDB;
alter table playerpetspells type=InnoDB;
alter table playersummonspells type=InnoDB;
alter table questlog type=InnoDB;
alter table server_settings type=InnoDB;
alter table social type=InnoDB;
alter table tutorials type=InnoDB;

ALTER TABLE `account_data` ADD UNIQUE `a`(`acct`);
ALTER TABLE `account_forced_permissions` ADD UNIQUE `a`(`login`);
ALTER TABLE `arenateams` ADD UNIQUE `a`(`id`);
ALTER TABLE `auctions` ADD UNIQUE `a`(`auctionId`), ADD INDEX `b`(`auctionHouse`);
ALTER TABLE `characters` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`acct`), ADD INDEX `c`(`name`), ADD INDEX `d`(`banned`, `online`);
ALTER TABLE `charters` ADD UNIQUE `a`(`charterId`);
ALTER TABLE `corpses` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`mapId`, `instanceId`);

ALTER TABLE `gm_tickets` ADD UNIQUE `a`(`guid`);
ALTER TABLE `groups` ADD UNIQUE `a`(`group_id`);
ALTER TABLE `guild_bankitems` ADD INDEX `a`(`guildId`, `tabId`, `slotId`);
ALTER TABLE `guild_banklogs` ADD INDEX `a`(`log_id`, `guildid`);
ALTER TABLE `guild_banktabs` ADD INDEX `a`(`guildId`, `tabId`);
ALTER TABLE `guild_data` ADD INDEX `a`(`guildid`, `playerid`);
ALTER TABLE `guild_logs` ADD INDEX `a`(`log_id`, `guildid`);
ALTER TABLE `guild_ranks` ADD INDEX `a`(`guildId`, `rankId`);
ALTER TABLE `guilds` ADD UNIQUE `a`(`guildId`);
ALTER TABLE `instances` ADD UNIQUE `a`(`id`), ADD INDEX `b`(`mapid`), ADD INDEX `c`(`expiration`);
ALTER TABLE `mailbox` ADD UNIQUE `a`(`message_id`), ADD INDEX `b`(`player_guid`);
ALTER TABLE `playercooldownitems` ADD INDEX `a`(`OwnerGuid`), ADD INDEX `b`(`ItemEntry`, `CooldownTimeStamp`, `Cooldown`);
ALTER TABLE `playercooldownsecurity` ADD INDEX `a`(`OwnerGuid`, `SpellID`, `TimeStamp`);
ALTER TABLE `playeritems` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`ownerguid`), ADD INDEX `c`(`itemtext`);
ALTER TABLE `playerpets` ADD INDEX `a`(`ownerguid`, `petnumber`);
ALTER TABLE `playerpetspells` ADD INDEX `a`(`ownerguid`, `petnumber`);
ALTER TABLE `playersummonspells` ADD INDEX `a`(`ownerguid`, `entryid`);
ALTER TABLE `questlog` ADD INDEX `a`(`index`, `player_guid`, `quest_id`, `slot`);
ALTER TABLE `server_settings` ADD UNIQUE `a`(`setting_id`);
ALTER TABLE `social` ADD INDEX `a`(`guid`, `socialguid`, `flags`);
ALTER TABLE `tutorials` ADD UNIQUE `a`(`playerId`);

alter table account_data drop index a;
alter table account_forced_permissions drop index a;
alter table arenateams drop index a;
alter table auctions drop index a;
alter table auctions drop index b;
alter table characters drop index a;
alter table characters drop index b;
alter table characters drop index c;
alter table characters drop index d;
alter table charters drop index a;
alter table corpses drop index a;
alter table corpses drop index b;
alter table gm_tickets drop index a;
alter table groups drop index a;
alter table guild_bankitems drop index a;
alter table guild_banklogs drop index a;
alter table guild_banktabs drop index a;
alter table guild_data drop index a;
alter table guild_logs drop index a;
alter table guild_ranks drop index a;
alter table guilds drop index a;
alter table instances drop index a;
alter table instances drop index b;
alter table instances drop index c;
alter table mailbox drop index a;
alter table mailbox drop index b;
alter table playercooldownitems drop index a;
alter table playercooldownitems drop index b;
alter table playercooldownsecurity drop index a;
alter table playeritems drop index a;
alter table playeritems drop index b;
alter table playeritems drop index c;
alter table playeritems_external drop index a;
alter table playeritems_external drop index b;
alter table playerpets drop index a;
alter table playerpetspells drop index a;
alter table playersummonspells drop index a;
alter table questlog drop index a;
alter table server_settings drop index a;
alter table social drop index a;
alter table tutorials drop index a;

ALTER TABLE `account_data` ADD UNIQUE `a`(`acct`);
ALTER TABLE `account_forced_permissions` ADD UNIQUE `a`(`login`);
ALTER TABLE `arenateams` ADD UNIQUE `a`(`id`);
ALTER TABLE `auctions` ADD UNIQUE `a`(`auctionId`), ADD INDEX `b`(`auctionHouse`);
ALTER TABLE `characters` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`acct`), ADD INDEX `c`(`name`), ADD INDEX `d`(`banned`), ADD INDEX `e`(`online`);
ALTER TABLE `charters` ADD UNIQUE `a`(`charterId`);
ALTER TABLE `corpses` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`mapId`), ADD INDEX `c`(`instanceId`);
ALTER TABLE `gm_tickets` ADD UNIQUE `a`(`guid`);
ALTER TABLE `groups` ADD UNIQUE `a`(`group_id`);
ALTER TABLE `guild_bankitems` ADD INDEX `a`(`guildId`),  ADD INDEX `b`(`tabId`),  ADD INDEX `c`(`slotId`);
ALTER TABLE `guild_banklogs` ADD INDEX `a`(`log_id`),  ADD INDEX `b`(`guildid`);
ALTER TABLE `guild_banktabs` ADD INDEX `a`(`guildId`),  ADD INDEX `b`(`tabId`);
ALTER TABLE `guild_data` ADD INDEX `a`(`guildid`),  ADD INDEX `b`(`playerid`);
ALTER TABLE `guild_logs` ADD INDEX `a`(`log_id`),  ADD INDEX `b`(`guildid`);
ALTER TABLE `guild_ranks` ADD INDEX `a`(`guildId`),  ADD INDEX `b`(`rankId`);
ALTER TABLE `guilds` ADD UNIQUE `a`(`guildId`);
ALTER TABLE `instances` ADD UNIQUE `a`(`id`), ADD INDEX `b`(`mapid`), ADD INDEX `c`(`expiration`);
ALTER TABLE `mailbox` ADD UNIQUE `a`(`message_id`), ADD INDEX `b`(`player_guid`);
ALTER TABLE `playercooldownitems` ADD INDEX `a`(`OwnerGuid`), ADD INDEX `b`(`ItemEntry`),  ADD INDEX `c`(`CooldownTimeStamp`),  ADD INDEX `d`(`Cooldown`);
ALTER TABLE `playercooldownsecurity` ADD INDEX `a`(`OwnerGuid`),  ADD INDEX `b`(`SpellID`),  ADD INDEX `c`(`TimeStamp`);
ALTER TABLE `playeritems` ADD UNIQUE `a`(`guid`), ADD INDEX `b`(`ownerguid`), ADD INDEX `c`(`itemtext`);
ALTER TABLE `playeritems_external` ADD INDEX `a`(`ownerguid`), ADD UNIQUE `b`(`guid`);
ALTER TABLE `playerpets` ADD INDEX `a`(`ownerguid`),  ADD INDEX `b`(`petnumber`);
ALTER TABLE `playerpetspells` ADD INDEX `a`(`ownerguid`),  ADD INDEX `b`(`petnumber`);
ALTER TABLE `playersummonspells` ADD INDEX `a`(`ownerguid`),  ADD INDEX `b`(`entryid`);
ALTER TABLE `questlog` ADD INDEX `a`(`index`), ADD INDEX `b`(`player_guid`), ADD INDEX `c`(`quest_id`), ADD INDEX `d`(`slot`);
ALTER TABLE `server_settings` ADD UNIQUE `a`(`setting_id`);
ALTER TABLE `social` ADD INDEX `a`(`guid`),  ADD INDEX `b`(`socialguid`),  ADD INDEX `c`(`flags`);
ALTER TABLE `tutorials` ADD UNIQUE `a`(`playerId`);

alter table mailbox drop column external_attached_item_guid;
alter table mailbox change column attached_item_guid attached_item_guids varchar(200) not null default "";


CREATE TABLE `mailbox_insert_queue` (
  `sender_guid` int(30) NOT NULL,
  `receiver_guid` int(30) NOT NULL,
  `subject` varchar(200) NOT NULL,
  `body` varchar(500) NOT NULL,
  `stationary` int(30) NOT NULL,
  `money` int(30) NOT NULL,
  `item_id` int(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;