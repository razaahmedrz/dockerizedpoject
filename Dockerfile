FROM ubuntu:18.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    wget \
    libomp-dev

# Set the working directory
WORKDIR /app

# Copy the code into the container
COPY hangman.c .

# Compile the code with OpenMP support
RUN gcc -fopenmp hangman.c -o hangman

# Run the code
CMD ["./hangman"]
