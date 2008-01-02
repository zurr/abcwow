alter table guild_ranks add column rankRights_1 int(30) not null default 0;
alter table guild_ranks add column rankRights_2 int(30) not null default 0;
alter table guild_ranks add column rankRights_3 int(30) not null default 0;
alter table guild_ranks add column rankRights_4 int(30) not null default 0;
alter table guild_ranks add column rankRights_5 int(30) not null default 0;
alter table guild_ranks add column rankRights_6 int(30) not null default 0;
alter table guild_ranks add column rankRights_7 int(30) not null default 0;
alter table guild_ranks add column rankRights_8 int(30) not null default 0;
alter table guild_ranks add column rankRights_9 int(30) not null default 0;
alter table guild_ranks add column rankRights_10 int(30) not null default 0;
alter table guild_ranks add column rankRights_11 int(30) not null default 0;
alter table guild_ranks add column rankRights_12 int(30) not null default 0;
alter table guild_ranks add column rankRights_13 int(30) not null default 0;


DROP TABLE IF EXISTS `guild_checkpoints`;
CREATE TABLE `guild_checkpoints` (
  `guildid` int(30) NOT NULL,
  `checkid` int(30) NOT NULL,
  PRIMARY KEY  (`guildid`,`checkid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

alter table characters drop column guildid;
alter table characters drop column guildRank;
alter table characters drop column publicNote;
alter table characters drop column officerNote;
alter table guilds modify column createdate int(30) not null default 0;

CREATE TABLE `guild_logs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  `event_type` int(30) NOT NULL,
  `misc1` int(30) NOT NULL,
  `misc2` int(30) NOT NULL,
  `misc3` int(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `guild_data` (
  `guildid` int(30) NOT NULL,
  `playerid` int(30) NOT NULL,
  `guildRank` int(30) NOT NULL,
  `publicNote` varchar(300) NOT NULL,
  `officerNote` varchar(300) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

ALTER TABLE `playerpets` CHANGE `entryId` `entry` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `playerpets` CHANGE `data` `fields` longtext COLLATE utf8_unicode_ci NOT NULL;
ALTER TABLE `playerpets` DROP COLUMN `autocastspell`;

DROP TABLE IF EXISTS `guild_bankitems`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_bankitems` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `slotId` int(30) NOT NULL,
  `itemGuid` int(30) NOT NULL,
  PRIMARY KEY (`guildId`,`tabId`,`slotId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`),
  KEY `c` (`slotId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `guild_banklogs`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_banklogs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `tabid` int(30) NOT NULL COMMENT 'tab 6 is money logs',
  `action` int(5) NOT NULL,
  `player_guid` int(30) NOT NULL,
  `item_entry` int(30) NOT NULL,
  `stack_count` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  PRIMARY KEY (`log_id`,`guildid`),
  KEY `a` (`guildid`),
  KEY `b` (`tabid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `guild_banktabs`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_banktabs` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `tabName` varchar(200) NOT NULL,
  `tabIcon` varchar(200) NOT NULL,
  PRIMARY KEY (`guildId`,`tabId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `guild_data`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_data` (
  `guildid` int(30) NOT NULL,
  `playerid` int(30) NOT NULL,
  `guildRank` int(30) NOT NULL,
  `publicNote` varchar(300) NOT NULL,
  `officerNote` varchar(300) NOT NULL,
  `lastWithdrawReset` int(30) NOT NULL DEFAULT '0',
  `withdrawlsSinceLastReset` int(30) NOT NULL DEFAULT '0',
  `lastItemWithdrawReset0` int(30) NOT NULL DEFAULT '0',
  `itemWithdrawlsSinceLastReset0` int(30) NOT NULL DEFAULT '0',
  `lastItemWithdrawReset1` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset1` int(30) NOT NULL,
  `lastItemWithdrawReset2` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset2` int(30) NOT NULL,
  `lastItemWithdrawReset3` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset3` int(30) NOT NULL,
  `lastItemWithdrawReset4` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset4` int(30) NOT NULL,
  `lastItemWithdrawReset5` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset5` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `guild_logs`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_logs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  `event_type` int(30) NOT NULL,
  `misc1` int(30) NOT NULL,
  `misc2` int(30) NOT NULL,
  `misc3` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

DROP TABLE IF EXISTS `guild_ranks`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guild_ranks` (
  `guildId` int(6) unsigned NOT NULL DEFAULT '0',
  `rankId` int(1) NOT NULL DEFAULT '0',
  `rankName` varchar(255) NOT NULL DEFAULT '',
  `rankRights` int(3) unsigned NOT NULL DEFAULT '0',
  `goldLimitPerDay` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags0` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay0` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags1` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay1` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags2` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay2` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags3` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay3` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags4` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay4` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags5` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay5` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildId`,`rankId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `guilds`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `guilds` (
  `guildId` bigint(20) NOT NULL AUTO_INCREMENT,
  `guildName` varchar(32) NOT NULL DEFAULT '',
  `leaderGuid` bigint(20) NOT NULL DEFAULT '0',
  `emblemStyle` int(10) NOT NULL DEFAULT '0',
  `emblemColor` int(10) NOT NULL DEFAULT '0',
  `borderStyle` int(10) NOT NULL DEFAULT '0',
  `borderColor` int(10) NOT NULL DEFAULT '0',
  `backgroundColor` int(10) NOT NULL DEFAULT '0',
  `guildInfo` varchar(100) NOT NULL DEFAULT '',
  `motd` varchar(100) NOT NULL DEFAULT '',
  `createdate` int(30) NOT NULL DEFAULT '0',
  `bankTabCount` int(30) NOT NULL DEFAULT '0',
  `bankBalance` int(30) NOT NULL,
  PRIMARY KEY (`guildId`),
  UNIQUE KEY `guildId` (`guildId`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;


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
