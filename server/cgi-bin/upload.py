import cgi, os

form = cgi.FieldStorage()

fileitem = form['file1']

if fileitem.filename:
	fn = os.path.basename(fileitem.filename)
	open("server/uploads/" + fn, 'wb').write(fileitem.file.read())
	message = "The file '" + fn + "' was uploaded successfully"

else:
	message = "No file was uploaded"

print """\
Content-Type: text/html\n
<html><body>
<p>%s</p>
</body></html>
""" % (message,)
