# Multithreaded_Tire_Program
This program uses multihreaded programming to calculate and compare the average tire temperatures from two different sensor data files. 

Usage: 
The program requires three command-line arguments.
1. First data file (e.g., 02day01.out)
2. Second data file(e.g.,02day02.out)
3. Number of temperature readings to process

Example:
./Multi_Threaded_Program 02day01.out 02day02.out 150
Sample Output:
Sensor 2 has the higher average temperature: 17.17136 degrees Celsius

Notes:
The program utilizes threads to process each file concurrently, improving efficiency with large datasets.
Make sure both input files are formatted correctly and contain sufficient data for the specified number of readings. 
