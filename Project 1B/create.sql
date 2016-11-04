-- Movie Table
CREATE TABLE Movie (
    id INT NOT NULL,
	title VARCHAR(100) NOT NULL,
	year INT,
	rating VARCHAR(10),
	company VARCHAR(50),
	PRIMARY KEY(id), 
	CHECK(id <= MaxMovieiD(id) AND id > 0)
) ENGINE=INNODB;
-- This check constraint will check whether the id will be greater than zero
-- and less than the maximum movie id.
-- The NOT NULL means the attribute cannot be a NULL value
-- The primary key constraint makes sure that the id is a unique value in the table

-- Actor Table
CREATE TABLE Actor(
	id INT NOT NULL,
	last VARCHAR(20) NOT NULL,
	first VARCHAR(20) NOT NULL,
	sex VARCHAR(6),
	dob DATE NOT NULL,
	dod DATE,
	PRIMARY KEY (id),
	CHECK( id <= MaxPersonID(id) AND id > 0)
) ENGINE=INNODB;
-- This check constraint will check whether the id will be greater than zero
-- and less than the maximum person id.
-- The NOT NULL means the attribute cannot be a NULL value
-- The primary key constraint makes sure that the id is a unique value in the table

-- Director Table
CREATE TABLE Director(
	id INT NOT NULL,
	last VARCHAR(20) NOT NULL,
	first VARCHAR(20) NOT NULL,
	dob DATE NOT NULL,
	dod DATE,
	PRIMARY KEY (id),
	CHECK( id < MaxPersonID(id) AND id > 0)
) ENGINE=INNODB;
-- This check constraint will check whether the id will be greater than zero
-- and less than the maximum person id.
-- The NOT NULL means the attribute cannot be a NULL value
-- The primary key constraint makes sure that the id is a unique value in the table

-- MovieGenre Table
CREATE TABLE MovieGenre(
	mid INT NOT NULL,
	genre VARCHAR(20),
	FOREIGN KEY(mid) REFERENCES Movie(id)
) ENGINE=INNODB;
-- This foreign key constraint makes sure that all movie ids in this table correlate
-- to the movie ids in the Movie table.
-- The NOT NULL means the attribute cannot be a NULL value, this keeps the keys unique.

-- MovieDirector Table
CREATE TABLE MovieDirector(
	mid INT NOT NULL,
	did INT NOT NULL,
	FOREIGN KEY(mid) REFERENCES Movie(id),
	FOREIGN KEY(did) REFERENCES Director(id)
) ENGINE=INNODB;
-- This foreign key constraint makes sure that all movie ids in this table correlate
-- to the movie ids in the Movie table.
-- This other foreign key constraint makes sure that all director ids in this table 
-- correlate to the director ids in the Director table.
-- The NOT NULL means the attribute cannot be a NULL value, this keeps the keys unique.

-- MovieActor Table
CREATE TABLE MovieActor(
	mid INT NOT NULL,
	aid INT NOT NULL,
	role VARCHAR(50),
	FOREIGN KEY(mid) REFERENCES Movie(id),
	FOREIGN KEY(aid) REFERENCES Actor(id)
) ENGINE=INNODB;
-- This foreign key constraint makes sure that all movie ids in this table correlate
-- to the movie ids in the Movie table.
-- This other foreign key constraint makes sure that all actor ids in this table 
-- correlate to the actor ids in the Actor table.
-- The NOT NULL means the attribute cannot be a NULL value, this keeps the keys unique.

-- Review Table
CREATE TABLE Review(
    name VARCHAR(20),
	time TIMESTAMP,
	mid INT NOT NULL,
	rating INT,
	comment VARCHAR(500),
	FOREIGN KEY(mid) REFERENCES Movie(id),
	CHECK( rating<=10 AND rating>=0)
) ENGINE=INNODB;
-- This foreign key constraint makes sure that all movie ids in this table correlate
-- to the movie ids in the Movie table.
-- In addition, this check contraint makes sure that the rating given in the review
-- is less than or equal to 10 but greater than or equal to 0.
-- The NOT NULL means the attribute cannot be a NULL value, this keeps the keys unique.

-- MaxPersonID Table
CREATE TABLE MaxPersonID(
	id INT
) ENGINE=INNODB;

-- MaxMovieID Table
CREATE TABLE MaxMovieID(
	id INT
) ENGINE=INNODB;