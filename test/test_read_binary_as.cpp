#include <libasd/read_binary_as.hpp>

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
        constexpr std::size_t size = sizeof(T);
        const auto value = generate_random<T>(mt);
        const char* const first =
            reinterpret_cast<const char*>(std::addressof(value));
        const char* const last  =
            reinterpret_cast<const char*>(std::addressof(value)) + size;

        const std::string str(first, last);
        std::istringstream iss(str);
        const T result = asd::detail::read_binary_as<T>(iss);

        BOOST_CHECK_EQUAL(value, result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_simple_value_ptr, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const auto value = generate_random<T>(mt);
        const char* ptr =
            reinterpret_cast<const char*>(std::addressof(value));

        const T result = asd::detail::read_binary_as<T>(ptr);
        BOOST_CHECK_EQUAL(value, result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_array_of_value_stream, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const std::size_t N = 10000;
        constexpr std::size_t size = sizeof(T);
        std::vector<T> values(N);
        for(std::size_t i=0; i<values.size(); ++i)
        {
            values[i] = generate_random<T>(mt);
        }

        const char* const first =
            reinterpret_cast<const char*>(values.data());
        const char* const last  =
            reinterpret_cast<const char*>(values.data()) + size * N;

        const std::string str(first, last);
        std::istringstream iss(str);
        const auto result =
            asd::detail::read_binary_as<T, asd::container::vec>(iss, N);

        BOOST_CHECK(values == result);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_array_of_value_ptr, T)
{
    std::mt19937 mt(123456789);

    for(std::size_t i=0; i<1000; ++i)
    {
        const std::size_t N = 10000;
        constexpr std::size_t size = sizeof(T);
        std::vector<T> values(N);
        for(std::size_t i=0; i<values.size(); ++i)
        {
            values[i] = generate_random<T>(mt);
        }

        const char* ptr =
            reinterpret_cast<const char*>(values.data());

        const auto result =
            asd::detail::read_binary_as<T, asd::container::vec>(ptr, N);

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


