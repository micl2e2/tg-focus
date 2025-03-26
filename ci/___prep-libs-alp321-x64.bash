set -x

cwd=$(pwd)
bname=$(basename $cwd)
[[ $bname == tg-focus ]] || exit 255

###

cd ci

[[ $? -eq 0 ]] || exit 255

source vars-x64-linux-musl.bash

[[ $? -eq 0 ]] || exit 255

source vars-allarch-linux-musl.bash

[[ $? -eq 0 ]] || exit 255

source prep-libs-alp321-allarch.bash
