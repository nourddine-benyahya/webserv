import os
import sys
from email.parser import BytesParser
from email.policy import default

# Ensure the upload directory exists
UPLOAD_DIR = 'uploads'
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

# Print the HTTP headers (required for CGI scripts)
print("Content-Type: text/plain\n")

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

# Process each part in the multipart form-data
if msg.is_multipart():
    for part in msg.iter_parts():
        content_disposition = part.get('Content-Disposition', '')
        if 'form-data' in content_disposition:
            # Extract the field name
            name = part.get_param('name', header='Content-Disposition')
            if name:
                # Handle file uploads
                filename = part.get_filename()
                if filename:
                    # Save the uploaded file
                    filepath = os.path.join(UPLOAD_DIR, filename)
                    with open(filepath, 'wb') as f:
                        f.write(part.get_payload(decode=True))
                    print(f"Uploaded file: {filename}")
                else:
                    # Handle regular form fields
                    value = part.get_payload(decode=True).decode('utf-8')
                    print(f"{name}: {value}")
else:
    print("The message is not multipart.")