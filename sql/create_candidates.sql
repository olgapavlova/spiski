USE spiski;

DROP TABLE IF EXISTS candidates;

CREATE TABLE IF NOT EXISTS candidates (
	id VARCHAR(15) PRIMARY KEY,
	points INT,
	department VARCHAR(15),
	priority INT
); 

INSERT INTO candidates (id) SELECT DISTINCT id FROM inbox;
