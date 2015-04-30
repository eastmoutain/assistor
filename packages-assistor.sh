#!/bin/bash

(cd ..
tar zcf assistor-1.0.tar.gz assistor
\mv -b assistor-1.0.tar.gz ~/rpmbuild/SOURCES/.
)
rpmbuild -bs --nodeps assistor.spec
if [ $? -ne 0 ]; then
	exit 1
fi

