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

IF(WIN32)
FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
  NAMES libvorbis libvorbis_static libvorbis-static-mt
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release Win32/Release VS2010/Win32/Release Win32/VS2010/Win32/Release
)
ELSE()
FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
  NAMES vorbis
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

IF(WIN32)
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES libvorbis libvorbis_static vorbis_d vorbisD vorbis_D libvorbis-static-mt-debug
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug Win32/Debug VS2010/Win32/Debug Win32/VS2010/Win32/Debug
)
ELSE()
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES vorbis vorbis_d vorbisD vorbis_D
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

IF(WIN32)
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES libvorbisfile
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release Win32/Release VS2010/Win32/Release Win32/VS2010/Win32/Release
)
ELSE()
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES vorbisfile
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

IF(WIN32)
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES libvorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug Win32/Debug VS2010/Win32/Debug Win32/VS2010/Win32/Debug
)
ELSE()
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES vorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATH_SUFFIXES lib
)
ENDIF(WIN32)


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
  SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY}) #WIN32 includes VORBISFILE as a static lib
ELSE()
  SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} )
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
