<?php
session_start();
$spaceship = $_SESSION['spaceship'] ?? 'Unknown Ship';
$crew = $_COOKIE['crew_member'] ?? 'No Crew Selected';
$outcomes = [
    'Cookie Cruiser' => 'outran the Space Pirates! 🏴‍☠️',
    'Chocolate Chimera' => 'snuck past the Laser Grid! 🔥',
    'Gingerbread Galaxy' => 'collected 500 space cookies! 🍪'
];
?>

<!DOCTYPE html>
<html>
<head>
    <title>Mission Report</title>
    <style>
        body {
            margin: 0;
            overflow: hidden;
            font-family: 'Arial', sans-serif;
            background: #000;
            color: #fff;
        }

        .nebula {
            position: fixed;
            width: 100%;
            height: 100%;
            background: radial-gradient(circle at 50% 50%, 
                #330033 0%, 
                #000033 70%, 
                #000000 100%);
            animation: pulse 5s infinite;
        }

        @keyframes pulse {
            0%, 100% { opacity: 0.5; }
            50% { opacity: 0.8; }
        }

        .report {
            position: relative;
            z-index: 1;
            text-align: center;
            padding: 2rem;
            background: rgba(0,0,0,0.7);
            margin: 2rem auto;
            width: 80%;
            border-radius: 20px;
            border: 2px solid #4CAF50;
        }

        .scanning-text {
            font-family: 'Courier New', monospace;
            color: #0f0;
            border-right: 2px solid #0f0;
            animation: typing 3s steps(40), blink-caret 0.75s step-end infinite;
            overflow: hidden;
            white-space: nowrap;
            margin: 0 auto;
            width: fit-content;
        }

        @keyframes typing {
            from { width: 0 }
            to { width: 100% }
        }

        @keyframes blink-caret {
            from, to { border-color: transparent }
            50% { border-color: #0f0 }
        }
    </style>
</head>
<body>
    <div class="nebula"></div>
    
    <div class="report">
        <h1>🌌 MISSION REPORT 🌠</h1>
        <div class="scanning-text">
            DECODING TRANSMISSION... MISSION SUCCESS!
        </div>

        <h2><?= htmlspecialchars($spaceship) ?></h2>
        <p>Piloted by <?= htmlspecialchars($crew) ?></p>
        
        <h3 style="color: #4CAF50">ACHIEVEMENT UNLOCKED:</h3>
        <p style="font-size: 1.2rem;">
            <?= $outcomes[htmlspecialchars($spaceship)] ?? 'Completed secret objectives! 🎖️' ?>
        </p>

        <div style="margin-top: 2rem;">
            <p>Cookie Status: 🍪 x <?= rand(100, 500) ?></p>
            <p>Fuel Remaining: ⛽ <?= rand(30, 100) ?>%</p>
        </div>

        <a href="spaceship.php" style="color: #4CAF50; text-decoration: none;">
            🚀 Launch New Mission
        </a>
    </div>
</body>
</html>