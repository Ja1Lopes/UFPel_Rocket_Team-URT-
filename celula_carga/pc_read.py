import serial

# Open a serial connection (adjust the port and baud rate as needed)
ser = serial.Serial("COMX", 115200)  # Replace 'COMX' with your Arduino's port

# Create and open a text file for writing
with open("weight_data.txt", "w") as file:
    while True:
        # Read a line of data from the Arduino
        data = ser.readline().decode().strip()

        # Write the data to the text file
        file.write(data + "\n")

        # Print the data to the console
        print(data)
