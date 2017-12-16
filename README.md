# QuadFile clients

This is a collection of scripts and programs for uploading stuff to
[QuadFile][qf]-powered servers.

Thanks to [this guy][quad] for making this thing and [hosting it][fqm]
for free.

[qf]: https://github.com/QuadPiece/QuadFile
[quad]: https://quad.moe/
[fqm]: https://file.quad.moe/

## What's in here?

### `quadmoe.sh`

Requires: bash, jq, curl

For people who want an easily-understandable script, or something they
can stick in their `.bashrc` as a function or alias.

### `quadmoe.py`

Requires: Python, requests

For people who like Python.

### `quadmoe.c`

Requires: A C compiler, make, libcurl

For people who love compiling. Or who want to run this thing on systems
without the dependencies for the other two. (But you've gotta ask
yourself: why?)
