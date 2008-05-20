
DROP TABLE IF EXISTS `spell_data_extra`;
CREATE TABLE `spell_data_extra` (
  `spellid` int(10) unsigned NOT NULL DEFAULT '0',
  `dmg_bonus` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Priest

-- mind flay
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES 
 (15407,59),
 (17311,59),
 (17312,59),
 (17313,59),
 (17314,59),
 (18807,59),
 (25387,59);

-- Mana Burn (oooh, my poor manaburn /cry)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES 
 (8129,0),
 (8131,0),
 (10874,0),
 (10875,0),
 (10876,0),
 (25379,0),
 (25380,0);

-- Holy Fire (85,7% dd, 16.5% dot)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (14914,86),
 (15262,86),
 (15263,86),
 (15264,86),
 (15265,86),
 (15266,86),
 (15267,86),
 (15261,86),
 (25384,86);

-- Devouring Plague
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (2944,80),
 (19276,80),
 (19277,80),
 (192784,80),
 (19279,80),
 (19280,80),
 (25467,80);

-- Starshards
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (10797,171),
 (19296,171),
 (19299,171),
 (19302,171),
 (19303,171),
 (19304,171),
 (19305,171),
 (25446,171);

-- Circle of Healing
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (34861,14),
 (34863,14),
 (34864,14),
 (34865,14),
 (34866,14);

-- Prayer of Healing
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (596,29),
 (996,29),
 (10960,29),
 (10961,29),
 (25316,29),
 (25308,29);
 
-- Mage

-- Pyroblast (115% dd, 20% dot)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (11366,115),
 (12505,115),
 (12522,115),
 (12523,115),
 (12524,115),
 (12525,115),
 (12526,115),
 (18809,115),
 (27132,115),
 (33938,115);

-- Arcane Missiles (total 142.9% over 5 sec)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (7268,29),
 (7269,29),
 (7270,29),
 (8419,29),
 (8418,29),
 (10273,29),
 (10274,29),
 (25346,29),
 (27076,29),
 (38700,29),
 (38703,29);
 
-- Frostbolt
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (116,41),
 (205,49),
 (837,60),
 (7322,71),
 (8406,81),
 (8407,81),
 (8408,81),
 (10179,81),
 (10180,81),
 (10181,81),
 (25304,81),
 (27071,81),
 (27072,81),
 (38697,81);
 
-- Blizzard (95% overall devided by 8 sec 1 every sec)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (42208,12),
 (42209,12),
 (42210,12),
 (42211,12),
 (42212,12),
 (42213,12),
 (42198,12);
 
-- Flamestrike
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (2120,24),
 (2121,24),
 (8422,24),
 (8423,24),
 (10215,24),
 (10216,24),
 (27086,24);
 
-- Arcane Explosion
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (1449,21),
 (8437,21),
 (8438,21),
 (8439,21),
 (10201,21),
 (10202,21),
 (27080,21),
 (27082,21);
 
-- Blast Wave
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (11113,19),
 (13018,19),
 (13019,19),
 (13020,19),
 (13021,19),
 (27133,19),
 (33933,19);
 
-- Dragon's Breath
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (31661,19),
 (33041,19),
 (33042,19),
 (33043,19);
 
-- Ice Lance
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (30455,14);
 
-- Cone of Cold
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (120,20),
 (8492,20),
 (10159,20),
 (10160,20),
 (10161,20),
 (27087,20);
 
-- Frost Nova
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (122,3),
 (865,3),
 (6131,3),
 (10230,3),
 (27088,3);

-- Druid

-- Moonfire (15% dd, 52% dot)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (8921,15),
 (8924,15),
 (8925,15),
 (8926,15),
 (8927,15),
 (8928,15),
 (8929,15),
 (9833,15),
 (9834,15),
 (9835,15),
 (26987,15),
 (26988,15);
 
-- Entangling Roots
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (339,40),
 (1062,50),
 (5195,60),
 (5196,70),
 (9852,80),
 (9853,90),
 (26989,90);
 
-- Hurricane
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (42231,128),
 (42232,128),
 (42233,128),
 (42230,128);

-- Shaman

-- Flame Shock (15% dd, 52% dot)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (8050,15),
 (8052,15),
 (8053,15),
 (10447,15),
 (10448,15),
 (29228,15),
 (25457,15);

-- Warlock

-- Curse of Agony
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (980,120),
 (1014,120),
 (6217,120),
 (11711,120),
 (11712,120),
 (11713,120),
 (27218,120);
 
-- Curse of Doom
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (603,200),
 (30910,200);

-- Immolate (20%  dd, 65%  dot)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (348,20),
 (707,20),
 (1094,20),
 (2941,20),
 (11665,20),
 (11667,20),
 (11668,20),
 (25309,20),
 (27215,20);
 
-- Siphon Life
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (18265,100),
 (18879,100),
 (18880,100),
 (18881,100),
 (27264,100),
 (30911,100);
 
-- Death Coil
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (6789,21),
 (17925,21),
 (17926,21),
 (27223,21);
 
-- Shadowfury
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (30283,20),
 (30413,20),
 (30414,20);
 
-- Soul Fire
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (6353,115),
 (17924,115),
 (27211,115),
 (30545,115);
 
-- Drain Life
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (689,50),
 (699,50),
 (709,50),
 (7651,50),
 (11699,50),
 (11700,50),
 (27219,50),
 (27220,50);
 
-- Drain Mana
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (5138,0),
 (6226,0),
 (11703,0),
 (11704,0),
 (27221,0),
 (30908,0);
 
-- Drain Soul
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (1120,214),
 (8288,214),
 (8289,214),
 (11675,214),
 (27217,214);
 
-- Rain of Fire (95% over 8 sec every 2 sec)
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (42223,24),
 (42224,24),
 (42225,24),
 (42226,24),
 (42218,24);

-- Life Tap
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (1454,96),
 (1455,96),
 (1456,96),
 (11687,96),
 (11688,96),
 (11689,96),
 (27222,96);

-- Dark Pact
INSERT INTO `spell_data_extra` (`spellid`,`dmg_bonus`) VALUES
 (18220,96),
 (18937,96),
 (18938,96),
 (27265,96);