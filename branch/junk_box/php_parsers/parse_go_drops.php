<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());


$xmlfilepath="http://www.wowhead.com/?object=";

$result = mysql_query("SELECT entry FROM gameobject_names WHERE type = 3 ORDER BY entry ") or die(mysql_error());
while ($mob = mysql_fetch_row($result)){
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$mob[0] HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $temp ="";
	fwrite($fp, $out);
	while (!feof($fp)) $temp .= fgets($fp, 1024);
	fclose($fp);
	
	$go_drops = array();
	

	preg_match("#name: 'Contains'(.*?);\n#", $temp, $m);
	if (isset($m[0])){
	
		preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);

		$item_array = explode('},{', $tempa[1]);

		foreach($item_array as $item_data) {
			preg_match("#id:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
			array_push($go_drops, $item_id[1]);
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

			mysql_query("INSERT INTO objectloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$mob[0]', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
		}
	}	


	if (!sizeof($go_drops)){	

	 preg_match("#name: 'Herb gathering'(.*?);\n#", $temp, $m);
	 if (isset($m[0])){
	
		preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		
		$item_array = explode('},{', $tempa[1]);
	
		foreach($item_array as $item_data) {
			preg_match("#id:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
			array_push($go_drops, $item_id[1]);
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);
			
			mysql_query("INSERT INTO objectloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$mob[0]', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')")  or die(mysql_error());
		}
		
		}
	}
	
	if (!sizeof($go_drops)){	

	 preg_match("#name: 'Mining'(.*?);\n#", $temp, $m);
	 if (isset($m[0])){
	
		preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		
		$item_array = explode('},{', $tempa[1]);
	
		foreach($item_array as $item_data) {
			preg_match("#id:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
			array_push($go_drops, $item_id[1]);
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);
			
			mysql_query("INSERT INTO objectloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$mob[0]', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')")  or die(mysql_error());
		}
		
	}
 }
 
 
	print "ENTRY $mob[0] DONE : ".(sizeof($go_drops))." drops.<br />";
}
?>