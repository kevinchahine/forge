#pragma once

namespace forge
{
	namespace colors
	{
		class Color
		{
		public:

		};

		class White : public Color {
		public:
			//static Black opposite() { return Black(); }
		};
		
		class Black : public Color {
			//static White opposite() { return White(); }
		};
		
		// TODO: Implement opposite functions. See reverse() in Direction.h 
		//template<White>
		//Black opposite() { return Black; }

	} // namespace colors
} // namespace forge