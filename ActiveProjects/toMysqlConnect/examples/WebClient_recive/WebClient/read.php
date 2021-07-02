<html>

<head>
  <title></title>
</head>

<body>

<?php

$db='u883914490_user';
$Passdb='5506487';
$table='pins';






$link = mysql_connect('mysql.hostinger.com.ua', $db, $Passdb)
    or die('Не удалось соединиться: ' . mysql_error());
echo 'Connected';
mysql_select_db($db) or die('Не удалось выбрать базу данных');

$today = date("Y-m-d H:i:s");
$user_agent = $_SERVER['HTTP_USER_AGENT'];

///////////////////////////////////////////////////////////////////////////////////ARDUINO
$query_select1="SELECT * FROM $table";
$result1 = mysql_query($query_select1);

$fields = mysql_list_fields($db, $table);
$columns = mysql_num_fields($fields);

echo "<!--";
    while ($row1 = mysql_fetch_assoc($result1)) {
     for ($i1 = 0; $i1 < $columns; $i1++) {

     $col_name = mysql_field_name($fields, $i1);
     echo "'db".$col_name.$row1['name_pin'].":".$row1[$col_name]."'";
     }
     }
echo "-->";
///////////////////////////////////////////////////////////////////////////////////////////////
	$query_for_arduino_settings="SELECT * FROM settings";
	$result_for_arduino_settings = mysql_query($query_for_arduino_settings)or die(mysql_error());
    while ($row = mysql_fetch_assoc($result_for_arduino_settings)) {
	echo "<!--<br>".$row['setting']."=".$row['value']."L-->";
	}
/////////////////////////////////////////////////////////
for ($i=0;$i<10;$i++){
	if (isset($_GET["a_s"][$i])){
	$actual_state=$_GET["a_s"][$i];
		$query_insert_new_actual_state ="UPDATE $table SET actual_state='$actual_state', date='$today',User='$user_agent' WHERE name_pin='".$i."'";
    	echo $query_insert_new_actual_state;
    	mysql_query($query_insert_new_actual_state, $link) or die(mysql_error());

	}

}
////////////////////////////////////////////////////////


?>

</body>

</html>