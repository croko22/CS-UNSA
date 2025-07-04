CREATE TABLE docs (line STRING);

LOAD DATA LOCAL INPATH './pg10.txt' OVERWRITE INTO TABLE docs;

CREATE TABLE word_counts AS
SELECT w.word, count(1) AS count FROM (SELECT explode(split(line, ' ')) AS word FROM docs) w
GROUP BY w.word
ORDER BY w.word;

-- DROP TABLES
DROP TABLE docs;
DROP TABLE word_counts;

-- SELECT FROM TABLES
 SELECT * FROM docs LIMIT 10;
 SELECT * FROM word_counts LIMIT 10;
