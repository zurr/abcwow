



<?php


/*

require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());


$item_list = "7190,16882,16885,13875,13918,6354,29569,16884,6355,7209,12033,16883,10456,15902,11883,5335,6755,11107,21509,21510,21511,21512,21513,28499,20231,20233,20236,31955,11955,25423,20603,6356,7973,6647,21243,6645,21162,13881,21164,13891,6643,8366,21812,10695,9541,9539,9540,6827,22746,25422,23921,16783,21191,11887,20602,21741,20808,15103,5738,27513,23022,19422,20469,20228,20229,20230,6351,30650,8647,11617,11024,11937,10834,21363,24336,21131,20805,21386,21133,8484,21310,25424,21270,21271,21979,21980,22164,21981,22165,22166,22167,22168,22169,22170,22171,22172,8049,11423,11422,13874,27481,22648,22649,22650,10569,27511,21150,24476,32777,21743,32629,21742,21132,21266,18804,21746,21640,6307,21228,32462,22320,21042,15876,23846,31408,32835,20768,31800,19153,19154,19155,11912,24402,9276,22155,22154,22156,21975,22158,22157,22159,22160,22161,22178,22162,22163,31522,11938,20601,21156,20767,22568,6357,19152,19150,19151,20766,32724,5523,15699,6353,32630,32625,21740,11966,21216,17727,17685,17726,21315,15874,9363,20809,20364,7870,5524,21327,32561,32543,32544,32545,32546,32547,32548,32549,32550,32551,32552,32555,32556,32557,32558,32559,32560,20708,11568,20393,15102,25419,12339,30260,6352,21113,22137,17962,21528,9265,12849,10752,5760,17964,19296,17963,10773,4633,4634,31952,10479,12122,19035,19297,19298,5758,19425,4632,32064,17969,4638,4637,4636,5759,17965,27446,30320";
$item_array = explode(',', $item_list);
 
$xmlfilepath="http://www.wowhead.com/?item=";


foreach ($item_array as $cont_item_id){

	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$cont_item_id HTTP/1.0\r\nHost: $proxy";
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
				VALUES ('$cont_item_id', '$item_id[1]', '$drop_chance_rep', 0, '$drop_stack[0]', '$drop_stack[1]')") or die(mysql_error());
	
		}
	}	



	print "ENTRY $item_data DONE<br />";
}


*/



require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());


$xmlfilepath="http://www.wowhead.com/?item=";

$result = mysql_query("SELECT entry FROM `items` WHERE `class` = 12 OR bonding = 4 OR bonding = 5") or die(mysql_error());
$c = 0;
while ($mob = mysql_fetch_row($result)){
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$mob[0] HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $temp ="";
	fwrite($fp, $out);
	while (!feof($fp)) $temp .= fgets($fp, 1024);
	fclose($fp);
	

	$c++;
	
	preg_match("#>Party loot<(.*?)>#", $temp, $m);
	if (isset($m[0])){
			print $mob[0].",";
			
			if ($c >= 20){
				print "<br />";
				$c = 0;
				}
		}
	}	

?>