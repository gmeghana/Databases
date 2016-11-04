<html>
<title>Relating Directors to Movies</title>
<h1> Relate a director to a movie that we didn't know about </h1>

<body>
<!-- Creating the variables for the drop down menu -->
<br>
Director:
<?php //making the connection to locahost on default port 3306 username cs143
$db_connection = mysql_connect("localhost", "cs143", ""); 

//checking proper connection
if(!$db_connection){
	$errmsg = mysql_error($db_connection);
	print "Connection failed: $errmsg <br />";
	exit(1);
}

//Establish connection to database
mysql_select_db("CS143", $db_connection); 

//Get actor names FIRST LAST
$sql_command = "SELECT CONCAT_WS(' ', first, last) AS Director FROM Director;";

//now issue the query
$rs = mysql_query($sql_command, $db_connection);
?>

<form method="get" action="<?php echo $_SERVER['PHP_SELF'];?>">

<!-- Populate Actor drop down menu -->
<select id="DirectorList" name="DirectorList">
<?php
//retrieve and print results
while($row = mysql_fetch_assoc($rs)){
	$director = $row['Director'];
	echo "<option value='$director'>$director</option>";
 } 
?>
</select>

<!-- Now for the movie name drop down menu -->
<?php 
//Get Movie names
$sql_command = "SELECT title FROM Movie;";

//now issue the query
$rs = mysql_query($sql_command, $db_connection);
?>
<br>
<br>
Movie:
<br>
<select id="MovieList" name="MovieList">
<?php
//retrieve and print results
while($row = mysql_fetch_assoc($rs)){
	$movie = $row['title'];
	echo "<option value='$movie'>$movie</option>";
} 
?>
</select>
<br>
<br>
<input type="submit" value="Add Relation"/>
</form>


<?php 
if (isset($_GET["DirectorList"]) and isset($_GET["MovieList"])) {
	// collect value of input field
	$chosenDirector = $_GET["DirectorList"];
	$chosenMovie = $_GET["MovieList"];
	
	$names = split(' ',$chosenDirector);
	$first = mysql_real_escape_string($names[0]);
	$last = mysql_real_escape_string($names[1]);
	$sql_command = "SELECT id FROM Director WHERE first = '$first' AND last = '$last';";
	$did = mysql_query($sql_command, $db_connection);
	$did = mysql_fetch_assoc($did);
	$did_in_sql = mysql_real_escape_string($did["id"]);
	$movieTitle_in_sql = mysql_real_escape_string($chosenMovie);
	$sql_command = "SELECT id FROM Movie WHERE title= '$movieTitle_in_sql';";
	$mid = mysql_query($sql_command, $db_connection);
	$mid = mysql_fetch_assoc($mid);
	$mid_in_sql = mysql_real_escape_string($mid["id"]) ;
	
	$insertcommand = "INSERT INTO MovieDirector VALUES ('$mid_in_sql','$did_in_sql');";
	mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand);
	echo "Added the relation that " . $first . " " . $last . " directed the movie " . $chosenMovie . " to our database.";
}

//close connection to localhost
mysql_close($db_connection);
?>
</body>
</html>