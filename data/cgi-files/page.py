import os
import sys

print("Content-Type: text/html\r\n\r\n")
print("<html><body>")
print("<h1>CGI Received Data:</h1>")
print(f"<p>Method: {os.environ['REQUEST_METHOD']}</p>")
print(f"<p>Content-Length: {os.environ['CONTENT_LENGTH']}</p>")
print("<pre>")
data = sys.stdin.read(int(os.environ['CONTENT_LENGTH']))
print(data)
print("</pre></body></html>")