#!/bin/bash

# Default value
USE_BUDDY=1

# Check the number of arguments
if [ "$#" -gt 1 ]; then
    echo "Error: Only one parameter or none is allowed."
    exit 1
fi

# Check if the argument is valid
if [ "$#" -eq 1 ]; then
    if [ "$1" == "--no-buddy" ]; then
        USE_BUDDY=0
    else
        echo "Error: Invalid parameter. Use '--no-buddy' or no parameter."
        exit 1
    fi
fi

docker start SO

# Run the commands inside the Docker container with the USE_BUDDY variable
docker exec -it SO make clean -C /root/Toolchain
docker exec -it SO make clean -C /root/
docker exec -it SO make -C /root/Toolchain
docker exec -it SO make -C /root/ USE_BUDDY=$USE_BUDDY

echo
if [ "$USE_BUDDY" -eq 1 ]; then
    echo "Compiled with Buddy Memory Manager"
else
    echo "Compiled with FreeArray Memory Manager"
fi