import sys

print ("Content-type:text/html\n")
print ("<html>")
print ("<head>")
data = sys.stdin.readlines()
print (data)
print ("</body>")
print ("</html>")