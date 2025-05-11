# Qonnect

Qonnect is a modern communication framework built with C++ and the Qt framework. It facilitates seamless client-server communication, offering essential features such as text messaging, file sharing, and user status management.

## Features

- **Messaging**: Supports text messaging between clients with acknowledgment mechanisms.
- **File Sharing**: Enables secure file transfer between connected users.
- **User Status Management**: Allows users to set and share their availability status (e.g., Available, Busy, Away).
- **Typing Indicators**: Indicates when a user is typing a message.
- **Client Management**: Manages connections, disconnections, and interactions between clients and the server.

## Requirements

- **C++ Compiler**: Compatible with C++17 or later.
- **Qt Framework**: Requires Qt 5 or 6 with Widgets and Network modules.
- **CMake**: Minimum version 3.16.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/MohamedAkash/Qonnect.git
   cd Qonnect
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the server and client applications:
   ```bash
   ./Server/QonnectServer
   ./Client/QonnectClient
   ```

## Usage

1. Start the server application to listen for client connections.
2. Launch the client application and connect to the server using the provided IP and port.
3. Enjoy real-time messaging and file-sharing features.

## Acknowledgments

- Built using the [Qt Framework](https://www.qt.io/).
