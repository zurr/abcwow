<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$tr_f = fopen('trainer_list.txt', 'w');
$non_f = fopen('nonexistant_list.txt', 'w');

$xmlfilepath="http://www.wowhead.com/?npc=";

$result = mysql_query("SELECT entry FROM creature_names ORDER BY entry") or die(mysql_error());
while ($mob = mysql_fetch_row($result)){
	$id = $mob[0];
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$id HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $temp ="";
	fwrite($fp, $out);
	while (!feof($fp)) $temp .= fgets($fp, 1024);
	fclose($fp);
	
	preg_match("#This NPC doesn't exist or is not yet in the database#", $temp, $empty);
	if (isset($empty[0])){
		fwrite($non_f, "$id,");
		print "ENTRY $id NO_EXISTANT<br />";
	}
	else
	{
		$creature_drops = array();
	
		preg_match("#id: 'drops'(.*?);\n#", $temp, $m);
		if (isset($m[0])){
			preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
			preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);

			$item_array = explode('},{i', $tempa[1]);

			foreach($item_array as $item_data) {
				preg_match("#d:(.*?),#", $item_data, $item_id);
				preg_match("#count:(.*?),#", $item_data, $drop_count);
				preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
					$drop_stack = explode(',', $drop_stack[1]);
	
				array_push($creature_drops, $item_id[1]);
				$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

				mysql_query("REPLACE INTO creatureloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
					VALUES ('$id', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
			}
		}	

		if (!sizeof($creature_drops)){	

		preg_match("#id: 'normal-drops'(.*?);\n#", $temp, $m);
		if (isset($m[0])){
			preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
			preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		
			$item_array = explode('},{i', $tempa[1]);
	
			foreach($item_array as $item_data) {
				preg_match("#d:(.*?),#", $item_data, $item_id);
				preg_match("#count:(.*?),#", $item_data, $drop_count);
				preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
				array_push($creature_drops, $item_id[1]);
				$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);
			
				mysql_query("REPLACE INTO creatureloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
					VALUES ('$id', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')")  or die(mysql_error());
			}
		}

		preg_match("#id: 'heroic-drops'(.*?);\n#", $temp, $ma);
		if (isset($ma[0])){
			preg_match("#_totalCount:(.*?),#", $ma[0], $tot_count);	
			preg_match("#data: \[\{(.*?)\}\]\}\);#", $ma[0], $tempa);
		
			$item_array = explode('},{i', $tempa[1]);
		
			foreach($item_array as $item_data) {
				preg_match("#d:(.*?),#", $item_data, $item_id);
				preg_match("#count:(.*?),#", $item_data, $drop_count);
				preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
			
				$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);
		
				if (in_array($item_id[1], $creature_drops)) mysql_query("UPDATE creatureloot SET heroicpercentchance='$drop_chance_rep' WHERE entryid='$id' AND  itemid= '$item_id[1]'") or die(mysql_error());
				else mysql_query("REPLACE INTO creatureloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
						VALUES ('$id', '$item_id[1]', 0, '$drop_chance_rep', '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
			}
		}
	}
	
	preg_match("#id: 'skinning'(.*?);\n#", $temp, $mb);
	if (isset($mb[0])){
	
		preg_match("#_totalCount:(.*?),#", $mb[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $mb[0], $tempa);

		$item_array = explode('},{i', $tempa[1]);

		foreach($item_array as $item_data) {

			preg_match("#d:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
			$drop_stack = explode(',', $drop_stack[1]);
	
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

			mysql_query("REPLACE INTO skinningloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$id', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
		}
	}	


	preg_match("#id: 'pickpocketing'(.*?);\n#", $temp, $mc);
	if (isset($mc[0])){
	
		preg_match("#_totalCount:(.*?),#", $mc[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $mc[0], $tempa);

		$item_array = explode('},{i', $tempa[1]);

		foreach($item_array as $item_data) {
			preg_match("#d:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

			mysql_query("REPLACE INTO pickpocketingloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$id', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
		}
	}

	preg_match("#id: 'teaches-recipe'(.*?);\n#", $temp, $k);
	if (isset($k[0])) fwrite($tr_f, "$id,");
	
	print "ENTRY $id DONE : ".(sizeof($creature_drops))." drops.<br />";
	
	}
}

fclose($non_f);
fclose($tr_f);
?>