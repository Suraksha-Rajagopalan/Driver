#!/bin/bash

MODULE_NAME="assignment"
DEVICE_NAME="memfile"

# Compile the kernel module
make

# Check if the module is already loaded
if lsmod | grep -q "^${MODULE_NAME}"; then
    echo "Module ${MODULE_NAME} is already loaded. Unloading it first."
    sudo rmmod ${MODULE_NAME}
fi

# Load the kernel module
sudo insmod ${MODULE_NAME}.ko

# Allow some time for the module to load and log messages
sleep 1

# Get the device name from dmesg with sudo
device_name=$(sudo dmesg | grep -oP "Device class created correctly: /dev/\K\S+")

# Check if the device name was found
if [ -z "$device_name" ]; then
    echo "Error: Could not find device name"
    sudo rmmod ${MODULE_NAME}
    exit 1
fi

echo "Device name is /dev/$device_name"

# Compile the test program
gcc -o test_memfs test_memfs.c

# Run the test program with the device name
sudo ./test_memfs /dev/$device_name

# Optionally, unload the kernel module
# sudo rmmod ${MODULE_NAME}

# Optionally, remove the device file
# sudo rm /dev/${DEVICE_NAME}

