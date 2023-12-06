import pandas as pd

# Path to CSV file
file_path = 'Data.csv'

# Prepare an empty list to collect rows
data_rows = []

# Open and process the file line by line
with open(file_path, 'r') as file:
    for line in file:
        # Split the line by comma and keep only the first 7 elements
        row = line.strip().split(',')[:7]
        # Append the processed row to the list
        data_rows.append(row)

# Create a DataFrame from the processed rows
cleaned_data = pd.DataFrame(data_rows, columns=['aX', 'aY', 'aZ', 'gX', 'gY', 'gZ', 'direction'])

# Save the cleaned data
cleaned_data.to_csv('cleaned_data.csv', index=False)
