set -x

cwd=$(pwd)
bname=$(basename $cwd)
[[ $bname == tg-focus ]] || exit 255

###

cd ci

[[ $? -eq 0 ]] || exit 255

source vars-aarch64-linux-gnu.bash

[[ $? -eq 0 ]] || exit 255

source vars-allarch-linux-gnu.bash

[[ $? -eq 0 ]] || exit 255

source prep-toolc-deb11-allarch.bash
