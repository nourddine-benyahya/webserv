<?php
// process.php
header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Transmission Received</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="container">
        <div class="transmission-container reveal">
            <h1 class="title">Transmission Received</h1>
            
            <div class="transmission-content">
                <?php
                echo '<div class="data-section">';
                echo '<h2 class="section-title">Text Data</h2>';
                echo '<div class="data-item">';
                echo '<span class="data-label">Name:</span>';
                echo '<span class="data-value">'.htmlspecialchars($_POST['name'] ?? '').'</span>';
                echo '</div>';
                
                echo '<div class="data-item">';
                echo '<span class="data-label">Email:</span>';
                echo '<span class="data-value">'.htmlspecialchars($_POST['email'] ?? '').'</span>';
                echo '</div>';
                
                echo '<div class="data-item">';
                echo '<span class="data-label">Message:</span>';
                echo '<div class="message-content">'.nl2br(htmlspecialchars($_POST['message'] ?? '')).'</div>';
                echo '</div>';

                if(isset($_FILES['file']) && $_FILES['file']['error'] == UPLOAD_ERR_OK) {
                    $uploadDir = 'uploads/';
                    if(!is_dir($uploadDir)) mkdir($uploadDir);
                    
                    $filename = basename($_FILES['file']['name']);
                    $targetPath = $uploadDir.$filename;
                    
                    if(move_uploaded_file($_FILES['file']['tmp_name'], $targetPath)) {
                        echo '<h2 class="section-title">File Received</h2>';
                        echo '<div class="file-download">';
                        echo '<a href="'.$targetPath.'" class="download-btn">';
                        echo '<i class="fas fa-download"></i>';
                        echo 'Download '.htmlspecialchars($filename);
                        echo '</a>';
                        echo '</div>';
                    }
                }
                echo '</div>';
                ?>
            </div>
            
            <div class="back-link">
                <a href="index.php" class="test-btn">
                    <i class="fas fa-arrow-left"></i>
                    Return to Main Page
                </a>
            </div>
        </div>
    </div>
    
    <script src="script.js"></script>
</body>
</html>