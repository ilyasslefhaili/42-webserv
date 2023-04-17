# 42-webserv
This is a custom HTTP server implemented in C/C++98. 
It allows you to host and serve web content, videos and images, and handle basic HTTP requests.
This project was developed collaboratively with <a href="https://github.com/mkorchi">Mohamed Korchi</a>
# Features
- HTTP/1.1 compliant.
- Support for GET, POST and DELETE methods.
- error handling for invalid requests or invalid config-file.
- handling concurrent connections.
# Getting Started
- ```make```
- set up your config file.
- ```./webserv [config file path]```
- ```./webserv``` Run with default config-file.
- You can access the server in your web browser.
# Supported HTTP Methods
The server currently supports the following HTTP methods:
- GET: Used to request a resource from the server.
- POST: Used to submit data to the server for processing.
- DELETE: Used to delete a resource that was previously created using the POST .
# Error Handling
The server performs basic error handling for invalid HTTP requests, 
such as responding with appropriate error codes (e.g., 404 for not found) and error pages.
