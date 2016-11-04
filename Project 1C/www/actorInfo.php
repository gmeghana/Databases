<html>
	<html>
	<head>
		<title>Actor Information</title>
	</head>	
	<body>	
		<h1>Actor Information</h1>
		<h4>Challenge: Click on different movie and actor links until you get to Tom Hanks! If you win, pat yourself on the back! You are a Moviepedia star!</h4>

	</body>

<?php 

if ($_GET['id']) {
	$id = $_GET['id'];
}
else {
	$id = 15927;
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

$actor = "SELECT first, last, dob, dod, sex FROM Actor WHERE id = '$id'";
$actor = mysql_query($actor, $db_connection);

$movie = "SELECT title, role, mid, year FROM MovieActor MA, Movie M WHERE MA.aid = '$id' AND MA.mid = M.id";
$movie = mysql_query($movie, $db_connection);

if ($actor && mysql_num_rows($actor) > 0) {
	$act = mysql_fetch_row($actor);
	echo "Name: <b>$act[0] $act[1]</b><br/>";
	echo "Gender: <b>$act[4]</b><br/>";
	$dob = date("F d, Y", strtotime($act[2]));
	echo "Birth: <b>$dob</b><br/>";
	if ($act[3])
		$dod = date("F d, Y", strtotime($act[3])); 
	echo "Death: <b>$dod</b><br/>"; 
} 

echo "</br></br>";
echo "<b> Filmography: </b></br>";

if ($movie && mysql_num_rows($movie) > 0) {
	while ($mov = mysql_fetch_row($movie)){
		echo "<a href = './movieInfo.php?id=$mov[2]'>";
		echo "$mov[0]</a> as \"$mov[1]\" </br>";
	}
} 

//close connection to localhost
mysql_close($db_connection);?>

</html>
