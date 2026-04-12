# Qonnect

A multi-client TCP chat application built in C++ with Qt 6. Qonnect 
implements a custom binary protocol over TCP for real-time messaging, 
file transfer, typing indicators, and user status management.

## Features

- **Real-time messaging** — send to a specific client by name or broadcast to all
- **File transfer** — accept/reject flow with file size preview dialog
- **Typing indicators** — live "X is typing..." notification that auto-clears
- **User status** — Available, Away, Busy with tab icon updates on the server
- **Name changes** — rename mid-session; all clients and file folders update automatically
- **Connection events** — clients notified when others join or leave

## Tech Stack

- **C++17** with Qt 6 (Widgets + Network modules)
- **QTcpSocket / QTcpServer** for TCP communication
- **QDataStream** for binary protocol serialization
- **CMake 3.16+**

## Project Structure

```
Qonnect/
├── Server/   — TCP server, client registry, per-client UI tabs, message routing
└── Client/   — Client socket, chat UI, file dialog, destination dropdown
```

## Build & Run

```bash
git clone https://github.com/MohamedAkash/Qonnect.git

# Server
cd Server && mkdir build && cd build
cmake .. && make
./QonnectServer

# Client (new terminal — repeat for multiple clients)
cd ../../Client && mkdir build && cd build
cmake .. && make
./Client
```

Start the server first, then launch one or more client instances. 
Connects to `localhost:4500` by default.
