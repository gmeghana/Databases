<html>
<title>Insert Movie</title>
<h1> Tell us about a movie that we didn't know about before </h1>

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
Title: <input type="text" name="movie_title">
<br>
Year: <input type="text" name="movie_year">
<br>

<!-- Populate Movie Rating drop down menu -->
Rating: 
<select id="RatingList" name="RatingList">
<?php
$sql_command = 'SELECT DISTINCT rating FROM Movie ORDER BY rating ASC;';
$rs = mysql_query($sql_command, $db_connection);
//retrieve and print results
while($row = mysql_fetch_assoc($rs)){
	$rating = $row['rating'];
	echo "<option value='$rating'>$rating</option>";
 } 
?>
</select>
<br>
Company:  <input type="TEXT" name="movie_company">
<br>
<br>
Genre:
<br>
<!-- Populate Genre Radio Button -->
<?php
$sql_command = 'SELECT DISTINCT genre FROM MovieGenre;';
$rs = mysql_query($sql_command, $db_connection);
//retrieve and print results
while($row = mysql_fetch_assoc($rs)){
	$genre = $row['genre'];
	echo "<input type='checkbox' name='GenreList[]' value=$genre>$genre</option>";
 } 
?>
<br>
<br>
<input type="submit" value="Add Movie"/>
</form>

<?php 
if (isset($_GET["RatingList"]) and !empty($_GET["GenreList"])) {
	// collect value of input field
	$chosenTitle = $_GET["movie_title"];
	$chosenYear = $_GET["movie_year"];
	$chosenRating = $_GET["RatingList"];
	$chosenCompany = $_GET["movie_company"];

	

	$title = mysql_real_escape_string($chosenTitle);
	$year = (int) $chosenYear;
	$rating = mysql_real_escape_string($chosenRating);
	$company = mysql_real_escape_string($chosenCompany);
	
	
	$sql_command = "SELECT id FROM MaxMovieID;";
	$max_id = mysql_query($sql_command, $db_connection);
	//create new movie id for new entry
	$maxID = mysql_fetch_assoc($max_id);
	$max_id = (int) $maxID["id"];
	$mid = $max_id + 1;
	
	$updatecommand = "UPDATE MaxMovieID SET id ='$mid' WHERE id = '$max_id';";
	mysql_query($updatecommand, $db_connection) or trigger_error(mysql_error()." in " . $updatecommand);
	"<br>";	
	$insertcommand = "INSERT INTO Movie VALUES('$mid', '$title', '$year', '$rating', '$company');";
	"<br>";
	mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand);

	foreach($_GET['GenreList'] as $chosenGenre){
		$chosenGenre = mysql_real_escape_string($chosenGenre);
		$insertcommand = "INSERT INTO MovieGenre VALUES('$mid', '$chosenGenre');";
		"<br>";
		mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand);
		"<br>";
	}
	
	echo "The movie " . $title . " has been added to our database. Thank you.";
}

//close connection to localhost
mysql_close($db_connection);
?>
</body>
</html>