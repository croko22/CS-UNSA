data = [
    ["Name", "url", "time"],
    ["Amy", "www.cnn.com", "8:00"],
    ["Amy", "www.crap.com", "8:05"],
    ["Amy", "www.myblog.com", "10:00"],
    ["Amy", "www.flickr.com", "10:05"],
    ["Fred", "cnn.com/index.htm", "12:00"],
    ["Fred", "cnn.com/index.htm", "13:00"],
]

with open("/home/kevin/docs/visits.log", "w") as f:
    for row in data:
        f.write('\t'.join(row) + '\n')
