# Preserving S1 and S2 cost as of 2.2
# SELECT entry,name1,ItemExtendedCost FROM `items` WHERE name1 LIKE "Gladiator's%" OR name1 LIKE "Merciless Gladiator's%" OR entry IN (33945,33942,28355,33936,28356,33948,33939,33951,28357,33076,33943,33946,33949,33077,33937,33078,33952,33940) AND quality=4;

UPDATE `items` set ItemExtendedCost=42 WHERE entry=24544; # Gladiator's Plate Chestpiece
UPDATE `items` set ItemExtendedCost=44 WHERE entry=24545; # Gladiator's Plate Helm
UPDATE `items` set ItemExtendedCost=46 WHERE entry=24546; # Gladiator's Plate Shoulders
UPDATE `items` set ItemExtendedCost=45 WHERE entry=24547; # Gladiator's Plate Legguards
UPDATE `items` set ItemExtendedCost=43 WHERE entry=24549; # Gladiator's Plate Gauntlets
UPDATE `items` set ItemExtendedCost=26 WHERE entry=24550; # Gladiator's Greatsword
UPDATE `items` set ItemExtendedCost=25 WHERE entry=24552; # Gladiator's Dreadweave Robe
UPDATE `items` set ItemExtendedCost=22 WHERE entry=24553; # Gladiator's Dreadweave Hood
UPDATE `items` set ItemExtendedCost=24 WHERE entry=24554; # Gladiator's Dreadweave Mantle
UPDATE `items` set ItemExtendedCost=23 WHERE entry=24555; # Gladiator's Dreadweave Leggings
UPDATE `items` set ItemExtendedCost=21 WHERE entry=24556; # Gladiator's Dreadweave Gloves
UPDATE `items` set ItemExtendedCost=52 WHERE entry=24557; # Gladiator's War Staff
UPDATE `items` set ItemExtendedCost=28 WHERE entry=25830; # Gladiator's Leather Helm
UPDATE `items` set ItemExtendedCost=31 WHERE entry=25831; # Gladiator's Leather Tunic
UPDATE `items` set ItemExtendedCost=30 WHERE entry=25832; # Gladiator's Leather Spaulders
UPDATE `items` set ItemExtendedCost=29 WHERE entry=25833; # Gladiator's Leather Legguards
UPDATE `items` set ItemExtendedCost=27 WHERE entry=25834; # Gladiator's Leather Gloves
UPDATE `items` set ItemExtendedCost=47 WHERE entry=25854; # Gladiator's Silk Amice
UPDATE `items` set ItemExtendedCost=48 WHERE entry=25855; # Gladiator's Silk Cowl
UPDATE `items` set ItemExtendedCost=50 WHERE entry=25856; # Gladiator's Silk Raiment
UPDATE `items` set ItemExtendedCost=49 WHERE entry=25857; # Gladiator's Silk Handguards
UPDATE `items` set ItemExtendedCost=51 WHERE entry=25858; # Gladiator's Silk Trousers
UPDATE `items` set ItemExtendedCost=32 WHERE entry=25997; # Gladiator's Linked Armor
UPDATE `items` set ItemExtendedCost=34 WHERE entry=25998; # Gladiator's Linked Helm
UPDATE `items` set ItemExtendedCost=36 WHERE entry=25999; # Gladiator's Linked Spaulders
UPDATE `items` set ItemExtendedCost=33 WHERE entry=26000; # Gladiator's Linked Gauntlets
UPDATE `items` set ItemExtendedCost=35 WHERE entry=26001; # Gladiator's Linked Leggings
UPDATE `items` set ItemExtendedCost=37 WHERE entry=27469; # Gladiator's Mail Armor
UPDATE `items` set ItemExtendedCost=38 WHERE entry=27470; # Gladiator's Mail Gauntlets
UPDATE `items` set ItemExtendedCost=39 WHERE entry=27471; # Gladiator's Mail Helm
UPDATE `items` set ItemExtendedCost=40 WHERE entry=27472; # Gladiator's Mail Leggings
UPDATE `items` set ItemExtendedCost=41 WHERE entry=27473; # Gladiator's Mail Spaulders
UPDATE `items` set ItemExtendedCost=42 WHERE entry=27702; # Gladiator's Lamellar Chestpiece
UPDATE `items` set ItemExtendedCost=43 WHERE entry=27703; # Gladiator's Lamellar Gauntlets
UPDATE `items` set ItemExtendedCost=44 WHERE entry=27704; # Gladiator's Lamellar Helm
UPDATE `items` set ItemExtendedCost=45 WHERE entry=27705; # Gladiator's Lamellar Legguards
UPDATE `items` set ItemExtendedCost=46 WHERE entry=27706; # Gladiator's Lamellar Shoulders
UPDATE `items` set ItemExtendedCost=21 WHERE entry=27707; # Gladiator's Satin Gloves
UPDATE `items` set ItemExtendedCost=22 WHERE entry=27708; # Gladiator's Satin Hood
UPDATE `items` set ItemExtendedCost=23 WHERE entry=27709; # Gladiator's Satin Leggings
UPDATE `items` set ItemExtendedCost=24 WHERE entry=27710; # Gladiator's Satin Mantle
UPDATE `items` set ItemExtendedCost=25 WHERE entry=27711; # Gladiator's Satin Robe
UPDATE `items` set ItemExtendedCost=71 WHERE entry=27879; # Gladiator's Scaled Chestpiece
UPDATE `items` set ItemExtendedCost=72 WHERE entry=27880; # Gladiator's Scaled Gauntlets
UPDATE `items` set ItemExtendedCost=73 WHERE entry=27881; # Gladiator's Scaled Helm
UPDATE `items` set ItemExtendedCost=74 WHERE entry=27882; # Gladiator's Scaled Legguards
UPDATE `items` set ItemExtendedCost=75 WHERE entry=27883; # Gladiator's Scaled Shoulders
UPDATE `items` set ItemExtendedCost=105 WHERE entry=28126; # Gladiator's Dragonhide Gloves
UPDATE `items` set ItemExtendedCost=106 WHERE entry=28127; # Gladiator's Dragonhide Helm
UPDATE `items` set ItemExtendedCost=107 WHERE entry=28128; # Gladiator's Dragonhide Legguards
UPDATE `items` set ItemExtendedCost=108 WHERE entry=28129; # Gladiator's Dragonhide Spaulders
UPDATE `items` set ItemExtendedCost=109 WHERE entry=28130; # Gladiator's Dragonhide Tunic
UPDATE `items` set ItemExtendedCost=110 WHERE entry=28136; # Gladiator's Wyrmhide Gloves
UPDATE `items` set ItemExtendedCost=111 WHERE entry=28137; # Gladiator's Wyrmhide Helm
UPDATE `items` set ItemExtendedCost=112 WHERE entry=28138; # Gladiator's Wyrmhide Legguards
UPDATE `items` set ItemExtendedCost=113 WHERE entry=28139; # Gladiator's Wyrmhide Spaulders
UPDATE `items` set ItemExtendedCost=114 WHERE entry=28140; # Gladiator's Wyrmhide Tunic
UPDATE `items` set ItemExtendedCost=132 WHERE entry=28294; # Gladiator's Heavy Crossbow
UPDATE `items` set ItemExtendedCost=133 WHERE entry=28295; # Gladiator's Slicer
UPDATE `items` set ItemExtendedCost=148 WHERE entry=28297; # Gladiator's Spellblade
UPDATE `items` set ItemExtendedCost=134 WHERE entry=28298; # Gladiator's Decapitator
UPDATE `items` set ItemExtendedCost=135 WHERE entry=28299; # Gladiator's Bonegrinder
UPDATE `items` set ItemExtendedCost=136 WHERE entry=28300; # Gladiator's Painsaw
UPDATE `items` set ItemExtendedCost=137 WHERE entry=28302; # Gladiator's Bonecracker
UPDATE `items` set ItemExtendedCost=138 WHERE entry=28305; # Gladiator's Pummeler
UPDATE `items` set ItemExtendedCost=139 WHERE entry=28307; # Gladiator's Quickblade
UPDATE `items` set ItemExtendedCost=140 WHERE entry=28308; # Gladiator's Cleaver
UPDATE `items` set ItemExtendedCost=141 WHERE entry=28309; # Gladiator's Hacker
UPDATE `items` set ItemExtendedCost=142 WHERE entry=28310; # Gladiator's Shiv
UPDATE `items` set ItemExtendedCost=143 WHERE entry=28312; # Gladiator's Shanker
UPDATE `items` set ItemExtendedCost=144 WHERE entry=28313; # Gladiator's Right Ripper
UPDATE `items` set ItemExtendedCost=145 WHERE entry=28314; # Gladiator's Left Ripper
UPDATE `items` set ItemExtendedCost=146 WHERE entry=28319; # Gladiator's War Edge
UPDATE `items` set ItemExtendedCost=147 WHERE entry=28320; # Gladiator's Touch of Defeat
UPDATE `items` set ItemExtendedCost=151 WHERE entry=28331; # Gladiator's Chain Helm
UPDATE `items` set ItemExtendedCost=152 WHERE entry=28332; # Gladiator's Chain Leggings
UPDATE `items` set ItemExtendedCost=153 WHERE entry=28333; # Gladiator's Chain Spaulders
UPDATE `items` set ItemExtendedCost=154 WHERE entry=28334; # Gladiator's Chain Armor
UPDATE `items` set ItemExtendedCost=155 WHERE entry=28335; # Gladiator's Chain Gauntlets
UPDATE `items` set ItemExtendedCost=159 WHERE entry=28346; # Gladiator's Endgame
UPDATE `items` set ItemExtendedCost=156 WHERE entry=28355; # Idol of Tenacity
UPDATE `items` set ItemExtendedCost=157 WHERE entry=28356; # Libram of Justice
UPDATE `items` set ItemExtendedCost=158 WHERE entry=28357; # Totem of the Third Wind
UPDATE `items` set ItemExtendedCost=160 WHERE entry=28358; # Gladiator's Shield Wall
UPDATE `items` set ItemExtendedCost=193 WHERE entry=28476; # Gladiator's Maul
UPDATE `items` set ItemExtendedCost=1389 WHERE entry=30186; # Gladiator's Felweave Amice
UPDATE `items` set ItemExtendedCost=1390 WHERE entry=30187; # Gladiator's Felweave Cowl
UPDATE `items` set ItemExtendedCost=1391 WHERE entry=30188; # Gladiator's Felweave Handguards
UPDATE `items` set ItemExtendedCost=1397 WHERE entry=30200; # Gladiator's Felweave Raiment
UPDATE `items` set ItemExtendedCost=1398 WHERE entry=30201; # Gladiator's Felweave Trousers
UPDATE `items` set ItemExtendedCost=1431 WHERE entry=30486; # Merciless Gladiator's Plate Chestpiece
UPDATE `items` set ItemExtendedCost=1432 WHERE entry=30487; # Merciless Gladiator's Plate Gauntlets
UPDATE `items` set ItemExtendedCost=1433 WHERE entry=30488; # Merciless Gladiator's Plate Helm
UPDATE `items` set ItemExtendedCost=1434 WHERE entry=30489; # Merciless Gladiator's Plate Legguards
UPDATE `items` set ItemExtendedCost=1435 WHERE entry=30490; # Merciless Gladiator's Plate Shoulders
UPDATE `items` set ItemExtendedCost=1570 WHERE entry=31375; # Gladiator's Kodohide Gloves
UPDATE `items` set ItemExtendedCost=1571 WHERE entry=31376; # Gladiator's Kodohide Helm
UPDATE `items` set ItemExtendedCost=1572 WHERE entry=31377; # Gladiator's Kodohide Legguards
UPDATE `items` set ItemExtendedCost=1573 WHERE entry=31378; # Gladiator's Kodohide Spaulders
UPDATE `items` set ItemExtendedCost=1574 WHERE entry=31379; # Gladiator's Kodohide Tunic
UPDATE `items` set ItemExtendedCost=1575 WHERE entry=31396; # Gladiator's Ringmail Armor
UPDATE `items` set ItemExtendedCost=1576 WHERE entry=31397; # Gladiator's Ringmail Gauntlets
UPDATE `items` set ItemExtendedCost=1577 WHERE entry=31400; # Gladiator's Ringmail Helm
UPDATE `items` set ItemExtendedCost=1578 WHERE entry=31406; # Gladiator's Ringmail Leggings
UPDATE `items` set ItemExtendedCost=1579 WHERE entry=31407; # Gladiator's Ringmail Spaulders
UPDATE `items` set ItemExtendedCost=1580 WHERE entry=31409; # Gladiator's Mooncloth Gloves
UPDATE `items` set ItemExtendedCost=1581 WHERE entry=31410; # Gladiator's Mooncloth Hood
UPDATE `items` set ItemExtendedCost=1582 WHERE entry=31411; # Gladiator's Mooncloth Leggings
UPDATE `items` set ItemExtendedCost=1583 WHERE entry=31412; # Gladiator's Mooncloth Mantle
UPDATE `items` set ItemExtendedCost=1584 WHERE entry=31413; # Gladiator's Mooncloth Robe
UPDATE `items` set ItemExtendedCost=1604 WHERE entry=31613; # Gladiator's Ornamented Chestguard
UPDATE `items` set ItemExtendedCost=1605 WHERE entry=31614; # Gladiator's Ornamented Gloves
UPDATE `items` set ItemExtendedCost=1606 WHERE entry=31616; # Gladiator's Ornamented Headcover
UPDATE `items` set ItemExtendedCost=1607 WHERE entry=31618; # Gladiator's Ornamented Legplates
UPDATE `items` set ItemExtendedCost=1608 WHERE entry=31619; # Gladiator's Ornamented Spaulders
UPDATE `items` set ItemExtendedCost=1663 WHERE entry=31958; # Merciless Gladiator's Bonecracker
UPDATE `items` set ItemExtendedCost=1664 WHERE entry=31959; # Merciless Gladiator's Bonegrinder
UPDATE `items` set ItemExtendedCost=1665 WHERE entry=31960; # Merciless Gladiator's Chain Armor
UPDATE `items` set ItemExtendedCost=1666 WHERE entry=31961; # Merciless Gladiator's Chain Gauntlets
UPDATE `items` set ItemExtendedCost=1667 WHERE entry=31962; # Merciless Gladiator's Chain Helm
UPDATE `items` set ItemExtendedCost=1668 WHERE entry=31963; # Merciless Gladiator's Chain Leggings
UPDATE `items` set ItemExtendedCost=1669 WHERE entry=31964; # Merciless Gladiator's Chain Spaulders
UPDATE `items` set ItemExtendedCost=1670 WHERE entry=31965; # Merciless Gladiator's Cleaver
UPDATE `items` set ItemExtendedCost=1671 WHERE entry=31966; # Merciless Gladiator's Decapitator
UPDATE `items` set ItemExtendedCost=1672 WHERE entry=31967; # Merciless Gladiator's Dragonhide Gloves
UPDATE `items` set ItemExtendedCost=1673 WHERE entry=31968; # Merciless Gladiator's Dragonhide Helm
UPDATE `items` set ItemExtendedCost=1674 WHERE entry=31969; # Merciless Gladiator's Dragonhide Legguards
UPDATE `items` set ItemExtendedCost=1676 WHERE entry=31971; # Merciless Gladiator's Dragonhide Spaulders
UPDATE `items` set ItemExtendedCost=1677 WHERE entry=31972; # Merciless Gladiator's Dragonhide Tunic
UPDATE `items` set ItemExtendedCost=1678 WHERE entry=31973; # Merciless Gladiator's Dreadweave Gloves
UPDATE `items` set ItemExtendedCost=1679 WHERE entry=31974; # Merciless Gladiator's Dreadweave Hood
UPDATE `items` set ItemExtendedCost=1680 WHERE entry=31975; # Merciless Gladiator's Dreadweave Leggings
UPDATE `items` set ItemExtendedCost=1681 WHERE entry=31976; # Merciless Gladiator's Dreadweave Mantle
UPDATE `items` set ItemExtendedCost=1682 WHERE entry=31977; # Merciless Gladiator's Dreadweave Robe
UPDATE `items` set ItemExtendedCost=1683 WHERE entry=31978; # Merciless Gladiator's Endgame
UPDATE `items` set ItemExtendedCost=1684 WHERE entry=31979; # Merciless Gladiator's Felweave Amice
UPDATE `items` set ItemExtendedCost=1685 WHERE entry=31980; # Merciless Gladiator's Felweave Cowl
UPDATE `items` set ItemExtendedCost=1686 WHERE entry=31981; # Merciless Gladiator's Felweave Handguards
UPDATE `items` set ItemExtendedCost=1687 WHERE entry=31982; # Merciless Gladiator's Felweave Raiment
UPDATE `items` set ItemExtendedCost=1688 WHERE entry=31983; # Merciless Gladiator's Felweave Trousers
UPDATE `items` set ItemExtendedCost=1689 WHERE entry=31984; # Merciless Gladiator's Greatsword
UPDATE `items` set ItemExtendedCost=1690 WHERE entry=31985; # Merciless Gladiator's Hacker
UPDATE `items` set ItemExtendedCost=1691 WHERE entry=31986; # Merciless Gladiator's Crossbow of the Phoenix
UPDATE `items` set ItemExtendedCost=1692 WHERE entry=31987; # Merciless Gladiator's Kodohide Gloves
UPDATE `items` set ItemExtendedCost=1693 WHERE entry=31988; # Merciless Gladiator's Kodohide Helm
UPDATE `items` set ItemExtendedCost=1694 WHERE entry=31989; # Merciless Gladiator's Kodohide Legguards
UPDATE `items` set ItemExtendedCost=1695 WHERE entry=31990; # Merciless Gladiator's Kodohide Spaulders
UPDATE `items` set ItemExtendedCost=1696 WHERE entry=31991; # Merciless Gladiator's Kodohide Tunic
UPDATE `items` set ItemExtendedCost=1697 WHERE entry=31992; # Merciless Gladiator's Lamellar Chestpiece
UPDATE `items` set ItemExtendedCost=1698 WHERE entry=31993; # Merciless Gladiator's Lamellar Gauntlets
UPDATE `items` set ItemExtendedCost=1699 WHERE entry=31995; # Merciless Gladiator's Lamellar Legguards
UPDATE `items` set ItemExtendedCost=1700 WHERE entry=31996; # Merciless Gladiator's Lamellar Shoulders
UPDATE `items` set ItemExtendedCost=1701 WHERE entry=31997; # Merciless Gladiator's Lamellar Helm
UPDATE `items` set ItemExtendedCost=1702 WHERE entry=31998; # Merciless Gladiator's Leather Gloves
UPDATE `items` set ItemExtendedCost=1703 WHERE entry=31999; # Merciless Gladiator's Leather Helm
UPDATE `items` set ItemExtendedCost=1704 WHERE entry=32000; # Merciless Gladiator's Leather Legguards
UPDATE `items` set ItemExtendedCost=1705 WHERE entry=32001; # Merciless Gladiator's Leather Spaulders
UPDATE `items` set ItemExtendedCost=1706 WHERE entry=32002; # Merciless Gladiator's Leather Tunic
UPDATE `items` set ItemExtendedCost=1707 WHERE entry=32003; # Merciless Gladiator's Left Ripper
UPDATE `items` set ItemExtendedCost=1708 WHERE entry=32004; # Merciless Gladiator's Linked Armor
UPDATE `items` set ItemExtendedCost=1709 WHERE entry=32005; # Merciless Gladiator's Linked Gauntlets
UPDATE `items` set ItemExtendedCost=1710 WHERE entry=32006; # Merciless Gladiator's Linked Helm
UPDATE `items` set ItemExtendedCost=1711 WHERE entry=32007; # Merciless Gladiator's Linked Leggings
UPDATE `items` set ItemExtendedCost=1712 WHERE entry=32008; # Merciless Gladiator's Linked Spaulders
UPDATE `items` set ItemExtendedCost=1713 WHERE entry=32009; # Merciless Gladiator's Mail Armor
UPDATE `items` set ItemExtendedCost=1714 WHERE entry=32010; # Merciless Gladiator's Mail Gauntlets
UPDATE `items` set ItemExtendedCost=1715 WHERE entry=32011; # Merciless Gladiator's Mail Helm
UPDATE `items` set ItemExtendedCost=1716 WHERE entry=32012; # Merciless Gladiator's Mail Leggings
UPDATE `items` set ItemExtendedCost=1717 WHERE entry=32013; # Merciless Gladiator's Mail Spaulders
UPDATE `items` set ItemExtendedCost=1718 WHERE entry=32014; # Merciless Gladiator's Maul
UPDATE `items` set ItemExtendedCost=1719 WHERE entry=32015; # Merciless Gladiator's Mooncloth Gloves
UPDATE `items` set ItemExtendedCost=1720 WHERE entry=32016; # Merciless Gladiator's Mooncloth Hood
UPDATE `items` set ItemExtendedCost=1721 WHERE entry=32017; # Merciless Gladiator's Mooncloth Leggings
UPDATE `items` set ItemExtendedCost=1722 WHERE entry=32018; # Merciless Gladiator's Mooncloth Mantle
UPDATE `items` set ItemExtendedCost=1723 WHERE entry=32019; # Merciless Gladiator's Mooncloth Robe
UPDATE `items` set ItemExtendedCost=1724 WHERE entry=32020; # Merciless Gladiator's Ornamented Chestguard
UPDATE `items` set ItemExtendedCost=1725 WHERE entry=32021; # Merciless Gladiator's Ornamented Gloves
UPDATE `items` set ItemExtendedCost=1726 WHERE entry=32022; # Merciless Gladiator's Ornamented Headcover
UPDATE `items` set ItemExtendedCost=1727 WHERE entry=32023; # Merciless Gladiator's Ornamented Legplates
UPDATE `items` set ItemExtendedCost=1728 WHERE entry=32024; # Merciless Gladiator's Ornamented Spaulders
UPDATE `items` set ItemExtendedCost=1729 WHERE entry=32025; # Merciless Gladiator's Painsaw
UPDATE `items` set ItemExtendedCost=1730 WHERE entry=32026; # Merciless Gladiator's Pummeler
UPDATE `items` set ItemExtendedCost=1731 WHERE entry=32027; # Merciless Gladiator's Quickblade
UPDATE `items` set ItemExtendedCost=1732 WHERE entry=32028; # Merciless Gladiator's Right Ripper
UPDATE `items` set ItemExtendedCost=1733 WHERE entry=32029; # Merciless Gladiator's Ringmail Armor
UPDATE `items` set ItemExtendedCost=1734 WHERE entry=32030; # Merciless Gladiator's Ringmail Gauntlets
UPDATE `items` set ItemExtendedCost=1735 WHERE entry=32031; # Merciless Gladiator's Ringmail Helm
UPDATE `items` set ItemExtendedCost=1736 WHERE entry=32032; # Merciless Gladiator's Ringmail Leggings
UPDATE `items` set ItemExtendedCost=1737 WHERE entry=32033; # Merciless Gladiator's Ringmail Spaulders
UPDATE `items` set ItemExtendedCost=1738 WHERE entry=32034; # Merciless Gladiator's Satin Gloves
UPDATE `items` set ItemExtendedCost=1739 WHERE entry=32035; # Merciless Gladiator's Satin Hood
UPDATE `items` set ItemExtendedCost=1740 WHERE entry=32036; # Merciless Gladiator's Satin Leggings
UPDATE `items` set ItemExtendedCost=1741 WHERE entry=32037; # Merciless Gladiator's Satin Mantle
UPDATE `items` set ItemExtendedCost=1742 WHERE entry=32038; # Merciless Gladiator's Satin Robe
UPDATE `items` set ItemExtendedCost=1743 WHERE entry=32039; # Merciless Gladiator's Scaled Chestpiece
UPDATE `items` set ItemExtendedCost=1744 WHERE entry=32040; # Merciless Gladiator's Scaled Gauntlets
UPDATE `items` set ItemExtendedCost=1745 WHERE entry=32041; # Merciless Gladiator's Scaled Helm
UPDATE `items` set ItemExtendedCost=1746 WHERE entry=32042; # Merciless Gladiator's Scaled Legguards
UPDATE `items` set ItemExtendedCost=1747 WHERE entry=32043; # Merciless Gladiator's Scaled Shoulders
UPDATE `items` set ItemExtendedCost=1748 WHERE entry=32044; # Merciless Gladiator's Shanker
UPDATE `items` set ItemExtendedCost=1749 WHERE entry=32045; # Merciless Gladiator's Shield Wall
UPDATE `items` set ItemExtendedCost=1750 WHERE entry=32046; # Merciless Gladiator's Shiv
UPDATE `items` set ItemExtendedCost=1751 WHERE entry=32047; # Merciless Gladiator's Silk Amice
UPDATE `items` set ItemExtendedCost=1752 WHERE entry=32048; # Merciless Gladiator's Silk Cowl
UPDATE `items` set ItemExtendedCost=1753 WHERE entry=32049; # Merciless Gladiator's Silk Handguards
UPDATE `items` set ItemExtendedCost=1754 WHERE entry=32050; # Merciless Gladiator's Silk Raiment
UPDATE `items` set ItemExtendedCost=1755 WHERE entry=32051; # Merciless Gladiator's Silk Trousers
UPDATE `items` set ItemExtendedCost=1756 WHERE entry=32052; # Merciless Gladiator's Slicer
UPDATE `items` set ItemExtendedCost=1757 WHERE entry=32053; # Merciless Gladiator's Spellblade
UPDATE `items` set ItemExtendedCost=1758 WHERE entry=32054; # Merciless Gladiator's War Edge
UPDATE `items` set ItemExtendedCost=1759 WHERE entry=32055; # Merciless Gladiator's War Staff
UPDATE `items` set ItemExtendedCost=1760 WHERE entry=32056; # Merciless Gladiator's Wyrmhide Gloves
UPDATE `items` set ItemExtendedCost=1761 WHERE entry=32057; # Merciless Gladiator's Wyrmhide Helm
UPDATE `items` set ItemExtendedCost=1762 WHERE entry=32058; # Merciless Gladiator's Wyrmhide Legguards
UPDATE `items` set ItemExtendedCost=1763 WHERE entry=32059; # Merciless Gladiator's Wyrmhide Spaulders
UPDATE `items` set ItemExtendedCost=1764 WHERE entry=32060; # Merciless Gladiator's Wyrmhide Tunic
UPDATE `items` set ItemExtendedCost=1898 WHERE entry=32450; # Gladiator's Gavel
UPDATE `items` set ItemExtendedCost=1899 WHERE entry=32451; # Gladiator's Salvation
UPDATE `items` set ItemExtendedCost=1900 WHERE entry=32452; # Gladiator's Reprieve
UPDATE `items` set ItemExtendedCost=1995 WHERE entry=32961; # Merciless Gladiator's Reprieve
UPDATE `items` set ItemExtendedCost=1996 WHERE entry=32962; # Merciless Gladiator's Touch of Defeat
UPDATE `items` set ItemExtendedCost=1997 WHERE entry=32963; # Merciless Gladiator's Gavel
UPDATE `items` set ItemExtendedCost=1998 WHERE entry=32964; # Merciless Gladiator's Salvation
UPDATE `items` set ItemExtendedCost=2036 WHERE entry=33076; # Idol of Perseverence
UPDATE `items` set ItemExtendedCost=2037 WHERE entry=33077; # Libram of Vengeance
UPDATE `items` set ItemExtendedCost=2038 WHERE entry=33078; # Totem of the Fourth Wave
UPDATE `items` set ItemExtendedCost=2043 WHERE entry=33309; # Merciless Gladiator's Redoubt
UPDATE `items` set ItemExtendedCost=2044 WHERE entry=33313; # Merciless Gladiator's Barrier

