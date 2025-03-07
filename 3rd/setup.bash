
[[ $(basename $(pwd)) == 3rd ]] || { exit 1; }

# ----------------------------- tdlib -----------------------------

[[ -d tdlib/.git ]] || git clone --depth=1000 https://github.com/tdlib/td tdlib

git --git-dir=tdlib/.git --work-tree=tdlib reset --hard $(cat ../dev/pick-src-tdlib)

# ------------------------------ toml ------------------------------

[[ -d toml11/.git ]] || git clone --depth=1000 https://github.com/ToruNiina/toml11 toml11

git --git-dir=toml11/.git --work-tree=toml11 reset --hard $(cat ../dev/pick-src-toml11)
