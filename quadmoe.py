#!/usr/bin/env python3
# Upload files to a QuadFile instance
# Requires requests

URL = "https://file.quad.moe/" # Change this to use a different server

import requests, sys

if len(sys.argv) <= 1:
    sys.stderr.write("Usage: quadmoe.py <file>\n")
    sys.exit(1)

with open(sys.argv[1]) as f:
    print(requests.post(URL, files={'file': f}).json()['url'])
