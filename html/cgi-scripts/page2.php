<?php
session_start();
$pet_name = $_SESSION['detective_pet'] ?? 'Mystery Creature';
$favorite_treat = $_COOKIE['pet_treat'] ?? 'unknown treats';
?>

<!DOCTYPE html>
<html>
<head>
    <title>Mission Briefing</title>
    <style>
        body { 
            font-family: 'Comic Sans MS', cursive; 
            background: #F0E6D6;
            text-align: center;
        }
        .mission-brief {
            background: white;
            padding: 2rem;
            border-radius: 20px;
            margin: 2rem auto;
            max-width: 600px;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
        }
        .secret-message {
            color: white;
            background: #2c3e50;
            padding: 1rem;
            border-radius: 10px;
            margin: 2rem;
        }
    </style>
</head>
<body>
    <div class="mission-brief">
        <h1>🕵️♂️ TOP SECRET MISSION BRIEFING 🐾</h1>
        
        <div class="secret-message">
            <p>Attention Detective <?= htmlspecialchars($pet_name) ?>!</p>
            <p>Your mission (should you choose to accept it):</p>
            <p>Infiltrate the human's kitchen and locate the...</p>
            <h2 style="color: #e67e22;"><?= htmlspecialchars($favorite_treat) ?>!</h2>
            <p>Use your cutest pose as distraction! 🥺</p>
        </div>

        <p>⚠️ This message will self-destruct in 10 seconds... ⏳</p>
        <p><em>(But your cookie will last 24 hours!)</em></p>

        <div style="margin-top: 2rem;">
            <a href="page1.php" style="color: #4CAF50;">↩️ Create New Profile</a>
        </div>
    </div>
</body>
</html>