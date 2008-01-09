<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$xmlfilepath="http://www.wowhead.com/?item=";

$tr_f = fopen('ffa_loot_list.txt', 'w');

$result = mysql_query("SELECT entry FROM items ORDER BY entry ") or die(mysql_error());
while ($item = mysql_fetch_row($result)){
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$item[0] HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $temp ="";
	fwrite($fp, $out);
	while (!feof($fp)) $temp .= fgets($fp, 1024);
	fclose($fp);
	
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
	
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

			mysql_query("INSERT INTO itemloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$item[0]', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
	
		}
	}

	preg_match("#>Party loot<(.*?)>#", $temp, $k);
	if (isset($k[0])) fwrite($tr_f, "$item_id[1],"); 

	print "ENTRY $item[0] DONE<br />";
}

fclose($tr_f);
?>