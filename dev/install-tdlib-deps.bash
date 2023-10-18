distro_id=$(cat /etc/os-release)

os_id=0 # 1:debian 2:fedora

# Check distros
if [[ $distro_id =~ "debian" ]]
then
   os_id=1
elif [[ $distro_id =~ "fedora" ]]
then
   os_id=2
else
    echo '[ERROR] this platform is not supported'
    exit 1
fi

# Install packages
if [[ $os_id -eq 1 ]]
then
   su -c 'apt install gperf zlib-dev libssl-dev -y --quiet'
   # TODO
elif [[ $os_id -eq 2 ]]
then
   su -c 'dnf install gperf zlib-devel openssl-devel cmake -y --quiet'
fi

test $? -eq 0 || (echo '[ERROR] installing' && exit 2)

echo "[INFO] done, tdlib dependencies installed!"
