link_directories(
	${GTKMM_LIBRARY_DIRS} )
include_directories(
	${GTKMM_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS} )



add_executable(gfunk main.cc)
add_library(parser parser.cc)
add_library(rangeparser rangeparser.cc)
add_library(gldraw gldraw.cc)

target_link_libraries(gfunk
	${GTKMM_LIBRARIES}
	parser
	rangeparser
  gldraw
)
target_link_libraries(gldraw
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES} )

target_include_directories(parser
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rangeparser
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(gldraw
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(gfunk
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
