# retrieve id: https://api.github.com/repos/micl2e2/tg-focus/actions/artifacts

ARTIFACT_ID="953603201"
PICK_SRC="030e440757478ef5f5fdb01f60c3c6d885330c72"

curl -L \
     -H "Accept: application/vnd.github+json" \
     -H "Authorization: Bearer $GITHUB_TOKEN" \
     -H "X-GitHub-Api-Version: 2022-11-28" \
     https://api.github.com/repos/micl2e2/tg-focus/actions/artifacts/$ARTIFACT_ID/zip \
     --output tdlib.zip

unzip tdlib.zip

podman load -i tdlib-$PICK_SRC.tar

CTN_TDLIB="package-tdlib-container"

buildah from --name $CTN_TDLIB localhost/tdlib-$PICK_SRC:latest
