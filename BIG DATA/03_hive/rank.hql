set hive.auto.convert.join = false;

-- VISITS
CREATE TABLE visits (name STRING, url STRING, visit_time STRING)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\t';
STORED AS TEXTFILE;
LOAD DATA LOCAL INPATH '/home/kevin/docs/visits.log'
OVERWRITE INTO TABLE visits;

CREATE TABLE pages (url STRING, pagerank DECIMAL)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\t';
STORED AS TEXTFILE;
LOAD DATA LOCAL INPATH '/home/kevin/docs/pages.log'
OVERWRITE INTO TABLE pages;

-- RANK RESULT
CREATE TABLE rank_result AS

SELECT pr.name FROM

(SELECT V.name, AVG(P.pagerank) AS prank

FROM visits V JOIN pages P ON (V.url = P.url)

GROUP BY name) pr

WHERE pr.prank > 0.5;

-- DROP TABLES
DROP TABLE visits;
DROP TABLE pages;
DROP TABLE rank_result;

-- DROP TABLES
 SELECT * FROM pages LIMIT 10;
