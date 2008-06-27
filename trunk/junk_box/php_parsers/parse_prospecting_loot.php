<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$xmlfilepath="http://www.wowhead.com/?item=";

$ores = array(23425, 2770, 23424, 2772, 3858, 10620, 2771);

foreach ($ores as $item){
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$item HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $temp ="";
	fwrite($fp, $out);
	while (!feof($fp)) $temp .= fgets($fp, 1024);
	fclose($fp);
	
	preg_match("#id: 'prospecting'(.*?);\n#", $temp, $m);
	if (isset($m[0])){
	
		preg_match("#_totalCount:(.*?),#", $m[0], $tot_count);	
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);

		$item_array = explode('},{i', $tempa[1]);

		foreach($item_array as $item_data) {

			preg_match("#d:(.*?),#", $item_data, $item_id);
			preg_match("#count:(.*?),#", $item_data, $drop_count);
			preg_match("#stack:\[(.*?)\]#", $item_data, $drop_stack);
				$drop_stack = explode(',', $drop_stack[1]);
	
			$drop_chance_rep = round( (($drop_count[1]*100)/$tot_count[1]),4);

			mysql_query("REPLACE INTO prospectingloot (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount) 
				VALUES ('$item', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
	
		}
	}

	print "ENTRY $item DONE<br />";
}

?>