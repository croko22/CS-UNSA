import random

WORDS = [
    "hive", "big", "data", "python", "spark", "sql", "table", "query", "cluster",
    "mapreduce", "analytics", "database", "column", "row", "partition", "load",
    "select", "insert", "update", "delete", "schema", "index", "view", "join"
]

lines = 1000  # Number of lines to generate
words_per_line = 10  # Words per line

with open('/home/kevin/docs/excite-small.log', 'w') as f:
    for _ in range(lines):
        line = ' '.join(random.choice(WORDS) for _ in range(words_per_line))
        f.write(line + '\n')