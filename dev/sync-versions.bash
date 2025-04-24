
if [[ ! $(pwd) =~ tg-focus$ ]]
then
    echo 'sync failed!'
    exit 1
fi

TDLIB_SRC_PREV=$(cat dev/pick-src-tdlib.prev)
test $? -eq 0 || { echo 'sync failed!'; exit 2; }
TDLIB_SRC_CURR=$(cat dev/pick-src-tdlib)
test $? -eq 0 || { echo 'sync failed!'; exit 3; }

TDLIB_VER_PREV=$(cat dev/pick-ver-tdlib.prev)
test $? -eq 0 || { echo 'sync failed!'; exit 4; }
TDLIB_VER_CURR=$(cat dev/pick-ver-tdlib)
test $? -eq 0 || { echo 'sync failed!'; exit 5; }

TF_VER_PREV=$(cat dev/pick-ver-tgfocus.prev)
test $? -eq 0 || { echo 'sync failed!'; exit 6; }
TF_VER_CURR=$(cat dev/pick-ver-tgfocus)
test $? -eq 0 || { echo 'sync failed!'; exit 7; }

# tdlib ver
grep --quiet "TDLIB_VER = \"${TDLIB_VER_PREV}\";" src/common.hh
if [[ $? -eq 0 ]]
then
    sed -i -E \
	"s/TDLIB_VER = \"${TDLIB_VER_PREV}\";/TDLIB_VER = \"${TDLIB_VER_CURR}\";/" \
	src/common.hh
    if [[ $? -eq 0 ]]
    then
	echo 'sync: tdlib version'
    fi

fi

# tdlib src
grep --quiet "TDLIB_SRC = \"${TDLIB_SRC_PREV}\";" src/common.hh
if [[ $? -eq 0 ]]
then
    sed -i -E \
	"s/TDLIB_SRC = \"${TDLIB_SRC_PREV}\";/TDLIB_SRC = \"${TDLIB_SRC_CURR}\";/" \
	src/common.hh
    if [[ $? -eq 0 ]]
    then
	echo 'sync: tdlib source'
    fi
    sed -i -E \
	"s/base=${TDLIB_SRC_PREV}/base=${TDLIB_SRC_CURR}/" \
	README.md
    sed -i -E \
	"s/tdlib-${TDLIB_VER_PREV}-%2332a9de/tdlib-${TDLIB_VER_CURR}-%2332a9de/" \
	README.md
    sed -i -E \
	"s/base=${TDLIB_SRC_PREV}/base=${TDLIB_SRC_CURR}/" \
	README.org
    sed -i -E \
	"s/tdlib-${TDLIB_VER_PREV}-%2332a9de/tdlib-${TDLIB_VER_CURR}-%2332a9de/" \
	README.org
    if [[ $? -eq 0 ]]
    then
	echo 'sync: tdlib readme source'
	# cp dev/pick-src-tdlib{,.prev}
    fi
fi

# tgf ver
grep --quiet "TF_VER = \"${TF_VER_PREV}\";" src/common.hh
if [[ $? -eq 0 ]]
then
    sed -i -E \
	"s/TF_VER = \"${TF_VER_PREV}\";/TF_VER = \"${TF_VER_CURR}\";/" \
	src/common.hh
    if [[ $? -eq 0 ]]
    then
	echo 'sync: tgf version'
	# cp dev/pick-ver-tgfocus{,.prev}
    fi
fi

echo 'sync successfully!'

