import os

query = os.getenv("QUERY_STRING")

print(query)

strings = []
strings = query.split("&")

file = open("out", "a")
file.seek(0, 0)
file.truncate()

i = 0
len = len(strings)
while i < len:
	file.write(strings[i])
	file.write("\n")
	i += 1

file.close()