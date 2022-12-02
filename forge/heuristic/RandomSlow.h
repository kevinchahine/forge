#pragma once

#include "forge/heuristic/Random.h"

#include <iostream>
#include <memory>
#include <chrono>

namespace forge
{
	namespace heuristic
	{
		// A heuristic which evaluates a position using random values.
		// The evaluation is independent of the position.
		// This class also adds a delay when calling eval() to simulate the effect of a better heuristic
		// which will take a longer computation time. 
		// The purpose of this class to be an easy to implement evaluation function that to produces
		// a meaningless evaluation.
		// It can be useful as a benchmark for better evaluation functions.
		class RandomSlow : public Random
		{
		private:
			using super_t = Random;

		public:
			RandomSlow() : m_delay(std::chrono::nanoseconds(1)) {}
			RandomSlow(const std::chrono::nanoseconds & delay) : m_delay(delay) {}
			RandomSlow(const RandomSlow&) = default;
			RandomSlow(RandomSlow&&) noexcept = default;
			virtual ~RandomSlow() noexcept = default;
			RandomSlow& operator=(const RandomSlow&) = default;
			RandomSlow& operator=(RandomSlow&&) noexcept = default;
			
			// Returns a random evaluation just like class heuristics::Random
			// Also delays execution for a set amount of time to simulate execution of better heuristic functions.
			virtual heuristic_t eval(const Position& pos, bool maximizeWhite = true) override;

			virtual std::unique_ptr<Base> clone() const override { return std::make_unique<RandomSlow>(*this); }

			virtual std::string name() const override { return "RolloutSlow"; }

			virtual void print(const Position& pos, std::ostream& os = std::cout) const override { os << this->name(); }

			std::chrono::nanoseconds& delay() { return m_delay; }
			const std::chrono::nanoseconds& delay() const { return m_delay; }

		private:
			// Duration of delay when calling eval()
			std::chrono::nanoseconds m_delay;
		};
	} // namespace heuristic
} // namespace forge
