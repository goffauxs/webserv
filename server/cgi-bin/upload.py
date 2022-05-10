import sys

data = sys.stdin.read()

# len = len(data)
# for i in range(len):
# 	start = data[i].find("filename=")
# 	end = data[i].find("\"", start + 10)
# 	if start > -1:
# 		break

# string = data[i][start + 10:end]
# file = open("server/uploads/" + string, "a")
# file.seek(0, 0)
# file.truncate()

# for i in range(len):
# 	if i > 3 and i < len - 2:
# 		file.write(data[i])

# file.close()
print("<html>")
print("<head>")
print("File ")
print(data)
print(" successfully uploaded")
print("</head>")
print("</html>")

# data = sys.stdin.read()

# file = open("upload", "a")
# file.seek(0, 0)
# file.truncate()
# file.write(data)
# file.close()
# print("<html>")
# print("<head>")
# print("File successfully uploaded")
# print("</head>")
# print("</html>")
