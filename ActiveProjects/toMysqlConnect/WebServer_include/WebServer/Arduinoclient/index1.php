<!DOCTYPE html>
<html>
<body>

<h1>arduino recieve</h1>





<?php
if (!$_GET["indata"]){
$str = "Now data:".$_GET ["indata"]."<br>";

echo $str;
}
else{
echo "<br>";
}
$indata=$_GET["indata"];
$field=$_GET["field"];
$field_data=$_GET["field_data"];


$link = mysql_connect('mysql.hostinger.com.ua', 'u883914490_user', '5506487')
    or die('�� ������� �����������: ' . mysql_error());
echo '���������� ������� �����������';
mysql_select_db('u883914490_user') or die('�� ������� ������� ���� ������');

// ��������� SQL-������

$user_agent = $_SERVER['HTTP_USER_AGENT'];





$query_select="SELECT * FROM datadb";
$result = mysql_query($query_select);

if (!$result) {
    $message  = '�������� ������: ' . mysql_error() . "\n";
    $message .= '������ �������: ' . $query;
    die($message);
}


echo '<BR>Number_fields:'.mysql_num_rows($result).':number';
echo"<table border='1'>";
while ($row = mysql_fetch_assoc($result)) {
echo "<tr>";
    echo "<td>".$row['id']."</td>";
    echo "<td>".$row['data1']."</td>";
echo "<td>".$row['checkfield']."</td>";
echo "</tr>";

}
echo"<table>";
//////////////////////////////////
if ($_GET["indata"]!=""){
$query_insert ="INSERT INTO datadb VALUES (0,'$indata','$indata')";
mysql_query($query_insert, $link) or die(mysql_error());
}
if ($_GET["field"]!=""){
$query_select_settings="SELECT * FROM settings WHERE field = '$field'";
$result_select_settings = mysql_query($query_select_settings) or die('������ ��');
$total_rows=mysql_num_rows($result_select_settings);
echo  "<br>".$total_rows;
if ($total_rows==0){$query_insert_new ="INSERT INTO settings VALUES (0,'$field','$field_data','$user_agent')";
$result_insert_new = mysql_query($query_insert_new) or die('������ ��');}
else{$change_id= mysql_result ( $result_select_settings ,0, 'id' );
echo "field:".mysql_result ( $result_select_settings ,0, 'id' ) ;
//$field_data_new=
//$query_select_for_insert_new="SELECT * FROM settings WHERE id = '$change_id'";//$result_query_select_for_insert_new = mysql_query($query_select_for_insert_new) or die('������ ��');

$query_insert_new_fielddata ="UPDATE  settings SET field_data = '$field_data', user='$user_agent' WHERE id = '$change_id'";
$result_insert_new_fielddata = mysql_query($query_insert_new_fielddata) or die('������ ��');
}
//$query_insert ="INSERT INTO settings VALUES (0,'$indata','$indata')";
//mysql_query($query_insert, $link) or die(mysql_error());
}
/////////////////////////////////////
//mysql_query($query_select, $link) or die(mysql_error());

// ��������� ����������




mysql_close($link);
?>



</body>
</html>
