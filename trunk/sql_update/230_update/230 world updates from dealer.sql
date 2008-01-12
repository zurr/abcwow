-- One Npc Deleted cause not needed anymore (Spawned by AI now)
DELETE from creature_spawns WHERE entry=18478;

-- Magtheridon Lair Channeler
DELETE from creature_spawns WHERE entry=17256;
update creature_proto set money=-1, mindamage=2000, maxdamage=4000 where entry=17256;

-- Magtheridon Lair Game Objects
DELETE FROM `gameobject_spawns` WHERE `map`='544';
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184635','544','19.0966','-29.2772','0.133036','-1.26399','0','0','-0.590754','0.806852','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184638','544','-60.8202','-21.9269','-0.03026','3.08278','0','0','0.999568','0.029401','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184639','544','-29.7699','-43.4445','-0.522461','-2.37604','0','0','0.927631','-0.373497','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184634','544','17.7522','34.5464','0.144816','0.179666','0','0','0.089712','0.995968','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184653','544','0','0','0','3.14159','0','0','1','-4.37114E-8','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'183847','544','-72.5866','1.559','0','3.14159','0','0','1','-4.37114E-8','0','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184636','544','-30.8852','46.6723','-0.497104','1.23468','0','0','0.578867','0.815422','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184637','544','-60.2491','27.9361','-0.018443','1.94711','0','0','0.826891','0.562362','1','32','1375','1','0');

-- Magtheridon Lair Equip Info
UPDATE `creature_proto` SET `equipmodel1`='21341',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='17256');
UPDATE `creature_proto` SET `equipmodel1`='24503',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='18829');
UPDATE `creature_proto` SET `equipmodel1`='39393',`equipinfo1`='33490434',`equipslot1`='529' WHERE (`entry`='17257');

-- Magtheridon Trigger spawn
INSERT INTO `creature_spawns` (`id`,`entry`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes`,`bytes2`,`emote_state`,`respawnnpclink`,`channel_spell`,`channel_target_sqlid`,`channel_target_sqlid_creature`) VALUES (NULL,'17376','544','8.79722','-19.4801','2.53646','3.07178','12','13069','114','0','16777472','4097','0','0','0','0','0');
INSERT INTO `creature_spawns` (`id`,`entry`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes`,`bytes2`,`emote_state`,`respawnnpclink`,`channel_spell`,`channel_target_sqlid`,`channel_target_sqlid_creature`) VALUES (NULL,'17376','544','9.3589','23.2286','2.34895','3.89208','12','13069','114','0','16777472','4097','0','0','0','0','0');
INSERT INTO `creature_spawns` (`id`,`entry`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes`,`bytes2`,`emote_state`,`respawnnpclink`,`channel_spell`,`channel_target_sqlid`,`channel_target_sqlid_creature`) VALUES (NULL,'17376','544','-31.471','-34.156','2.3351','1.06465','12','13069','114','0','16777472','4097','0','0','0','0','0');
INSERT INTO `creature_spawns` (`id`,`entry`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes`,`bytes2`,`emote_state`,`respawnnpclink`,`channel_spell`,`channel_target_sqlid`,`channel_target_sqlid_creature`) VALUES (NULL,'17376','544','-54.2772','2.34374','2.40456','1.3439','12','13069','114','0','16777472','4097','0','0','0','0','0');
INSERT INTO `creature_spawns` (`id`,`entry`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes`,`bytes2`,`emote_state`,`respawnnpclink`,`channel_spell`,`channel_target_sqlid`,`channel_target_sqlid_creature`) VALUES (NULL,'17376','544','-31.8918','38.4303','2.28647','5.07891','12','13069','114','0','16777472','4097','0','0','0','0','0');

-- Almost Half of GOs Spawns in Blood Furnace
DELETE FROM `gameobject_spawns` WHERE `map`='524';
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES 
	(NULL,'181766','542','-2.96045','-115.525','-29.8004','3.08943','-0.001138','-0.043605','0.998708','0.0260539','1','32','1375','0.707898','0'),
	(NULL,'181925','542','266.452','-104.919','-25.3138','-2.53073','0','0','0.953717','-0.300706','0','33','168','1','0'),
	(NULL,'181823','542','260.408','-125.812','-10.3249','3.14159','0','0','1','-4.37114E-8','1','32','1375','0.672502','0'),
	(NULL,'181922','542','260.167','-46.2649','-25.8508','2.70526','0','0','0.976296','0.21644','0','33','168','1','0'),
	(NULL,'181926','542','324.071','-23.2872','-25.5655','-2.53073','0','0','0.953717','-0.300706','0','33','168','1','0'),
	(NULL,'181924','542','292.173','-143.163','-25.4868','-1.65806','0','0','-0.737278','0.67559','0','33','168','1','0'),
	(NULL,'181921','542','371.722','-47.6372','-25.8508','-2.53073','0','0','0.953717','-0.300706','0','33','168','1','0'),
	(NULL,'181923','542','386.239','-138.305','-25.7111','-0.872664','0','0','-0.422618','0.906308','0','33','168','1','0'),
	(NULL,'181927','542','386.689','-101.665','-25.2719','2.53073','0','0','0.953717','0.300706','0','33','168','1','0'),
	(NULL,'181713','542','368.347','-118.024','-137.451','1.22173','0','0','0.573576','0.819152','1','48','0','1.5','0'),
	(NULL,'181713','542','308.112','-33.208','-137.451','-1.20428','0','0','0.566406','-0.824126','1','48','0','1.5','0'),
	(NULL,'181713','542','308.466','-140.398','-137.302','2.44346','0','0','0.939693','0.34202','1','48','0','1.5','0'),
	(NULL,'181713','542','367.855','-56.3836','-137.359','0.0174533','0','0','0.00872654','0.999962','1','48','0','1.5','0'),
	(NULL,'181713','542','275.211','-87.2234','-137.451','-2.49582','0','0','0.948324','-0.317305','1','48','0','1.5','0'),
	(NULL,'181813','542','365.545','69.3179','11.1708','3.14159','0','0','1','-4.37114E-8','1','32','1375','1.09675','0'),
	(NULL,'181816','542','288.815','68.8318','11.5749','-0.0223213','0.003783','-0.000667','-0.011158','0.99993','1','32','1375','1.01935','0'),
	(NULL,'181814','542','366.847','100.435','11.1708','3.14159','0','0','1','-4.37114E-8','1','32','1375','1.09675','0'),
	(NULL,'181811','542','327.221','34.7391','25.2633','3.14159','0','0','1','-4.37114E-8','0','32','1375','0.903812','0'),
	(NULL,'181815','542','287.979','99.5641','11.1708','-0.0223213','0.003783','-0.000667','-0.011158','0.99993','1','32','1375','1.11797','0'),
	(NULL,'181821','542','417.967','114.674','11.1708','0','0','0','4.37114E-8','1','1','32','1375','1.09675','0'),
	(NULL,'181819','542','456.291','34.1513','23.8317','0','0','0','4.37114E-8','1','1','32','1375','0.903812','0'),
	(NULL,'181818','542','416.665','83.5567','11.1708','0','0','0','4.37114E-8','1','1','32','1375','1.09675','0'),
	(NULL,'181812','542','327.221','149.84','23.8318','3.14159','0','0','1','-4.37114E-8','1','32','1375','0.903812','0'),
	(NULL,'181822','542','455.753','149.726','24.3749','0','0','0','4.37114E-8','1','0','32','1375','0.903812','0'),
	(NULL,'184175','542','-0.626152','25.2928','-45.1701','-1.9627','0','0','-0.831249','0.5559','1','0','0','1.77176','0'),
	(NULL,'181817','542','495.532','84.4274','11.1708','3.11927','0.000667','0.003783','0.99993','0.011158','1','32','1375','1.11797','0'),
	(NULL,'181820','542','494.697','115.16','11.5749','3.11927','0.000667','0.003783','0.99993','0.011158','1','32','1375','1.01935','0');

-- Tempest Keep: The Eye Equip Info
UPDATE `creature_proto` SET `equipmodel1`='36725',`equipinfo1`='33490690',`equipslot1`='269',`equipmodel2`='41653',`equipinfo2`='33490690',`equipslot2`='269' WHERE (`entry`='20031');
UPDATE `creature_proto` SET `equipmodel1`='42556',`equipinfo1`='50267138',`equipslot1`='781',`equipmodel2`='38182',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='20032');
UPDATE `creature_proto` SET `equipmodel1`='44233',`equipinfo1`='33492738',`equipslot1`='781' WHERE (`entry`='20034');
UPDATE `creature_proto` SET `equipmodel1`='44213',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='20033');
UPDATE `creature_proto` SET `equipmodel1`='40655',`equipinfo1`='33490690',`equipslot1`='789',`equipmodel3`='41875',`equipinfo3`='50266626',`equipslot3`='15' WHERE (`entry`='20037');
UPDATE `creature_proto` SET `equipmodel1`='44234',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='44234',`equipinfo2`='33490690',`equipslot2`='781' WHERE (`entry`='20035');
UPDATE `creature_proto` SET `equipmodel1`='23948',`equipinfo1`='50267138',`equipslot1`='781',`equipmodel2`='33307',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='20036');
UPDATE `creature_proto` SET `equipmodel1`='44236',`equipinfo1`='33490690',`equipslot1`='781' WHERE (`entry`='20046');
UPDATE `creature_proto` SET `equipmodel1`='40438',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='20045');
UPDATE `creature_proto` SET `equipmodel1`='35151',`equipinfo1`='33490690',`equipslot1`='269' WHERE (`entry`='19516');
UPDATE `creature_proto` SET `equipmodel1`='42162',`equipinfo1`='33490690',`equipslot1`='789' WHERE (`entry`='18805');
-- Aura of Astromancer
UPDATE `creature_proto` SET `auras`='35915' WHERE (`entry`='20033');
-- Buff cast of Tempest Falconer
DELETE FROM `ai_agents` WHERE `entry`='20037';
INSERT INTO `ai_agents` (`entry`,`type`,`chance`,`maxcount`,`spell`,`spelltype`,`targettype`,`cooldown`) VALUES ('20037','4','100','2','37318','2','4','600000');
DELETE FROM `gameobject_spawns` WHERE `Entry`='184069';
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184069','550','743.057','4.63443','137.796','3.14159','0','0','1','-4.37114E-8','1','32','1375','1','0');

-- Gruul Creatures Equip Info
UPDATE `creature_proto` SET `equipmodel1`='3797',`equipinfo1`='33489154',`equipslot1`='273' WHERE (`entry`='19389');
UPDATE `creature_proto` SET `equipmodel1`='25180',`equipinfo1`='50267394',`equipslot1`='273' WHERE (`entry`='18831');
UPDATE `creature_proto` SET `equipmodel1`='33163',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='18832');
UPDATE `creature_proto` SET `equipmodel1`='24394',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='18834');
UPDATE `creature_proto` SET `equipmodel1`='24926',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='18836');
UPDATE `creature_proto` SET `equipmodel1`='24499',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='18835');

-- Gruuls Lair GO Spawns
DELETE FROM `gameobject_spawns` WHERE `map`='565';
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'183817','565','99.0729','249.704','18.3478','3.14159','0','0','1','-4.37114E-8','1','32','1375','1','0');
INSERT INTO `gameobject_spawns` (`id`,`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`orientation1`,`orientation2`,`orientation3`,`orientation4`,`State`,`Flags`,`Faction`,`Scale`,`stateNpcLink`) VALUES (NULL,'184662','565','166.897','368.226','16.9209','-1.5708','0','0','-0.707107','0.707107','0','32','1375','0.873267','0');

-- SSC Creatures Equip Info
UPDATE `creature_proto` SET `equipmodel1`='39974',`equipinfo1`='33492738',`equipslot1`='781' WHERE (`entry`='21339');
UPDATE `creature_proto` SET `equipmodel1`='23283',`equipinfo1`='33490434',`equipslot1`='529',`equipmodel3`='22671',`equipinfo3`='33492994',`equipslot3`='25' WHERE (`entry`='21221');
UPDATE `creature_proto` SET `equipmodel1`='44490',`equipinfo1`='33492226',`equipslot1`='1805',`equipmodel2`='44491',`equipinfo2`='33492226',`equipslot2`='1805' WHERE (`entry`='21301');
UPDATE `creature_proto` SET `equipmodel1`='44489',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='21220');
UPDATE `creature_proto` SET `equipmodel1`='24461',`equipinfo1`='33490946',`equipslot1`='273' WHERE (`entry`='21218');
UPDATE `creature_proto` SET `equipmodel1`='7440',`equipinfo1`='50267138',`equipslot1`='781' WHERE (`entry`='21263');

-- Hellfire Citadel: The Blood Furnace
-- Creatures Equip Info
UPDATE `creature_proto` SET `equipmodel1`='20502',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='23387',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='17624');
UPDATE `creature_proto` SET `equipmodel1`='34557',`equipinfo1`='33492226',`equipslot1`='1805',`equipmodel2`='34557',`equipinfo2`='33492226',`equipslot2`='1814' WHERE (`entry`='17398');
UPDATE `creature_proto` SET `equipmodel1`='7494',`equipinfo1`='50267138',`equipslot1`='1805' WHERE (`entry`='17414');
UPDATE `creature_proto` SET `equipmodel1`='24014',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='17377');
UPDATE `creature_proto` SET `equipmodel1`='5128',`equipinfo1`='33489154',`equipslot1`='273' WHERE (`entry`='17626');
UPDATE `creature_proto` SET `equipmodel1`='20502',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='20502',`equipinfo2`='33490690',`equipslot2`='781' WHERE (`entry`='17491');
UPDATE `creature_proto` SET `equipmodel1`='20502',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='23387',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='17370');

-- Coilfang: The Underbog
-- Creatures Equip Info
UPDATE `creature_proto` SET `equipmodel1`='7482',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='1755',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='17727');
UPDATE `creature_proto` SET `equipmodel1`='8747',`equipinfo1`='33490434',`equipslot1`='529' WHERE (`entry`='17726');
UPDATE `creature_proto` SET `equipmodel1`='7439',`equipinfo1`='50267138',`equipslot1`='781',`equipmodel2`='7439',`equipinfo2`='50267138',`equipslot2`='781',`equipmodel3`='6235',`equipinfo3`='50266626',`equipslot3`='15' WHERE (`entry`='17728');
UPDATE `creature_proto` SET `equipmodel1`='1600',`equipinfo1`='50268674',`equipslot1`='529' WHERE (`entry`='17771');
UPDATE `creature_proto` SET `equipmodel1`='7487',`equipinfo1`='33490690',`equipslot1`='781' WHERE (`entry`='17730');
UPDATE `creature_proto` SET `equipmodel1`='7430',`equipinfo1`='33492226',`equipslot1`='1805',`equipmodel3`='22671',`equipinfo3`='33492994',`equipslot3`='25' WHERE (`entry`='17729');
UPDATE `creature_proto` SET `equipmodel1`='23574',`equipinfo1`='33490690',`equipslot1`='781',`equipmodel2`='1685',`equipinfo2`='33490436',`equipslot2`='1038' WHERE (`entry`='17735');

-- Faction in TK: The Eye
UPDATE `creature_proto` SET `faction`='16' WHERE `entry` IN('20043', 20033, 20046, 20031, 20035, 20036, 20049, 20048, 20050, 20040, 20052, 20041, 19551, 20047, 20062, 20060, 20063, 20044, 20045, 20039, 20038, 20034, 20037, 20042, 20064);
UPDATE `creature_spawns` SET `faction`='16' WHERE `entry` IN('20043', 20033, 20046, 20031, 20035, 20036, 20049, 20048, 20050, 20040, 20052, 20041, 19551, 20047, 20062, 20060, 20063, 20044, 20045, 20039, 20038, 20034, 20037, 20042, 20064);

-- Add Zul'Aman to recall table
# By bartus
DELETE FROM recall WHERE name='ZulAman';
INSERT INTO recall (name,mapid,positionx,positiony,positionz) VALUES ('ZulAman',530,6850,-7950,170);

-- SSC AI_Agents cleanup:
delete from ai_agents where entry in ('21230', '21251');
