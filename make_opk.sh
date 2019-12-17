#!/bin/sh

VERSION=$(git log --pretty=format:'%h' -n 1)
OPK_NAME=nestopia-$VERSION.opk

echo nestopia


# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=nestopia
Comment=Nestopia 1.50-WIP $VERSION
Exec=nestopia %f
Terminal=false
Type=Application
StartupNotify=true
Icon=nestopia
Categories=emulators;
#X-OD-NeedsDownscaling=true
EOF

# create opk
FLIST="nestopia"
FLIST="${FLIST} default.gcw0.desktop"
FLIST="${FLIST} nestopia.png"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop
