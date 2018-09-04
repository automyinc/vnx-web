#!/bin/bash

VNX_INTERFACE_DIR=${VNX_INTERFACE_DIR:-/usr/interface}

cd $(dirname "$0")

vnxcppcodegen --cleanup generated/ vnx.web interface/ modules/ ${VNX_INTERFACE_DIR}/vnx/ query/interface/

./query/codegen.sh

