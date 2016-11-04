-- Violate the Primary Keys
-- INSERT INTO Movie VALUES (234, 'Database The Movie', 2015, 'G', 'Meghana Inc.');
-- This violates primary key constraint because we are trying to insert into the 
-- Movie table a entry with the same id as another entry that already exists in
-- the Movie table
-- ERROR 1062 (23000): Duplicate entry '234' for key 'PRIMARY'

-- INSERT INTO Actor VALUES (234, 'Rango', 'Star', 'Male', '1985-01-01', NULL);
-- This violates primary key constraint because we are trying to insert into the 
-- Actor table a entry with the same id as another entry that already exists in
-- the Actor table
-- ERROR 1062 (23000): Duplicate entry '234' for key 'PRIMARY'

-- INSERT INTO Director VALUES (2917, 'Rango Jr.', 'Star', '2005-01-01', NULL);
-- This violates primary key constraint because we are trying to insert into the 
-- Director table a entry with the same id as another entry that already exists in
-- the Director table
-- ERROR 1062 (23000): Duplicate entry '2917' for key 'PRIMARY'



-- Violate the Foreign Keys
-- INSERT INTO MovieGenre VALUES (1, 'Horror');
-- This violates our Foreign Key constraint because it is adding an mid that does not
-- exist in the list of ids in the Movie table. Our Foreign Key constraint does not
-- allow the insertion of mid that does not exist in the Movie table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'MovieGenre', CONTRAINT 'MovieGenre_ibfk_1' FOREIGN KEY ('mid') 
-- REFERENCES 'Movie' ('id'))

-- INSERT INTO MovieDirector VALUES (1, 2917);
-- This violates our Foreign Key constraint because it is adding an mid that does not
-- exist in the list of ids in the Movie table. Our Foreign Key constraint does not
-- allow the insertion of mid that does not exist in the Movie table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'MovieDirector', CONTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY ('mid') 
-- REFERENCES 'Movie' ('id'))

-- INSERT INTO MovieDirector VALUES (234, 234);
-- This violates our Foreign Key constraint because it is adding an did that does not
-- exist in the list of ids in the Director table. Our Foreign Key constraint does not
-- allow the insertion of did that does not exist in the Director table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'MovieDirector', CONTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY ('did') 
-- REFERENCES 'Director' ('id'))

-- INSERT INTO MovieActor VALUES (1, 2917, 'Villain');
-- This violates our Foreign Key constraint because it is adding an mid that does not
-- exist in the list of ids in the Movie table. Our Foreign Key constraint does not
-- allow the insertion of mid that does not exist in the Movie table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'MovieActor', CONTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('mid') 
-- REFERENCES 'Movie' ('id'))

-- INSERT INTO MovieActor VALUES (234, 5, 'Villain');
-- This violates our Foreign Key constraint because it is adding an aid that does not
-- exist in the list of ids in the Actor table. Our Foreign Key constraint does not
-- allow the insertion of did that does not exist in the Actor table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'MovieActor', CONTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('aid') 
-- REFERENCES 'Actor' ('id'))

-- INSERT INTO Review VALUES ('Sara Melvin', CURRENT_TIMESTAMP, 1, 8, 'This is awesome!' );
-- This violates our Foreign Key constraint because it is adding an mid that does not
-- exist in the list of ids in the Movie table. Our Foreign Key constraint does not
-- allow the insertion of mid that does not exist in the Movie table.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint 
-- fails('CS143', 'Review', CONTRAINT 'Review_ibfk_1' FOREIGN KEY ('mid') 
-- REFERENCES 'Movie' ('id'))



-- Check contraints
-- INSERT INTO Movie VALUES (-234, 'Database The Movie', 2015, 'G', 'Meghana Inc.');
-- This violates our check constraint because the id inserted is a negative value
-- which violates our constraint that the movie id has to be > 0

-- INSERT INTO Actor VALUES (69001, 'Rango', 'Star', 'Male', '1985-01-01', NULL);
-- This violates our check constraint because the id inserted is a greater than
-- the value of MaxPersonID which violates our constraint that the actor id has to 
-- be < MaxPersonID

-- INSERT INTO Director VALUES (-69002, 'Rango Jr.', 'Star', '2005-01-01', NULL);
-- This violates our check constraint because the id inserted is a negative value
-- which violates our constraint that the director id has to be > 0