<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$result = mysql_query("SELECT entry FROM items WHERE class = 12 OR bonding = 4") or die(mysql_error());
while ($item = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT COUNT(*), SUM(percentchance) FROM creatureloot WHERE itemid = '$item[0]' ") or die(mysql_error());
	$mob_drops = mysql_fetch_row($result1);
	if(($mob_drops[1]/$mob_drops[0]) >= 60)
		mysql_query("UPDATE creatureloot SET percentchance='100' WHERE itemid = '$item[0]'") or die(mysql_error());
}
?>