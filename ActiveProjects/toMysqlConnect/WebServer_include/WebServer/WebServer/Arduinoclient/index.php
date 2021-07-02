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
$db='u922482446_user';
$Passdb='5506487';
$table='pins';






$link = mysql_connect('mysql.hostinger.com.ua', $db, $Passdb)
    or die('Не удалось соединиться: ' . mysql_error());
echo 'Connected';
mysql_select_db($db) or die('Не удалось выбрать базу данных');

date_default_timezone_set('Europe/Zaporozhye');
//echo date_default_timezone_get();

$today = date("Y-m-d H:i:s");
$user_agent = $_SERVER['HTTP_USER_AGENT'];
//include 'read.php';
///////////////////////////////////////////////////////////////////////////////////ARDUINO
$query_select1="SELECT * FROM $table ORDER BY name_pin ASC";
$result1 = mysql_query($query_select1);

$fields = mysql_list_fields($db, $table);
$columns = mysql_num_fields($fields);

//echo "<!--";
    while ($row1 = mysql_fetch_assoc($result1)) {
     for ($i1 = 0; $i1 < $columns; $i1++) {
     //$col_name = mysql_field_name($fields, $i1);
     //echo "'db".$col_name.$row1['name_pin'].":".$row1[$col_name]."'";

     }
     $col_name = mysql_field_name($fields, 2);
     echo "______________________________________________I".$row1['name_pin'].":".$row1[$col_name].";";
     $col_name = mysql_field_name($fields, 3);
     echo "______________________________________________S".$row1['name_pin'].":".$row1[$col_name].";";
     $col_name = mysql_field_name($fields, 5);
     echo "______________________________________________P".$row1['name_pin'].":".$row1[$col_name].";";
     $col_name = mysql_field_name($fields, 6);
     echo "______________________________________________L".$row1['name_pin'].":".$row1[$col_name]."";

     }
//echo "-->";
///////////////////////////////////////////////////////////////////////////////////////////////
	$query_for_arduino_settings="SELECT * FROM settings";
	$result_for_arduino_settings = mysql_query($query_for_arduino_settings)or die(mysql_error());
    while ($row = mysql_fetch_assoc($result_for_arduino_settings)) {
			echo "<br>___________________________________".$row['setting']."=".$row['value']."L";
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

$field_get[0]="reboot_milis";
$field_get[1]="last_update";
echo"<form action='index.php' method='get'> <table border='1'>";
for ($i=0;$i<2;$i++){
	$query_is_isset="SELECT * FROM settings WHERE setting='$field_get[$i]'";
	$result_is_isset = mysql_query($query_is_isset)or die(mysql_error());
	if (mysql_num_rows($result_is_isset)==0){
    $query_insert ="INSERT INTO settings (setting,value,date) VALUES ('$field_get[$i]','0','$today')";
	echo $query_insert;
	mysql_query($query_insert, $link) or die(mysql_error());
	}

	if (isset($_GET[$field_get[$i]])&&($_GET[$field_get[$i]]!="")){
	     $field_value= $_GET[$field_get[$i]];
         $query_count="SELECT * FROM settings WHERE setting='$field_get[$i]'";
         $result_count = mysql_query($query_count)or die(mysql_error());
         if (mysql_num_rows($result_count)>1){
   		 $delete = "DELETE FROM settings WHERE setting='$field_get[$i]'";
     	 echo $delete;
     	 mysql_query($delete, $link) or die(mysql_error());
   		}
		if (mysql_num_rows($result_count)==1){
			while ($row = mysql_fetch_assoc($result_count)) {
     		$query_update ="UPDATE settings SET setting='$field_get[$i]', value='$field_value',date='$today' WHERE id = '".$row['id']."'";
    		echo $query_update;
    		mysql_query($query_update, $link) or die(mysql_error());
   			}
   		}

    }
$q_s_1="SELECT * FROM settings WHERE setting='$field_get[$i]'";
$r_q_1 = mysql_query($q_s_1)or die(mysql_error());
while ($row = mysql_fetch_assoc($r_q_1)) {echo "<tr><td>$field_get[$i]:</td><td>".$row['value']."</td><td>".$row['date']."</td></tr>";
}
}
//while ($row1 = mysql_fetch_assoc($r_q_1)) {
//echo "<tr><td></td></tr>";//кнопка пробуждения
//}
echo "</table></form>";
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// button

$Button_name[0]="wakeup";
$Button_name[1]="test";
echo"<form action='index.php' method='get'> <table border='1'>";
for ($i=0;$i<2;$i++){	$query_is_isset="SELECT * FROM settings WHERE setting='$Button_name[$i]'";
	$result_is_isset = mysql_query($query_is_isset)or die(mysql_error());
	if (mysql_num_rows($result_is_isset)==0){    $query_insert ="INSERT INTO settings (setting,value,date) VALUES ('$Button_name[$i]','0','$today')";
	echo $query_insert;
	mysql_query($query_insert, $link) or die(mysql_error());
	}

	if (isset($_GET[$Button_name[$i]])&&($_GET[$Button_name[$i]]!="")){
	     $Button_value= $_GET[$Button_name[$i]];
         $query_count="SELECT * FROM settings WHERE setting='$Button_name[$i]'";
         $result_count = mysql_query($query_count)or die(mysql_error());
         if (mysql_num_rows($result_count)>1){
   		 $delete = "DELETE FROM settings WHERE setting='$Button_value[$i]'";
     	 echo $delete;
     	 mysql_query($delete, $link) or die(mysql_error());
   		}
		if (mysql_num_rows($result_count)==1){			while ($row = mysql_fetch_assoc($result_count)) {
     		$query_update ="UPDATE settings SET setting='$Button_name[$i]', value='$Button_value',date='$today' WHERE id = '".$row['id']."'";
    		echo $query_update;
    		mysql_query($query_update, $link) or die(mysql_error());
   			}
   		}

    }
$query_display="SELECT * FROM settings WHERE setting='$Button_name[$i]'";
$result_display = mysql_query($query_display)or die(mysql_error());
while ($row = mysql_fetch_assoc($result_display)) {	if ($row['value']!=1){$change_value=1;}
	//if ($row['value']==1){$change_value=0;}
echo "<tr><td><button value='".$change_value."'  name='".$Button_name[$i]."'>".$Button_name[$i].$change_value."</button></td><td>".$row['value']."</td><td>".$row['date']."</td></tr>";
}
}

echo "</table></form>";
// Выполняем SQL-запрос
$query_select_for_insert="SELECT * FROM $table ORDER BY name_pin ASC";
$result_for_insert = mysql_query($query_select_for_insert);

	while ($row = mysql_fetch_assoc($result_for_insert)) {    $name_pin=$row['name_pin'];
	$new_state_value=$_POST['button'][$name_pin];
	$new_in_out_value=$_POST['buttonIN'][$name_pin];
	$new_PWM_status=$_POST['PWM_status'][$name_pin];
	//$new_state= $_POST['state'][$name_pin_i];
		//echo  "state:".$new_state;
		//echo ">"
	if ((isset($_POST['button'][$name_pin]))&&($row['state']!=$new_state_value)){    	$query_insert_new_state ="UPDATE $table SET state='$new_state_value',User='$user_agent',date='$today' WHERE name_pin='".$name_pin."'";
    	echo "<br>".$query_insert_new_state;
    	mysql_query($query_insert_new_state, $link) or die(mysql_error());
    }
    	if ((isset($_POST['buttonIN'][$name_pin]))&&($row['in_out']!=$new_in_out_value)){
    	$query_insert_new_state ="UPDATE $table SET in_out='$new_in_out_value' WHERE name_pin='".$name_pin."'";
    	echo $query_insert_new_state;
    	mysql_query($query_insert_new_state, $link) or die(mysql_error());
    }
     $Post_is= "pwm_st";
        if(   (isset($_POST[$Post_is][$name_pin]))&&( $row[$Post_is]!=$_POST[$Post_is][$name_pin] ) ){
       //if ((isset($_POST['PWM_ststus'][$name_pin]))&&($row['PWM_ststus']!=($_POST['PWM_status'][$name_pin]))) {
    	$query_insert_new_PWM_length ="UPDATE $table SET $Post_is='".$_POST[$Post_is][$name_pin]."' WHERE name_pin='".$name_pin."'";
    	echo $query_insert_new_PWM_length;
    	mysql_query($query_insert_new_PWM_length, $link) or die(mysql_error());
    }
     $Post_is= "pwm_len";
        if(   (isset($_POST[$Post_is][$name_pin]))&&( $row[$Post_is]!=$_POST[$Post_is][$name_pin] ) ){
       //if ((isset($_POST['PWM_ststus'][$name_pin]))&&($row['PWM_ststus']!=($_POST['PWM_status'][$name_pin]))) {
    	$query_insert_new_PWM_length ="UPDATE $table SET $Post_is='".$_POST[$Post_is][$name_pin]."' WHERE name_pin='".$name_pin."'";
    	echo $query_insert_new_PWM_length;
    	mysql_query($query_insert_new_PWM_length, $link) or die(mysql_error());
    }
         $Post_is= "comment";
        if(   (isset($_POST[$Post_is][$name_pin]))&&( $row[$Post_is]!=$_POST[$Post_is][$name_pin] ) ){
       //if ((isset($_POST['PWM_ststus'][$name_pin]))&&($row['PWM_ststus']!=($_POST['PWM_status'][$name_pin]))) {
    	$query_insert_new_PWM_length ="UPDATE $table SET $Post_is='".$_POST[$Post_is][$name_pin]."' WHERE name_pin='".$name_pin."'";
    	echo $query_insert_new_PWM_length;
    	mysql_query($query_insert_new_PWM_length, $link) or die(mysql_error());
    }
	}






$query_select="SELECT * FROM $table ORDER BY name_pin ASC";
$result = mysql_query($query_select);

if (!$result) {
    $message  = 'Неверный запрос: ' . mysql_error() . "\n";
    $message .= 'Запрос целиком: ' . $query;
    die($message);

}

echo"<form action='index.php' method='post'> <table border='1'>";

$fields = mysql_list_fields($db, $table);
$columns = mysql_num_fields($fields);
echo "<tr>";
//for ($i = 0; $i < $columns; $i++) {
$col_name = mysql_field_name($fields, 1) . "\n";echo "<td>".$col_name."</td>";
$col_name = mysql_field_name($fields, 2) . "\n";echo "<td>".$col_name."</td>";
$col_name = mysql_field_name($fields, 3) . "\n";echo "<td>".$col_name."</td>";
$col_name = mysql_field_name($fields, 4) . "\n";echo "<td>".$col_name."</td>";
$col_name = mysql_field_name($fields, 5) . "\n";echo "<td>".$col_name."</td>";
$col_name = mysql_field_name($fields, 6) . "\n";echo "<td>".$col_name."</td>";
//}
echo "</tr>";

while ($row = mysql_fetch_assoc($result)) {
echo "<tr>";

    $col_name = mysql_field_name($fields, 1); 	echo"<td>";echo $row[$col_name];echo"</td>";//name pin;
	$col_name = mysql_field_name($fields, 2);
	//	echo"<td>";	echo $row[$col_name]; echo"</td>";
	if ($row['in_out']==1){$in_out="IN";$value=0;};if ($row['in_out']!=1){$in_out="OUT";$value=1;}
	echo " <td><button value='".$value."'  class='$button_class_IN_OUT' name='buttonIN[".$row['name_pin']."]' >".$in_out."</button></td>";

	$col_name = mysql_field_name($fields, 3);  	echo"<td>";echo $row[$col_name];  echo"</td>"; //state
	$col_name = mysql_field_name($fields, 4); 	echo"<td>";	echo $row[$col_name];	echo"</td>"; //actual state
	$col_name = mysql_field_name($fields, 5);  echo"<td>"; echo "<input type='text' maxlength='3' size='3' name=".$col_name."[".$row['name_pin']."] value='".$row[$col_name]."'>";echo"</td>"; //PWM status
	$col_name = mysql_field_name($fields, 6);  echo"<td>"; echo "<input type='text' maxlength='3' size='3' name=".$col_name."[".$row['name_pin']."] value='".$row[$col_name]."'>";echo"</td>"; //PWM length


if (($row['state']!=$row['actual_state'])){$button_class='yellow';}
if (($row['state']>0)&&($row['actual_state']>0)){$button_class='green1';}
if (($row['state']==0)&&($row['actual_state']==0)){$button_class='grey';}

if (($row['name_pin']==9)||($row['name_pin']==10)||($row['name_pin']==11)||($row['name_pin']==3)||($row['name_pin']==5)||($row['name_pin']==6)){
echo " <td><button value='3'  class='$button_class' name='button[".$row['name_pin']."]' >D:".$row['name_pin']."</button></td>";}
else{echo " <td><button value='2'  class='$button_class' name='button[".$row['name_pin']."]' >P:".$row['name_pin']."</button></td>";
}




if ($row['state']!=1){$Button_value=1;}
if ($row['state']==1){$Button_value=0;}

//echo " <td><button value='".$row['name_pin']."'  class='$button_class_IN_OUT' name='button_IN[".$row['name_pin']."]' >".$in_out."</button></td>";
echo " <td><button value='".$Button_value."'  class='$button_class' name='button[".$row['name_pin']."]' >SW:".$row['name_pin']."</button></td>";
	$col_name = "comment";  echo"<td>"; echo "<input type='text' maxlength='20' size='20' name=".$col_name."[".$row['name_pin']."] value='".$row[$col_name]."'>";echo"</td>"; //comment

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
