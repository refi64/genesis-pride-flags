#!/bin/sh

RELEASES_URL=https://github.com/kubilus1/gendev/releases/download
VERSION=0.7.1

ARCHIVE=gendev_$VERSION.txz

curl -LO $RELEASES_URL/$VERSION/$ARCHIVE
tar -xvf $ARCHIVE --strip-components=1
