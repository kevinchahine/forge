cmake_minimum_required(VERSION 3.20)

# --- Forge ---

message(STATUS "\n----- Forge -----")

# --- Establish a Project called forge ---
set(PROJECT_NAME "forge")
project(${PROJECT_NAME})

# --- Group source files ---
set(CONTROLLERS
)

set(CORE
	forge/core/BitBoard.cpp
	forge/core/BitBoard.h
	forge/core/Board.cpp
	forge/core/Board.h
	forge/core/BoardSquare.cpp
	forge/core/BoardSquare.h
	forge/core/Color.cpp
	forge/core/Color.h
	forge/core/Direction.cpp
	forge/core/Direction.h
	forge/core/MoveCounter.h
	forge/core/Move.cpp
	forge/core/Move.h
	forge/core/MovePositionPair.cpp
	forge/core/MovePositionPair.h
	forge/core/Movers.h
	forge/core/Piece.cpp
	forge/core/Piece.h
	forge/core/Position.cpp
	forge/core/Position.h
)

set(FEATURE_EXTRACTOR
)

set(GAME
)

set(HEURISTICS
)

set(ML
)

set(MOVEGEN
)

set(NOTES
	forge/notes/AI.txt
	forge/notes/Benchmarks.txt
	forge/notes/BenchmarkTesting.txt
	forge/notes/Bugs.txt
	forge/notes/Classes.txt
	forge/notes/Database.txt
	forge/notes/DatasetGenerator.txt
	forge/notes/Features.txt
	forge/notes/GameStates.txt
	forge/notes/Goals.txt
	forge/notes/HighLevelDesign.txt
	forge/notes/Ideas.txt
	forge/notes/MCTS.txt
	forge/notes/ML.txt
	forge/notes/MoveGeneratorQuintessential2.txt
	forge/notes/MoveGeneratorQuintessential.txt
	forge/notes/MoveGeneratorSpecial.txt
	forge/notes/Multi_threading.txt
	forge/notes/NameSpaces.txt
	forge/notes/NN.txt
	forge/notes/NodeTree.txt
	forge/notes/Notation.txt
	forge/notes/Optimizations.txt
	forge/notes/ProblemPositions.txt
	forge/notes/ProgramOptions.txt
	forge/notes/Progress.txt
	forge/notes/SearchFramework.txt
	forge/notes/SearchMonitor.txt
	forge/notes/Stockfish.txt
	forge/notes/TimeManagement.txt
	forge/notes/TODO.txt
	forge/notes/Training.txt
	forge/notes/UCI_DefinitionNotes.txt
	forge/notes/UCI_Definition.txt
	forge/notes/UCI.txt
	forge/notes/UCI_UML.txt
	forge/notes/ValidMoveGenerator.txt
	forge/notes/Versions.txt
	forge/notes/View.txt
)

set(SOURCE
	forge/source/globals.cpp
	forge/source/globals.h
	forge/source/HashCombine.h
	forge/source/main.cpp
)

set(TEST
	forge/test/positions.cpp
	forge/test/positions.h
	forge/test/Test.cpp
	forge/test/Test.h
	#forge/test/TestFeatureExtractor.cpp
	#forge/test/TestFeatureExtractor.h
	#forge/test/TestMoveGen.cpp
	#forge/test/TestMoveGen.h
	#forge/test/TestUci.cpp
	#forge/test/TestUci.h
)

set(THIRD_PARTY
)

set(TIME
)

set(UCI
)

set(VIEWS
)

source_group(${PROJECT_NAME} ${ML_FILES})
source_group(${PROJECT_NAME} ${CONTROLLERS})
source_group(${PROJECT_NAME} ${CORE})
source_group(${PROJECT_NAME} ${FEATURE_EXTRACTOR})
source_group(${PROJECT_NAME} ${GAME})
source_group(${PROJECT_NAME} ${HEURISTICS})
source_group(${PROJECT_NAME} ${ML})
source_group(${PROJECT_NAME} ${MOVEGEN})
source_group(${PROJECT_NAME} ${NOTES})
source_group(${PROJECT_NAME} ${SOURCE})
source_group(${PROJECT_NAME} ${TEST})
source_group(${PROJECT_NAME} ${THIRD_PARTY})
source_group(${PROJECT_NAME} ${TIME})
source_group(${PROJECT_NAME} ${UCI})
source_group(${PROJECT_NAME} ${VIEWS})

add_executable(${PROJECT_NAME}
	${ML_FILES}
	${CONTROLLERS}
	${CORE}
	${FEATURE_EXTRACTOR}
	${GAME}
	${HEURISTICS}
	${ML}
	${MOVEGEN}
	${NOTES}
	${SOURCE}
	${TEST}
	${THIRD_PARTY}
	${TIME}
	${UCI}
	${VIEWS}
)

include_directories(.)

# --- Compiler Flags ---
set(CMAKE_CXX_STANDARD 17)												# TODO: Do we need both?
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(VS_STARTUP_PROJECT ${PROJECT_NAME})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION} standard ${CMAKE_CXX_STANDARD}")

# --- Set Preprocessor Macros ---
add_compile_definitions(NOMINMAX)	# same as '#define NOMINMAX'

# --- Libraries (no VCPKG) ---

# --- Guten ---
message(STATUS "\n----- Guten -----\n")

add_subdirectory(guten)

target_link_libraries(${PROJECT_NAME} PUBLIC
	Guten
)

target_include_directories(${PROJECT_NAME} PUBLIC
	guten
)

# --- Torch ---
message(STATUS "\n----- Torch -----")

set(CMAKE_CUDA_ARCHITECTURES 75)# all)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(${PROJECT_NAME} PUBLIC
	"${TORCH_LIBRARIES}"
)

# --- Libraries (VCPKG) ---
if (MSVC)
	message(STATUS "\nVCPKG packages will be automatically linked to visual studio\n")
else ()
	# --- For VCPKG packages ---
	message(STATUS "call cmake with: cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake")

	# ----- BOOST -----

	message(STATUS "\n----- BOOST -----")
	set(Boost_USE_STATIC_LIBS		OFF)
	set(Boost_USE_MULTITHREADED		OFF)
	set(Boost_USE_STATIC_RUNTIME	OFF)

	find_package(Boost REQUIRED COMPONENTS
		##asio 
		system 
		filesystem 
		##process
		REQUIRED)

	target_link_libraries(${PROJECT_NAME} PUBLIC
		#${BOOST_LIBRARIES}
		#Boost::asio
		Boost::system
		Boost::filesystem
	)

	message(STATUS "Include dirs ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libraries ${Boost_LIBRARIES}")

	include_directories(${BOOST_INCLUDE_DIRS})

	target_include_directories(${PROJECT_NAME} PUBLIC
		${BOOST_INCLUDE_DIRS}
	)

	# --- XLNT ---
	#message(STATUS "\n----- XLNT -----")
	#
	#find_package(Xlnt CONFIG REQUIRED)
	#
	#target_link_libraries(${PROJECT_NAME} PRIVATE xlnt:xlnt)

endif (MSVC)

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
	message(STATUS "--- :P d: :D :)BX8==; MSVC :) ---")

	file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
	add_custom_command(
		TARGET ${PROJECT_NAME} 
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${TORCH_DLLS}
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endif (MSVC)

