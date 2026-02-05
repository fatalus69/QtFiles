#!/usr/bin/env bash

set -euo pipefail

apt_updated=0

# Only install deps on Debian based systems
. /etc/os-release
if [[ "$ID" == "debian" || "$ID_LIKE" == *debian* ]]; then
    for pkg in qt6-base-dev qt6-declarative-dev cmake g++; do
        if ! dpkg -s "$pkg" &> /dev/null; then
            read -p "May I install missing dependency: $pkg? (y/n) " proceed
            
            if [[ "$proceed" != "y" && "$proceed" != "Y" ]]; then
               exit 1
            fi

            if [ $apt_updated -eq 0 ]; then
                sudo apt update > /dev/null 2>&1
                apt_updated=1
            fi

            sudo apt install -y "$pkg" > /dev/null 2>&1
        fi
    done
fi