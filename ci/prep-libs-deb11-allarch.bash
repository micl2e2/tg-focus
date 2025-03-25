set -x

sudo apt-get -o Acquire::ForceIPv4=true update
[[ $? -eq 0 ]] || exit 255

# ----------------------------- tdlib -----------------------------

sudo apt-get -o Acquire::ForceIPv4=true install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet
[[ $? -eq 0 ]] || exit 255

[[ -d ../3rd/tdlib/.git ]] || $PXY_FRONTEND git clone --depth=1000 https://github.com/tdlib/td ../3rd/tdlib
[[ $? -eq 0 ]] || exit 255

git --git-dir=../3rd/tdlib/.git --work-tree=../3rd/tdlib reset --hard $(cat ../dev/pick-src-tdlib)
[[ $? -eq 0 ]] || exit 255

# ------------------------------ toml ------------------------------

[[ -d ../3rd/toml11/.git ]] || $PXY_FRONTEND git clone --depth=1000 https://github.com/ToruNiina/toml11 ../3rd/toml11
[[ $? -eq 0 ]] || exit 255

git --git-dir=../3rd/toml11/.git --work-tree=../3rd/toml11 reset --hard $(cat ../dev/pick-src-toml11)
[[ $? -eq 0 ]] || exit 255

# ---------------------------- tgfocus ----------------------------

sudo apt-get -o Acquire::ForceIPv4=true install ccache locales -y --quiet
[[ $? -eq 0 ]] || exit 255

sudo sed -i 's/# en_HK.UTF-8/en_HK.UTF-8/' /etc/locale.gen
[[ $? -eq 0 ]] || exit 255
sudo sed -i 's/# en_US.UTF-8/en_US.UTF-8/' /etc/locale.gen
[[ $? -eq 0 ]] || exit 255
sudo sed -i 's/# en_ZW.UTF-8/en_ZW.UTF-8/' /etc/locale.gen
[[ $? -eq 0 ]] || exit 255
sudo sed -i 's/# zh_CN.UTF-8/zh_CN.UTF-8/' /etc/locale.gen
[[ $? -eq 0 ]] || exit 255
sudo sed -i 's/# zh_HK.UTF-8/zh_HK.UTF-8/' /etc/locale.gen
[[ $? -eq 0 ]] || exit 255
sudo locale-gen
[[ $? -eq 0 ]] || exit 255
