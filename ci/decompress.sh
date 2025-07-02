#!/bin/bash
set -e


TARGET_DIR=${1:-.}
OUTPUT_DIR=${2:-"$TARGET_DIR/tg-focus"}

TAR_GZ_FILE=$(ls "$TARGET_DIR"/tg-focus-*-linux-*.tar.gz 2>/dev/null | head -n 1)

if [[ -z "$TAR_GZ_FILE" ]]; then
  echo "Could not find any tg-focus archive file in '$TARGET_DIR'"
  exit 1
fi


if [[ "$TAR_GZ_FILE" == *aarch64* ]]; then
  ARCH="aarch64"
elif [[ "$TAR_GZ_FILE" == *x86_64* ]]; then
  ARCH="x86_64"
else
  echo "Could not determine architecture from filename: $TAR_GZ_FILE"
  exit 1
fi

TAR_GZ_BASENAME=$(basename "$TAR_GZ_FILE")
CHECKSUM_FILE="$TARGET_DIR/CHECKSUM-${TAR_GZ_BASENAME}"

if [[ ! -f "$CHECKSUM_FILE" ]]; then
  echo "Could not find the checksum file: $CHECKSUM_FILE"
  exit 1
fi

TEMP_VERSION=${TAR_GZ_BASENAME#tg-focus-}
VERSION=${TEMP_VERSION%-linux-${ARCH}.tar.gz}

echo "Checking files in '$TARGET_DIR':"
echo "  ARCH: $ARCH"
echo "  TAR: $TAR_GZ_FILE"
echo "  CHECKSUM: $CHECKSUM_FILE"
echo "  Version: $VERSION"
echo "  Output directory: $OUTPUT_DIR"

CHECKSUM_FILE_BASENAME=$(basename "$CHECKSUM_FILE")
CHECKSUM_RESULT=$( (cd "$TARGET_DIR" && sha512sum -c "$CHECKSUM_FILE_BASENAME") 2>&1)

if echo "$CHECKSUM_RESULT" | grep -q "OK"; then
  echo "Checksum verification passed"
else
  echo "Checksum verification failed:"
  echo "$CHECKSUM_RESULT"
  exit 2
fi

mkdir -p "$OUTPUT_DIR"
tar -xzf "$TAR_GZ_FILE" --strip-components=1 -C "$OUTPUT_DIR"

echo "Decompression complete, contents extracted to '$OUTPUT_DIR' directory"
echo "Successfully decompressed tg-focus version: $VERSION"