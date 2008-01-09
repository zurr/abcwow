<?php
require_once('config.php');
mysql_connect($db['addr'],$db['user'],$db['pass']) or die(mysql_error());
mysql_select_db($db['name']) or die(mysql_error());

$xmlfilepath="http://www.wowhead.com/?npc=";
/*
$trainer_list = array(
198,223,328,331,375,376,377,459,460,461,514,543,812,837,895,906,908,911,912,913,914,915,916,917,918,925,926,927,928,944,957,
985,986,987,988,1103,1215,1218,1226,1228,1229,1231,1232,1234,1241,1246,1292,1300,1317,1346,1355,1382,1383,1385,1386,1404,1411,
1430,1458,1466,1470,1473,1632,1651,1676,1681,1683,1699,1700,1701,1702,1703,1901,2114,2119,2122,2123,2124,2126,2127,2128,2129,
2130,2131,2132,2326,2327,2329,2367,2390,2391,2399,2485,2489,2492,2627,2704,2798,2818,2834,2836,2837,2855,2856,2857,2878,2879,
2998,3001,3004,3007,3008,3009,3011,3013,3026,3028,3030,3031,3032,3033,3034,3036,3038,3039,3040,3041,3042,3043,3044,3045,3046,
3047,3048,3049,3059,3060,3061,3062,3063,3064,3065,3066,3067,3069,3087,3136,3137,3153,3154,3155,3156,3157,3169,3170,3171,3172,
3173,3174,3175,3179,3181,3184,3185,3290,3306,3324,3325,3326,3327,3328,3332,3344,3345,3347,3352,3353,3354,3355,3357,3363,3365,
3373,3399,3401,3403,3404,3406,3407,3408,3412,3478,3484,3494,3523,3545,3549,3555,3557,3593,3594,3595,3596,3597,3598,3599,3600,
3601,3602,3603,3604,3605,3606,3607,3620,3622,3624,3688,3690,3703,3704,3706,3707,3963,3964,3965,3967,4087,4089,4090,4091,4092,
4138,4146,4156,4159,4160,4163,4165,4193,4204,4205,4210,4211,4212,4213,4214,4215,4217,4218,4219,4254,4258,4320,4552,4563,4564,
4565,4566,4567,4568,4573,4576,4578,4582,4583,4584,4586,4588,4591,4593,4594,4595,4596,4598,4605,4606,4607,4608,4609,4611,4614,
4616,4732,4752,4753,4772,4773,4898,4900,5113,5114,5115,5116,5117,5127,5137,5141,5142,5143,5144,5145,5146,5147,5148,5149,5150,
5153,5157,5159,5161,5164,5165,5166,5167,5171,5172,5173,5174,5177,5392,5479,5480,5482,5484,5489,5491,5492,5493,5495,5496,5497,
5498,5499,5500,5501,5504,5505,5506,5511,5513,5515,5516,5517,5518,5564,5566,5567,5612,5690,5695,5759,5784,5811,5880,5882,5883,
5884,5885,5938,5939,5941,5943,5957,5958,5994,6014,6018,6094,6251,6286,6287,6288,6289,6290,6291,6292,6295,6297,6299,6306,6387,
6707,7087,7088,7089,7230,7231,7232,7311,7312,7315,7406,7866,7867,7868,7869,7870,7871,7944,7948,7949,7953,7954,8126,8128,8140,
8141,8142,8144,8153,8306,8736,8738,9465,9584,10086,10088,10089,10090,10266,10276,10277,10278,10930,10993,11017,11025,11026,
11028,11029,11031,11037,11041,11042,11044,11046,11047,11048,11049,11050,11051,11052,11065,11066,11067,11068,11070,11071,11072,
11073,11074,11081,11083,11084,11096,11097,11098,11146,11177,11178,11397,11401,11406,11557,11865,11866,11867,11868,11869,11870,
12025,12030,12032,12042,12961,13084,13283,13417,14740,15279,15280,15283,15284,15285,15400,15501,15513,16160,16161,16253,16266,
16269,16270,16271,16272,16273,16275,16276,16277,16278,16279,16280,16366,16367,16499,16500,16501,16502,16503,16583,16588,16621,
16633,16634,16639,16640,16642,16643,16644,16646,16647,16648,16651,16652,16653,16654,16655,16658,16659,16660,16662,16663,16667,
16668,16669,16671,16672,16673,16674,16675,16676,16679,16680,16681,16684,16685,16686,16687,16688,16692,16703,16712,16719,16721,
16723,16724,16725,16726,16727,16728,16729,16731,16736,16738,16740,16741,16742,16743,16745,16746,16749,16752,16755,16756,16761,
16763,16771,16773,16774,16780,16823,17005,17089,17101,17110,17120,17121,17122,17204,17212,17214,17215,17219,17222,17245,17246,
17424,17434,17441,17442,17480,17481,17482,17483,17484,17487,17488,17504,17505,17509,17510,17511,17513,17514,17519,17520,17634,
17637,17844,17983,18018,18747,18748,18749,18751,18752,18753,18754,18755,18771,18772,18773,18774,18775,18776,18777,18779,18802,
18804,18987,18988,18993,19052,19063,19180,19184,19185,19186,19187,19248,19249,19250,19251,19252,19340,19341,19369,19478,19539,
19540,19576,19775,19778,20124,20125,20406,20407,20500,20511,20791,20791,20914,21087,22477,23127,23128,23534,24868,25099);
*/
/*
foreach($trainer_list as $trainer){
	$result6 = mysql_query("SELECT entry FROM trainer_defs WHERE entry = $trainer")  or die(mysql_error());
	if(!mysql_num_rows($result6)) print "missing trainer_defs entry id : $trainer<br />";
	}
*/
	
//is_prof: 2155,2020,2372,3911,2275,8615,7414,2581,4039,25245 casing spells

/*
// GET TRAINER SPELLS
foreach($trainer_list as $trainer){

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
			mysql_query("INSERT INTO trainer_spells (entry, base_spell, type) 
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
			mysql_query("INSERT INTO trainer_spells (entry, base_spell, type) 
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
			mysql_query("INSERT INTO trainer_spells (entry, base_spell, type) 
					VALUES ('$trainer', '$spell_id[1]', '$type')") or die(mysql_error());
		}
	}
	
	print"<br />";
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

$result = mysql_query("SELECT distinct base_spell FROM trainer_spells ORDER BY base_spell") or die(mysql_error());
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
		mysql_query("UPDATE trainer_spells SET spellcost = '$cost', reqskill = '$req_skill', reqskillvalue = '$req_value', reqlevel = '$req_level' WHERE base_spell = $spell[0]") or die(mysql_error());
	} else print "<br \> ERROR GETTING INFO FOR ID:$spell[0]<br />";
}
*/

//GET THE TEACHING SPELL
/*
$result = mysql_query("SELECT distinct base_spell FROM trainer_spells ORDER BY base_spell") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT entry FROM dbc_spell WHERE CastingTimeIndex = 1 AND 
	(EffectTriggerSpell_0 = $spell[0] OR EffectTriggerSpell_1 = $spell[0] OR EffectTriggerSpell_2 = $spell[0])") or die(mysql_error());
	
	if(mysql_num_rows($result1) != 1)
	{
		print "MULTIPLE TEACHING SPELLS FOR: $spell[0]<br />";
		while ($teaching_spell = mysql_fetch_row($result1))
			print "$teaching_spell[0]<br />";
		print "***********************************<br />";
	}
	else
	{
		$teaching_spell = mysql_fetch_row($result1);
		mysql_query("UPDATE trainer_spells SET cast_spell = $teaching_spell[0] WHERE base_spell = $spell[0]") or die(mysql_error());
	}
}
*/

//GET REQ. SPELLS
/*
$result = mysql_query("SELECT distinct base_spell FROM trainer_spells WHERE type = 0 ORDER BY base_spell") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	$result1 = mysql_query("SELECT name,rank FROM dbc_spell WHERE entry = $spell[0] AND rank LIKE '%Rank%'") or die(mysql_error());
	
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
				mysql_query("UPDATE trainer_spells SET reqspell = '$entry[0]' WHERE base_spell = '$spell[0]'") or die(mysql_error());
				print "SPELL: $spell[0] REQ: $entry[0] <br />";
			}
			else print "SPELL: $spell[0] NONE-REQ --- RANKED!!!!<br />";

		}
	}
	else print "SPELL: $spell[0] NONE-REQ <br />";
}
*/



$result = mysql_query("SELECT distinct deletespell, cast_spell FROM trainer_spells_backup where deletespell != 0") or die(mysql_error());
while ($spell = mysql_fetch_row($result)){
	mysql_query("UPDATE trainer_spells SET deletespell = $spell[0] WHERE cast_spell = $spell[1]") or die(mysql_error());

}

?>