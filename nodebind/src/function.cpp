#include "function.hpp"

namespace nodebind
{
	namespace detail
	{
		ArgumentScore::ArgumentScore(const v8::Arguments& args, bool isThisCall)
			:score(1)
			,args(args)
			,isThisCall(isThisCall)
		{}

		void ArgumentScore::addScore(int s)
		{
			if (s < 0)
			{
				//Remove this from consideration by simply setting the score
				//to such a low value that it can never recover.
				score = -10000;
			}

			score += s;
		}
	}
}