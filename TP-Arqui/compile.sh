#! /bin/bash

docker start SO

docker exec -it SO make clean -C/root/Toolchain
docker exec -it SO make clean -C/root
docker exec -it SO make -C/root/Toolchain
docker exec -it SO make -C/root

