cmake_minimum_required(VERSION 3.20)

set(LIBRARY_NAME forgelib)
project(${LIBRARY_NAME})

# --- Source Files ---

set(SRC
	forge/BitBoard.cpp
	forge/BitBoard.h
	forge/Board.cpp
	forge/Board.h
	forge/BoardSquare.cpp
	forge/BoardSquare.h
	forge/Color.cpp
	forge/Color.h
	forge/Direction.cpp
	forge/Direction.h
	forge/IntBoard.cpp
	forge/IntBoard.h	
	forge/MoveCounter.h
	forge/Move.cpp
	forge/Move.h
	forge/MovePositionPair.cpp
	forge/MovePositionPair.h
	forge/Movers.h
	forge/Piece.cpp
	forge/Piece.h
	forge/Position.cpp
	forge/Position.h
	forge/MoveGenerator2.cpp
	forge/MoveGenerator2_Definitions.h
	forge/MoveGenerator2.h
	forge/MoveList.cpp
	forge/MoveList.h
)

# --- Create Folders for VS ---

source_group(${LIBRARY_NAME} FILES ${SRC})

# --- Make this a library (not an app) ---

add_library(${LIBRARY_NAME} STATIC
	${SRC}
)

include_directories(.)

# --- Compiler Flags ---
#set(CMAKE_CXX_STANDARD 17)												# TODO: Do we need both?
#set_property(TARGET ${LIBRARY_NAME} PROPERTY CXX_STANDARD 17)
#set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_STANDARD 17)
#target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_17)

# --- Set Preprocessor Macros ---
add_compile_definitions(NOMINMAX)	# same as '#define NOMINMAX'

# --------------------------- Libraries ---------------------------------------

# --- Guten ---
message(STATUS "\n----- Guten -----\n")

add_subdirectory(guten)
target_link_libraries(${PROJECT_NAME} PUBLIC Guten)
target_include_directories(${PROJECT_NAME} PUBLIC guten)

