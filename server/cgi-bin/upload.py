# import sys

# data = sys.stdin.readlines()
# print ("Content-type:text/html\n")
# print ("<html>")
# print ("<head>")
# print (data)
# print ("</body>")
# print ("</html>")
import fileinput

for line in fileinput.input():
    print(line)