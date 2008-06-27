<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$result = mysql_query("SELECT distinct entry FROM creature_spawns where map in (552, 554, 553, 540, 542, 543, 547, 546, 545, 560, 269, 556, 555, 557, 558)") or die(mysql_error());
while ($mob = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT SUM(percentchance),SUM(heroicpercentchance) FROM creatureloot WHERE entryid = '$mob[0]' ") or die(mysql_error());
	$mob_drops = mysql_fetch_row($result1);
	if ($mob_drops[0] && !$mob_drops[1])
		mysql_query("UPDATE creatureloot SET creatureloot.heroicpercentchance=creatureloot.percentchance WHERE entryid = '$mob[0]'") or die(mysql_error());

	print "$mob[0] DONE <br>";
}

$result = mysql_query("SELECT distinct entry FROM gameobject_spawns where map in (552, 554, 553, 540, 542, 543, 547, 546, 545, 560, 269, 556, 555, 557, 558)") or die(mysql_error());
while ($go = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT SUM(percentchance),SUM(heroicpercentchance) FROM objectloot WHERE entryid = '$go[0]' ") or die(mysql_error());
	$go_drops = mysql_fetch_row($result1);
	if ($go_drops[0] && !$go_drops[1]){
		mysql_query("UPDATE objectloot SET objectloot.heroicpercentchance=objectloot.percentchance WHERE entryid = '$go[0]'") or die(mysql_error());
	}
	print "$go[0] DONE <br>";
}
?>