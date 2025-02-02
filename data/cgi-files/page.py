import cgi

# Create an instance of FieldStorage to parse the query string
form = cgi.FieldStorage()

# Iterate over all form fields (both GET and POST data)
for field in form.keys():
    value = form.getvalue(field)
    print(f"Key: {field}, Value: {value}")
