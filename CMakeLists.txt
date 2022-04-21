# --- Forge ---

cmake_minimum_required(VERSION 3.20)

# --- Establish a Project called forge ---
set(PROJECT_NAME "forge")
project(${PROJECT_NAME})

set(VS_STARTUP_PROJECT ${PROJECT_NAME})

# --- Add subdirectories that contain code ---
add_subdirectory(guten)

# --- Specify the header files ---
set(HEADER_FILES
	forge/ApplePieHeuristic.h
	forge/ApplePieWeights.h
	forge/AttackChecker.h
	forge/AttackersDefinitions.h
	forge/Attackers.h
	forge/BitBoard.h
	forge/Board.h
	forge/BoardSquare.h
	forge/ChessMatch.h
	forge/Clock.h
	forge/Color.h
	forge/ControllerBase.h
	forge/CSVParser.h
	forge/DataMonitor.h
	forge/DatasetGenerator.h
	forge/DataSet.h
	forge/Direction.h
	forge/EvaluationBase.h
	forge/ExitCondition.h
	forge/FiftyMoveRule.h
	forge/FeatureExtractor.h
	forge/GameHistory.h
	forge/GameState.h
	forge/globals.h
	forge/HashCombine.h
	forge/HeuristicBase.h
	forge/IWeightsArchive.h
	forge/KeyboardController.h
	forge/KingAttacker.h
	forge/KingAttackersDefinitions.h
	forge/KingAttackers.h
	forge/Line.h				# TODO: do we need this
	forge/MCTS_Node.h
	forge/MCTS_Solver.h
	forge/MiniMaxNode.h
	forge/MinimaxSolver.h
	forge/MoveCounter.h
	forge/MoveGenerator2_Definitions.h
	forge/MoveGenerator2.h
	forge/MoveGenerator.h
	forge/MoveGeneratorInline.h
	forge/Move.h
	forge/MoveList.h
	forge/MovePositionPair.h
	forge/Movers.h
	forge/NeuralNetworkHeuristic.h
	forge/Node.h
	forge/OWeightsArchive.h
	forge/PerformanceLogger.h
	forge/PerformanceTester.h
	forge/Piece.h
	forge/PinDefinitions.h
	forge/Pin.h
	forge/PositionDefinitions.h
	forge/Position.h
	forge/PositionEvalPair.h
	forge/PositionTable.h
	forge/PseudoMoveGenerator.h
	forge/RandomHeuristic.h
	forge/RandomSolver.h
	forge/rapidcsv.h
	forge/ReplayController.h
	forge/SearchMonitor.h
	forge/SolverBase.h
	forge/StopWatch.h
	forge/Test.h
	forge/TestMoveGen.h
	forge/TestUci.h
	forge/TextView.h
	forge/ThreatsDefinitions.h
	forge/Threats.h
	forge/Timer.h
	forge/UciBase.h
	forge/UciCurrLine.h
	forge/UciEngine.h
	forge/UciGui.h
	forge/UciInfo.h
	forge/UciOptions.h
	forge/UciScore.h
	forge/UciSearchCommands.h
	forge/ViewBase.h
	forge/WeightsArchive.h
	forge/WeightsBase.h
	forge/ml/Optimizer.h
	forge/ml/PosEvalPair.h
	)

# --- Specify the source files ---
set(SOURCE_FILES
	forge/main.cpp
	forge/ApplePieHeuristic.cpp
	forge/ApplePieWeights.cpp
	forge/AttackChecker.cpp
	forge/Attackers.cpp
	forge/BitBoard.cpp
	forge/Board.cpp
	forge/BoardSquare.cpp
	forge/ChessMatch.cpp
	forge/Clock.cpp
	forge/Color.cpp
	forge/ControllerBase.cpp
	forge/CSVParser.cpp
	forge/DataMonitor.cpp
	forge/DatasetGenerator.cpp
	forge/DataSet.cpp
	forge/Direction.cpp
	forge/EvaluationBase.cpp
	forge/ExitCondition.cpp
	forge/FiftyMoveRule.cpp
	forge/FeatureExtractor.cpp
	forge/GameHistory.cpp
	forge/GameState.cpp
	forge/globals.cpp
	forge/HeuristicBase.cpp
	forge/IWeightsArchive.cpp
	forge/KeyboardController.cpp
	forge/KingAttacker.cpp
	forge/KingAttackers.cpp
	forge/Line.cpp
	forge/MCTS_Node.cpp
	forge/MCTS_Solver.cpp
	forge/MiniMaxNode.cpp
	forge/MinimaxSolver.cpp
	forge/MoveCounter.cpp
	forge/Move.cpp
	forge/MoveGenerator2.cpp
	forge/MoveGenerator.cpp
	forge/MoveList.cpp
	forge/MovePositionPair.cpp
	forge/Movers.cpp
	forge/NeuralNetworkHeuristic.cpp
	forge/Node.cpp
	forge/OWeightsArchive.cpp
	forge/PerformanceLogger.cpp
	forge/PerformanceTester.cpp
	forge/Piece.cpp
	forge/Position.cpp
	forge/PositionTable.cpp
	forge/PseudoMoveGenerator.cpp
	forge/RandomHeuristic.cpp
	forge/RandomSolver.cpp
	forge/ReplayController.cpp
	forge/SearchMonitor.cpp
	forge/SolverBase.cpp
	forge/StopWatch.cpp
	forge/Test.cpp
	forge/TestMoveGen.cpp
	forge/TestUci.cpp
	forge/TextView.cpp
	forge/Threats.cpp
	forge/Timer.cpp
	forge/UciBase.cpp
	forge/UciCurrLine.cpp
	forge/UciEngine.cpp
	forge/UciGui.cpp
	forge/UciInfo.cpp
	forge/UciOptions.cpp
	forge/UciScore.cpp
	forge/UciSearchCommands.cpp
	forge/ViewBase.cpp
	forge/WeightsArchive.cpp
	forge/WeightsBase.cpp
	forge/ml/Optimizer.cpp
	)

# --- Specify the text files ---
set(TEXT_FILES
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

# --- 3rd Party Libraries (VCPKG only) ---

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
		#system 
		#filesystem 
		##process
		REQUIRED)

	include_directories(${BOOST_INCLUDE_DIRS})

	message(STATUS "Include dirs ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libraries ${Boost_LIBRARIES}")
	
	target_link_libraries(${PROJECT_NAME} PUBLIC
		Guten
		#${BOOST_LIBRARIES}
		#Boost::asio
		Boost::system
		Boost::filesystem
		opennn
	)

	target_include_directories(${PROJECT_NAME} PUBLIC
		guten
		${BOOST_INCLUDE_DIRS}
	)
endif (MSVC)

# --- OpenNN ---

message(STATUS "\n----- OpenNN -----")

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS}")

add_subdirectory(opennn)
include_directories(opennn)

if(UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(PROJECT_LINK_LIBS ${CMAKE_SOURCE_DIR}/Release/opennn/libopennn.a)
endif()

if(WIN32)
	set(PROJECT_LINK_LIBS ../../opennn/Release/opennn)
endif()

message(STATUS "\n----- Forge -----")

# --- Establish the executable and specify which files will be used to create it ---
add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
	${TEXT_FILES}
)

# --- Link external libraries ---
target_link_libraries(${PROJECT_NAME} PUBLIC
	Guten
	${OpenCV_LIBS}
	# ${BOOST_LIBRARIES}
	#Boost::asio
	#Boost::system
	#Boost::filesystem
	opennn
)

target_include_directories(${PROJECT_NAME} PUBLIC
	guten
	#${BOOST_INCLUDE_DIRS}
)