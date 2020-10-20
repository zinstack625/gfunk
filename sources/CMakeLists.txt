link_directories(
	${GTKMM_LIBRARY_DIRS} )
include_directories(
	${GTKMM_INCLUDE_DIRS} )



add_executable(gfunk main.cc)
add_library(parser parser.cc)

target_link_libraries(gfunk
	${GTKMM_LIBRARIES}
	parser
	)

target_include_directories(parser
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(gfunk
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
