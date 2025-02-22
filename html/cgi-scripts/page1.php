<?php
session_start();
// session.save_path = "/Users/nbenyahy/Desktop/42-webserv/sessiondta";

// Set session and cookie when form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Store pet name in session
    $_SESSION['detective_pet'] = $_POST['pet_name'];
    
    // Store favorite treat in cookie (1 day expiration)
    $favorite_treat = $_POST['favorite_treat'];
    setcookie('pet_treat', $favorite_treat, time() + 86400, '/');
}

?>

<?php
phpinfo();
?>

<!DOCTYPE html>
<html>
<head>
    <title>Pet Detective HQ - Profile Setup</title>
    <style>
        body { 
            font-family: 'Comic Sans MS', cursive; 
            background: #F0E6D6;
            text-align: center;
        }
        .container {
            background: white;
            padding: 2rem;
            border-radius: 20px;
            display: inline-block;
            margin: 2rem;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
        }
        button { 
            background: #4CAF50;
            color: white;
            padding: 1rem 2rem;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            font-size: 1.2rem;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🐾 Welcome to Pet Detective HQ! 🕵️♂️</h1>
        <p>Create your animal detective profile!</p>
        
        <form method="POST">
            <h3>Choose your detective partner:</h3>
            <select name="pet_name" required>
                <option value="Bark Twain">Bark Twain 🐶</option>
                <option value="Sherlock Bones">Sherlock Bones 🦴</option>
                <option value="Agent Fluffington">Agent Fluffington 😼</option>
                <option value="Hedwig">Hedwig 🦉</option>
            </select>

            <h3>Choose your secret weapon treat:</h3>
            <select name="favorite_treat" required>
                <option value="Bacon Bribes">Bacon Bribes 🥓</option>
                <option value="Tuna Tokens">Tuna Tokens 🐟</option>
                <option value="Mystery Cookies">Mystery Cookies 🍪</option>
                <option value="Seed Satchels">Seed Satchels 🌻</option>
            </select>

            <br><br>
            <button type="submit">🐾 Create Profile! 🕵️♂️</button>
        </form>

        <?php if(isset($_SESSION['detective_pet'])): ?>
            <p style="color: green; margin-top: 1rem;">
                Profile created! <a href="page2.php">Go to Mission Briefing →</a>
            </p>
        <?php endif; ?>
    </div>
</body>
</html>