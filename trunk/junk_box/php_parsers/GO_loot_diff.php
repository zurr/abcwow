<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());


$result = mysql_query("SELECT entry as ID,name FROM gameobject_names WHERE type = 3 AND entry NOT IN (SELECT distinct entryid FROM objectloot WHERE entryid=ID) ORDER BY name") or die(mysql_error());
while ($object = mysql_fetch_row($result)){
	
	$result1 = mysql_query("SELECT entry as ID FROM gameobject_names WHERE type = 3 AND entry IN (SELECT entryid FROM objectloot WHERE entryid=ID) AND name = \"$object[1]\"") or die(mysql_error());
	$filled_id = mysql_fetch_row($result1);
	
	$result2 = mysql_query("SELECT itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot FROM objectloot WHERE entryid = '$filled_id[0]'") or die(mysql_error());
	while ($loot_entry = mysql_fetch_row($result2)){
		mysql_query("INSERT INTO objectloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot)
			VALUES ('$object[0]', '$loot_entry[0]','$loot_entry[1]','$loot_entry[2]','$loot_entry[3]','$loot_entry[4]','$loot_entry[5]')") or die(mysql_error());
		print "$object[0]'s loot added<br />";
	}
}

?>