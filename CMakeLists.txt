cmake_minimum_required(VERSION 3.20)

# --- Establish a Project called forge ---
set(PROJECT_NAME "forge")
project(${PROJECT_NAME})

# --- Add subdirectories that contain code ---
add_subdirectory(guten)

# --- Specify the header files ---
set(HEADER_FILES
	forge/ApplePieHeuristic.h
	forge/ApplePieWeights.h
	forge/AttackChecker.h
	forge/AttackersDefinitions.h
	forge/Attackers.h
	forge/BitBoardDefinitions.h
	forge/BitBoard.h
	forge/BoardDefinitions.h		# TODO: Do we need this anymore
	forge/Board.h
	forge/BoardSquare.h
	forge/ChessMatch.h
	forge/Clock.h
	forge/Color.h
	forge/ControllerBase.h
	forge/DataMonitor.h
	forge/DatasetGenerator.h
	forge/Direction.h
	forge/EvaluationBase.h
	forge/ExitCondition.h
	forge/FiftyMoveRule.h
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
	forge/Optimizer.h
	forge/OWeightsArchive.h
	forge/PerformanceLogger.h
	forge/PerformanceTester.h
	forge/Piece.h
	forge/PinDefinitions.h
	forge/Pin.h
	forge/PositionDefinitions.h
	forge/Position.h
	forge/PositionTable.h
	forge/PseudoMoveGenerator.h
	forge/RandomHeuristic.h
	forge/RandomSolver.h
	forge/ReplayController.h
	forge/SearchMonitor.h
	forge/SolverBase.h
	forge/StopWatch.h
	forge/Test.h
	forge/TestMoveGen.h
	forge/TestOptimizer.h
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
	forge/DataMonitor.cpp
	forge/DatasetGenerator.cpp
	forge/Direction.cpp
	forge/EvaluationBase.cpp
	forge/ExitCondition.cpp
	forge/FiftyMoveRule.cpp
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
	forge/Optimizer.cpp
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
	forge/TestOptimizer.cpp
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
)

# --- Specify the text files ---
set(TEXT_FILES
	forge/AI.txt
	forge/Benchmarks.txt
	forge/BenchmarkTesting.txt
	forge/Bugs.txt
	forge/Classes.txt
	forge/Database.txt
	forge/DatasetGenerator.txt
	forge/Features.txt
	forge/GameStates.txt
	forge/Goals.txt
	forge/HighLevelDesign.txt
	forge/Ideas.txt
	forge/MCTS.txt
	forge/ML.txt
	forge/MoveGeneratorQuintessential2.txt
	forge/MoveGeneratorQuintessential.txt
	forge/MoveGeneratorSpecial.txt
	forge/Multi_threading.txt
	forge/NameSpaces.txt
	forge/NN.txt
	forge/NodeTree.txt
	forge/Notation.txt
	forge/Optimizations.txt
	forge/ProblemPositions.txt
	forge/ProgramOptions.txt
	forge/Progress.txt
	forge/SearchFramework.txt
	forge/SearchMonitor.txt
	forge/Stockfish.txt
	forge/TimeManagement.txt
	forge/TODO.txt
	forge/Training.txt
	forge/UCI_DefinitionNotes.txt
	forge/UCI_Definition.txt
	forge/UCI.txt
	forge/UCI_UML.txt
	forge/ValidMoveGenerator.txt
	forge/Versions.txt
	forge/View.txt
)

# --- OPENCV ---

message(STATUS "----- OpenCV -----")
set("OpenCV_DIR" "/usr/share/OpenCV")
find_package(OpenCV REQUIRED)

MESSAGE(STATUS "Include ders ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

# --- BOOST ---

message(STATUS "----- BOOST -----")
set(Boost_USE_STATIC_LIBS		OFF)
set(Boost_USE_MULTITHREADED		OFF)
set(Boost_USE_STATIC_RUNTIME	OFF)

find_package(Boost REQUIRED COMPONENTS
	#asio 
	system 
	filesystem 
	#process
	REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

message(STATUS "Include dirs ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries ${Boost_LIBRARIES}")

# --- Establish the executable and specify which files will be used to create it ---
add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
	${TEXT_FILES}
)

# --- Link external libraries ---
target_link_libraries(${PROJECT_NAME} PUBLIC
	guten
	${OpenCV_LIBS}
	# ${BOOST_LIBRARIES}
	#Boost::asio
	Boost::system
	Boost::filesystem
)

target_include_directories(${PROJECT_NAME} PUBLIC
	guten
	${BOOST_INCLUDE_DIRS}
)