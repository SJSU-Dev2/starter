#!/bin/bash

# Find all directories that start with that "lib"
LIBRARIES_PACKAGES=$(ls packages | grep "^lib")

for library in ${LIBRARIES_PACKAGES}
do
  CURRENT_DIRECTORY=$(pwd)
  LIBRARY_SOURCE_DIRECTORY="packages/$library/$library"
  if [ -d ${LIBRARY_SOURCE_DIRECTORY} ] ; then
    echo "Adding library '${LIBRARY_SOURCE_DIRECTORY}' to library/ "
    ln -f -s ${CURRENT_DIRECTORY}/${LIBRARY_SOURCE_DIRECTORY} library/
  fi
done
