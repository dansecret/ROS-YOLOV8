#!/bin/bash

echo "Memory Usage"
while true
do
free -m | awk '/^Mem:/{printf("\033[1;33m %d kb\r",$4)}'
done

