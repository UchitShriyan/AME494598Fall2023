import pandas as pd
import os

# Directory where files are located
directory = '/home/tulasi/ame/proj/Data'

# List to hold data from each file
all_data = []

# Iterate over the files in the directory
for filename in os.listdir(directory):
    if filename.endswith('.csv'):  # Checks if the file is a CSV
        file_path = os.path.join(directory, filename)
        df = pd.read_csv(file_path)
        all_data.append(df)

# Concatenate all data into a single DataFrame
combined_data = pd.concat(all_data, ignore_index=True)

# Save the combined data to a new CSV file
combined_data.to_csv('Data.csv', index=False)
