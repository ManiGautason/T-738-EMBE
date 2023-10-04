import matplotlib.pyplot as plt
import numpy as np

# Initialize empty lists to store the data
timestamps = []
x_values = []
y_values = []
z_values = []

# Read the data from the file
with open('p_response.dat', 'r') as file:
    for line in file:
        # Split each line into three values
        values = line.strip().split(',')
        if len(values) == 3:
            time = len(timestamps)  # Use the number of timestamps as time values
            timestamps.append(time)
            x, y, z = map(int, values)
            x_values.append(x)
            y_values.append(y)
            z_values.append(z)

# Create a time series plot
plt.figure(figsize=(10, 6))
plt.plot(timestamps, x_values, label='Setpoint', marker='o')
plt.plot(timestamps, y_values, label='RPM', marker='o')
plt.plot(timestamps, z_values, label='PWM', marker='o')

# Add labels and legend
plt.xlabel('Time (ms)')
plt.ylabel('Value')
plt.legend()

# Show the plot
plt.title('Step response with P=3. ')
plt.grid(True)
plt.show()
