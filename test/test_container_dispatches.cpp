#include <libasd/container_traits.hpp>
#include <libasd/container_dispatcher.hpp>
#include <libasd/boost/container_traits.hpp>
#include <libasd/boost/container_dispatcher.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

BOOST_TEST_CASE_TEMPLATE_FUNCTION(get_ptr, T)
{
    {
        typename T::template rebind<int>::other vec(10, 42);
        const std::intptr_t data = reinterpret_cast<intptr_t>(vec.data());
        const std::intptr_t ptr  =
            reinterpret_cast<intptr_t>(asd::container::get_ptr(vec));

        BOOST_CHECK_EQUAL(data, ptr);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(size, T)
{
    {
        typename T::template rebind<int>::other vec(10, 42);
        const std::size_t normal = vec.size();
        const std::size_t libasd = asd::container::size(vec);
        BOOST_CHECK_EQUAL(normal, libasd);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(resize, T)
{
    {
        typename T::template rebind<int>::other vec(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 10ul);
        asd::container::resize(vec, 20);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 20ul);
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(clear, T)
{
    {
        typename T::template rebind<int>::other vec(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 10ul);
        asd::container::clear(vec);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 0ul);
    }
}

boost::unit_test::test_suite*
init_unit_test_suite(int, char**)
{
    typedef boost::mpl::list<
        asd::container::vec,
        asd::container::boost_vec,
        asd::container::boost_small_vec<100>,
        asd::container::boost_static_vec<200>
        > pointer_available;

    typedef boost::mpl::list<
        asd::container::vec,
        asd::container::deq,
        asd::container::boost_vec,
        asd::container::boost_deq,
        asd::container::boost_small_vec<100>,
        asd::container::boost_static_vec<200>
        > all;

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(get_ptr, pointer_available));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(size, all));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(resize, all));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(clear, all));
    return 0;
}


