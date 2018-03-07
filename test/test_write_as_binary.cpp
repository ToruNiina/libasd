#include <libasd/write_as_binary.hpp>

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include <type_traits>
#include <utility>
#include <sstream>
#include <limits>
#include <random>

template<typename T, typename RNG>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
generate_random(RNG& rng)
{
    std::uniform_int_distribution<T> distro(std::numeric_limits<T>::min(),
                                            std::numeric_limits<T>::max());
    return distro(rng);
}

template<typename T, typename RNG>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type
generate_random(RNG& rng)
{
    std::uniform_real_distribution<T> distro(std::numeric_limits<T>::lowest(),
                                             std::numeric_limits<T>::max());
    return distro(rng);
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_simple_value_stream, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const auto value = generate_random<T>(mt);

        std::ostringstream oss;
        asd::detail::write_as_binary(oss, value);

        const std::string str = oss.str();

        T result;
        char* const first =
            reinterpret_cast<char*>(std::addressof(result));
        std::copy(str.begin(), str.end(), first);

        BOOST_CHECK_EQUAL(value, result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_simple_value_ptr, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const auto value = generate_random<T>(mt);

        T result;
        char* first = reinterpret_cast<char*>(std::addressof(result));
        asd::detail::write_as_binary(first, value);

        BOOST_CHECK_EQUAL(value, result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_array_of_value_stream, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const std::size_t N = 100;

        std::vector<T> values(N);
        for(std::size_t i=0; i<values.size(); ++i)
        {
            values[i] = generate_random<T>(mt);
        }

        std::ostringstream oss;
        asd::detail::write_as_binary(oss, values,
            typename asd::container_traits<std::vector<T>>::ptr_accessibility{});

        const std::string str = oss.str();

        std::vector<T> result(N);
        char* const first = reinterpret_cast<char*>(result.data());
        std::copy(str.begin(), str.end(), first);

        BOOST_CHECK(values == result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_array_of_value_ptr, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const std::size_t N = 100;
        std::vector<T> values(N);
        for(std::size_t i=0; i<values.size(); ++i)
        {
            values[i] = generate_random<T>(mt);
        }

        std::vector<T> result(N);
        char* first = reinterpret_cast<char*>(result.data());
        asd::detail::write_as_binary(first, values,
            typename asd::container_traits<std::vector<T>>::ptr_accessibility{});

        BOOST_CHECK(values == result);
    }
}

boost::unit_test::test_suite*
init_unit_test_suite(int, char**)
{
    typedef boost::mpl::list<
        char,
        std::int16_t,
        std::int32_t,
        float,
        double
        > list_of_values;

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(test_simple_value_stream, list_of_values));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(test_simple_value_ptr, list_of_values));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(test_array_of_value_stream, list_of_values));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(test_array_of_value_ptr, list_of_values));

    return 0;
}


