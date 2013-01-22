# Find Boost includes and library - use versions that are distributed with Ogre.
# This is very important, as using different library versions is not possible with the SDK.
# INCLUDE(DetermineVersion)
# INCLUDE(FindPackageHandleAdvancedArgs)

# Only tested for Win32 so far.
IF(WIN32)
  INCLUDE(FindPackageHandleStandardArgs)
  INCLUDE(HandleLibraryTypes)
  INCLUDE(BoostOGREGetVersion)
  INCLUDE(DetermineOgreThreadProvider)

  FIND_PATH(OGRE_BUILD_SETTINGS_FILE /OgreBuildSettings.h
    PATHS ${OGRE_INCLUDE_DIR}
    PATH_SUFFIXES /include /OgreMain/include
  )
  Message (STATUS "Checking file ${OGRE_BUILD_SETTINGS_FILE}/OgreBuildSettings.h")

  # Check to see which Thread Provider is used by OGRE
  DETERMINE_OGRE_THREAD_PROVIDER(OGRE_THREAD_PROVIDER ${OGRE_BUILD_SETTINGS_FILE}/OgreBuildSettings.h)
  Message (STATUS "Found OGRE_THREAD_PROVIDER as: ${OGRE_THREAD_PROVIDER}")
  # provider legend:
  # 1 = boost
  # 2 = poco
  # 3 = tbb
  
  IF(${OGRE_THREAD_PROVIDER} EQUAL 1)
    Message (STATUS "OGRE_THREAD_PROVIDER is BOOST. Will add boost dependencies...")
    
    
    # Set the boost search path. Prioritize the Dependencies folder.
    SET (BST_SRCPATH ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost $ENV{OGRE_HOME}/boost_1_44 $ENV{OGRE_HOME}/boost_1_47 $ENV{BOOST_ROOT})
    FIND_PATH(BoostOGRE_INCLUDE_DIR /boost/version.hpp
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES /boost /boost_1_44 /boost_1_47
    )
    
    Message (STATUS "Found BoostOGRE_INCLUDE_DIR as: ${BoostOGRE_INCLUDE_DIR}")
    
    # Get the used version (stored in version.h). Required to know against which libs to link.
    # This will store the result into ${BoostOGRE_LIB_VERSION}
    boost_ogre_get_version(${BoostOGRE_INCLUDE_DIR}/boost/version.hpp)
    Message (STATUS "Found BoostOGRE_LIB_VERSION as: ${BoostOGRE_LIB_VERSION}")
    
    FIND_LIBRARY(BoostOGRE_THREAD_LIBRARY_DEBUG
      NAMES libboost_thread-vc100-mt-gd-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
  
    FIND_LIBRARY(BoostOGRE_THREAD_LIBRARY_OPTIMIZED
      NAMES libboost_thread-vc100-mt-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
    
    FIND_LIBRARY(BoostOGRE_DATETIME_LIBRARY_DEBUG
      NAMES libboost_date_time-vc100-mt-gd-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
  
    FIND_LIBRARY(BoostOGRE_DATETIME_LIBRARY_OPTIMIZED
      NAMES libboost_date_time-vc100-mt-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
  
    FIND_LIBRARY(BoostOGRE_SYSTEM_LIBRARY_DEBUG
      NAMES libboost_system-vc100-mt-gd-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
  
    FIND_LIBRARY(BoostOGRE_SYSTEM_LIBRARY_OPTIMIZED
      NAMES libboost_system-vc100-mt-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
    
    FIND_LIBRARY(BoostOGRE_CHRONO_LIBRARY_DEBUG
      NAMES libboost_chrono-vc100-mt-gd-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
  
    FIND_LIBRARY(BoostOGRE_CHRONO_LIBRARY_OPTIMIZED
      NAMES libboost_chrono-vc100-mt-${BoostOGRE_LIB_VERSION}
      PATHS ${BST_SRCPATH}
      PATH_SUFFIXES lib bin
    )
    
    # Handle the REQUIRED argument
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(BoostOGRE DEFAULT_MSG
        BoostOGRE_INCLUDE_DIR
        BoostOGRE_THREAD_LIBRARY_DEBUG
        BoostOGRE_THREAD_LIBRARY_OPTIMIZED
        BoostOGRE_DATETIME_LIBRARY_DEBUG
        BoostOGRE_DATETIME_LIBRARY_OPTIMIZED
        # Augustin Preda, 2013.01.18: Starting with Boost 1.50, Boost's Thread Library uses the Boost.Chrono, which 
        # in turn uses Boost.System. Sooooo we need these as well.
        BoostOGRE_SYSTEM_LIBRARY_DEBUG
        BoostOGRE_SYSTEM_LIBRARY_OPTIMIZED
        BoostOGRE_CHRONO_LIBRARY_DEBUG
        BoostOGRE_CHRONO_LIBRARY_OPTIMIZED
    )
  
    # Collect optimized and debug libraries (will auto-use names _LIBRARY_DEBUG & _LIBRARY_OPTIMIZED
    HANDLE_LIBRARY_TYPES(BoostOGRE_THREAD)
    # Collect optimized and debug libraries
    HANDLE_LIBRARY_TYPES(BoostOGRE_DATETIME)
    # Collect optimized and debug libraries
    HANDLE_LIBRARY_TYPES(BoostOGRE_SYSTEM)
    # Collect optimized and debug libraries
    HANDLE_LIBRARY_TYPES(BoostOGRE_CHRONO)
    
    Message (STATUS "Found BoostOGRE_THREAD_LIBRARY as: ${BoostOGRE_THREAD_LIBRARY}")  
    Message (STATUS "Found BoostOGRE_DATETIME_LIBRARY as: ${BoostOGRE_DATETIME_LIBRARY}")  
    Message (STATUS "Found BoostOGRE_SYSTEM_LIBRARY as: ${BoostOGRE_SYSTEM_LIBRARY}")  
    Message (STATUS "Found BoostOGRE_CHRONO_LIBRARY as: ${BoostOGRE_CHRONO_LIBRARY}")  
    SET(BoostOGRE_LIBRARIES ${BoostOGRE_THREAD_LIBRARY} ${BoostOGRE_DATETIME_LIBRARY} ${BoostOGRE_SYSTEM_LIBRARY} ${BoostOGRE_CHRONO_LIBRARY})
    
    # Mark these entries visible only in the advanced section.
    MARK_AS_ADVANCED(
      BoostOGRE_INCLUDE_DIR
      BoostOGRE_THREAD_LIBRARY_OPTIMIZED
      BoostOGRE_THREAD_LIBRARY_DEBUG
      BoostOGRE_DATETIME_LIBRARY_OPTIMIZED
      BoostOGRE_DATETIME_LIBRARY_DEBUG
      BoostOGRE_SYSTEM_LIBRARY_DEBUG
      BoostOGRE_SYSTEM_LIBRARY_OPTIMIZED
      BoostOGRE_CHRONO_LIBRARY_DEBUG
      BoostOGRE_CHRONO_LIBRARY_OPTIMIZED
    )
    
    Message (STATUS "Found BoostOGRE_LIBRARIES as: ${BoostOGRE_LIBRARIES}")
  ENDIF(${OGRE_THREAD_PROVIDER} EQUAL 1)

ENDIF(WIN32)

