#!/bin/bash

# Configurable settings
max_threads=100             # Maximum number of threads to test
sample_size=1               # Number of samples to collect per thread count
program_name="mdu"          # Executable to run
target_dir="/pkg/"          # Directory to measure disk usage

# Output files
result_file="performance_results.txt"    # File to save timing results
error_file="errors.log"                       # File to capture error messages

# Compile the program
make

# Set locale to ensure float numbers use periods instead of commas
export LC_NUMERIC="en_US.UTF-8"

# Configure time output format
TIMEFORMAT='%3R'  # Only record real (elapsed) time in seconds

# Initialize output files
> "$result_file"
> "$error_file"

# Display configuration settings
echo -e "\nConfiguration: max_threads=$max_threads, samples_per_thread=$sample_size, target_directory=$target_dir\n"
echo "Starting performance tests:"

# Loop through each thread count from 1 up to max_threads
for ((threads = 1; threads <= max_threads; threads++)); do
    # Inform about the current thread count in progress
    printf "Threads = %-3d: Running " "$threads"
    printf "%-3d " "$threads" >> "$result_file"

    # Run the executable for the specified sample size
    for ((sample = 1; sample <= sample_size; sample++)); do
        # Measure execution time and log to performance file
        exec_time=$( { time ./"$program_name" -j"$threads" "$target_dir" > /dev/null 2>> "$error_file"; } 2>&1 )
        printf "%s " "$exec_time" >> "$result_file"
        printf "."
    done

    # End line in performance file after all samples for this thread count
    echo >> "$result_file"
    echo " completed."
done

# Indicate the end of testing
echo "The performance analysis of the program has been completed."
