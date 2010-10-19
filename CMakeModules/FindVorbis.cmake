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
IF(WIN32)
  NAMES libvorbis libvorbis-static-mt
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release
ELSE()
  NAMES vorbis
  PATH_SUFFIXES lib
ENDIF(WIN32)
)
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
IF(WIN32)
  NAMES libvorbis vorbis_d vorbisD vorbis_D libvorbis-static-mt-debug
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug
ELSE()
  NAMES vorbis vorbis_d vorbisD vorbis_D
  PATH_SUFFIXES lib
ENDIF(WIN32)
)
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
IF(WIN32)
  NAMES libvorbisfile
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release
ELSE()
  NAMES vorbisfile
  PATH_SUFFIXES lib
ENDIF(WIN32)
)
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
IF(WIN32)
  NAMES libvorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug
ELSE()
  NAMES vorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATH_SUFFIXES lib
ENDIF(WIN32)
)

# Handle the REQUIRED argument and set VORBIS_FOUND
IF(NOT WIN32)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
  VORBIS_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBIS_INCLUDE_DIR
)
ELSE()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
  VORBIS_LIBRARY_OPTIMIZED
  VORBIS_INCLUDE_DIR
)
ENDIF(NOT WIN32)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(VORBIS)
IF(NOT WIN32)
  HANDLE_LIBRARY_TYPES(VORBISFILE) 
  SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY}) #WIN32 includes VORBISFILE as a static lib
ELSE()
  SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})
ENDIF(NOT WIN32)

MARK_AS_ADVANCED(
  VORBIS_INCLUDE_DIR
  VORBIS_LIBRARY_OPTIMIZED
  VORBIS_LIBRARY_DEBUG
  IF(NOT WIN32)
    VORBISFILE_LIBRARY_OPTIMIZED
    VORBISFILE_LIBRARY_DEBUG
  ENDIF(NOT WIN32)
)
