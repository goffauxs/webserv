import sys

data = sys.stdin.read()

file = open("upload", "a")
file.seek(0, 0)
file.truncate()
file.write(data)
file.close()
print("<html>")
print("<head>")
print("File successfully uploaded")
print("</head>")
print("</html>")

# print("<html>")
# print("<head>")
# for arg in data:
# 	print(arg + "</br>")
# print("</body>")
# print("</html>")
