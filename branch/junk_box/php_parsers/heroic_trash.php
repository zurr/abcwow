<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$heroic_instances = array(552, 554, 553, 540, 542, 543, 547, 546, 545, 560, 269, 556, 555, 557, 558);

$result = mysql_query("SELECT entry FROM creature_names ORDER BY entry") or die(mysql_error());
while ($mob = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT SUM(percentchance),SUM(heroicpercentchance) FROM creatureloot WHERE entryid = '$mob[0]' ") or die(mysql_error());
	$mob_drops = mysql_fetch_row($result1);
	if ($mob_drops[0] && !$mob_drops[1]){
		$result2 = mysql_query("SELECT Map FROM creature_spawns WHERE entry = '$mob[0]' ") or die(mysql_error());
		while ($spawn_map = mysql_fetch_row($result2)){
			if(in_array($spawn_map[0], $heroic_instances)){
				mysql_query("UPDATE creatureloot SET creatureloot.heroicpercentchance=creatureloot.percentchance WHERE entryid = '$mob[0]'") or die(mysql_error());
			}
		}
	}
}

?>