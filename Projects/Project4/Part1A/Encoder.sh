#!/bin/bash

# Export the GPIO pins
echo "20" > /sys/class/gpio/export
echo "21" > /sys/class/gpio/export

# Set the direction of the GPIO pins
echo "in" > /sys/class/gpio/gpio20/direction
echo "out" > /sys/class/gpio/gpio21/direction


# Get the current usage of CPU and memory
cpuUsage=$(top -bn1 | awk '/Cpu/ { print $2}')
memUsage=$(free -m | awk '/Mem/{print $3}')

# Print the usage
echo "CPU Usage: $cpuUsage%"
echo "Memory Usage: $memUsage MB"


while true; do
    # Read the encoder pin state
    encoder_state=$(cat /sys/class/gpio/gpio20/value)

    # Mirror the encoder state to the output GPIO pin
    echo $encoder_state > /sys/class/gpio/gpio21/value
done
