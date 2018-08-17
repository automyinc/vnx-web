#!/bin/bash

cd $(dirname "$0")

vnxcppcodegen --cleanup generated/ vnx.web interface/ modules/ /usr/interface/vnx/

