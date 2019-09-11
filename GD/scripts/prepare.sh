#!/usr/bin/env bash -e
# This script extracts a Blackberry Dynamics TAR file, yanks out the x86_64/arm64 images, and then replaces it.
# You can use this to 
if [[ ! "$#" -gt 0 ]]; then
  echo $0 "<BB tar file>"
  exit
fi

BB_FILE=$1

if [[ ! -e $BB_FILE ]]; then
  echo "File $BB_FILE does not exist"
fi

TMPDIR=`mktemp -d`

pushd $TMPDIR
tar xvfp $BB_FILE

PKG_FILE=`ls *.pkg`
EXPAND_DIR="pkg"
pkgutil --expand $PKG_FILE $EXPAND_DIR

tar xvfpz $EXPAND_DIR/goodplatform.pkg/Payload 

GOOD_DIR="GD"
mkdir -p $GOOD_DIR
tar xvfp Good.platform.tar -C $TMPDIR/$GOOD_DIR
pushd $GOOD_DIR

MACH_BINARY="Good.platform/iOS/Frameworks/GD.framework/Versions/A/GD"
MACH_BINARY_NEW="$MACH_BINARY.new"
lipo -extract arm64 -extract x86_64 $MACH_BINARY -output $MACH_BINARY_NEW
mv $MACH_BINARY_NEW $MACH_BINARY

pushd Good.platform/iOS

NEW_ZIP_DIR=`mktemp -d`
FRAMEWORK_DEST="$NEW_ZIP_DIR/GD"
FIPS_DIR="$NEW_ZIP_DIR/GD/FIPS_module"

mkdir -p $FRAMEWORK_DEST
mkdir -p $FIPS_DIR

cp -a Frameworks/ $NEW_ZIP_DIR/GD
tar xvfp FIPS_module.tar -C $NEW_ZIP_DIR/GD/FIPS_module arm64.sdk x86_64.sdk 

pushd $NEW_ZIP_DIR
rm -f /tmp/Blackberry.tar
tar cvfp /tmp/Blackberry.tar .

echo "Done!  Look inside /tmp/Blackberry.tar for the latest."