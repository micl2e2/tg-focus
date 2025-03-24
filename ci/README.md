
# Image Dependency Relationship:

target `x64-linux-gnu`:

`tg-focus-cicd-toolc:gcc12-deb10-x64` depends on:
- `debian:buster-slim`

`tg-focus-cicd-tdlib:GITCMM-gcc12-deb10-x64` depends on:
- `debian:buster-slim`
- `tg-focus-cicd-toolc:gcc12-deb10-x64`

`tg-focus:GITTAG-x64-linux-gnu` depends on:
- `debian:buster-slim`
- `tg-focus-cicd-tdlib:GITCMM-gcc12-deb10-x64`

target `x64-linux-musl`:

`tg-focus-cicd-toolc:default-alp318-x64` depends on:
- `alpine:3.18`

`tg-focus-cicd-tdlib:GITCMM-default-alp318-x64` depends on:
- `alpine:3.18`
- `tg-focus-cicd-toolc:default-alp318-x64`

`tg-focus:GITTAG-x64-linux-musl` depends on:
- `alpine:3.18`
- `tg-focus-cicd-tdlib:GITCMM-default-alp318-x64`



# Artifacts

`docker.io/micl2e2/tg-focus-cicd-toolc:gcc12-deb10-x64`:
- id: 3dc5511482c72deb026884e955cb9063d15973217200a1b28a003e43cdb55dee
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6886233121
- notes: gcc version is 12.3.0

`docker.io/micl2e2/tg-focus-cicd-tdlib:030e440757478ef5f5fdb01f60c3c6d885330c72-gcc12-deb10-x64`:
- id: d158c48baa7091637da9bb383a51b5e1ad14b755f2c7a5a1f4f48160c50f7c44
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6887318188
- corresponding tdlib version: 1.8.14

`docker.io/micl2e2/tg-focus-cicd-tdlib:030e440757478ef5f5fdb01f60c3c6d885330c72-default-alp318-x64`:
- id: 7d7758d7afcdf4e7dd491b5cac96452a33c8d05410b05159a87dcedda62be298
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6892528556
- corresponding tdlib version: 1.8.14

`docker.io/micl2e2/tg-focus:1.4.1814-x64-linux-musl`
- id: 7acea365b23975c2cdb1ceed3a46dd8977cc36609ae2bbce0eba3494c19d5b48
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6899217558

`tg-focus-1.4.1814-x64-linux-gnu`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6888621611

`docker.io/micl2e2/tg-focus-cicd-tdlib:ec788c7505c4f2b31b59743d2f4f97d6fdcba451-gcc12-deb10-x64`
- id: ec4c47c38b820bf0596268dc15c8acef68c8bab06a6a36b3e2e508a5fb097161
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6911157079
- corresponding tdlib version: 1.8.21

`docker.io/micl2e2/tg-focus-cicd-tdlib:ec788c7505c4f2b31b59743d2f4f97d6fdcba451-default-alp318-x64`
- id: 42e063e7dbb26ef3871f3fbca936d00ed2ee64712c6a466226099b00a2a86b7d
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6912676245
- corresponding tdlib version: 1.8.21

`tg-focus-1.5-x64-linux-gnu`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6914534485

`docker.io/micl2e2/tg-focus:1.5-x64-linux-musl`
- id: 65a2124148a47174008aca7b3afc21adb8d3de0f5fe62a095c3d8bbf34d46bcc
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/6914536879

`docker.io/micl2e2/tg-focus-cicd-tdlib:5b9c066b345448c5f0eaa0642b8d2166007f82d8-gcc12-deb10-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/7931268658
- corresponding tdlib version: 1.8.24(5b9c066b345448c5f0eaa0642b8d2166007f82d8)

`docker.io/micl2e2/tg-focus-cicd-tdlib:5b9c066b345448c5f0eaa0642b8d2166007f82d8-default-alp318-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/7931268661
- corresponding tdlib version: 1.8.24(5b9c066b345448c5f0eaa0642b8d2166007f82d8)

`tg-focus-1.6-x64-linux-gnu`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/7941644282

`docker.io/micl2e2/tg-focus:1.6-x64-linux-musl`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/7941644284

`docker.io/micl2e2/tg-focus-cicd-toolc:gcc12-deb10-x64`:
- id: ddeffaa9ba1eb7427964c9932b7a6673440059bab481785c713da5f636940043
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/10962706054
- notes: gcc version is 12.4.0

`docker.io/micl2e2/tg-focus-cicd-tdlib:983b09578ee90cac504767c7f46b4ec915a56beb-gcc12-deb10-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11139710320
- tdlib version: 1.8.35(983b09578ee90cac504767c7f46b4ec915a56beb)

`docker.io/micl2e2/tg-focus-cicd-tdlib:983b09578ee90cac504767c7f46b4ec915a56beb-default-alp318-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11141376563
- tdlib version: 1.8.35(983b09578ee90cac504767c7f46b4ec915a56beb)

`docker.io/micl2e2/tg-focus-cicd-tdlib:15b86a17dc51caf5a3ba0b0acdb671cf8a27c80c-gcc12-deb10-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11406221324
- tdlib version: 1.8.36(15b86a17dc51caf5a3ba0b0acdb671cf8a27c80c)

`docker.io/micl2e2/tg-focus-cicd-tdlib:15b86a17dc51caf5a3ba0b0acdb671cf8a27c80c-default-alp318-x64`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11417919482
- tdlib version: 1.8.36(15b86a17dc51caf5a3ba0b0acdb671cf8a27c80c)

`tg-focus-2.0-x64-linux-gnu`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11417885682

`docker.io/micl2e2/tg-focus:2.0-x64-linux-musl`
- artifacts: https://github.com/micl2e2/tg-focus-cicd/actions/runs/11418348226

`docker.io/micl2e2/tg-focus-cicd-toolc:gcc12-deb11-x64`
- artifacts: https://github.com/micl2e2/tg-focus/actions/runs/13490163240
- digest: sha256:1e371d447729cadd53a295d33369b02205b369c6319b96dc2926be9bdc3996a6

`docker.io/micl2e2/tg-focus-cicd-toolc:gcc12-deb11-arm64`
- artifacts: https://github.com/micl2e2/tg-focus/actions/runs/13490163245
- digest: sha256:3c9354b439fdcb9adbe47ba4917f07bfb059af1fa735d755e04c1feb38dd6ed7


# Trigger An Action

## Step 1

For example, for target `build-tgfocus-gittag-x64-linux-musl`:

```
bash _before-action.bash 'run build-tgfocus-gittag-x64-linux-musl'
```

## Step 2

```
git add actiontrigger && git commit -m 'run build-tgfocus-gittag-x64-linux-musl'
```

# OCI Image Uploading

1. Download artifactes, including xxx.tar, checksum.
2. Verify artifacts.
1. `podman load -i xxx.tar`.
2. `podman tag xxx yyy`, `s/localhost/<REMOTE>\/<USERID>/`.
3. `podman login <REMOTE>`.
4. Push to <REMOTE>.
