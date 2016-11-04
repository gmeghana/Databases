<html>
	<html>
	<head>
		<title>Movie Information</title>
	</head>	
	<body>
		<h1>Movie Information</h1>
		<h4>Challenge: Click on different movie and actor links until you get to Tom Hanks! If you win, pat yourself on the back! You are a Moviepedia star!</h4>
	</body>

<?php 

if ($_GET['id']) {
	$id = $_GET['id'];
}
else {
	$id = 4224;
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

$movie = "SELECT title, rating, year, company FROM Movie WHERE id = '$id'";
$movie = mysql_query($movie, $db_connection);

$director = "SELECT first, last, dob, id FROM Director D, MovieDirector MD WHERE MD.mid = '$id' AND MD.did = D.id";
$director = mysql_query($director, $db_connection);

$actor = "SELECT first, last, dob, id, role FROM Actor A, MovieActor MA WHERE MA.mid = '$id' AND MA.aid = A.id ORDER BY first, last";
$actor = mysql_query($actor, $db_connection);

$genre = "SELECT genre FROM MovieGenre WHERE mid = '$id'";
$genre = mysql_query($genre, $db_connection);

$review = "SELECT name, time, mid, rating, comment FROM Review WHERE mid = '$id'";
$review = mysql_query($review, $db_connection);


$rating = "SELECT AVG(rating) FROM Review WHERE mid = '$id' GROUP BY mid";
$rating = mysql_query($rating, $db_connection);



if (($movie && mysql_num_rows($movie) > 0) || $director || $genre) {
	$movie = mysql_fetch_row($movie);
	
	echo "<h2>$movie[0]</h2>";
	echo "<p>";
	echo "Title: <b>$movie[0]</b><br/>";
	echo "Production Company: <b>$movie[3]</b><br/>";
	echo "MPAA Rating: <b>$movie[1]</b><br/>";

	$dir = mysql_fetch_row($director);
	echo "Director: <b>$dir[0] $dir[1]</b><br/>";

	$gen = mysql_fetch_row($genre);
	echo "Genre: <b>$gen[0]</b><br/>";

	echo "</br></br>";

	echo "<b>Cast: </b><br>";
	if ($actor && mysql_num_rows($actor) > 0){
		while ($act = mysql_fetch_row($actor)){
			echo "<a href = './actorInfo.php?id=$act[3]'>";
			for($a=0; $a<2; $a++){
				echo " $act[$a]";
			}
			echo "</a> ";
			echo "as \"" .$act[4]. "\"<br/>";
		}			
	} 
	
	echo "</p>";
	echo "<b>Reviews: </b>";
	echo "</br>";
	echo "<a href = './insertComments.php'> Add Comment</a></br>";
	echo "Average rating: <b> ";
		if ($rating && mysql_num_rows($rating) > 0) {
			$avgRating = mysql_fetch_row($rating);
			if ($avgRating[0] > 0)	
				echo "$avgRating[0]</b>";
		}
		else {
			echo "No Rating</b>";
		}
	
	if ($review && mysql_num_rows($review)>0){
		while($rev = mysql_fetch_row($review)){
			echo "<p>$rev[0] rated this movie $rev[3] stars</br>";
			echo "Comment: $rev[4]</p>";	
		}
	} 
	else { 
		echo "<p> No reviews yet </p>"; 
	}
	
} 


//close connection to localhost
mysql_close($db_connection);?>

</html>
