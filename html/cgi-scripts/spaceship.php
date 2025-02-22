<?php
// Start output buffering at the VERY TOP
ob_start();
session_start();

// Enable error reporting
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    try {
        // Validate input
        if (empty($_POST['spaceship']) || empty($_POST['crew'])) {
            throw new Exception('Please select all options!');
        }

        // Store in session
        $_SESSION['spaceship'] = $_POST['spaceship'];
        
        // Store in cookie (1 hour expiration)
        setcookie('crew_member', $_POST['crew'], time() + 3600, '/', '', false, true);
        
        // Clear output buffer before redirect
        ob_end_clean();
        // header('Location: mission.php');
        // exit();
    } catch (Exception $e) {
        $error = $e->getMessage();
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Space Cookie Adventure Launch</title>
    <style>
        body {
            margin: 0;
            overflow: hidden;
            font-family: 'Arial', sans-serif;
            background: #000;
            color: #fff;
        }
        
        .stars {
            position: fixed;
            width: 100%;
            height: 100%;
            background: transparent;
            pointer-events: none;
        }
        
        .star {
            position: absolute;
            background: white;
            border-radius: 50%;
            animation: twinkle 1.5s infinite;
        }

        @keyframes twinkle {
            0%, 100% { opacity: 0.3; }
            50% { opacity: 1; }
        }

        .container {
            position: relative;
            z-index: 1;
            text-align: center;
            padding: 2rem;
            background: rgba(0,0,0,0.8);
            margin: 2rem auto;
            width: 80%;
            border-radius: 20px;
        }

        .ship-option {
            border: 2px solid #44f;
            margin: 1rem;
            padding: 1rem;
            cursor: pointer;
            transition: all 0.3s;
            border-radius: 10px;
        }

        .ship-option:hover {
            background: #44f3;
            transform: scale(1.05);
        }

        button {
            background: #4CAF50;
            color: white;
            padding: 1rem 2rem;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            font-size: 1.2rem;
            margin-top: 1rem;
        }
    </style>
</head>
<body>
    <div class="stars" id="stars"></div>
    
    <div class="container">
        <h1>🚀 Welcome to Space Cookie Command!</h1>
        <p>Configure your mission to retrieve the Galactic Cookies!</p>
        
        <form method="POST">
            <h2>Choose Your Spaceship:</h2>
            <label class="ship-option">
                <input type="radio" name="spaceship" value="Cookie Cruiser" required>
                🍪 Cookie Cruiser (Speed +3, Defense +1)
            </label>
            
            <label class="ship-option">
                <input type="radio" name="spaceship" value="Chocolate Chimera" required>
                🍫 Chocolate Chimera (Stealth +5)
            </label>
            
            <label class="ship-option">
                <input type="radio" name="spaceship" value="Gingerbread Galaxy" required>
                � Gingerbread Galaxy (Cargo +4)
            </label>

            <h2>Select Your First Officer:</h2>
            <select name="crew" required style="padding: 0.5rem; font-size: 1.1rem;">
                <option value="👾 Alien Baker">👾 Alien Baker (Cookie Science +2)</option>
                <option value="🤖 Robot Chef">🤖 Robot Chef (Precision +4)</option>
                <option value="👽 Space Koala">👽 Space Koala (Diplomacy +3)</option>
            </select>

            <br>
            <button type="submit">LAUNCH MISSION! 🚀</button>

        </form>
        <?php if(isset($_SESSION['spaceship'])): ?>
            <p style="color: green; margin-top: 1rem;">
                Profile created! <a href="mission.php">Go to Mission Briefing →</a>
            </p>
        <?php endif; ?>
    </div>

    <script>
        // Create animated stars background
        function createStars() {
            const stars = document.getElementById('stars');
            for(let i = 0; i < 200; i++) {
                const star = document.createElement('div');
                star.className = 'star';
                star.style.left = Math.random() * 100 + '%';
                star.style.top = Math.random() * 100 + '%';
                star.style.width = Math.random() * 3 + 'px';
                star.style.height = star.style.width;
                star.style.animationDelay = Math.random() * 1.5 + 's';
                stars.appendChild(star);
            }
        }
        createStars();
    </script>
</body>
</html>