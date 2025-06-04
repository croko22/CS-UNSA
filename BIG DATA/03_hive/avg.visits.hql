CREATE TABLE pages (name STRING, url STRING, visit_time STRING)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\t';

LOAD DATA LOCAL INPATH '/home/kevin/docs/visits.log'
OVERWRITE INTO TABLE pages;

CREATE TABLE avg_visit AS
SELECT AVG(num_pages) FROM
(SELECT name, count(1) AS num_pages
FROM pages
GROUP BY name) np;

-- DROP TABLES
DROP TABLE pages;
DROP TABLE avg_visit;

-- SELECT FROM TABLES
SELECT * FROM pages LIMIT 10;
SELECT * FROM avg_visit LIMIT 10;