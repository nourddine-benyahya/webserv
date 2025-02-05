<?php

// Set response header
header("Content-Type: text/plain");

// Read and display QUERY parameters (GET method)
echo "=== QUERY PARAMETERS (GET) ===\n";
if (!empty($_GET)) {
    foreach ($_GET as $key => $value) {
        echo "$key: $value\n";
    }
} else {
    echo "No query parameters\n";
}
echo "\n";

// Read and display POST body (Raw Data or Form Data)
echo "=== POST BODY ===\n";
$rawInput = file_get_contents("php://input");
if (!empty($rawInput)) {
    echo "Raw Body:\n$rawInput\n";
} else {
    echo "No body data\n";
}
echo "\n";

// Read and display FORM DATA (application/x-www-form-urlencoded)
echo "=== FORM DATA (POST) ===\n";
if (!empty($_POST)) {
    foreach ($_POST as $key => $value) {
        echo "$key: $value\n";
    }
} else {
    echo "No form data\n";
}
echo "\n";

// Read and display UPLOADED FILES (multipart/form-data)
echo "=== UPLOADED FILES ===\n";
if (!empty($_FILES)) {
    foreach ($_FILES as $key => $file) {
        if ($file["error"] === UPLOAD_ERR_OK) {
            echo "File Field: $key\n";
            echo "Original Name: " . $file["name"] . "\n";
            echo "Temp Path: " . $file["tmp_name"] . "\n";
            echo "Size: " . $file["size"] . " bytes\n";
            echo "Type: " . $file["type"] . "\n";

            // Move file to a permanent location
            $destination = "/tmp/" . basename($file["name"]);
            if (move_uploaded_file($file["tmp_name"], $destination)) {
                echo "File saved to: $destination\n";
            } else {
                echo "Failed to move file\n";
            }
        } else {
            echo "File upload error (code: " . $file["error"] . ")\n";
        }
    }
} else {
    echo "No uploaded files\n";
}

?>
