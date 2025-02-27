<?php
session_start();

// Handle form submission
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['action'])) {
    if ($_POST['action'] === 'set_preferences') {
        // Handle session for name
        if (!empty($_POST['user_name'])) {
            $_SESSION['user_name'] = htmlspecialchars($_POST['user_name']);
        } else {
            unset($_SESSION['user_name']);
        }

        // Handle cookie for color
        if (!empty($_POST['user_color'])) {
            $color = htmlspecialchars($_POST['user_color']);
            setcookie('user_color', $color, time() + (86400 * 30), "/"); // 30 days
        } else {
            setcookie('user_color', '', time() - 3600, "/"); // Remove cookie
        }
    };
    // Handle CGI POST
    if (isset($_POST['cgi_action'])) {
        $response = [
            'status' => 'success',
            'message' => 'CGI data received',
            'data' => [
                'name' => $_POST['name'] ?? '',
                'username' => $_POST['username'] ?? ''
            ]
        ];
        header('Content-Type: application/json');
        echo json_encode($response);
        exit;
    }
}

// Handle file upload
if (isset($_FILES['file'])) {
    $uploadDir = 'uploads/';
    if (!is_dir($uploadDir)) mkdir($uploadDir);
    
    $fileName = basename($_FILES['file']['name']);
    $targetPath = $uploadDir . $fileName;
    
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetPath)) {
        $response = ['status' => 'success', 'message' => 'File uploaded successfully'];
    } else {
        $response = ['status' => 'error', 'message' => 'File upload failed'];
    }
    header('Content-Type: application/json');
    echo json_encode($response);
    exit;
}
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>WebServ - 42 School Project</title>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
        <link rel="stylesheet" href="style.css">
    </head>

<body>
    <?php if (isset($_SESSION['user_name'])): ?>
            <div class="user-info reveal">
                Welcome, <?php echo $_SESSION['user_name']; ?>
                <?php if (isset($_COOKIE['user_color'])): ?>
                    <div class="color-preview" style="background: <?php echo $_COOKIE['user_color']; ?>"></div>
                <?php endif; ?>
            </div>
    <?php endif; ?>

    <!-- Add preference modal -->
    <div id="preferenceModal" class="modal">
    <div class="modal-content">
        <span class="close-btn">&times;</span>
        <form method="POST">
            <h3>Set Preferences</h3>
            <div style="margin: 1rem 0;">
                <label>Name:</label>
                <input type="text" name="user_name" 
                       value="<?php echo $_SESSION['user_name'] ?? ''; ?>">
            </div>
            <div style="margin: 1rem 0;">
                <label>Color:</label>
                <input type="color" name="user_color" 
                       value="<?php echo $_COOKIE['user_color'] ?? '#6C5CE7'; ?>">
            </div>
            <input type="hidden" name="action" value="set_preferences">
            <button type="submit" class="test-btn">Save Preferences</button>
        </form>
        </div>
    </div>

    <!-- Add these modals -->
    <div id="cgiModal" class="modal">
    <div class="modal-content">
        <span class="close-btn">&times;</span>
        <form id="cgiForm" action="/page.php" method="POST" enctype="multipart/form-data">
            <h3>Enter CGI Data</h3>
            <div style="margin: 1rem 0;">
                <label>Name:</label>
                <input type="text" name="name" required>
            </div>
            <div style="margin: 1rem 0;">
                <label>email:</label>
                <input type="email" id="email" name="email" required>
            </div>
            <div style="margin: 1rem 0;">
                <label>message:</label>
                <textarea id="message" name="message" rows="5"></textarea>
            </div>
            <div style="margin: 1rem 0;">
                <label>upload file:</label>
                <input  type="file" name="file" required>
            </div>
            <button type="submit" class="test-btn">Submit</button>
        </form>
        </div>
    </div>

        <!-- Add these modals -->
        <div id="pycgiModal" class="modal">
    <div class="modal-content">
        <span class="close-btn">&times;</span>
        <form id="cgiForm" action="/page.py" method="POST" enctype="multipart/form-data">
            <h3>Enter CGI Data</h3>
            <div style="margin: 1rem 0;">
                <label>Name:</label>
                <input type="text" name="name" required>
            </div>
            <div style="margin: 1rem 0;">
                <label>email:</label>
                <input type="email" id="email" name="email" required>
            </div>
            <div style="margin: 1rem 0;">
                <label>message:</label>
                <textarea id="message" name="message" rows="5"></textarea>
            </div>
            <div style="margin: 1rem 0;">
                <label>upload file:</label>
                <input  type="file" name="file" required>
            </div>
            <button type="submit" class="test-btn">Submit</button>
        </form>
        </div>
    </div>

    <div id="deleteModal" class="modal">
    <div class="modal-content">
        <span class="close-btn">&times;</span>
        <form id="deleteForm">
            <h3>Delete Item</h3>
            <div style="margin: 1rem 0;">
                <label>Item Name:</label>
                <input type="text" name="item_name" required>
            </div>
            <button type="submit" class="test-btn">Delete</button>
        </form>
        </div>
</div>

<div>
    <div id="uploadModal" class="modal">
    <div class="modal-content">
        <span class="close-btn">&times;</span>
        <form id="uploadForm" action="/uploads" method="POST" enctype="multipart/form-data">
            <h3>upload Item</h3>
            <div style="margin: 1rem 0;">
                <label>Item Name:</label>
                <input type="file" name="item_name" required>
            </div>
            <button type="submit" class="test-btn">Delete</button>
        </form>
        </div>
    </div>
</div>

    <!-- Add this navigation header -->
    <nav class="main-header reveal">
        <div class="nav-container">
            <a href="#about" class="nav-link">About</a>
            <a href="#team" class="nav-link">Team</a>
            <a href="#features" class="nav-link">Features</a>
            <a href="#test" class="nav-link">Tests</a>
            <a href="#config" class="nav-link">Config</a>
        </div>
    </nav>
    <div class="container">
        <h1 class="title reveal">WebServ</h1>

        <!-- Project Description -->
        <div class="project-description reveal" id="about">
            <h2 class="section-title">About the Project</h2>
            <p>WebServ is a custom HTTP server implementation developed as part of the 42 School curriculum. This lightweight web server supports core HTTP features and can be configured through config files.</p>
            <div class="github-repo">
                <a href="[YOUR_GITHUB_REPO_URL]" target="_blank" class="repo-link">
                    <i class="fab fa-github"></i>
                    View on GitHub
                </a>
            </div>
        </div>

        <!-- Project Team -->
        <h2 class="section-title reveal" id="team" >Project Team</h2>
        <div class="team-section" >
            <!-- Team Member 1 -->
            <div class="member-card reveal">
                <img src="./assets/nbenyahy.jpg" alt="Nourddine Benyahya" class="member-img">
                <h3>Nourddine Benyahya</h3>
                <div class="links">
                    <a href="[Github URL]" target="_blank" aria-label="GitHub">
                        <i class="fab fa-github"></i>
                    </a>
                    <a href="[LinkedIn URL]" target="_blank" aria-label="LinkedIn">
                        <i class="fab fa-linkedin"></i>
                    </a>
                    <a href="[42 Intra URL]" target="_blank" aria-label="42 Intra">
                        <i class="fa-42"></i>
                    </a>
                </div>
            </div>
            <!-- Repeat for other members -->
            <!-- Team Member 1 -->
            <div class="member-card reveal">
                <img src="./assets/amejdoub.jpg" alt="Anas Mejdoub" class="member-img">
                <h3>Anas Mejdoub</h3>
                <div class="links">
                    <a href="[Github URL]" target="_blank" aria-label="GitHub">
                        <i class="fab fa-github"></i>
                    </a>
                    <a href="[LinkedIn URL]" target="_blank" aria-label="LinkedIn">
                        <i class="fab fa-linkedin"></i>
                    </a>
                    <a href="[42 Intra URL]" target="_blank" aria-label="42 Intra">
                        <i class="fa-42"></i>
                    </a>
                </div>
            </div>
            <!-- Team Member 1 -->
            <div class="member-card reveal">
                <img src="./assets/abounab.jpg" alt="Abdellah Bounab" class="member-img">
                <h3>Abdellah Bounab</h3>
                <div class="links">
                    <a href="[Github URL]" target="_blank" aria-label="GitHub">
                        <i class="fab fa-github"></i>
                    </a>
                    <a href="https://www.linkedin.com/in/abdellah-bounab" target="_blank" aria-label="LinkedIn">
                        <i class="fab fa-linkedin"></i>
                    </a>
                    <a href="[42 Intra URL]" target="_blank" aria-label="42 Intra">
                        <i class="fa-42"></i>
                    </a>
                </div>
            </div>
        </div>

        <!-- Project Features -->
        <h2 class="section-title reveal" id="features">Key Features</h2>
        <div class="features-section reveal">
            <h2>Project Features</h2>
            <p>Our web server implements:</p>
            <ul>
                <li>HTTP Methods: GET, POST, DELETE</li>
                <li>File Upload Handling</li>
                <li>CGI Implementation (PHP/Python)</li>
                <li>Session Management</li>
                <li>Cookie Handling</li>
                <li>Configuration File Support</li>
            </ul>
        </div>

        <!-- Test Project -->
        <h2 class="section-title reveal"  id="test">Test the Server</h2>
        <div class="test-buttons reveal">
            <button class="test-btn" style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>" onclick="testMethod('GET')">GET</button>
            <button class="test-btn" style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>" onclick="testMethod('UPLOAD')">POST using UPLOAD</button>
            <button class="test-btn" style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>" onclick="testCGI()">POST using php CGI</button>
                            <button class="test-btn" style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>" onclick="testPyCGI()">POST using python CGI</button>
            <button class="test-btn" style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>" onclick="testMethod('DELETE')">DELETE</button>
        <button class="test-btn" onclick="showPreferences()" 
                style="<?php echo isset($_COOKIE['user_color']) ? 
                'background: '.$_COOKIE['user_color'] : ''; ?>">
            Test Session & Cookie
        </button>
        </div>

        <!-- Example Config File -->
        <h2 class="section-title reveal"  id="config">Configuration Example</h2>
        <div class="code-section reveal">
            <pre>
            server : {
                port : 81,
                name : localhost,
                root : html,
                error : {
                    404 : 404.html,
                    500 : 500.html,
                },
                route : {
                    path :  /index.php,
                    allowed_mthods : {
                    POST,
                    GET,
                    },
                    has_cgi : yes,
                    cgi : {
                        .php=cgi-bin/php-cgi,
                        .py=/usr/local/bin/python3,
                    },
                    index : test.html,
                    },
                }
                </pre>
            </div>

            <footer class="reveal">
                <p>🚀 Built with passion by WebServ Team</p>
                <p>📍 42 Network - 2023</p>
            </footer>
    </div>
    <script src="script.js"></script>
</body>
</html>