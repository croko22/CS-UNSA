# make
# ./build/tensor_cpu
#!/bin/bash
set -e
make
for bin in build/bin/*; do
    echo "🔹 Running $bin"
    "$bin"
    echo ""
done
