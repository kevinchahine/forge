cmake_minimum_required(VERSION 3.20)

# --- Establish a Project called forge ---
set(PROJECT_NAME "forge")
project(${PROJECT_NAME})

# --- Group source files ---
set(CONTROLLERS
	controllers/ControllerBase.cpp
	controllers/ControllerBase.h
	controllers/Controllers.hpp
	controllers/KeyboardController.cpp
	controllers/KeyboardController.h
	controllers/MCTS_Base.cpp
	controllers/MCTS_Base.h
	controllers/MCTS_Sequential.cpp
	controllers/MCTS_Sequential.h
	controllers/MCTS_Concurrent.cpp
	controllers/MCTS_Concurrent.h
	controllers/MCTS_ProducerConsumer.cpp
	controllers/MCTS_ProducerConsumer.h
	controllers/MinimaxSolver.cpp
	controllers/MinimaxSolver.h
	controllers/RandomSolver.cpp
	controllers/RandomSolver.h
	controllers/ReplayController.cpp
	controllers/ReplayController.h
	controllers/SolverBase.cpp
	controllers/SolverBase.h
	controllers/Solvers.hpp
)

set(DRIVERS
	drivers/dataset_helpers.cpp
	drivers/dataset_helpers.h
	drivers/train.cpp
	drivers/train.h
	drivers/play.cpp
	drivers/play.h
)

set(GAME
	game/ChessMatch.cpp
	game/ChessMatch.h
	game/UciMatch.cpp
	game/UciMatch.h
)

set(HEURISTIC
	heuristic/Material.cpp
	heuristic/Material.h
	heuristic/MaterialWeights.h
	heuristic/FeatureExtractor.cpp
	heuristic/FeatureExtractor.h
	heuristic/Base.cpp
	heuristic/Base.h
	heuristic/Heuristics.hpp
	heuristic/NeuralNetwork.cpp
	heuristic/NeuralNetwork.h
	heuristic/Random.cpp
	heuristic/Random.h
	heuristic/RandomSlow.cpp
	heuristic/RandomSlow.h
	heuristic/Rollout.cpp
	heuristic/Rollout.h
	heuristic/Weights.hpp
)

set(ML
	ml/CheckpointManager.cpp
	ml/CheckpointManager.h
	ml/Network.cpp
	ml/Network.h
	ml/StockfishDataset.cpp
	ml/StockfishDataset.h
	ml/Trainer.cpp
	ml/Trainer.h
	ml/TrainingHistory.cpp
	ml/TrainingHistory.h	
)

set(NOTES
	notes/AI.txt
	notes/Benchmarks.txt
	notes/BenchmarkTesting.txt
	notes/Bugs.txt
	notes/Classes.txt
	notes/Database.txt
	notes/DatasetGenerator.txt
	notes/Features.txt
	notes/GameStates.txt
	notes/Goals.txt
	notes/HighLevelDesign.txt
	notes/Ideas.txt
	notes/MCTS.txt
	notes/ML.txt
	notes/MoveGeneratorQuintessential2.txt
	notes/MoveGeneratorQuintessential.txt
	notes/MoveGeneratorSpecial.txt
	notes/Multi_threading.txt
	notes/NameSpaces.txt
	notes/NN.txt
	notes/NodeTree.txt
	notes/Notation.txt
	notes/Optimizations.txt
	notes/ProblemPositions.txt
	notes/ProgramOptions.txt
	notes/Progress.txt
	notes/SearchFramework.txt
	notes/SearchMonitor.txt
	notes/Stockfish.txt
	notes/TimeManagement.txt
	notes/TODO.txt
	notes/Training.txt
	notes/UCI_DefinitionNotes.txt
	notes/UCI_Definition.txt
	notes/UCI.txt
	notes/UCI_UML.txt
	notes/ValidMoveGenerator.txt
	notes/Versions.txt
	notes/View.txt
)

set(PERFT 
)

set(SEARCH
	search/ExitCondition.cpp
	search/ExitCondition.h
	search/MCTS_Node.cpp
	search/MCTS_Node.h
	#search/MCTS_Node_MT.cpp
	#search/MCTS_Node_MT.h
	search/MiniMaxNode.cpp
	search/MiniMaxNode.h
	search/SearchMonitor.h
	search/SearchMonitorAtomic.h 
	search/SearchMonitorTemplate.h
	search/UCB.cpp
	search/UCB.h
)

set(SOURCE
	source/globals.cpp
	source/globals.h
	source/globals_torch.cpp
	source/globals_torch.h
	source/main.cpp
)

set(TEST
	test/Benchmark.cpp
	test/Benchmark.h
	test/heuristics.cpp
	test/heuristics.h
	test/mcts.cpp
	test/mcts.h
	test/positions.cpp
	test/positions.h
	test/Test.cpp
	test/Test.h
	#test/TestFeatureExtractor.cpp
	#test/TestFeatureExtractor.h
	test/TestML.cpp
	test/TestML.h
	#test/TestUci.cpp
	#test/TestUci.h
	test/tree.cpp
	test/tree.h
)

set(THIRD_PARTY
	third_party/rapidcsv.h
)

set(UTIL
	util/composite.h
	util/demangle.h
	util/formatted_int.h
)

set(VIEWS
	views/TextView.cpp
	views/TextView.h
	views/ViewBase.cpp
	views/ViewBase.h
)

source_group(ml_files FILES ${ML_FILES})
source_group(controllers FILES ${CONTROLLERS})
source_group(core FILES ${CORE})
source_group(drivers FILES ${DRIVERS})
source_group(features_extractor FILES ${FEATURE_EXTRACTOR})
source_group(game FILES ${GAME})
source_group(heuristic FILES ${HEURISTIC})
source_group(ml FILES ${ML})
source_group(notes FILES ${NOTES})
source_group(search FILES ${SEARCH})
source_group(source FILES ${SOURCE})
source_group(test FILES ${TEST})
source_group(third_party FILES ${THIRD_PARTY})
source_group(util FILES ${UTIL})
source_group(views FILES ${VIEWS})

add_executable(${PROJECT_NAME}
	${ML_FILES}
	${CONTROLLERS}
	${CORE}
	${DRIVERS}
	${FEATURE_EXTRACTOR}
	${GAME}
	${HEURISTIC}
	${ML}
	${NOTES}
	${SEARCH}
	${SOURCE}
	${TEST}
	${THIRD_PARTY}
	${UTIL}
	${VIEWS}
)

include_directories(..)

# --- Compiler Flags ---
set(CMAKE_CXX_STANDARD 17)												# TODO: Do we need both?
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(VS_STARTUP_PROJECT ${PROJECT_NAME})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION} standard ${CMAKE_CXX_STANDARD}")

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")	# parallel compilation
endif()

# --- Set Preprocessor Directives ---
# NOMINMAX
add_compile_definitions(NOMINMAX)	# same as '#define NOMINMAX'

# CHECKPOINT_DIR
option(CHECKPOINT_DIR "Sets directory for saving pytorch checkpoints")

message(STATUS "CHECKPOINT_DIR: ${CHECKPOINT_DIR}")

if (CHECKPOINT_DIR)
	add_compile_definitions(CHECKPOINT_DIR="${CHECKPOINT_DIR}")
else()
	message(WARNING "CHECKPOINT_DIR has not been set. Please call cmake command with the option -DCHECKPOINT_DIR=path/to/checkpoint/dir")
endif()

# --- Libraries (no VCPKG) ---

# --- forgelib ---

message(STATUS "\n----- forgelib -----\n")

target_link_libraries(${PROJECT_NAME} PUBLIC forgelib)
target_include_directories(${PROJECT_NAME} PUBLIC forgelib)
include_directories(forgelib)

# --- ucilib ---

message(STATUS "\n----- ucilib -----\n")

target_link_libraries(${PROJECT_NAME} PUBLIC ucilib)

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
	#message(STATUS "call cmake with: cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake")

	# ----- BOOST -----

	message(STATUS "\n----- BOOST -----")
	set(Boost_USE_STATIC_LIBS		OFF)
	set(Boost_USE_MULTITHREADED		OFF)
	set(Boost_USE_STATIC_RUNTIME	OFF)

	find_package(Boost REQUIRED COMPONENTS
		#asio 
		core
		system 
		filesystem 
		#process
		lockfree
		REQUIRED)

	target_link_libraries(${PROJECT_NAME} PUBLIC
		#Boost::asio
		Boost::core
		Boost::system
		Boost::filesystem
		Boost::lockfree
	)

	message(STATUS "Include dirs ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libraries ${Boost_LIBRARIES}")

	include_directories(${BOOST_INCLUDE_DIRS})

	target_include_directories(${PROJECT_NAME} PUBLIC
		${BOOST_INCLUDE_DIRS}
	)

	# --- XLNT ---
	message(STATUS "\n----- XLNT -----")
	
	find_package(Xlnt CONFIG REQUIRED)
	find_package(Xlnt CONFIG REQUIRED)
	
	target_link_libraries(${PROJECT_NAME} PRIVATE xlnt::xlnt)

	# --- MatPlot++ ---
	find_package(matplot++ CONFIG REQUIRED)
	
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		Matplot++::cimg 
		std::filesystem 
		Matplot++::matplot 
		Matplot++::nodesoup
	)

endif (MSVC)

# Torch:
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

