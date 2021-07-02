<html>

<head>
  <title></title>
</head>

<body>

<?php





$db='u922482446_user';
$Passdb='5506487';
$table='pins';






$link = mysqli_connect('mysql.hostinger.com.ua', $db, $Passdb)
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
?>

</body>

</html>