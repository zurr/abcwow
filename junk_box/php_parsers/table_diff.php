<?php

ini_set('max_execution_time',0);
mysql_connect("127.0.0.1:3306","root","xxx") or die(mysql_error());


$table_arr = array(
  array("ai_threattospellid","spell",0),
  array("areatriggers","entry",0),
  array("creature_names","entry",0),
  array("creature_proto","entry",0),
  array("creature_quest_finisher","id",0),
  array("creature_quest_starter","id",0),
  array("gameobject_names","entry",0),
  array("gameobject_quest_finisher","id",0),
  array("gameobject_quest_item_binding","entry",0),
  array("gameobject_quest_pickup_binding","entry",0),
  array("gameobject_quest_starter","id",0),
  array("itempetfood","entry",0),
  array("quests","entry",0),
  array("trainer_defs","entry",0),
  array("trainer_spells","entry",0),
  array("creature_spawns","entry",355681),
  array("gameobject_spawns","entry",279183),
 );
 
foreach($table_arr as $table_ex){
 
    $table = $table_ex[0];
	$order = $table_ex[1];
	
	mysql_select_db("antrix_abc_new") or die(mysql_error());
	$result = mysql_query("SELECT DISTINCT $order FROM $table ORDER BY $order") or die(mysql_error());

	$fp = fopen("diff/".$table.".sql", 'w+');
	fseek($fp,0,SEEK_END);
 
	while ($object = mysql_fetch_row($result)){

		mysql_select_db("antrix_db") or die(mysql_error());
		$result1 = mysql_query("SELECT $order FROM $table WHERE $order = $object[0] LIMIT 1") or die(mysql_error());
		if (!mysql_num_rows($result1)){
			mysql_select_db("antrix_abc_new") or die(mysql_error());
			$query = mysql_query("SELECT * FROM $table WHERE `$order` = $object[0]") or die(mysql_error());

			$num_fields = mysql_num_fields($query);
			$numrow = mysql_num_rows($query);
			$row_counter = 0;
 
			
			for ($i =0; $i<$numrow; $i++) {
				fwrite($fp, "INSERT INTO $table VALUES (");
				if ($table_ex[2]){
					$table_ex[2]++;
					fwrite($fp, "$table_ex[2],");
					$start = 1;
				else $start = 0;

				$row_counter++;
				$row = mysql_fetch_row($query);
				for($j=$start; $j<$num_fields; $j++) {
					$row[$j] = addslashes($row[$j]);
					$row[$j] = ereg_replace("\n","\\n",$row[$j]);
					if (isset($row[$j])) {
						if (mysql_field_type($query,$j) == "int") fwrite($fp, "$row[$j]");
						else fwrite($fp, "'$row[$j]'");
					}else fwrite($fp, "''");
					if ($j<($num_fields-1)) fwrite($fp, ",");
				}
				fwrite($fp, ");\n");
			}
		}
	}
	fclose($fp);
}
?>