import numpy as np
import matplotlib.pyplot as plt

# Load data from your .dat file
data = np.loadtxt("pcontroll.dat")

# Generate time values based on 10ms intervals
sample_interval_ms = 1
time = np.arange(len(data)) * (sample_interval_ms / 1000.0)  # Convert ms to seconds

# Define the time range you want to calculate the average for
start_time = 0  # seconds
end_time = 26.1604  # seconds

# Find the indices corresponding to the specified time range
start_index = int(start_time / (sample_interval_ms / 1000.0))
end_index = int(end_time / (sample_interval_ms / 1000.0))

# Extract the data values within the specified range
selected_data = data[start_index:end_index]

# Calculate the average of the selected data
average_value = np.mean(selected_data)

# Plot the data with a highlighted time range
plt.plot(time, data)
plt.axvline(x=start_time, color='r', linestyle='--', label='Start Time t_1')
plt.axvline(x=end_time, color='g', linestyle='--', label='End Time t_2')
plt.xlabel('Time (seconds)')
plt.ylabel('Data Value')
plt.axhline(y = 70, color = 'r', linestyle = '--')
plt.title('Timeconstand tau = %i' %32)
plt.grid(True)
plt.legend()
plt.show()

# Print the calculated average value
print(f"Average from {start_time} to {end_time} seconds: {average_value}")
