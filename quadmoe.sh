#!/bin/sh
# Upload files to a QuadFile instance
# Requires jq

if [ "$#" -eq 0 ]; then
  echo "Usage: quadmoe.sh <file>" >&2
  exit 1
fi

url="https://file.quad.moe/" # Change this to use a different server
curl -F "file=@$1" "$url" | jq -r .url
