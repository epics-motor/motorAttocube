#!/bin/bash

export LD_LIBRARY_PATH=$(pwd)/lib/linux-x86_64:$LD_LIBRARY_PATH

cd iocs/attocubeIOC/iocBoot/iocAttocube && ./st.cmd
