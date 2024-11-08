# Install script for directory: /mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.4.3x" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.5.4.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.5"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/bin/libassimp.so.5.4.3"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/bin/libassimp.so.5"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.5.4.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.5"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.4.3x" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/bin/libassimp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/anim.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/aabb.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ai_assert.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/camera.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/color4.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/color4.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/config.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ColladaMetaData.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/commonMetaData.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/defs.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/cfileio.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/light.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/material.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/material.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/matrix3x3.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/matrix3x3.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/matrix4x4.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/matrix4x4.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/mesh.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ObjMaterial.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/pbrmaterial.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/GltfMaterial.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/postprocess.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/quaternion.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/quaternion.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/scene.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/metadata.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/texture.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/types.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/vector2.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/vector2.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/vector3.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/vector3.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/version.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/cimport.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/AssertHandler.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/importerdesc.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Importer.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/DefaultLogger.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ProgressHandler.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/IOStream.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/IOSystem.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Logger.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/LogStream.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/NullLogger.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/cexport.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Exporter.hpp"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/DefaultIOStream.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/DefaultIOSystem.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SceneCombiner.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/fast_atof.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/qnan.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/BaseImporter.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Hash.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ParsingUtils.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/StreamReader.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/StreamWriter.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/StringComparison.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/StringUtils.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SGSpatialSort.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/GenericProperty.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SpatialSort.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SmallVector.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SmoothingGroups.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/SmoothingGroups.inl"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/StandardShapes.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/RemoveComments.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Subdivision.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Vertex.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/LineSplitter.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/TinyFormatter.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Profiler.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/LogAux.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Bitmap.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/XMLTools.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/IOStreamBuffer.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/CreateAnimMesh.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/XmlParser.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/BlobIOSystem.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/MathFunctions.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Exceptional.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/ByteSwapper.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Compiler/poppack1.h"
    "/mnt/c/Users/Emmanuel/Desktop/Bentardv1/libs/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

