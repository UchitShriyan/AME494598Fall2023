# import pandas as pd

# # Load the combined data
# combined_df = pd.read_csv('forward_backward.csv')  # Update with the correct path

# # Split the data based on the sign of 'aX'
# forward_df = combined_df[combined_df['aX'] > 0.22]
# backward_df = combined_df[combined_df['aX'] < -0.22]
# flat_df = combined_df[(combined_df['aX'] > -0.22) & (combined_df['aX'] < 0.22)]


# # Save the separated data to new CSV files
# forward_df.to_csv('forward.csv', index=False)
# backward_df.to_csv('backward.csv', index=False)
# flat_df.to_csv('flat.csv', index=False)

# print("Data has been split into 'forward.csv' and 'backward.csv'.")



import pandas as pd

# Load the combined data
combined_df = pd.read_csv('left_right.csv')  # Update with the correct path

# Split the data based on the sign of 'aX'
right_df = combined_df[combined_df['aY'] > 0.22]
left_df = combined_df[combined_df['aY'] < -0.22]
flatlr_df = combined_df[(combined_df['aY'] > -0.22) & (combined_df['aY'] < 0.22)]


# Save the separated data to new CSV files
right_df.to_csv('right.csv', index=False)
left_df.to_csv('left.csv', index=False)
flatlr_df.to_csv('flatlr.csv', index=False)

print("Data has been split into 'left.csv' and 'right.csv'.")
