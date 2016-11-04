<html>
<title>Insert Comments</title>
<h1> Tell us what you think about a movie </h1>

<body>
<!-- Creating the variables for the drop down menu -->
<h2> Add New Movie:</h2>

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
?>

<form method="get" action="<?php echo $_SERVER['PHP_SELF'];?>">
Your Name: <input type="text" name="reviewer_name">
<br>
<!-- Now for the movie name drop down menu -->
<?php 
//Get Movie names
$sql_command = "SELECT title FROM Movie;";

//now issue the query
$rs = mysql_query($sql_command, $db_connection);
?>
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
Rating: <input type="number" name="rating" min="1" max ="10">
<br>
Enter a score between 1 and 10 where 10 means the movie was awesome.
<br>
<br>
Comment:  
<br>
<textarea name="comment" cols="60" rows="8"></textarea>
<br>
<br>
<input type="submit" value="Add Comment"/>
</form>

<?php 
if ($_GET["comment"] and $_GET["rating"] and $_GET["reviewer_name"] and isset($_GET["MovieList"])) {
	// collect value of input field
	$chosenName = $_GET["reviewer_name"];
	$chosenRating = $_GET["rating"];
	$chosenComment = $_GET["comment"];
	$chosenMovie = $_GET["MovieList"];
	"</br>";
	$name = mysql_real_escape_string($chosenName);
	$rating = mysql_real_escape_string($chosenRating);
	$comment = mysql_real_escape_string($chosenComment);
	"</br>";
	
	//get mid
	$movieTitle_in_sql = mysql_real_escape_string($chosenMovie);
	$sql_command = "SELECT id FROM Movie WHERE title= '$movieTitle_in_sql';";
	$mid = mysql_query($sql_command, $db_connection);
	$mid = mysql_fetch_assoc($mid);
	$mid_in_sql = mysql_real_escape_string($mid["id"]) ;
	
	"<br>";	
	$insertcommand = "INSERT INTO Review VALUES('$name', NOW(), '$mid_in_sql', '$rating', '$comment');";
	
	mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand);
	"<br>";
	echo $name . "! Your review on " . $chosenMovie . " has been added to our database. Thank you.";
}

//close connection to localhost
mysql_close($db_connection);
?>
</body>
</html>
