#include <gtest/gtest.h>

#include <shogun/mathematics/graph/Graph.h>
#include <shogun/mathematics/graph/nodes/Add.h>
#include <shogun/mathematics/graph/nodes/Input.h>

#include "../test/GraphTest.h"

using namespace shogun;
using namespace shogun::graph;
using namespace std;

TYPED_TEST(GraphTest, add)
{
	using NumericType = TypeParam;

	if constexpr (std::is_same_v<NumericType, bool>)
		return;

	auto X1 = SGVector<NumericType>(10);
	auto X2 = SGVector<NumericType>(10);

	X1.range_fill();
	X2.range_fill();

	auto expected_result1 = X1 + X2;
	auto expected_result2 = expected_result1 + X2;

	auto input = make_shared<node::Input>(
	    Shape{Shape::Dynamic}, get_enum_from_type<NumericType>::type);
	auto input1 = make_shared<node::Input>(
	    Shape{10}, get_enum_from_type<NumericType>::type);

	auto intermediate = make_shared<node::Add>(input, input);

	auto output = make_shared<node::Add>(intermediate, input1);

	auto graph = make_shared<Graph>(
	    vector{input, input1},
	    vector<shared_ptr<node::Node>>{intermediate, output});
	this->test_binary_op_results(
		graph, X1, X2, expected_result1, expected_result2);
}