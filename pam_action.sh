#!/bin/sh

PIN=$1
SECRET=
if echo "$PIN" | grep -q '[13579]$'; then
  read -s -p "Context secret: " SECRET
fi
/usr/sbin/cernvm-online.sh pair "$PIN" "$SECRET"
