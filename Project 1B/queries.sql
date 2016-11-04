-- Give me the names of actors in the movie 'Die Another Day'
SELECT CONCAT_WS(' ', first, last) AS Actor
FROM Actor, Movie, MovieActor
WHERE Movie.title = 'Die Another Day' AND Actor.id = MovieActor.aid AND Movie.id = MovieActor.mid;

-- Give me the count of all the actors who acted in multiple movies
SELECT COUNT(*) AS Total_Number_Of_Actors_In_Multiple_Movies
FROM (SELECT aid FROM MovieActor GROUP BY aid HAVING COUNT(*)>1) Stars;

-- Our own query -- The number of deceased people who were actors AND directors
SELECT COUNT(dod) AS Number_Of_Deceased_Directors_Actors
FROM Director
WHERE id IN (SELECT id FROM Actor);


