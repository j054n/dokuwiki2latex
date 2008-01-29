PROJECT(wiki2latex)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.   
ADD_DEFINITIONS(-Wall -O2)   

#list all source files here   
ADD_EXECUTABLE(wiki2latex retezec.c retezce.c parser.c zasobnik.c main.c)   
      
#need to link to some other libraries ? just add them here   
#TARGET_LINK_LIBRARIES(wiki2latex png jpeg)

