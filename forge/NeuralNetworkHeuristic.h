#pragma once

#include "HeuristicBase.h"

#include "../opennn/opennn/opennn.h"	// TODO: include only what we need
#include "../opennn/opennn/layer.h"

namespace forge
{
	class NeuralNetworkHeuristic : public HeuristicBase
	{
	public:
		NeuralNetworkHeuristic();
		//NeuralNetworkHeuristic(const std::string& model_file_name);
		NeuralNetworkHeuristic(const NeuralNetworkHeuristic&) = default;
		NeuralNetworkHeuristic(NeuralNetworkHeuristic&&) = default;
		virtual ~NeuralNetworkHeuristic() noexcept = default;
		NeuralNetworkHeuristic& operator=(const NeuralNetworkHeuristic&) = default;
		NeuralNetworkHeuristic& operator=(NeuralNetworkHeuristic&&) noexcept = default;

		virtual heuristic_t eval(const Position& pos) override;

		virtual std::unique_ptr<HeuristicBase> clone() const override;

		virtual std::string name() const override { return "NeuralNetworkHeuristic"; }

		virtual void print(const Position& pos, std::ostream& os = std::cout) const override;

		//OpenNN::NeuralNetwork & model() { return m_model; }
		//const OpenNN::NeuralNetwork & model() const { return m_model; }
		
	protected:
		// A neural network model used to approximate the fitness or favorability of a chess Position.
		// Input is a 2-D Matrix (12 rows, 64 columns)
		//	Each of the 12 rows contains a hot encoding of a feature. 
		//		Ex: material: 
		//			Where are OUR queens on the board
		//			Where are THEIR queens on the board
		//	
		//	It is very important that the input layer is passed data in the same way as it is during training.
		//	For example, if the NN is trained with its 1st layer being `our pawns`, then we should pass in the
		//	one-hot encoding of our pawns to it and not something else.
		// 
		// Output is a single value that is the evaluation of the position in units of centi-pawns.
		// 
		// Input Layer:
		//	Row:
		//		0  - our pawns
		//		1  - their pawns
		//		2  - our knights
		//		3  - their knights
		//		4  - our bishops
		//		5  - their bishops
		//		6  - our rooks
		//		7  - their rooks
		//		8  - our queens
		//		9  - their queens
		//		10 - our kings
		//		11 - their kings
		OpenNN::NeuralNetwork m_model;
	};
} // namespace forge
