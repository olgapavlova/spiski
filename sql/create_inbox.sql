USE spiski;

DROP TABLE IF EXISTS inbox;

CREATE TABLE IF NOT EXISTS inbox(
	id VARCHAR(15),
	points INT,
	priority INT,
	department VARCHAR(15)
); 

LOAD DATA INFILE '/var/lib/mysql-files/inbox.csv'
	INTO TABLE inbox
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	IGNORE 1 LINES
;

UPDATE inbox SET department=SUBSTRING(department, 1, 8);
