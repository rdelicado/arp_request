# ARP Request Tool - Copilot Instructions

## Project Overview
This is a C networking tool that sends ARP requests to discover MAC addresses on the local network. The tool automatically detects the source interface and IP, requiring only the target IP as input.

## Architecture
- **Core Module**: `src/arp_request.c` - Main program entry and orchestration
- **Network Module**: `src/network.c` - Socket operations and interface detection
- **Packet Module**: `src/packet.c` - ARP packet construction and Ethernet framing
- **Utilities**: `src/utils.c` - Validation and helper functions
- **Headers**: `include/arp_request.h` - Shared structures and function declarations

## Norma v4 Compliance (CRITICAL)
This project follows strict 42 School coding standards:

### Naming Convention
- Functions, variables, files: `snake_case` (e.g., `send_arp_packet`)
- Struct prefix: `s_` (e.g., `s_arp_header`)
- Typedef prefix: `t_` (e.g., `t_packet_data`)
- Enum prefix: `e_` (e.g., `e_error_type`)
- Global prefix: `g_` (e.g., `g_interface_name`)

### Function Rules
- Max 25 lines per function (excluding braces)
- Max 5 variables per function
- Max 4 parameters per function
- Max 80 characters per line
- Braces on new lines always
- Variable declarations at function start only

### Forbidden Constructs
- `for`, `do...while`, `switch`, `case`, `goto`, ternary operators
- Variable length arrays (VLAs)
- Comments inside functions

### File Organization
- Max 5 functions per `.c` file
- Include guards in all headers: `#ifndef ARP_REQUEST_H`
- No `.c` file includes in other files

## Build System
- Uses custom Makefile with rules: `all`, `clean`, `fclean`, `re`
- Compilation: `gcc -Wall -Wextra -std=c99 -O2`
- Debug build available with `make debug`
- Automatic dependency management

## Key Patterns
- Raw sockets require root privileges (`sudo ./arp_request <target_ip>`)
- Interface detection tries common names: eth0, enp0s3, wlan0
- Error handling with descriptive messages and proper cleanup
- Network byte order conversion using `htons`/`ntohs`
- Broadcast MAC (ff:ff:ff:ff:ff:ff) for ARP requests

## Development Workflow
1. Edit source files in `src/` or headers in `include/`
2. Build with `make` (or `Ctrl+Shift+B` in VSCode)
3. Test with `sudo ./arp_request <target_ip>`
4. Check compliance with norminette before commits

## Critical Implementation Notes
- Always use `_GNU_SOURCE` for full POSIX functionality
- Socket creation uses `AF_PACKET` and `SOCK_RAW`
- ARP packet: 14-byte Ethernet header + 28-byte ARP payload
- Source IP/MAC obtained automatically from interface
- Proper cleanup of sockets and memory on exit
