import datetime
import random
import os

def generate_log_file_for_hive(filename='/home/kevin/docs/excite-small.log', num_entries=5000):
    users = ["user" + str(i) for i in range(1, 21)]
    queries = [
        "search 'big data'",
        "browse 'hadoop tutorial'",
        "search 'apache hive'",
        "view 'spark examples'",
        "login",
        "logout",
        "search 'machine learning'",
        "download 'report.pdf'",
        "upload 'data.csv'",
        "filter 'category:tech'"
    ]

    # Asegura que el directorio exista
    os.makedirs(os.path.dirname(filename), exist_ok=True)

    with open(filename, 'w') as f:
        for i in range(num_entries):
            user = random.choice(users)
            start_date = datetime.datetime.now() - datetime.timedelta(days=30)
            random_minutes = random.randint(0, 30 * 24 * 60)
            timestamp = start_date + datetime.timedelta(minutes=random_minutes)
            time_str = timestamp.strftime("%Y-%m-%d %H:%M:%S")
            query = random.choice(queries)
            f.write(f"{user}\t{time_str}\t{query}\n")

if __name__ == "__main__":
    generate_log_file_for_hive()