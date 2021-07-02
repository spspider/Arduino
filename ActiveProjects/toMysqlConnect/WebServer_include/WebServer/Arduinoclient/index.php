<!DOCTYPE html>
<html>
<body>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1251" />
<h1>arduino recieve</h1>
<style>
   .green1 {
    background: green; /* Синий цвет фона */
    color: white;
   }
    .grey {
    background: grey; /* Синий цвет фона */
    color: white;
   }
    .yellow {
    background: yellow; /* Синий цвет фона */
    color: black;
   }
  </style>
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
     for ($i1 = 0; $i1 < $columns; $i1++) {     $col_name = mysql_field_name($fields, $i1);
     echo "'db".$col_name.$row1['name_pin'].":".$row1[$col_name]."'";
     }
     }
echo "-->";
///////////////////////////////////////////////////////////////////////////////////////////////

for ($i=0;$i<10;$i++){
	if (isset($_GET["a_s"][$i])){
	$actual_state=$_GET["a_s"][$i];
		$query_insert_new_actual_state ="UPDATE $table SET actual_state='$actual_state', date='$today',User='$user_agent' WHERE name_pin='".$i."'";
    	echo $query_insert_new_actual_state;
    	mysql_query($query_insert_new_actual_state, $link) or die(mysql_error());

	}

}
////////////////////////////////////////////////////////

$field_get[0]="reboot_milis";
$field_get[1]="last_update";
echo"<form action='index.php' method='post'> <table border='1'>";
for ($i=0;$i<2;$i++){
	if (isset($_GET[$field_get[$i]])){             $field_data=$_GET[$field_get[$i]];
		$delete = "DELETE FROM settings WHERE setting='$field_get[$i]'";
        mysql_query($delete, $link) or die(mysql_error());
     		$query_insert_new_reboot_millisec ="INSERT INTO settings (setting,value,date) VALUES ('$field_get[$i]','$field_data','$today')";
    		echo $query_insert_new_reboot_millisec;
    		mysql_query($query_insert_new_reboot_millisec, $link) or die(mysql_error());
      	}

$q_s_1="SELECT * FROM settings WHERE setting='$field_get[$i]'";
$r_q_1 = mysql_query($q_s_1)or die(mysql_error());
while ($row = mysql_fetch_assoc($r_q_1)) {echo "<tr><td>$field_get[$i]:</td><td>".$row['value']."</td>";
echo "<td>".$row['date']."</td></tr>";
}
}
/////////////////////////////////////////////////////////
// Выполняем SQL-запрос
$query_select_for_insert="SELECT * FROM $table";
$result_for_insert = mysql_query($query_select_for_insert);

	while ($row = mysql_fetch_assoc($result_for_insert)) {    $name_pin_i=$row['name_pin'];
	$name_pin_change=$_POST['button'][$name_pin_i];
		$new_state= $_POST['state'][$name_pin_i];
		//echo  "state:".$new_state;
	if ($name_pin_change!=""){        if ($new_state==1){$new_state_write=0;}
        if ($new_state==0){$new_state_write=1;}
    	$query_insert_new_state ="UPDATE $table SET state='$new_state_write',User='$user_agent',date='$today' WHERE name_pin='".$name_pin_change."'";
    	//echo $query_insert_new_state;
    	mysql_query($query_insert_new_state, $link) or die(mysql_error());
    }

	}






$query_select="SELECT * FROM $table";
$result = mysql_query($query_select);

if (!$result) {
    $message  = 'Неверный запрос: ' . mysql_error() . "\n";
    $message .= 'Запрос целиком: ' . $query;
    die($message);

}
//////////////////////////////////

if (mysql_num_rows($result)<10){
	for ($i=0;$i<10;$i++){
    $query_insert ="INSERT INTO $table VALUES (0,'$i',1,0,0,'$today','$user_agent')";
	mysql_query($query_insert, $link) or die(mysql_error());
	}
}


echo '<BR>Number_fields:'.mysql_num_rows($result).':number';
echo"<form action='index.php' method='post'> <table border='0'>";

$fields = mysql_list_fields($db, $table);
$columns = mysql_num_fields($fields);
echo "<tr>";
for ($i = 0; $i < $columns; $i++) {
$col_name = mysql_field_name($fields, $i) . "\n";
echo "<td>".$col_name."</td>";
}
echo "</tr>";

while ($row = mysql_fetch_assoc($result)) {
echo "<tr>";
	//$name_pin=$row['name_pin'];
    //echo $_POST['button'][$name_pin];

 for ($i1 = 0; $i1 < $columns; $i1++) {
    $col_name = mysql_field_name($fields, $i1);
	echo"<td>";
  	echo "<input type='text' size='10' maxlength='10' name='".$col_name."[".$row['name_pin']."]'  value='".$row[$col_name]."'>";
	//echo "<input type='hidden' value='db".$col_name.$row['name_pin'].":".$row[$col_name]."'";
	//echo "<!--'db".$col_name.$row['name_pin'].":".$row[$col_name]."'-->";

	echo"</td>";
 }
//echo  "<input type='submit' name='button[".$row['name_pin']."]' value='SW:[".$row['name_pin']."]' />";
if (($row['state']!=$row['actual_state'])){$button_class='yellow';}
if (($row['state']==1)&&($row['actual_state']==1)){$button_class='green1';}
if (($row['state']==0)&&($row['actual_state']==0)){$button_class='grey';}
if ($row['in_out']==1){$in_out="IN";};if ($row['in_out']==0){$in_out="OUT";}
echo " <td><button value='".$row['name_pin']."'  class='$button_class_IN_OUT' name='button_IN[".$row['name_pin']."]' >".$in_out."</button></td>";

echo " <td><button value='".$row['name_pin']."'  class='$button_class' name='button[".$row['name_pin']."]' >SW:".$row['name_pin']."</button></td>";
echo "</tr>";
//echo "<tr>";
//echo "<input type='button' value=db".$col_name.$row['name_pin'].":".$row[$col_name].":ve".$col_name.$row['name_pin'];

//echo "</tr>";

}

echo"</table> </form>";

mysql_close($link);
?>



</body>
</html>
