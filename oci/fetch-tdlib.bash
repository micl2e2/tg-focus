ARTIFACT_ID="952104308"
PICK_SRC="a0d026239439c76847c4f75f2fa32f3e56b9b789"

curl -L \
     -H "Accept: application/vnd.github+json" \
     -H "Authorization: Bearer $GITHUB_TOKEN" \
     -H "X-GitHub-Api-Version: 2022-11-28" \
     https://api.github.com/repos/micl2e2/tg-focus/actions/artifacts/$ARTIFACT_ID/zip \
     --output tdlib.zip

unzip tdlib.zip

podman load -i tdlib-$PICK_SRC.tar

CTN_TDLIB="package-tdlib-container"

buildah from --name $CTN_TGFOCUS localhost/tdlib-$PICK_SRC:latest
