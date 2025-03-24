cwd=$(pwd)
bname=$(basename $cwd)
[[ $bname == tg-focus ]] || exit 255

###

cd ci

[[ $? -eq 0 ]] || exit 101

source vars-x64-linux-gnu.bash

[[ $? -eq 0 ]] || exit 102

source vars-allarch-linux-gnu.bash

[[ $? -eq 0 ]] || exit 103

source toolc-gcc12-deb11-allarch.bash
