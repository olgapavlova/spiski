USE spiski;

DROP TABLE IF EXISTS departments;

CREATE TABLE IF NOT EXISTS departments (
	id VARCHAR(15) PRIMARY KEY,
	name VARCHAR(100),
	budget INT,
	inbox INT,
	counter INT DEFAULT 0
); 

LOAD DATA INFILE '/var/lib/mysql-files/departments.csv'
	INTO TABLE departments
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	IGNORE 1 LINES
	(id, name, budget, inbox)
;

