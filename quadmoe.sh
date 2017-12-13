#!/bin/sh
# Upload files to a QuadFile instance
# Requires jq

url="https://file.quad.moe/" # Change this to use a different server
curl -F "file=@$1" "$url" | jq -r .url
