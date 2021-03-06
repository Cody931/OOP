#include "stdafx.h"
#include "VectorProcessor.h"
#include <algorithm>
#include <numeric>
#include <functional>
#include <boost/range/algorithm/transform.hpp>
#pragma warning (push, 3)
#include <boost/phoenix.hpp>
#pragma warning (pop)

using namespace std;
using namespace std::placeholders;
using namespace boost::phoenix::placeholders;
using namespace boost::phoenix;
using boost::transform;

void ProcessVector(std::vector<double> & numbers)
{
	if (!numbers.empty())
	{
		double maxNum = numbers[0];
		for (unsigned i = 0; i < numbers.size(); ++i)
		{
			if (maxNum < numbers[i])
			{
				maxNum = numbers[i];
			}
		}
		double halfMaxNum = maxNum / 2;
		boost::transform(numbers, numbers.begin(), arg1 * halfMaxNum);
		sort(numbers.begin(), numbers.end());
	}
}