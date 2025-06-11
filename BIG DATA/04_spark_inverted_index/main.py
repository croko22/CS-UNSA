from pyspark import SparkContext
import os

sc = SparkContext("local", "InvertedIndexRDD")

folder_path = "files"
files_rdd = sc.wholeTextFiles(os.path.join(folder_path, "*.txt"))

result = (
    files_rdd
    .flatMap(lambda x: [(word.lower(), os.path.basename(x[0])) for word in x[1].split()])
    .groupByKey()
    .mapValues(lambda doc_list: list(set(doc_list)))
)

output = result.collect()
with open("output.txt", "w") as f:
    for word, files in output:
        f.write(f"{word}: {files}\n")
        
print("data saved output.txt")