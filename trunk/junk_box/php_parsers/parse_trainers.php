<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$xmlfilepath="http://www.wowhead.com/?npc=";

/*
$trainer_list = array(
198);
*/

/*
foreach($trainer_list as $trainer){
	$result6 = mysql_query("SELECT entry FROM trainer_defs WHERE entry = $trainer")  or die(mysql_error());
	if(!mysql_num_rows($result6)) print "missing trainer_defs entry id : $trainer<br />";
	}
*/
	

// GET TRAINER SPELLS
/*
$handle = fopen("trainer_list", "r");

//foreach($trainer_list as $trainer)
if ($handle) {
while (!feof($handle))
{
	$trainer = fgets($handle, 4096);

	$trainer = ereg_replace ("\n","",$trainer);
	$trainer = ereg_replace ("\r\n$","",$trainer);
	$trainer = ereg_replace ("\r$","",$trainer);
					
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$trainer HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $page ="";
	fwrite($fp, $out);
	while (!feof($fp)) $page .= fgets($fp, 1024);
	fclose($fp);

	print"*******   Trainer ID: $trainer **********<br />";
	preg_match("#id: 'teaches-ability'(.*?);\n#", $page, $m);
	if (isset($m[0])){
		print"teaches-ability :::<br />";
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		$spell_array = explode('},{', $tempa[1]);

		foreach($spell_array as $spell_data) {
			preg_match("#id:(.*?),#", $spell_data, $spell_id);
			$type = 0;
			
			print"Spell_ID: $spell_id[1]<br />";
			mysql_query("INSERT INTO trainer_spells (entry, learn_spell, type) 
					VALUES ('$trainer', '$spell_id[1]', '$type')") or die(mysql_error());
		}
	}

	preg_match("#id: 'teaches-recipe'(.*?);\n#", $page, $m);
	if (isset($m[0])){
		print"teaches-recipe :::<br />";
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		$spell_array = explode('},{', $tempa[1]);

		foreach($spell_array as $spell_data) {
			preg_match("#id:(.*?),#", $spell_data, $spell_id);
			$type = 1;
			
			print"Spell_ID: $spell_id[1]<br />";
			mysql_query("INSERT INTO trainer_spells (entry, learn_spell, type) 
					VALUES ('$trainer', '$spell_id[1]', '$type')") or die(mysql_error());
		}
	}
	
	preg_match("#id: 'teaches-other'(.*?);\n#", $page, $m);
	if (isset($m[0])){
		print"teaches-other :::<br />";
		preg_match("#data: \[\{(.*?)\}\]\}\);#", $m[0], $tempa);
		$spell_array = explode('},{', $tempa[1]);

		foreach($spell_array as $spell_data) {
			preg_match("#id:(.*?),#", $spell_data, $spell_id);
			$type = 2;

			print"Spell_ID: $spell_id[1]<br />";
			mysql_query("INSERT INTO trainer_spells (entry, learn_spell, type) 
					VALUES ('$trainer', '$spell_id[1]', '$type')") or die(mysql_error());
		}
	}
	
	print"<br />";

}

 fclose($handle);
}
// END Of GETTING TRAINER SPELLS
*/

//GET SPELL DATA   COST AND REQ. SKILL
/*
$prof_arr = Array(
	129 => array(129,"First Aid"),
	762 => array(762,"Riding"),
	755 => array(755,"Jewelcrafting"),
	393 => array(393,"Skinning"),
	356 => array(356,"Fishing"),
	333 => array(333,"Enchanting"),
	202 => array(202,"Engineering"),
	197 => array(197,"Tailoring"),
	186 => array(186,"Mining"),
	185 => array(185,"Cooking"),
	182 => array(182,"Herbalism"),
	171 => array(171,"Alchemy"),
	165 => array(165,"Leatherworking"),
	164 => array(164,"Blacksmithing"),
);

$xmlfilepath="http://www.wowhead.com/?spell=";

$result = mysql_query("SELECT distinct learn_spell FROM trainer_spells ORDER BY learn_spell") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	$fp = fsockopen($proxy, $proxy_port, $errno, $errstr, 10);
	$out = "GET $xmlfilepath$spell[0] HTTP/1.0\r\nHost: $proxy";
	$out .="Connection: Close\r\n\r\n";
    $page ="";
	fwrite($fp, $out);
	while (!feof($fp)) $page .= fgets($fp, 1024);
	fclose($fp);
	
	preg_match("#<th>Quick Facts</th></tr>\n(.*?)\n<tr><th>Screenshots</th>#", $page, $m);
	if (isset($m[0])){
		$data = $m[0];
		
		$req_level = 0;
		$d_glag = 0;
		preg_match("#Level: (.*?)<#", $data, $l);
		if (isset($l[0])) $req_level = $l[1];
		else
		{
			preg_match("#Requires level (.*?)<#", $data, $l2);
			if (isset($l2[0])){
				$req_level = $l2[1];
				$d_glag =1;
			}
		}
		
		$req_skill = 0;
		$req_value = 0;
		if($d_glag)
			preg_match("#</li><li><div>Requires (.*?)\)<#", $data, $s);
		else
			preg_match("#>Requires (.*?)\)<#", $data, $s);
			
		if (isset($s[1])){
			$req_value = ereg_replace ('[^0-9]+', '', $s[1]);
			
			foreach($prof_arr as $proff)
				if (strstr($s[1],$proff[1])) $req_skill = $proff[0];
		}
		
		$cost = 0;
		preg_match("#gold\">(.*?)<#", $data, $t1);
		if (isset($t1[1])) $cost += $t1[1]*10000;
		preg_match("#silver\">(.*?)<#", $data, $t2);
		if (isset($t2[1])) $cost += $t2[1]*100;
		preg_match("#moneycopper\">(.*?)<#", $data, $t3);
		if (isset($t3[1])) $cost += $t3[1];

		print "SPELLID:$spell[0] - req.level: $req_level  req.skill: $req_skill  skill.level: $req_value cost: $cost<br/>";
		mysql_query("UPDATE trainer_spells SET spellcost = '$cost', reqskill = '$req_skill', reqskillvalue = '$req_value', reqlevel = '$req_level' WHERE learn_spell = $spell[0]") or die(mysql_error());
	} else print "<br \> ERROR GETTING INFO FOR ID:$spell[0]<br />";
}

*/
//GET REQ. SPELLS
/*
$result = mysql_query("SELECT distinct learn_spell FROM trainer_spells WHERE type = 0 ORDER BY learn_spell") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT name,rank FROM dbc_spell_2_4 WHERE entry = $spell[0] AND rank LIKE '%Rank%'") or die(mysql_error());
	
	if(mysql_num_rows($result1))
	{
		$_spell = mysql_fetch_row($result1);
	
		$data = sscanf($_spell[1], "%s %d");
		if ($data[1] > 1){
			$data[1]--;
			$result2 = mysql_query("SELECT entry FROM dbc_spell WHERE rank = 'Rank $data[1]' and name = \"$_spell[0]\"") or die(mysql_error());
			if(mysql_num_rows($result2))
			{
				$entry = mysql_fetch_row($result2);
				mysql_query("UPDATE trainer_spells SET reqspell = '$entry[0]' WHERE learn_spell = '$spell[0]'") or die(mysql_error());
				print "SPELL: $spell[0] REQ: $entry[0] <br />";
			}
			else print "SPELL: $spell[0] NONE-REQ --- RANKED!!!!<br />";

		}
	}
	else print "SPELL: $spell[0] NONE-REQ <br />";
}
*/


/*
$result = mysql_query("SELECT distinct deletespell, learn_spell FROM trainer_spells_pho WHERE deletespell != 0") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	mysql_query("UPDATE trainer_spells SET deletespell = $spell[0] WHERE learn_spell = $spell[1]") or die(mysql_error());
*/
}

?>