<?php
ini_set('max_execution_time',0);
mysql_connect("127.0.0.1:3306","root","xxx") or die(mysql_error());

$table = "accounts";
$order = "acct";

mysql_select_db("antrix_db_old") or die(mysql_error());
$result = mysql_query("SELECT `$order` FROM $table ORDER BY `$order`") or die(mysql_error());


$fp = fopen($table.".sql", 'w+');
fseek($fp,0,SEEK_END);
 
 
while ($object = mysql_fetch_row($result)){

	mysql_select_db("antrix_db_new") or die(mysql_error());
	$result1 = mysql_query("SELECT `$order` FROM $table WHERE `$order` = $object[0]") or die(mysql_error());
	if (!mysql_num_rows($result1)){
	
	
		mysql_select_db("antrix_db_old") or die(mysql_error());
		$query = mysql_query("SELECT * FROM $table WHERE `$order` = $object[0]") or die(mysql_error());


		$num_fields = mysql_num_fields($query);
		$numrow = mysql_num_rows($query);
		$row_counter = 0;
 

	fwrite($fp, "INSERT INTO $table VALUES (");
	for ($i =0; $i<$numrow; $i++) {
		$row_counter++;
		$row = mysql_fetch_row($query);
		for($j=0; $j<$num_fields; $j++) {
			$row[$j] = addslashes($row[$j]);
			$row[$j] = ereg_replace("\n","\\n",$row[$j]);
			if (isset($row[$j])) {
				if (mysql_field_type($query,$j) == "int") fwrite($fp, "$row[$j]");
					else fwrite($fp, "'$row[$j]'");
				}else fwrite($fp, "''");
			if ($j<($num_fields-1)) fwrite($fp, ",");
			}
	}

	fwrite($fp, ");\n");

	}


}
fclose($fp);
?>