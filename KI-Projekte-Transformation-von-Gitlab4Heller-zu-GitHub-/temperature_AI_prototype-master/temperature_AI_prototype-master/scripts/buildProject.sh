#!/bin/bash
# Copyright (c) Siemens AG 2021  All Rights Reserved.

rm -rf build
mkdir build

# general steps
cp ./package/docker-compose.yaml ./build
cp ./package/Dockerfile ./build
cp -r ./source/EdgeAPI ./build/EdgeAPI
cp ./config/delta.json ./build

#python specific steps
cp ./source/appconfig.json ./build
cp ./source/requirements.txt./build
cp ./source/main.py ./build
cp ./source/database_service.py ./build
cp ./source/parameter_handler.py ./build
cp ./source/specificConfig_reader.py ./build

exit 0

