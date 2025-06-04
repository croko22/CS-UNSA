CREATE TABLE logs (
    user_a STRING,
    `time` STRING,
    query STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\t';
LOAD DATA LOCAL INPATH '/home/kevin/docs/excite-small.log' OVERWRITE INTO TABLE logs;
CREATE TABLE result AS
SELECT user_a, count(1) AS log_entries
FROM logs
GROUP BY user_a
ORDER BY user_a;

-- DROP TABLES
DROP TABLE logs;
DROP TABLE result;

-- SELECT FROM TABLES
SELECT * FROM logs LIMIT 10;
SELECT * FROM result LIMIT 10;