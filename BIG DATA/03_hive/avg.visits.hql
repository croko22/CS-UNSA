CREATE TABLE pages (name STRING, url STRING, time STRING)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ' ';

LOAD DATA LOCAL INPATH './data/visits.log'
OVERWRITE INTO TABLE pages;

CREATE TABLE avg_visit AS
SELECT AVG(num_pages) FROM
(SELECT name, count(1) AS num_pages
FROM pages
GROUP BY name) np;