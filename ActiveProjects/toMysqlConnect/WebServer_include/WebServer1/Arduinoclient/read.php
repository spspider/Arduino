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
//echo "<!--";
///////////////////////////////////////////////////////////////////////////////////ARDUINO
$query_select="SELECT * FROM $table";
$result = mysql_query($query_select);
$fields = mysql_list_fields($db, $table);
$columns = mysql_num_fields($fields);//10
$row_t=0;
while ($row = mysql_fetch_array($result)) {
	for ($column = 0; $column < $columns; $column++) {
	$col_name = mysql_field_name($fields, $column);
	$Array_table[$col_name][$row_t]=$row[$col_name];
	}
	$row_t++;
}
	echo "<br>---";
	$name_pin = mysql_field_name($fields, 1);
	$col_name = mysql_field_name($fields, 2);
	for ($row = 0; $row < mysql_num_rows($result); $row++) {
	echo "<br>'db".$col_name.$Array_table[$name_pin][$row].":".$Array_table[$col_name][$row]."'";
	}
	$col_name = mysql_field_name($fields, 3);
	for ($row = 0; $row < mysql_num_rows($result); $row++) {
	echo "<br>'db".$col_name.$Array_table[$name_pin][$row].":".$Array_table[$col_name][$row]."'";
	}

///////////////////////////////////////////////////////////////////////////////////////////////
	$query_for_arduino_settings="SELECT * FROM settings";
	$result_for_arduino_settings = mysql_query($query_for_arduino_settings)or die(mysql_error());
    while ($row = mysql_fetch_assoc($result_for_arduino_settings)) {
	echo "<br>".$row['setting']."=".$row['value']."L";
	}
//echo "--!>";


?>

</body>

</html>