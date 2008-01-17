<?php
ini_set('max_execution_time',0);
mysql_connect("***","root","***") or die(mysql_error());
mysql_select_db("abc_realm") or die(mysql_error());

$result = mysql_query("SELECT distinct player_guid FROM questlog") or die(mysql_error());
while ($obj = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT quest_id FROM questlog WHERE player_guid = $obj[0]") or die(mysql_error());
	$qarray = array();
		
	while ($q = mysql_fetch_row($result1)){
		if (!in_array($q[0], $qarray)) array_push($qarray, $q[0]);
		else
		{
			$result2 = mysql_query("SELECT * FROM questlog WHERE player_guid = $obj[0] AND quest_id = $q[0]") or die(mysql_error());
			$del_limit = mysql_num_rows($result2) - 1;
			mysql_query("DELETE FROM questlog WHERE player_guid = $obj[0] AND quest_id = $q[0] limit $del_limit") or die(mysql_error());
			print "player_guid = $obj[0] quest_id = $q[0]<br>";
		}
	}
}

print "ALL DONE";
?>