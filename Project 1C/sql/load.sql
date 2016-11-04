# Load movie.del into Movie Table
LOAD DATA LOCAL INFILE '~/data/movie.del' INTO TABLE Movie 
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

# Load actor1.del, actor2.del, actor3.del into Actor Table
LOAD DATA LOCAL INFILE '~/data/actor1.del' INTO TABLE Actor
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE '~/data/actor2.del' INTO TABLE Actor
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE '~/data/actor3.del' INTO TABLE Actor
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Load director.del into Director Table
LOAD DATA LOCAL INFILE '~/data/director.del' INTO TABLE Director
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Load moviegenre.del into MovieGenre Table
LOAD DATA LOCAL INFILE '~/data/moviegenre.del' INTO TABLE MovieGenre
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Load moviedirector.del into MovieDirector Table
LOAD DATA LOCAL INFILE '~/data/moviedirector.del' INTO TABLE MovieDirector
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Load movieactor1.del and movieactor2.del into MovieActor Table
LOAD DATA LOCAL INFILE '~/data/movieactor1.del' INTO TABLE MovieActor
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE '~/data/movieactor2.del' INTO TABLE MovieActor
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Load (69000) into MaxPersonID Table
INSERT INTO MaxPersonID VALUES (69000);

-- Load (4750) into MaxMovieID Table
INSERT INTO MaxMovieID VALUES (4750);