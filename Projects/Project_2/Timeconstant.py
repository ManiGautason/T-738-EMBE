import numpy as np
import matplotlib.pyplot as plt

# Load data from your .dat file
data = np.loadtxt("encoder.dat")

# Generate time values based on 10ms intervals
sample_interval_ms = 10
time = np.arange(len(data)) * (sample_interval_ms / 1000.0)  # Convert ms to seconds

# Plot the data with time on the x-axis
plt.plot(time, data)
plt.xlabel('Time (seconds)')
plt.ylabel('Data Value')
plt.title('Data vs. Time')
plt.grid(True)
plt.show()