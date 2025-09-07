# ARP Request Tool

A C networking utility that sends ARP (Address Resolution Protocol) requests to discover MAC addresses on the local network. This tool automatically detects the source interface and IP address, requiring only the target IP as input.

## Features

- **Automatic interface detection**: Tries common interface names (eth0, enp0s3, wlan0)
- **Source IP/MAC auto-discovery**: No need to specify source parameters
- **Raw socket implementation**: Direct packet construction and transmission
- **Professional structure**: Modular design with proper separation of concerns

## Installation

```bash
# Clone the repository
git clone <repository-url>
cd arp_request

# Compile the program
make

# Install system-wide (optional)
sudo make install
```

## Usage

```bash
# Basic usage (requires root privileges)
sudo ./arp_request <target_ip>

# Example
sudo ./arp_request 192.168.1.1
```

## Architecture

```
├── src/
│   ├── arp_request.c    # Main program entry point
│   ├── network.c        # Socket operations and interface detection
│   ├── packet.c         # ARP packet construction and Ethernet framing
│   └── utils.c          # Input validation and utility functions
├── include/
│   └── arp_request.h    # Shared structures and function declarations
├── Makefile             # Build system with Norma v4 compliance
└── README.md            # Project documentation
```

## Build Targets

- `make` or `make all` - Build the program
- `make debug` - Build with debug symbols
- `make clean` - Remove object files
- `make fclean` - Full clean (remove all generated files)
- `make re` - Rebuild everything
- `make install` - Install system-wide (requires sudo)
- `make uninstall` - Remove from system
- `make help` - Show all available targets

## Requirements

- Linux operating system
- GCC compiler with C99 support
- Root privileges (for raw socket creation)
- Network interface (eth0, enp0s3, or wlan0)

## Technical Details

- **Protocol**: ARP (Address Resolution Protocol)
- **Packet size**: 42 bytes (14-byte Ethernet header + 28-byte ARP payload)
- **Socket type**: Raw packet socket (AF_PACKET)

## License

This project is developed for educational purposes following 42 School standards.
