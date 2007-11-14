<?php
ini_set('max_execution_time',0);
mysql_connect("127.0.0.1:3306","root","xxx") or die(mysql_error());
mysql_select_db("antrix_worlds_abc") or die(mysql_error());

$heroic_instances = array(552, 554, 553, 540, 542, 543, 547, 546, 545, 560, 269, 556, 555, 557, 558);

$result = mysql_query("SELECT entry FROM gameobject_names WHERE type = 3 ORDER BY entry") or die(mysql_error());
while ($go = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT SUM(percentchance),SUM(heroicpercentchance) FROM objectloot WHERE entryid = '$go[0]' ") or die(mysql_error());
	$go_drops = mysql_fetch_row($result1);
	if ($go_drops[0] && !$go_drops[1]){
		$result2 = mysql_query("SELECT Map FROM gameobject_spawns WHERE entry = '$go[0]' ") or die(mysql_error());
		while ($spawn_map = mysql_fetch_row($result2)){
			if(in_array($spawn_map[0], $heroic_instances)){
				mysql_query("UPDATE objectloot SET objectloot.heroicpercentchance=objectloot.percentchance WHERE entryid = '$go[0]'") or die(mysql_error());
			}
		}
	}
}
print "ALL DONE";
?>