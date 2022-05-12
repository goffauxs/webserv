#!/usr/bin/python)

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
user_name = form.getvalue('user_name')
user_message  = form.getvalue('user_message')

print ("Content-type:text/html\n\n")
print ("<html>")
print ("<head>")
print ("<title>CGI xana</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello %s %s</h2>" % (user_name, user_message))
print ("</body>")
print ("</html>")
