run_what="$1"

if [[ $run_what =~ ^run\ build-[a-z0-9]+ ]]
then
    echo "$run_what $(TZ=UTC date)" >>.actiontrigger
    echo '[INFO] done'
else
    echo '[INFO] bad input, arg1 should be <run xxx>'
fi

