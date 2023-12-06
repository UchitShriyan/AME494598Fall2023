import serial
import csv
import time

# Setup the serial connection
PORT = '/dev/ttyACM0'  # Arduino's port
BAUD_RATE = 115200  # Baud rate same as mentioned in the Arduino code
ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
ser.flush()

# Data collection code
gestures = ['forward_backward', 'left_right', 'diagonal_forward_left', 'backward_right', 'diagonal_forward_right', 'backward_left', 'rotation', 'speed_up_down']
current_gesture = ''

while True:
    print("Choose a gesture for data collection:")
    for idx, gesture in enumerate(gestures):
        print(f"{idx}. {gesture}")
    choice = input("Enter the number (or 'q' to quit): ")

    if choice == 'q':
        break
    elif choice in map(str, range(len(gestures))):
        current_gesture = gestures[int(choice)]
        print(f"Collecting data for {current_gesture} gesture. Press 'CTRL+C' to stop...")
        
        with open(f"{current_gesture}.csv", "a", newline='') as file:
            writer = csv.writer(file)
            
            try:
                while True:
                    line = ser.readline().decode('utf-8').strip()
                    if line:
                        values = line.split(',')
                        if len(values) == 6:
                            aX, aY, aZ, gX, gY, gZ = map(float, values)
                            writer.writerow([aX, aY, aZ, gX, gY, gZ, current_gesture])
                    time.sleep(0.1)  # To match Arduino's delay
            except KeyboardInterrupt:
                print(f"Data collection for {current_gesture} gesture stopped.")
    else:
        print("Invalid choice!")

ser.close()
