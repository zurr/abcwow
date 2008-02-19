<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$map_array = array(558, 556);

foreach($map_array as $map_id){
	mysql_query("DELETE FROM gameobject_spawns WHERE map = $map_id") or die(mysql_error());
	$result = mysql_query("SELECT * FROM gameobject_spawns_1 WHERE map = $map_id") or die(mysql_error());
	while ($obj = mysql_fetch_row($result)){
		mysql_query("INSERT INTO gameobject_spawns (Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
			VALUES('$obj[1]','$obj[2]','$obj[3]','$obj[4]','$obj[5]','$obj[6]','$obj[7]','$obj[8]','$obj[9]','$obj[10]','$obj[11]','$obj[12]','$obj[13]','$obj[14]','$obj[15]') ") or die(mysql_error());
	}

	mysql_query("DELETE FROM creature_spawns WHERE map = $map_id") or die(mysql_error());
	$result = mysql_query("SELECT * FROM creature_spawns_1 WHERE map = $map_id") or die(mysql_error());
	while ($obj = mysql_fetch_row($result)){
		mysql_query("INSERT INTO creature_spawns (entry, map, position_x, position_y, position_z, orientation, movetype, displayid, faction, flags, bytes, bytes2, emote_state, npc_respawn_link, channel_spell, channel_target_sqlid, channel_target_sqlid_creature)
			VALUES('$obj[1]','$obj[2]','$obj[3]','$obj[4]','$obj[5]','$obj[6]','$obj[7]','$obj[8]','$obj[9]','$obj[10]','$obj[11]','$obj[12]','$obj[13]','$obj[14]','$obj[15]','$obj[16]','$obj[17]') ") or die(mysql_error());

		$last_id = mysql_insert_id();
	
		$result1 = mysql_query("SELECT * FROM creature_waypoints_1 WHERE spawnid = $obj[0]") or die(mysql_error());
		while ($wp = mysql_fetch_row($result1)){
			mysql_query("INSERT INTO creature_waypoints VALUES($last_id, '$wp[1]','$wp[2]','$wp[3]','$wp[4]','$wp[5]','$wp[6]','$wp[7]','$wp[8]','$wp[9]','$wp[10]','$wp[11]','$wp[12]') ") or die(mysql_error());
		}
	}
	print "MAP: $map_id DONE<br />";
}
print "ALL DONE";
?>