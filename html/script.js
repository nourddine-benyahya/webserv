// Add smooth scroll and active link logic
document.querySelectorAll('.nav-link').forEach(link => {
    link.addEventListener('click', function (e) {
        e.preventDefault();
        const targetId = this.getAttribute('href');
        const targetSection = document.querySelector(targetId);

        targetSection.scrollIntoView({
            behavior: 'smooth',
            block: 'start'
        });

        // Update active class
        document.querySelectorAll('.nav-link').forEach(l => l.classList.remove('active'));
        this.classList.add('active');
    });
});

// Add scroll spy to update active link
const sections = document.querySelectorAll('[id]');
const options = {
    threshold: 0.5
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            document.querySelectorAll('.nav-link').forEach(link => {
                link.classList.remove('active');
                if (link.getAttribute('href') === `#${entry.target.id}`) {
                    link.classList.add('active');
                }
            });
        }
    });
}, options);

sections.forEach(section => observer.observe(section));

// Scroll Reveal Animation
const revealElements = document.querySelectorAll('.reveal');
const revealOptions = {
    threshold: 0.5
};

const revealObserver = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add('active');
        }
    });
}, revealOptions);

revealElements.forEach(element => {
    revealObserver.observe(element);
});

// Mobile Menu Toggle (Add if needed)
// Smooth Scroll
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        document.querySelector(this.getAttribute('href')).scrollIntoView({
            behavior: 'smooth'
        });
    });
});

// Add preference modal handling
function showPreferences() {
    const modal = document.getElementById('preferenceModal');
    modal.style.display = 'block';
}

// Close modal when clicking outside
window.onclick = function (event) {
    const modal = document.getElementById('preferenceModal');
    if (event.target === modal) {
        modal.style.display = 'none';
    }
}

// Modified test functions
function testMethod(method) {
    switch (method) {
        case 'GET':
            window.location.href = 'get_page.html';
            break;

        case 'DELETE':
            showDeleteModal();
            break;
        case 'UPLOAD':
            showUploadModal();
            break;
    }
}

function testCGI() {
    showCGIModal();
}

// New modal handling functions
function showCGIModal() {
    const modal = document.getElementById('cgiModal');
    modal.style.display = 'block';
}

function showDeleteModal() {
    const modal = document.getElementById('deleteModal');
    modal.style.display = 'block';
}

function showUploadModal() {
    const modal = document.getElementById('uploadModal');
    modal.style.display = 'block';
}


// Delete Form handling
document.getElementById('deleteForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const formData = new FormData(e.target);
    const itemName = formData.get('item_name');

    try {
        const response = await fetch(`/uploads/${encodeURIComponent(itemName)}`, {
            method: 'DELETE'
        });

        if (response.ok) {
            alert(`Item "${itemName}" deleted successfully`);
        } else {
            alert('Delete request failed');
        }
        document.getElementById('deleteModal').style.display = 'none';
    } catch (error) {
        alert('Error making delete request');
    }
});

// Update modal handling in script.js
function showPreferences() {
    document.getElementById('preferenceModal').style.display = 'block';
}

// Close modals when clicking outside
window.onclick = function(event) {
    document.querySelectorAll('.modal').forEach(modal => {
        if (event.target === modal) {
            modal.style.display = 'none';
        }
    });
};

// Close with ESC key
document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
        document.querySelectorAll('.modal').forEach(modal => {
            modal.style.display = 'none';
        });
    }
});

// Close button functionality
document.querySelectorAll('.close-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        btn.closest('.modal').style.display = 'none';
    });
});