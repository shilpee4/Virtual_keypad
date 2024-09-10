#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/input.h> 
 
#define DEVICE_NAME "virtual_keypad"
 
static int major_number;
// Buffer for storing the last command
static char message[256] = {0};
static short size_of_message;

// Read function; to be used by the script keypad_listener.sh
static ssize_t virtual_keypad_read(struct file *file, char *buffer, size_t len, loff_t *offset) {

    int error_count = 0;
    // Sends the message to the user space

    error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) {
        int bytes_read = size_of_message;
        size_of_message = 0;
        return bytes_read;
    } 
    else {
        return -EFAULT;
    }

}
 
// Write function; to be used by the GUI app
static ssize_t virtual_keypad_write(struct file *file, const char *buffer, size_t length, loff_t *offset) {

    char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
    
    char *argv[5];
    
    // Buffer to store input
    // Takes in data from the user space
    char input[length + 2];
    if (copy_from_user(input, buffer, length) != 0) {
        return -EFAULT;
    }
    input[length]='\n';
    input[length+1] = '\0';
 
    // Case 1: Username
    if (input[0] == 'U') { 

        // Append the user data into the file username.txt
        argv[0] = "/bin/bash";
        argv[1] = "-c";
        argv[2] = kasprintf(GFP_KERNEL, "echo -n '%s' >> /home/yatharth/Downloads/OSfinal/username.txt", input + 4); // User data starts at 20th character
        argv[3] = NULL;
        call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
        kfree(argv[2]);

        strcpy(message, "UATF");  // Command for user space
        printk(KERN_INFO "Username appended to file");

    }
    // Case 2: Password
    else if (input[0] == 'P' && input[2] == 'S') {  

        // Append the user data into the file password.txt
        argv[0] = "/bin/bash";
        argv[1] = "-c";
        argv[2] = kasprintf(GFP_KERNEL, "echo -n '%s' >> /home/yatharth/Downloads/OSfinal/password.txt", input + 4); // User data starts at 20th character
        argv[3] = NULL;
        call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
        kfree(argv[2]); 

        strcpy(message, "PATF");  // Command for user space
        printk(KERN_INFO "Password appended to file");

    } 
    // Case 3: Handled by keypadlistener.sh
    else {

        // Buffer for button code
        char code[5]; 
        strncpy(code, input, 4);
        // Null terminate the string
        code[4] = '\0';

        strcpy(message, code);  // Command for user space
        printk(KERN_INFO "Command sent to bash");

    }
    
    size_of_message = strlen(message);
 
    printk(KERN_INFO "VirtualKeypad: Received input %s, stored message: %s\n", input, message);
    return length;

}

// File operations
static struct file_operations fops = {
    .read = virtual_keypad_read,
    .write = virtual_keypad_write,
};
 
// Initialization method
static int __init virtual_keypad_init(void) {

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "Virtual keypad module loaded with major number %d\n", major_number);
    return 0;

}

// Exit method
static void __exit virtual_keypad_exit(void) {

    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Virtual keypad module unloaded\n");

}
 
module_init(virtual_keypad_init);
module_exit(virtual_keypad_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 16");
MODULE_DESCRIPTION("Virtual Keypad Module");