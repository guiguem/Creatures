# Minimum cmake verison 3.1 required for the variable CMAKE_CXX_STANDARD
cmake_minimum_required (VERSION 3.1)

#########
# setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( ToolDAQ VERSION 2.2.0 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( PackageBuilder )

pbuilder_prepare_project()


# pbuilder_add_submodule( Scarab src/external/scarab/library )


######
# ROOT
######


list (APPEND Root_COMPONENTS treeplayer)
find_package (ROOT 6.00 REQUIRED COMPONENTS ${Root_COMPONENTS})
if (ROOT_FOUND)
    pbuilder_add_ext_libraries (${ROOT_LIBRARIES})
else (ROOT_FOUND)
    message(FATAL "Unable to find ROOT")
endif (ROOT_FOUND)
include_directories (${ROOT_INCLUDE_DIR})

# Boost (1.46 required for filesystem version 3)
list (APPEND Boost_COMPONENTS date_time filesystem program_options system thread)
#find_package (Boost 1.46.0 REQUIRED COMPONENTS date_time filesystem program_options system thread)
find_package (Boost 1.46.0 REQUIRED COMPONENTS ${Boost_COMPONENTS})
# make sure dynamic linking is assumed for all boost libraries
add_definitions (-DBOOST_ALL_DYN_LINK)
include_directories (${Boost_INCLUDE_DIRS})
pbuilder_add_ext_libraries (${Boost_LIBRARIES})

#######
# BOOST
#######

# Boost
# Boost (1.48 required for container; scarab minimum is 1.46)
list (APPEND Boost_COMPONENTS date_time filesystem iostreams program_options serialization system thread)
# python optional
if (Katydid_USE_PYTHON)
    list(APPEND Boost_COMPONENTS python)
endif (Katydid_USE_PYTHON)
#find_package (Boost 1.46.0 REQUIRED COMPONENTS date_time filesystem program_options system thread)
find_package (Boost 1.46.0 REQUIRED COMPONENTS ${Boost_COMPONENTS})
# make sure dynamic linking is assumed for all boost libraries
add_definitions (-DBOOST_ALL_DYN_LINK)
include_directories (${Boost_INCLUDE_DIRS})
pbuilder_add_ext_libraries (${Boost_LIBRARIES})


## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

set( ToolDAQ_HEADERS)

###########
# MyTools #
###########

set ( MyTools_headers
    Tool.h
    Wrench.h
    ScrewDriver.h
)

set( MyTools_sources
    Wrench.cpp
    ScrewDriver.cpp
)

pbuilder_library( MyTools MyTools_sources "")
pbuilder_install_headers( ${MyTools_headers} )

###########
# ToolChain #
###########

set ( ToolChain_headers
    Tool.h
    ToolChain.h
)

set( ToolChain_sources
    ToolChain.cpp
)

pbuilder_library( ToolChain ToolChain_sources "")
pbuilder_install_headers( ${ToolChain_headers} )

###########
# Main #
###########

set (APPS_HEADERFILES

)

set( LIB_DEPENDENCIES
ToolChain
MyTools
)

set( PROGRAMS
test
)

pbuilder_executables( PROGRAMS LIB_DEPENDENCIES )
pbuilder_install_headers (${APPS_HEADERFILES})