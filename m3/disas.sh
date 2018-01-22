base="objdump -b binary --architecture=i386 -D -M intel floppya.img"

if [ $# -lt 1 ]; then
    echo "Usage: $0 [-pDir] | [sector [additional sectors]]"
elif [ "$1" == "-pDir" ]; then
    hexdump -C -n 512 -s 1024 floppya.img
else
    while [ "$1" != "" ]; do
        inst="$base --start-address=$(($1*512)) --stop-address=$(($1*512+512))"
        $inst
        shift
    done
fi