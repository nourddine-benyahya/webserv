#!/usr/bin/env python3
import os
import sys
import html
from email.parser import BytesParser
from email.policy import default

# Ensure the upload directory exists
UPLOAD_DIR = 'html/cgi-scripts/uploads'
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

# Print the HTTP headers (required for CGI scripts)
print("Content-Type: text/html\n")

# Read the entire input from stdin (request body)
request_body = sys.stdin.buffer.read()  # Use .buffer to read binary data

# Extract the boundary from the Content-Type header
content_type = os.environ.get('CONTENT_TYPE', '')
if 'boundary=' not in content_type:
    print("Error: No boundary found in Content-Type header.")
    sys.exit(1)

boundary = content_type.split('boundary=')[1].strip()

# Construct the full multipart body with boundaries
full_body = b''
full_body += b'Content-Type: ' + content_type.encode('utf-8') + b'\r\n\r\n'
full_body += request_body

# Parse the multipart form-data
parser = BytesParser(policy=default)
msg = parser.parsebytes(full_body)

# Initialize variables to store form data
name = ""
email = ""
message = ""
filename = None

# Process each part in the multipart form-data

#if the message is multipart, iterate over the parts
if msg.is_multipart():
    for part in msg.iter_parts():
        content_disposition = part.get('Content-Disposition', '')
        if 'form-data' in content_disposition:
            # Extract the field name
            field_name = part.get_param('name', header='Content-Disposition')
            if field_name:
                # Handle file uploads
                if part.get_filename():
                    filename = part.get_filename()
                    filepath = os.path.join(UPLOAD_DIR, filename)
                    with open(filepath, 'wb') as f:
                        f.write(part.get_payload(decode=True))
                else:
                    # Handle regular form fields
                    value = part.get_payload(decode=True).decode('utf-8')
                    if field_name == 'name':
                        name = value
                    elif field_name == 'email':
                        email = value
                    elif field_name == 'message':
                        message = value

# Check if a file was received
file_received = filename is not None

if file_received:
    file_section = (
        "<h2 class='section-title'>File Received</h2>"
        "<div class='file-download'>"
        "<a href='" + os.path.join(UPLOAD_DIR, filename).replace(os.sep, '/') + "' class='download-btn'>"
        "<i class='fas fa-download'></i> "
        "Download " + html.escape(filename) +
        "</a>"
        "</div>"
    )
else:
    file_section = ""


html_response = (
    "<!DOCTYPE html>"
    "<html lang='en'>"
    "<head>"
    "<meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>Transmission Received</title>"
    "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css'>"
    "<link rel='stylesheet' href='style.css'>"
    "</head>"
    "<body>"
    "<div class='container'>"
    "<div class='transmission-container reveal'>"
    "<h1 class='title'>Transmission Received Using python CGI</h1>"
    "<div class='transmission-content'>"
    "<div class='data-section'>"
    "<h2 class='section-title'>Text Data</h2>"
    "<div class='data-item'>"
    "<span class='data-label'>Name:</span>"
    "<span class='data-value'>" + html.escape(name) + "</span>"
    "</div>"
    "<div class='data-item'/>Email:</span>"
    "<span class='data-value'>" + html.escape(email) + "</span>"
    "</div>"
    "<div class='data-item'>"
    "<span class='data-label'>Message:</span>"
    "<div class='message-content'>" + html.escape(message).replace('\n', '<br>') + "</div>"
    "</div>"
    + file_section +
    "</div>"
    "</div>"
    "<div class='back-link'>"
    "<a href='index.php' class='test-btn'>"
    "<i class='fas fa-arrow-left'></i>"
    "Return to Main Page"
    "</a>"
    "</div>"
    "</div>"
    "</div>"
    "<script src='script.js'></script>"
    "</body>"
    "</html>"
)

print(html_response)