import sys

data = sys.stdin.read()

print("<html>")
print("<head>")
for arg in data:
	print(arg + "</br>")
print("</body>")
print("</html>")
