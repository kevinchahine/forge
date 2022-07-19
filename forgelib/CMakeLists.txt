cmake_minimum_required(VERSION 3.20)

set(LIBRARY_NAME forgelib)
project(${LIBRARY_NAME})

# --- Source Files ---

set(FEATURE_EXTRACTOR
	forge/feature_extractor/AttackChecker.cpp
	forge/feature_extractor/AttackChecker.h
	forge/feature_extractor/Attackers.cpp
	forge/feature_extractor/AttackersDefinitions.h
	forge/feature_extractor/Attackers.h
	forge/feature_extractor/FeatureExtractor.cpp
	forge/feature_extractor/FeatureExtractor.h
	forge/feature_extractor/KingAttacker.cpp
	forge/feature_extractor/KingAttacker.h
	forge/feature_extractor/KingAttackers.cpp
	forge/feature_extractor/KingAttackersDefinitions.h
	forge/feature_extractor/KingAttackers.h
	forge/feature_extractor/PinDefinitions.h
	forge/feature_extractor/Pin.h
	forge/feature_extractor/Threats.cpp
	forge/feature_extractor/ThreatsDefinitions.h
	forge/feature_extractor/Threats.h
)

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
	forge/FiftyMoveRule.cpp
	forge/FiftyMoveRule.h
	forge/HashCombine.h
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

target_include_directories(${LIBRARY_NAME} PUBLIC guten)

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

target_link_libraries(${PROJECT_NAME} PUBLIC guten)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
