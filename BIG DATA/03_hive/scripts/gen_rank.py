import os
import random
from datetime import datetime, timedelta

DATA_DIR = '/home/kevin/docs'
os.makedirs(DATA_DIR, exist_ok=True)

# Generar pages.log
urls = [f"http://site{i}.com" for i in range(1, 11)]
with open(f"{DATA_DIR}/pages.log", "w") as f:
    for url in urls:
        pagerank = round(random.uniform(0, 1), 3)
        f.write(f"{url}\t{pagerank}\n")

# Generar visits.log
names = [f"user{i}" for i in range(1, 16)]
start_time = datetime(2025, 5, 1, 8, 0, 0)
with open(f"{DATA_DIR}/visits.log", "w") as f:
    for _ in range(100):
        name = random.choice(names)
        url = random.choice(urls)
        time = (start_time + timedelta(minutes=random.randint(0, 10000))).strftime('%Y-%m-%d %H:%M:%S')
        f.write(f"{name}\t{url}\t{time}\n")