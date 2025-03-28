set -x

cwd=$(pwd)
bname=$(basename $cwd)
[[ $bname == tg-focus ]] || exit 255

###

cd ci

[[ $? -eq 0 ]] || exit 255

source vars-x86_64-linux-gnu.bash

[[ $? -eq 0 ]] || exit 255

source vars-allarch-linux-gnu.bash

[[ $? -eq 0 ]] || exit 255

source pack-tgfocus-deb11-allarch.bash
