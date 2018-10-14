#!/bin/bash

mkdir -p data

screen -X -S vnxwebserver quit

screen -d -m -S vnxwebserver vnxwebserver -c config/

