#pragma once

#include "forge/heuristics/HeuristicBase.h"
#include "forge/ml/Net.h"

namespace forge
{
	// TODO: rename this Network
	class NeuralNetworkHeuristic : public HeuristicBase
	{
	public:
		NeuralNetworkHeuristic() = default;
		NeuralNetworkHeuristic(const std::string& net_file);
		NeuralNetworkHeuristic(const NeuralNetworkHeuristic&) = default;
		NeuralNetworkHeuristic(NeuralNetworkHeuristic&&) = default;
		virtual ~NeuralNetworkHeuristic() noexcept = default;
		NeuralNetworkHeuristic& operator=(const NeuralNetworkHeuristic&) = default;
		NeuralNetworkHeuristic& operator=(NeuralNetworkHeuristic&&) noexcept = default;
		
		virtual heuristic_t eval(const Position& pos) override;

		virtual std::unique_ptr<HeuristicBase> clone() const override;

		virtual std::string name() const override { return "NeuralNetworkHeuristic"; }

		virtual void print(const Position& pos, std::ostream& os = std::cout) const override;

		ml::Net & model() { return m_net; }
		const ml::Net & model() const { return m_net; }
		
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
		//	one-hot encoding of our pawns during evaluations.
		// 
		// Output Layer:
		//		A single value that is the evaluation of the position in units of centi-pawns.
		// 		From perspective of moving player.
		ml::Net m_net;
	};
} // namespace forge
