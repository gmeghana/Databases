<html>
	<html>
	<head>
		<title>Search Actor / Movie</title>
	</head>	
	<body>	
		<h1>Search</h1>
		<form action="./search.php" method="GET" action="<?php echo $_SERVER['PHP_SELF'];?>">		
			Search: <input name="input" placeholder = "Search for Actor/Movie" type="text" name="keyword"></input>
			<input type="submit" value="Search"/>
		</form>
		<p>Directions: Enter actor in this format [first last]</p>
		<br/>
	</body>

<?php 
if ($_GET["input"]) {
	// collect value of input field
	$input = $_GET['input'];
	$input_words = explode(" ", $input); 
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

echo "<h2>Search results for $input: </h2>";
$queries = array();

$queries[0] = "SELECT DISTINCT id, first, last, dob FROM Actor WHERE first LIKE '%$input_words[0]%' AND last like '%$input_words[1]%' ORDER BY first, last";
$queries[1] = "SELECT DISTINCT id, first, last, dob FROM Actor WHERE last LIKE '%$input_words[0]%' ORDER BY first, last";
$queries[2] = "SELECT DISTINCT id, title, year FROM Movie WHERE title LIKE '%$input_words[0]%' AND title LIKE '%$input_words[1]%' ORDER BY title";

echo "<h4> Searching match records in the Actor database...</h4>";
	
$rs0 = mysql_query($queries[0], $db_connection);
$rs1 = mysql_query($queries[1], $db_connection);


if ($rs0 && mysql_num_rows($rs0)>0){
	echo "<p>";
	while ($row = mysql_fetch_row($rs0)){
		echo "<a href = './actorInfo.php?id=$row[0]'>";
		$dob = date("F d, Y", strtotime($row[3]));
		echo "$row[1] $row[2]</a>  ($dob)";
		echo "<br/>";
	} 
	echo "</p>";
}
elseif ($rs1 && mysql_num_rows($rs1)>0) {
	echo "<p>";
	while ($row = mysql_fetch_row($rs1)){
		echo "<a href = './actorInfo.php?id=$row[0]'>";
		$dob = date("F d, Y", strtotime($row[3]));
		echo "$row[1] $row[2]</a>  ($dob)";
		echo "<br/>";
	} 
	echo "</p>";
} 
else { 
	echo "<p><b> No Actor found with \"$input\"</b></p>"; 
}


echo "<h4> Searching match records in the Movie database...</h4>";
	
$rs2 = mysql_query($queries[2], $db_connection);

if ($rs2){
	echo "<p>";
	while ($row = mysql_fetch_row($rs2)){
		echo "<a href = './movieInfo.php?id=$row[0]'>";
		echo "$row[1]</a> ($row[2])";
		echo "<br/>";
	} 
	echo "</p>";
} 
else {
	echo "<p><b> No Movie found with \"$input\"</b></p>"; 
}

//close connection to localhost
mysql_close($db_connection);?>




</html>