# How to Use This Program

## Getting Started

1. **Clone the repository:**

   ```sh
   git clone <repository-url>
   cd <repository-name>
   ```

2. **Build and run the program using Docker Compose:**

   ```sh
   make
   ```

   - This command will start the application on port **80**.
   - Wait for the process to complete.

3. **Access the program:**

   - Open your browser and go to `http://localhost:80`.

## Stopping and Cleaning Up

- **Stop the running Docker containers:**

  ```sh
  make stop
  ```

- **Remove the Docker images:**

  ```sh
  make rmi
  ```

- **Remove object files from the program:**

  ```sh
  make fclean
  ```

## Documentation

- **Socket Documentation:** Located inside the `server` folder.
- **HTTP Documentation:** Found inside the `request` and `response` folders.

---

For more details, check the relevant documentation files in the repository.

