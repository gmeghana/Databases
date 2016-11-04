<html>
<h1> CS143 Project 1B By Sara Melvin and Meghana Ginjpalli</h1>

<body>
<p>Type an SQL query in the following box:</p>
<p>Example: SELECT * FROM Actor WHERE id=10; </p>
<br>
<p></p>
<form method="get" action="<?php echo $_SERVER['PHP_SELF'];?>">
<textarea name="query" cols="60" rows="8"></textarea>
<br>
<input type="submit" value="Submit"></form>
<p></p>
</body>
<?php 
if ($_GET["query"]) {
	// collect value of input field
	$sql_command = $_GET['query'];
	
}
//making the connection to locahost on default port 3306 username cs143
$db_connection = mysql_connect("localhost", "cs143", ""); 

//checking proper connection
if(!$db_connection){
	$errmsg = mysql_error($db_connection);
	print "Connection failed: $errmsg <br />";
	exit(1);
}

//Establish connection to database
mysql_select_db("CS143", $db_connection); 

//now issue the query
$rs = mysql_query($sql_command, $db_connection);

//print the results of the sql query in a table
echo "<h2>Results from MySQL:</h2>";

//count the number of columns in the sql result
$fields_num = mysql_num_fields($rs);

echo "<table border='1'><tr>";

//print the column names 
for($i=0; $i<$fields_num; $i++)
{
    $field = mysql_fetch_field($rs);
    echo "<td>{$field->name}</td>";
}
echo "</tr>\n";

//retrieve and print results
while($row = mysql_fetch_row($rs)){
	echo "<tr>";

	foreach($row as $cell)
        echo "<td>$cell</td>";

    echo "</tr>\n";
}

//close connection to localhost
mysql_close($db_connection);?>

</html>