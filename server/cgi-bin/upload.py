import sys

# data = sys.stdin.readlines()
# print ("Content-type:text/html\n")
# print ("<html>")
# print ("<head>")
# print (data)
# print ("</body>")
# print ("</html>")
for line in sys.stdin:
    if 'Exit' == line.rstrip():
        break
    print(f'Processing Message from sys.stdin *****{line}*****')
print("Done")
