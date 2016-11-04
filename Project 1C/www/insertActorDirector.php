<html>
<title>Insert Actor or Director</title>
<h1> Tell us about an actor or director that we didn't know about before </h1>

<body>
<!-- Creating the variables for the drop down menu -->
<h2> Add New Actor:</h2>

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
First Name: <input type="text" name="actor_first">
<br>
Last Name:  <input type="text" name="actor_last">
<br>
Date of Birth: <input type="date" name="actor_dob"> 
year-month-day 
<br>
Date of Death: <input type="date" name="actor_dod">
year-month-day (If still living, just leave box blank)

<br>
Gender:
<br>
<input type="radio" name="GenderList" value="Male" checked> Male
<br>
<input type="radio" name="GenderList" value="Female"> Female
<br>
<br>
<input type="submit" value="Add Actor"/>
</form>

<?php 
if ($_GET["actor_first"] and $_GET["actor_last"] and $_GET["actor_dob"]  and isset($_GET["GenderList"])) {
	// collect value of input field
	$chosenActorFirst = $_GET["actor_first"];
	$chosenActorLast = $_GET["actor_last"];
	$chosenDOB = $_GET["actor_dob"]; 
	$chosenGender = $_GET["GenderList"];
	if ($_GET["actor_dod"]){
		$chosenDOD = $_GET["actor_dod"]; //this can be null if actor is still alive
		$dod = date('Y-m-d', strtotime($chosenDOD));
	}
	
	$first = mysql_real_escape_string($chosenActorFirst);
	$last = mysql_real_escape_string($chosenActorLast);
	$dob = date('Y-m-d', strtotime($chosenDOB));
	$sex = mysql_real_escape_string($chosenGender);
	
	$sql_command = "SELECT id FROM MaxPersonID;";
	$max_id = mysql_query($sql_command, $db_connection);
	//create new actor id for new entry
	$maxID = mysql_fetch_assoc($max_id);
	$max_ID = (int) $maxID["id"];
	$aid = $max_ID + 1;
	"<br>";

	$updatecommand = "UPDATE MaxPersonID SET id ='$aid' WHERE id = '$max_ID';";
	mysql_query($updatecommand, $db_connection) or trigger_error(mysql_error()." in " . $updatecommand);
	"<br>";	
	if ($_GET["movie_dob"]){
		$insertcommand = "INSERT INTO Actor VALUES('$aid', '$last', '$first', '$sex', '$dob', $dod);";
	}
	else{
		$insertcommand = "INSERT INTO Actor VALUES('$aid', '$last', '$first', '$sex', '$dob', NULL );";
	}
	"<br>";
	"<br>";
	mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand); 
	
	echo "The actor " . $first . " " .  $last . " has been added to our database. Thank you.";
}

?>

<h2> Add New Director </h2>
<form method="get" action="<?php echo $_SERVER['PHP_SELF'];?>">
First Name: <input type="text" name="director_first">
<br>
Last Name:  <input type="text" name="director_last">
<br>
Date of Birth: <input type="date" name="director_dob"> 
year-month-day
<br>
Date of Death: <input type="date" name="director_dod">
year-month-day (If still living, just leave box blank)
<br>
<br>
<input type="submit" value="Add Director"/>
</form>

<?php 
if ($_GET["director_first"] and $_GET["director_last"] and $_GET["director_dob"]) {
	// collect value of input field
	$chosenDirFirst = $_GET["director_first"];
	$chosenDirLast = $_GET["director_last"];
	"<br>";
	$chosenDirDOB = $_GET["director_dob"]; 
	"<br>";
	if ($_GET["director_dod"]){
		$chosenDirDOD = $_GET["director_dod"]; //this can be null if actor is still alive
		$dirdod = date('Y-m-d', strtotime($chosenDirDOD));
	}
	$dirdob = date('Y-m-d', strtotime($chosenDirDOB));
	
	$sql_command = "SELECT id FROM MaxPersonID;";
	$max_per_id = mysql_query($sql_command, $db_connection);
	//create new director id for new entry
	$maxPerID = mysql_fetch_assoc($max_per_id);
	$max_Per_ID = (int) $maxPerID["id"];
	$did = $max_Per_ID + 1;
	"</br>";
	
	$updatecommand = "UPDATE MaxPersonID SET id ='$did' WHERE id = '$max_Per_ID';";
	mysql_query($updatecommand, $db_connection) or trigger_error(mysql_error()." in " . $updatecommand);
	"<br>";	
	if ($_GET["director_dod"]){
		$insertcommand = "INSERT INTO Director VALUES('$did', '$chosenDirLast', '$chosenDirFirst', '$dirdob', '$dirdod');";
	}
	else{
		$insertcommand = "INSERT INTO Director VALUES('$did', '$chosenDirLast', '$chosenDirFirst', '$dirdob', NULL );";
	}
	"<br>";
	"<br>";
	mysql_query($insertcommand, $db_connection) or trigger_error(mysql_error()." in " . $insertcommand); 
	
	echo "The Director " . $first . " " .  $last . " has been added to our database. Thank you.";
}

//close connection to localhost
mysql_close($db_connection);
?>

</body>
</html>
