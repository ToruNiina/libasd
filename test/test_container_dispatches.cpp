#define BOOST_TEST_MODULE "test_container_dispatcher"
#include <libasd/container_dispatcher.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(get_ptr)
{
    {
        std::vector<int> vec(10, 42);
        const std::intptr_t data = reinterpret_cast<intptr_t>(vec.data());
        const std::intptr_t ptr  =
            reinterpret_cast<intptr_t>(asd::container::get_ptr(vec));

        BOOST_CHECK_EQUAL(data, ptr);
    }

    {
        std::array<int, 10> arr;
        const std::intptr_t data = reinterpret_cast<intptr_t>(arr.data());
        const std::intptr_t ptr  =
            reinterpret_cast<intptr_t>(asd::container::get_ptr(arr));

        BOOST_CHECK_EQUAL(data, ptr);
    }
}

BOOST_AUTO_TEST_CASE(size)
{
    {
        std::vector<int> vec(10, 42);
        const std::size_t normal = vec.size();
        const std::size_t libasd = asd::container::size(vec);
        BOOST_CHECK_EQUAL(normal, libasd);
    }

    {
        std::deque<int> deq(10, 42);
        const std::size_t normal = deq.size();
        const std::size_t libasd = asd::container::size(deq);
        BOOST_CHECK_EQUAL(normal, libasd);
    }

    {
        std::array<int, 10> arr;
        const std::size_t normal = arr.size();
        const std::size_t libasd = asd::container::size(arr);
        BOOST_CHECK_EQUAL(normal, libasd);
    }
}

BOOST_AUTO_TEST_CASE(resize)
{
    {
        std::vector<int> vec(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 10ul);
        asd::container::resize(vec, 20);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 20ul);
    }

    {
        std::deque<int> deq(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(deq), 10ul);
        asd::container::resize(deq, 20);
        BOOST_CHECK_EQUAL(asd::container::size(deq), 20ul);
    }

    {
        std::array<int, 10> arr;
        BOOST_CHECK_EQUAL(asd::container::size(arr), 10ul);
        try
        {
            asd::container::resize(arr, 5);
        }
        catch(...)
        {
            BOOST_CHECK(false);
        }

        bool exception_thrown = false;
        try
        {
            asd::container::resize(arr, 20);
        }
        catch(std::out_of_range oor)
        {
            exception_thrown = true;
        }
        catch(...)
        {
            BOOST_CHECK(false);
        }
        BOOST_CHECK(exception_thrown);
    }
}

BOOST_AUTO_TEST_CASE(clear)
{
    {
        std::vector<int> vec(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 10ul);
        asd::container::clear(vec);
        BOOST_CHECK_EQUAL(asd::container::size(vec), 0ul);
    }

    {
        std::deque<int> deq(10, 42);
        BOOST_CHECK_EQUAL(asd::container::size(deq), 10ul);
        asd::container::clear(deq);
        BOOST_CHECK_EQUAL(asd::container::size(deq), 0ul);
    }

    {
        std::array<int, 10> arr;
        BOOST_CHECK_EQUAL(asd::container::size(arr), 10ul);
        asd::container::clear(arr);
        BOOST_CHECK_EQUAL(asd::container::size(arr), 10ul);
    }
}
