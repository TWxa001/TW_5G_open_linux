#!/usr/bin/env bash
aclocal
libtoolize -c -f
autoconf
automake --add-missing --copy
