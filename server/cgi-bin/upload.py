import cgi, os

form = cgi.FieldStorage()

fileitem = form['file1']

path = "server/uploads/"

isExist = os.path.exists(path)

if not isExist:
	os.makedirs(path)

if fileitem.filename:
	fn = os.path.basename(fileitem.filename)
	open(path + fn, 'wb').write(fileitem.file.read())
	message = "The file '" + fn + "' was uploaded successfully"

else:
	message = "No file was uploaded"

print """\
<html><body>
<p>%s</p>
</body></html>
""" % (message,)


# import sys

# data = sys.stdin.read()

# file = open("server/uploads/file.jpeg", "a")
# file.seek(0, 0)
# file.truncate()

# file.write(data)

# file.close()
# print("<html>")
# print("<head>")
# print("File ")
# print(" successfully uploaded")
# print("</head>")
# print("</html>")
# test new cgi

# data = sys.stdin.readlines()

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
# 	if i > 3 and i < len - 1:
# 		file.write(data[i])

# file.close()
# print("<html>")
# print("<head>")
# print("File ")
# print(string)
# print(" successfully uploaded")
# print("</head>")
# print("</html>")
