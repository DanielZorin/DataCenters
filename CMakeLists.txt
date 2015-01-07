CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(DCGUI)

macro(configure_files srcDir destDir)
   message(STATUS "Configuring directory ${destDir}")
   make_directory(${destDir})

   file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
   foreach(templateFile ${templateFiles})
      set(srcTemplatePath ${srcDir}/${templateFile})
      if(NOT IS_DIRECTORY ${srcTemplatePath})
         message(STATUS "Configuring file ${templateFile}")
         configure_file(
            ${srcTemplatePath}
            ${destDir}/${templateFile}
            @ONLY)
      else(NOT IS_DIRECTORY ${srcTemplatePath})
         configure_files(${srcTemplatePath} ${destDir}/${templateFile})
      endif(NOT IS_DIRECTORY ${srcTemplatePath})
   endforeach(templateFile)
endmacro(configure_files)

SET(GUI_COPY_DIRS
   Core
   Methods
   plugins
   DCGUI)


foreach(DIR ${GUI_COPY_DIRS})
   configure_files(${CMAKE_CURRENT_SOURCE_DIR}/${DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}/${DIR})
endforeach(DIR)
configure_file(DCGUI.py ${CMAKE_CURRENT_BINARY_DIR}/DCGUI.py COPYONLY)
configure_file(gui ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

ADD_SUBDIRECTORY(algo)
