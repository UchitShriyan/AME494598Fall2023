import pandas as pd
import matplotlib.pyplot as plt

# Load 'flat.csv'
flat_df = pd.read_csv('flat.csv', header=None)
flat_df.columns = ['aX', 'aY', 'aZ', 'gX', 'gY', 'gZ', 'direction']

# Load 'forward.csv'
forward_df = pd.read_csv('forward.csv', header=None)
forward_df.columns = ['aX', 'aY', 'aZ', 'gX', 'gY', 'gZ', 'direction']

# Load 'backward.csv'
backward_df = pd.read_csv('backward.csv', header=None)
backward_df.columns = ['aX', 'aY', 'aZ', 'gX', 'gY', 'gZ', 'direction']

# Plotting accelerometer data for 'flat.csv'
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(flat_df['aX'], label='aX')
plt.plot(flat_df['aY'], label='aY')
plt.plot(flat_df['aZ'], label='aZ')
plt.title('Accelerometer Data (Flat)')
plt.ylabel('Acceleration')
plt.legend()

# Plotting gyroscope data for 'flat.csv'
plt.subplot(2, 1, 2)
plt.plot(flat_df['gX'], label='gX')
plt.plot(flat_df['gY'], label='gY')
plt.plot(flat_df['gZ'], label='gZ')
plt.title('Gyroscope Data (Flat)')
plt.ylabel('Angular Velocity')
plt.legend()

plt.xlabel('Sample Number')
plt.tight_layout()
plt.show()
# Plotting accelerometer data for 'flat.csv'
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(backward_df['aX'], label='aX')
plt.plot(backward_df['aY'], label='aY')
plt.plot(backward_df['aZ'], label='aZ')
plt.title('Accelerometer Data (backward)')
plt.ylabel('Acceleration')
plt.legend()

# Plotting gyroscope data for 'flat.csv'
plt.subplot(2, 1, 2)
plt.plot(backward_df['gX'], label='gX')
plt.plot(backward_df['gY'], label='gY')
plt.plot(backward_df['gZ'], label='gZ')
plt.title('Gyroscope Data (backward)')
plt.ylabel('Angular Velocity')
plt.legend()

plt.xlabel('Sample Number')
plt.tight_layout()
plt.show()

# Plotting accelerometer data for 'flat.csv'
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(forward_df['aX'], label='aX')
plt.plot(forward_df['aY'], label='aY')
plt.plot(forward_df['aZ'], label='aZ')
plt.title('Accelerometer Data (Forward)')
plt.ylabel('Acceleration')
plt.legend()

# Plotting gyroscope data for 'flat.csv'
plt.subplot(2, 1, 2)
plt.plot(forward_df['gX'], label='gX')
plt.plot(forward_df['gY'], label='gY')
plt.plot(forward_df['gZ'], label='gZ')
plt.title('Gyroscope Data (Forward)')
plt.ylabel('Angular Velocity')
plt.legend()

plt.xlabel('Sample Number')
plt.tight_layout()
plt.show()

