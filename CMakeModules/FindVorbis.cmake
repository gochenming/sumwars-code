# - Try to find ogg/vorbis
# Once done this will define
#
#  VORBIS_FOUND - system has vorbis
#  VORBIS_INCLUDE_DIR
#  VORBIS_LIBRARIES - vorbis and vorbisfile libraries
#
# $VORBISDIR is an environment variable used
# for finding vorbis.
#
# Several changes and additions by Fabian 'x3n' Landau
# Most of all rewritten by Adrian Friedli
# Debug versions and simplifications by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/codec.h
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES include
)
FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
  NAMES libvorbis
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release
)
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES libvorbis vorbis_d vorbisD vorbis_D
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug
)
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES libvorbisfile
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release
)
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES libvorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug
)

# Handle the REQUIRED argument and set VORBIS_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
  VORBIS_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBIS_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(VORBIS)
HANDLE_LIBRARY_TYPES(VORBISFILE)
SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})

MARK_AS_ADVANCED(
  VORBIS_INCLUDE_DIR
  VORBIS_LIBRARY_OPTIMIZED
  VORBIS_LIBRARY_DEBUG
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_DEBUG
)
