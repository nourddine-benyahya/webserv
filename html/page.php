<?php
// process.php
header("Content-Type: text/html");
phpinfo();
?>
<!DOCTYPE html>
<html>
<head>
    <title>Transmission Received</title>
    <style type="text/css">
    @import url('https://fonts.googleapis.com/css?family=Audiowide&display=swap');
    
    html, body {
        margin: 0;
        padding: 0;
        height: 100%;
        background: radial-gradient(circle, #240015 0%, #12000b 100%);
        color: #ff005d;
        font-family: 'Audiowide', cursive;
    }

    .container {
        max-width: 800px;
        margin: 50px auto;
        padding: 20px;
        border: 2px solid #ff005d;
        box-shadow: 0 0 15px #ff005d;
        animation: hueRotate 6s ease-in-out infinite;
    }

    h1 {
        text-align: center;
        text-shadow: 0 0 10px #ff005d;
        animation: flicker4 5s linear infinite;
    }

    .form-group {
        margin: 20px 0;
    }

    label {
        display: block;
        margin-bottom: 5px;
        text-shadow: 0 0 5px #ff005d;
    }

    input, textarea, select {
        width: 100%;
        padding: 10px;
        background: #12000a;
        border: 1px solid #ff005d;
        color: #ff005d;
        font-family: 'Audiowide', cursive;
        transition: all 0.3s ease;
    }

    input:focus, textarea:focus, select:focus {
        outline: none;
        box-shadow: 0 0 10px #ff005d;
    }

    button {
        background: #ff005d;
        color: #12000a;
        border: none;
        padding: 15px 30px;
        font-family: 'Audiowide', cursive;
        cursor: pointer;
        transition: all 0.3s ease;
    }

    button:hover {
        box-shadow: 0 0 20px #ff005d;
        transform: scale(1.05);
    }

    @keyframes drawLine1 {
  0%  {stroke-dashoffset: -940px;}
  100%{stroke-dashoffset: 0px;}
}

@keyframes drawLine2 {
  0%  {stroke-dashoffset: -735px;}
  100%{stroke-dashoffset: 0px;}
}

@keyframes drawLine3 {
  0%  {stroke-dashoffset: -940px;}
  100%{stroke-dashoffset: 0px;}
}

@keyframes flicker1{
  0%  {stroke: #ff005d;}
  1%  {stroke: transparent;}
  3%  {stroke: transparent;}
  4%  {stroke: #ff005d;}
  6%  {stroke: #ff005d;}
  7%  {stroke: transparent;}
  13% {stroke: transparent;}
  14% {stroke: #ff005d;}
  100%{stroke: #ff005d;}
}

@keyframes flicker2{
  0%  {stroke: #ff005d;}
  50% {stroke: #ff005d;}
  51% {stroke: transparent;}
  61% {stroke: transparent;}
  62% {stroke: #ff005d;}
  100%{stroke: #ff005d;}
}

@keyframes flicker3{
  0%  {stroke: #ff005d;}
  1%  {stroke: transparent;}
  10% {stroke: transparent;}
  11% {stroke: #ff005d;}
  40% {stroke: #ff005d;}
  41% {stroke: transparent;}
  45% {stroke: transparent;}
  46% {stroke: #ff005d;}
  100%{stroke: #ff005d;}
}

@keyframes flicker4{
  0%  {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  30% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  31% {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  32% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  36% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  37% {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  41% {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  42% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  85% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  86% {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  95% {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  96% {color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
  100%{color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
}

@keyframes fadeInText{
  1%  {color: #12000a;text-shadow:0px 0px 4px #12000a;}
  70% {color: #ff005d;text-shadow:0px 0px 14px #ff005d;}
  100%{color: #ff005d;text-shadow:0px 0px 4px #ff005d;}
}

@keyframes hueRotate{
  0%  {
    filter: hue-rotate(0deg);
  }
  50%  {
    filter: hue-rotate(-120deg);
  }
  100%  {
    filter: hue-rotate(0deg);
  }
}
    </style>
</head>
<body>
    <div class="container">
        <h1>Transmission Received</h1>
        
        <?php
        echo '<div class="result-box">';
        echo '<h2>Text Data:</h2>';
        echo '<p><strong>Name:</strong> '.htmlspecialchars($_POST['name'] ?? '').'</p>';
        echo '<p><strong>Email:</strong> '.htmlspecialchars($_POST['email'] ?? '').'</p>';
        echo '<p><strong>Message:</strong><br>'.nl2br(htmlspecialchars($_POST['message'] ?? '')).'</p>';

        if(isset($_FILES['file']) && $_FILES['file']['error'] == UPLOAD_ERR_OK) {
            $uploadDir = 'uploads/';
            if(!is_dir($uploadDir)) mkdir($uploadDir);
            
            $filename = basename($_FILES['file']['name']);
            $targetPath = $uploadDir.$filename;
            
            if(move_uploaded_file($_FILES['file']['tmp_name'], $targetPath)) {
                echo '<h2>File Received:</h2>';
                echo '<p><a href="download.php?file='.urlencode($filename).'" style="color:#ff005d;text-shadow:0 0 5px #ff005d;">Download '.htmlspecialchars($filename).'</a></p>';
            }
        }
        echo '</div>';
        ?>
</div>

</body>
</html>