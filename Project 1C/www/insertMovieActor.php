<html>
<title>Relating Actors to Movies</title>
<h1> Relate an actor to a movie that we didn't know about </h1>

<body>
<!-- Creating the variables for the drop down menu -->
<br>
Actor:
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
$sql_command = "SELECT CONCAT_WS(' ', first, last) AS Actor FROM Actor;";

//now issue the query
$rs = mysql_query($sql_command, $db_connection);
?>

<form method="get" action="<?php echo $_SERVER['PHP_SELF'];?>">

<!-- Populate Actor drop down menu -->
<select id="ActorList" name="ActorList">
<?php
//retrieve and print results
while($row = mysql_fetch_assoc($rs)){
	$actor = $row['Actor'];
	echo "<option value='$actor'>$actor</option>";
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
Role: <textarea name="role" cols="40" rows="1"></textarea>
<br>
<br>
<input type="submit" value="Add Relation"/>
</form>


<?php 
if (isset($_GET["ActorList"]) and isset($_GET["MovieList"]) and $_GET["role"]) {
	// collect value of input field
	$chosenActor = $_GET["ActorList"];
	$chosenMovie = $_GET["MovieList"];
	$chosenRole = $_GET["role"];
	
	$names = split(' ',$chosenActor);
	$first = mysql_real_escape_string($names[0]);
	$last = mysql_real_escape_string($names[1]);
	$sql_command = "SELECT id FROM Actor WHERE first = '$first' AND last = '$last';";
	$aid = mysql_query($sql_command, $db_connection);
	$aid = mysql_fetch_assoc($aid);
	$aid_in_sql = mysql_real_escape_string($aid["id"]);
	$movieTitle_in_sql = mysql_real_escape_string($chosenMovie);
	$sql_command = "SELECT id FROM Movie WHERE title= '$movieTitle_in_sql';";
	$mid = mysql_query($sql_command, $db_connection);
	$mid = mysql_fetch_assoc($mid);
	$mid_in_sql = mysql_real_escape_string($mid["id"]) ;
	$role_in_sql = mysql_real_escape_string($chosenRole);
	
	$insertcommand = "INSERT INTO MovieActor VALUES ('$mid_in_sql','$aid_in_sql','$role_in_sql');";
	mysql_query($insertcommand) or trigger_error(mysql_error()." in " . $insertcommand);
	echo "Added the relation that " . $first . " " . $last . "  played the role of " . $chosenRole . " in the movie " . $chosenMovie . " to our database.";
}

//close connection to localhost
mysql_close($db_connection);
?>
</body>
</html>

