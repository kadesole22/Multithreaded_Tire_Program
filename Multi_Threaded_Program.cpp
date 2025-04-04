//Name:		Kade Sole
//Program: 	CSC 377 Program 2
//Due date: 	March 15, 2025
#include <iostream>
#include <fstream>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	// Mutex to protect calculations
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;		// Condition variable

float sensor1_avg = 0.0;	// Global variable for sensor 1 average temp
float sensor2_avg = 0.0;	// Gloval variable for sensor 2 avg temp
int completed_threads = 0;	// Count to keep track of completions

// Global space for command line arguments
std::string file1, file2;
int N;

// Sensor reader function
void* sensor_reader(void* arg) {
    int sensor_id = *(int*)arg; // Get the sensor ID from argument
    std::string filename = (sensor_id == 1) ? file1 : file2;

    std::ifstream file(filename);
    if (!file) { //If file cannot be opened throw an error for debugging
	std::cerr << "Error: Could not open file" << filename << std::endl;
	return nullptr;
    }
    // Variables for calculating the average Temp
    float sum = 0.0;
    int count = 0;
    float value;

    while (count < N && file >> value) {
	sum += value; //Sum up the termperatures
	count++;      //Increment the current count so we approach N
    } 
    
    file.close();     //Once done close the file

    float average = sum / count; // Calculate the average temperature

    pthread_mutex_lock(&mutex);  //Lock the mutex before modifying the global variables

    // Update the global averages for the current sensor
    if (sensor_id  == 1) {
	sensor1_avg = average;
    } else{
	sensor2_avg = average;
    }

    completed_threads++;	// Increment the thread counter

    if(completed_threads == 2) {// Once both are done let the aggregator proceed
	pthread_cond_signal(&cond); // Signal condition variable
    }

    pthread_mutex_unlock(&mutex); // Unlock the mutex

    return nullptr;
}



// Aggregator thread function
void* aggregator(void* arg) {
    pthread_mutex_lock(&mutex); //Lock the mutex

     // Wait until both sensors are done
    while (completed_threads < 2){
	pthread_cond_wait(&cond, &mutex);
    }

    // Commpare the averages and print the highest
    if (sensor1_avg > sensor2_avg) {
	std::cout << "Sensor 1 has the higher average temperature: " << sensor1_avg << "\u00B0C" << std::endl;
    }else{
	std::cout << "Sensor 2 has the higher average temperature: " << sensor2_avg << "\u00B0C" << std::endl;
    }

    pthread_mutex_unlock(&mutex); // Unlock the mutex

    return nullptr;
}


// Main Function
int main(int argc, char* argv[]){
    if (argc != 4) { // Make sure the command lind argument are correct
	std::cerr << "Usage: " << argv[0] << " <file1> <file2> <N>" << std::endl;
	return -1;   // If incorrect, exit
    }

    // Assign command line arguments to the global variables
    file1 = argv[1];
    file2 = argv[2];
    N = std::stoi(argv[3]);

    // Create sensor reader threads
    pthread_t thread1, thread2, aggregator_thread;
    int id1 = 1, id2 = 2;	//Sensor ID
    pthread_create(&thread1, nullptr, sensor_reader, &id1);
    pthread_create(&thread2, nullptr, sensor_reader, &id2);
    pthread_create(&aggregator_thread, nullptr, aggregator, nullptr);

    // Wait for threads to finish
    pthread_join(thread1,nullptr);
    pthread_join(thread2,nullptr);
    pthread_join(aggregator_thread,nullptr);

    return 0;
}

