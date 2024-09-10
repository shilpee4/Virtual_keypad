# Virtual Keypad Kernel Module with Enhanced User Interface

## Overview

The Virtual Keypad Kernel Module creates a character device in the Linux kernel, allowing user space programs to interact with it by reading from and writing to the device. This module is integrated with a simple yet powerful graphical user interface (GUI) built using the GTK framework, providing a seamless and intuitive user experience. The virtual keypad includes various features for secure and efficient user interactions, including credential management, privacy enhancements, and quick access to information and payment integrations.


## Key Features

### Simple Graphical User Interface
- **Neat and Easy-to-Use Interface**: The virtual keypad features a numeric keypad interface designed with the GTK framework. The intuitive design ensures seamless user interactions while enhancing the aesthetic appeal and usability of the application.

### Secure Credential Management
- **USERNAME and PASSWORD Buttons**: Securely enter and store sensitive data directly into a file.

### Improved Privacy
- **ASTERISK Button**: Conceal sensitive data by substituting asterisks for characters when entering information. This feature is crucial for maintaining secrecy in shared or public areas.

### Effective Interaction
- **CLEAR Button**: Quickly remove any active inputs from the screen with the CLEAR button. This functionality is particularly useful in busy environments where accuracy and time are critical.

### Smooth Payments Integration
- **UPI Button**: Bring up a saved QR codefor easy scanning and payment. 

### Quick Access to Information
- **FAQ Button**: Display frequently asked questions and related material immediately, serving as a quick reference tool for all the buttons . 

### Web Search at Your Fingertips
- **HELP Button**: Instantly open Google in a browser, providing quick access to the internet for comprehensive information or rapid problem-solving.

## Installation

## Note
Please make necessary changes to the make file to run the same at your end , for demonstration purposes the makefile has been hardcoded with certain values.

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/virtual_keypad.git
    cd virtual_keypad
    ```

2. **Build the module**:
    ```bash
    make
    ```

3. **Load the module**:
    ```bash
    sudo insmod virtual_keypad.ko
    ```

4. **Check the logs**:
    ```bash
    dmesg | grep VirtualKeypad
    ```

5. **Create a device file**:
    ```bash
    sudo mknod /dev/virtual_keypad c [major_number] 0
    sudo chmod 666 /dev/virtual_keypad
    ```

## Usage

### GUI Interface

1. **Run the GUI Application**: Launch the GTK-based GUI application to interact with the virtual keypad.

### Reading from the Device

The read function is intended for use by a script (`keypad_listener.sh`). It sends the message stored in the kernel buffer to the user space.

```bash
cat /dev/virtual_keypad
```

### Writing to the Device

The write function accepts commands from a GUI application or any other user space program. The input commands should be formatted as follows:

1. **Username**: Starts with 'U'.
    ```bash
    echo "U username" > /dev/virtual_keypad
    ```

2. **Password**: Starts with 'P' followed by 'S'.
    ```bash
    echo "PS password" > /dev/virtual_keypad
    ```

3. **Other Commands**: Handled by `keypad_listener.sh`.
    ```bash
    echo "command" > /dev/virtual_keypad
    ```

### Example

```bash
echo "U user123" > /dev/virtual_keypad
echo "PS pass123" > /dev/virtual_keypad
echo "command" > /dev/virtual_keypad
```

### Log Messages

Check `dmesg` for kernel log messages to verify the operations:

```bash
dmesg | grep VirtualKeypad
```

## Uninstallation

1. **Remove the device file**:
    ```bash
    sudo rm /dev/virtual_keypad
    ```

2. **Unload the module**:
    ```bash
    sudo rmmod virtual_keypad
    ```

3. **Clean the build**:
    ```bash
    make clean
    ```

## Files

- **virtual_keypad.c**: The main kernel module source file.
- **Makefile**: Makefile for building the kernel module.

## Authors

- Yatharth Nanda
- Soham Walimbe
- Hrishav Deka

## Contact

For issues or questions, please mail @yatharthnanda25@gmail.com

---
