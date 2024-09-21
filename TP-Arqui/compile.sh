#! /bin/bash

docker start SO

docker exec -it SO make clean -C/root/TP2-SO/TP-Arqui/Toolchain
docker exec -it SO make clean -C/root/TP2-SO/TP-Arqui
docker exec -it SO make -C/root/TP2-SO/TP-Arqui/Toolchain
docker exec -it SO make -C/root/TP2-SO/TP-Arqui

