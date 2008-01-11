
DROP TABLE IF EXISTS `prospectingloot`;
CREATE TABLE `prospectingloot` (
  `index` int(11) NOT NULL AUTO_INCREMENT,
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `prospectingloot` DISABLE KEYS */;
INSERT INTO `prospectingloot` VALUES  (1,23425,24243,100,0,1,1,0),
 (2,23425,21929,18.6142,0,1,2,0),
 (3,23425,23079,18.5007,0,1,2,0),
 (4,23425,23112,18.3588,0,1,2,0),
 (5,23425,23077,18.3494,0,1,2,0),
 (6,23425,23117,18.2462,0,1,2,0),
 (7,23425,23107,18.1439,0,1,2,0),
 (8,23425,23437,3.4165,0,1,1,0),
 (9,23425,23439,3.3423,0,1,1,0),
 (10,23425,23440,3.2769,0,1,1,0),
 (11,23425,23436,3.223,0,1,1,0),
 (12,23425,23438,3.1689,0,1,1,0),
 (13,23425,23441,3.1551,0,1,1,0),
 (14,2770,24186,100,0,1,1,0),
 (15,2770,774,50.2276,0,1,1,0),
 (16,2770,818,49.7695,0,1,1,0),
 (17,2770,1210,9.8351,0,1,1,0),
 (18,23424,24242,100,0,1,1,0),
 (19,23424,23079,17.3911,0,1,2,0),
 (20,23424,21929,17.3555,0,1,2,0),
 (21,23424,23112,17.2763,0,1,2,0),
 (22,23424,23107,17.1239,0,1,2,0),
 (23,23424,23077,17.0003,0,1,2,0),
 (24,23424,23117,16.6451,0,1,2,0),
 (25,23424,23439,1.0884,0,1,1,0),
 (26,23424,23436,1.0864,0,1,1,0),
 (27,23424,23437,1.0775,0,1,1,0),
 (28,23424,23440,1.0399,0,1,1,0),
 (29,23424,23438,1.0349,0,1,1,0),
 (30,23424,23441,1.03,0,1,1,0);
INSERT INTO `prospectingloot` VALUES  (31,2772,24190,100,0,1,1,0),
 (32,2772,1529,33.8205,0,1,2,0),
 (33,2772,3864,32.9617,0,1,2,0),
 (34,2772,1705,32.8787,0,1,2,0),
 (35,2772,7909,5.1093,0,1,1,0),
 (36,2772,7910,5.066,0,1,1,0),
 (37,3858,24234,100,0,1,1,0),
 (38,3858,7909,33.1497,0,1,2,0),
 (39,3858,7910,33.0193,0,1,2,0),
 (40,3858,3864,32.5734,0,1,2,0),
 (41,3858,12799,2.6626,0,1,1,0),
 (42,3858,12361,2.6289,0,1,1,0),
 (43,3858,12800,2.4481,0,1,1,0),
 (44,3858,12364,2.4018,0,1,1,0),
 (45,10620,24235,100,0,1,1,0),
 (46,10620,7910,25.9852,0,1,2,0),
 (47,10620,12799,18.932,0,1,2,0),
 (48,10620,12364,18.8545,0,1,2,0),
 (49,10620,12361,18.8382,0,1,2,0),
 (50,10620,12800,18.671,0,1,2,0),
 (51,10620,21929,0.9015,0,1,1,0),
 (52,10620,23112,0.8893,0,1,1,0),
 (53,10620,23107,0.8444,0,1,1,0),
 (54,10620,23079,0.8403,0,1,1,0),
 (55,10620,23077,0.8118,0,1,1,0),
 (56,10620,23117,0.7669,0,1,1,0),
 (57,2771,24188,100,0,1,1,0),
 (58,2771,1206,38.4578,0,1,2,0),
 (59,2771,1705,37.5696,0,1,2,0),
 (60,2771,1210,37.1039,0,1,2,0),
 (61,2771,1529,3.3801,0,1,1,0);
INSERT INTO `prospectingloot` VALUES  (62,2771,3864,3.3176,0,1,1,0),
 (63,2771,7909,3.1832,0,1,1,0);
/*!40000 ALTER TABLE `prospectingloot` ENABLE KEYS */;
