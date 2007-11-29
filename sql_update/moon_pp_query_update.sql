delete from creature_names where entry in (22917, 22990, 22996, 22997, 23069, 23197, 23259, 23336, 30000, 30001);
delete from creature_proto where entry in (22917, 22990, 22996, 22997, 23069, 23197, 23259, 23336, 30000, 30001);
delete from creature_spawns where entry in (22917, 22990, 22996, 22997, 23336, 30001);
delete from gameobject_names where entry in (185905, 200000, 200001);
delete from gameobject_spawns where entry in (185905, 200000, 200001);
delete from npc_text where entry in (229901, 229902);


INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (22917, 'Illidan Stormrage', 'The Betrayer', 0, 3, 0, 3, 0, 0, 21135, 0, 0, 0, 2, 1, 0, 1);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (22990, 'Akama', '', 0, 7, 0, 3, 0, 0, 20681, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (22996, 'Blade of Azzinoth', '', 0, 0, 0, 0, 0, 0, 21431, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`,  `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (22997, 'Flame of Azzinoth', '', 0, 3, 0, 0, 0, 0, 20431, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`,  `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (23197, 'Maiev Shadowsong', '', 0, 10, 0, 3, 0, 0, 20628, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (23259, 'Blaze', 'Blaze Effect', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (23336, 'Flame Crash', 'Flame Crash Effect', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (30000, 'Trigger', '', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (23069, 'Demon Fire', ' ', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (30001, 'Door Event Trigger', ' ', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);


INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
  (22917, 73, 73, 1825, 5534309, 5534309, 6774, 1.3, 0, 2000, 10000, 14000, 0, 0, 0, 0, 45479, 33488898, 0, 45481, 33488898, 0, 0, 0, 0, 360000, 8000, 25, 25, 25, 25, 25, 25, 1.25, 1, "", 1, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
  (22990, 73, 73, 1858, 480000, 480000, 338700, 2.0, 1, 2000, 350, 550, 0, 0, 0, 0, 43903, 33488898, 781, 43903, 33488898, 781, 0, 0, 0, 360000, 0, 0, 0, 0, 0, 0, 0, 1.3, 1.3, "", 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
  (22996, 73, 73, 35, 16000, 16000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, " ", 1, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
  (22997, 73, 73, 1825, 1100000, 1100000, 0, 4, 0, 2000, 650, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3600, 0, 0, 0, 0, 0, 0, 0, 2, 3, "", 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
  (23197, 73, 73, 1867, 27000, 27000, 3387, 2, 0, 2000, 643, 898, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 360000, 0, 0, 0, 0, 0, 0, 0, 2, 1, "", 1, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (23259, 70, 70, 1825, 6000, 6000, 6000, 1, 0, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (23336, 70, 70, 1825, 6000, 6000, 6000, 1, 0, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (23069, 70, 70, 1825, 100000, 100000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (30000, 70, 70, 1825, 100000, 100000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (30001, 70, 70, 35, 100000, 100000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);


INSERT INTO creature_spawns
   (id, `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes`, `bytes2`, `emote_state`, `respawnNpcLink`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`)
VALUES
   (140268503,22917, 564, 704.539, 305.282, 353.919, 6.14417, 0, 21135, 1825, 256, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO creature_spawns
   (id, `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes`, `bytes2`, `emote_state`, `respawnNpcLink`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`)
VALUES
   (140268504, 22990, 564, 661.563, 305.711, 271.689, 0.00628138, 4, 20681, 1858, 256, 0, 1, 0, 0, 0, 0, 0);

INSERT INTO creature_spawns
   (id,`entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes`, `bytes2`, `emote_state`, `respawnNpcLink`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`)
VALUES
   (140268505, 30001, 564, 771.5, 304.7, 319, 3.10568, 0, 15294, 35, 0, 0, 0, 0, 0, 0, 0, 0);


INSERT INTO gameobject_names
  (entry, Type, DisplayID, Name, spellfocus, sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8, sound9, unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7, unknown8, unknown9, unknown10, unknown11, unknown12, unknown13, unknown14)
VALUES
  (185905, 0, 7388, "Gate", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO gameobject_names
   (`entry`, `Type`, `DisplayID`, `Name`, `spellfocus`, `sound1`, `sound2`, `sound3`, `sound4`, `sound5`, `sound6`, `sound7`, `sound8`, `sound9`, `unknown1`, `unknown2`, `unknown3`, `unknown4`, `unknown5`, `unknown6`, `unknown7`, `unknown8`, `unknown9`, `unknown10`, `unknown11`, `unknown12`, `unknown13`, `unknown14`)
VALUES
   (200000, 0, 7421, 'BT_ILLIDAN_DOOR_RIGHT', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO gameobject_names
   (`entry`, `Type`, `DisplayID`, `Name`, `spellfocus`, `sound1`, `sound2`, `sound3`, `sound4`, `sound5`, `sound6`, `sound7`, `sound8`, `sound9`, `unknown1`, `unknown2`, `unknown3`, `unknown4`, `unknown5`, `unknown6`, `unknown7`, `unknown8`, `unknown9`, `unknown10`, `unknown11`, `unknown12`, `unknown13`, `unknown14`)
VALUES
   (200001, 0, 7422, 'BT_ILLIDAN_DOOR_LEFT', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO gameobject_spawns
  (Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
VALUES
  (185905, 564, 774.7, 304.6, 314.85, 1.53, 0, 0, 0.706767, 0.707446, 1, 1, 0, 1, 0);

INSERT INTO gameobject_spawns
   (`id`, `Entry`, `map`, `position_x`, `position_y`, `position_z`, `Facing`, `orientation1`, `orientation2`, `orientation3`, `orientation4`, `State`, `Flags`, `Faction`, `Scale`, `stateNpcLink`)
VALUES
   (278961, 200000, 564, 745.07, 241.802, 354.292, 0.79225, 0, 0, 0.385846, 0.922563, 0, 1, 0, 1, 0);

INSERT INTO gameobject_spawns
   (`id`, `Entry`, `map`, `position_x`, `position_y`, `position_z`, `Facing`, `orientation1`, `orientation2`, `orientation3`, `orientation4`, `State`, `Flags`, `Faction`, `Scale`, `stateNpcLink`)
VALUES
   (278962, 200001, 564, 744.829, 369.276, 354.324, 2.35855, 0, 0, 0.92433, 0.381594, 0, 1, 0, 1, 0);


INSERT INTO npc_text
   (`entry`, `prob0`, `text0_0`, `text0_1`, `lang0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `prob1`, `text1_0`, `text1_1`, `lang1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `prob2`, `text2_0`, `text2_1`, `lang2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `prob3`, `text3_0`, `text3_1`, `lang3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `prob4`, `text4_0`, `text4_1`, `lang4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `prob5`, `text5_0`, `text5_1`, `lang5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `prob6`, `text6_0`, `text6_1`, `lang6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `prob7`, `text7_0`, `text7_1`, `lang7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`)
VALUES
   (229901, 1, 'The time has come to face Illidan, $N. Are you ready?', 'The time has come to face Illidan, $N. Are you ready?', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0);

INSERT INTO npc_text
   (`entry`, `prob0`, `text0_0`, `text0_1`, `lang0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `prob1`, `text1_0`, `text1_1`, `lang1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `prob2`, `text2_0`, `text2_1`, `lang2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `prob3`, `text3_0`, `text3_1`, `lang3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `prob4`, `text4_0`, `text4_1`, `lang4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `prob5`, `text5_0`, `text5_1`, `lang5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `prob6`, `text6_0`, `text6_1`, `lang6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `prob7`, `text7_0`, `text7_1`, `lang7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`)
VALUES
   (229902, 1, 'Be wary, friends. The Betrayer meditates in the court just beyond.', 'Be wary, friends. The Betrayer meditates in the court just beyond.', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, ' ', ' ', 0, 0, 0, 0, 0, 0, 0);


   
   delete from creature_names where entry = 21364;
delete from creature_proto where entry = 21364;
delete from creature_names where entry = 21369;
delete from creature_proto where entry = 21369;
delete from creature_names where entry = 21362;
delete from creature_proto where entry = 21362;
delete from creature_proto where entry = 20064;
delete from creature_proto where entry = 20063;
delete from creature_proto where entry = 20062;
delete from creature_proto where entry = 20060;

INSERT INTO creature_names
  (entry, name, Subname, Flags1, type, Family, Rank, unk4, SpellDataID, male_displayid, female_displayid,unknown_float1, unknown_float2, Civilian, Leader)
VALUES
  (21364, 'Phoenix Egg', '', 0, 1, 0, 0, 0, NULL, 15512, 0, '1', '1', 0, NULL);

INSERT INTO creature_proto
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) 
VALUES
  ('21364','70','70','14','69000','69000','0','1.5','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','','0','0','0','0','0','0','0','0');

INSERT INTO creature_names
  (entry, name, Subname, Flags1, type, Family, Rank, unk4, SpellDataID, male_displayid, female_displayid, unknown_float1, unknown_float2, Civilian, Leader)
VALUES
  (21369, 'Flame Strike Trigger (Kael)', '', 0, 1, 0, 0, 0, NULL, 15294, 0, '1', '1', 0, NULL);

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) 
VALUES
  ('21369','70','70','1620','10000','10000','0','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','','0','0','0','0','0','0','0','0');

INSERT INTO creature_names
  (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
  ('21362','Phoenix','','104','10','0','1','0',NULL,19682,0,0,0,'1','1','0',NULL);

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)  
VALUES
  ('21362','70','70','14','11000','44000','0','1','0','1500','624','871','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2','1','0','0',0,'0','0','2.5','8','14',0);

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)  
VALUES
  ('20064','73','73','14','602756','602756','0','3','0','2000','6350','8550','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','4','2','','0','0','0','0','2.5','4.5','14','0');

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) 
VALUES
  ('20063','73','73','14','610299','610299','0','3','0','2000','7700','8550','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','4','2','','0','0','0','0','2.5','8','14','0');

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)  
VALUES
  ('20062','73','73','14','230323','230323','500000','3','0','1500','6600','7550','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','4','2','','0','0','0','0','2.5','8','14','0');

INSERT INTO creature_proto 
  (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)  
VALUES
  ('20060','73','73','14','613482','613482','0','3','0','2300','8100','10550','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','4','2','','0','0','0','0','2.5','8','14','0');
DELETE FROM creature_names WHERE entry IN (15990, 16427, 16428, 16429, 16441, 30002);
DELETE FROM creature_proto WHERE entry IN (15990, 16427, 16428, 16429, 16441, 30002);
DELETE FROM creature_spawns WHERE ENTRY IN (15990, 30002);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (15990, 'Kel\'Thuzad', '', 0, 6, 0, 3, 0, 0, 15945, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (16427, 'Soldier of the Frozen Wastes', '', 0, 6, 0, 1, 0, 0, 7869, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (16428, 'Unstoppable Abomination', '', 0, 6, 0, 1, 0, 0, 12818, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (16429, 'Soul Weaver', '', 0, 6, 0, 1, 0, 0, 16178, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (16441, 'Guardian of Icecrown', '', 0, 6, 0, 0, 0, 0, 16586, 0, 0, 0, 1, 1, 0, 0);

INSERT INTO creature_names
   (`entry`, `name`, `subname`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`)
VALUES
   (30002, 'The Lich King', '', 0, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 1, 1, 0, 0);


INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (15990, 73, 73, 14, 2527186, 2527186, 1572000, 4, 0, 1150, 950, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.842, '0', 0, 277838, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (16427, 60, 60, 14, 8900, 8900, 0, 1.5, 0, 1550, 1750, 2250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (16428, 60, 60, 14, 97000, 97000, 0, 2, 0, 1250, 1550, 2300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (16429, 60, 60, 14, 70000, 70000, 0, 2.5, 0, 1500, 366, 472.14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (16441, 60, 60, 14, 350000, 350000, 0, 0.5, 0, 1250, 3150, 4100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

INSERT INTO creature_proto
   (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `mountdisplayid`, `equipmodel1`, `equipinfo1`, `equipslot1`, `equipmodel2`, `equipinfo2`, `equipslot2`, `equipmodel3`, `equipinfo3`, `equipslot3`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`)
VALUES
   (30002, 70, 70, 14, 100000, 100000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);


INSERT INTO creature_spawns
   (id,`entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes`, `bytes2`, `emote_state`, `respawnNpcLink`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`)
VALUES
   (140268506, 15990, 533, 3760.64, -5115.65, 143.808, 2.9767, 0, 15945, 14, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO creature_spawns
   (id, `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes`, `bytes2`, `emote_state`, `respawnNpcLink`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`)
VALUES
   (140268507, 30002, 533, 3767.58, -5117.15, 174.49, 2.95544, 0, 15294, 14, 33554432, 0, 0, 0, 0, 0, 0, 0);
   
   UPDATE `gameobject_spawns` SET `flags` = '1' WHERE `entry` = '18972';
UPDATE `gameobject_spawns` SET `flags` = '1' WHERE `entry` = '18971';