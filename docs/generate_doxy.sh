#!/usr/bin/bash
# Generate doxygen documentation
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)
DOXYDIR=$SCRIPT_DIR/doxygen
mkdir -p $DOXYDIR
cd $DOXYDIR && doxygen Doxyfile
echo "API reference generated: $DOXYDIR/html/index.html"
