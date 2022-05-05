import sys

data = sys.stdin.read()

start = data.find("filename=")
end = data.find("\"", start + 1)
string = "server/uploads/" + data[start:end]

file = open(string, "a")
file.seek(0, 0)
file.truncate()
file.write(data)
file.close()
print("<html>")
print("<head>")
print("File successfully uploaded")
print("</head>")
print("</html>")
