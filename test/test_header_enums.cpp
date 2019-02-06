#include <libasd/header.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

BOOST_TEST_CASE_TEMPLATE_FUNCTION(to_voltage, T)
{
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::unipolar_1_0V);
        BOOST_CHECK_EQUAL(minmax.first,  0.0);
        BOOST_CHECK_EQUAL(minmax.second, 1.0);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::unipolar_2_5V);
        BOOST_CHECK_EQUAL(minmax.first,  0.0);
        BOOST_CHECK_EQUAL(minmax.second, 2.5);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::unipolar_5_0V);
        BOOST_CHECK_EQUAL(minmax.first,  0.0);
        BOOST_CHECK_EQUAL(minmax.second, 5.0);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::bipolar_1_0V);
        BOOST_CHECK_EQUAL(minmax.first, -1.0);
        BOOST_CHECK_EQUAL(minmax.second, 1.0);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::bipolar_2_5V);
        BOOST_CHECK_EQUAL(minmax.first, -2.5);
        BOOST_CHECK_EQUAL(minmax.second, 2.5);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::bipolar_5_0V);
        BOOST_CHECK_EQUAL(minmax.first, -5.0);
        BOOST_CHECK_EQUAL(minmax.second, 5.0);
    }
    {
        const auto minmax = asd::to_voltage<T>(asd::AD_range::dummy_value);
        BOOST_CHECK_EQUAL(minmax.first, -80.0);
        BOOST_CHECK_EQUAL(minmax.second, 80.0);
    }
}

boost::unit_test::test_suite*
init_unit_test_suite(int, char**)
{
    typedef boost::mpl::list<float, double> real_types;

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(to_voltage, real_types));

    return 0;
}


