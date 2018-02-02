#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include <libasd/frame_data.hpp>
#include <libasd/boost/container_dispatcher.hpp>
#include <libasd/boost/container_traits.hpp>

BOOST_TEST_CASE_TEMPLATE_FUNCTION(access_interface, T)
{
    {
        asd::FrameData<std::uint8_t, T> data(10, 10);

        for(std::uint8_t i=0; i<100; ++i)
        {
            data.raw_at(i) = i;
        }

        for(std::uint8_t i=0; i<100; ++i)
        {
            BOOST_CHECK_EQUAL(data.raw_at(i), i);
        }

        for(std::uint8_t y=0; y<10; ++y)
        {
            const auto& line = data.at(y);
            for(std::uint8_t x=0; x<10; ++x)
            {
                BOOST_CHECK_EQUAL(line.at(x), y * 10 + x);
            }
        }
    }

    {
        asd::FrameData<std::uint8_t, T> data(10, 10);

        for(std::uint8_t i=0; i<100; ++i)
        {
            data.raw_at(i, std::nothrow) = i;
        }

        for(std::uint8_t i=0; i<100; ++i)
        {
            BOOST_CHECK_EQUAL(data.raw_at(i, std::nothrow), i);
        }

        for(std::uint8_t y=0; y<10; ++y)
        {
            const auto& line = data[y];
            for(std::uint8_t x=0; x<10; ++x)
            {
                BOOST_CHECK_EQUAL(line[x], y * 10 + x);
            }
        }
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(reset, T)
{
    asd::FrameData<std::uint8_t, T> data(10, 10);
    BOOST_CHECK(data.raw_size() == 100);
    BOOST_CHECK(data.x_pixel()  == 10);
    BOOST_CHECK(data.y_pixel()  == 10);
    BOOST_CHECK(data.size()     == data.y_pixel());

    data.reset(12,12);
    BOOST_CHECK(data.raw_size() == 144);
    BOOST_CHECK(data.x_pixel()  == 12);
    BOOST_CHECK(data.y_pixel()  == 12);
    BOOST_CHECK(data.size()     == data.y_pixel());
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(iterator, T)
{
    asd::FrameData<std::uint8_t, T> data(10, 10);

    for(std::uint8_t i=0; i<100; ++i)
    {
        data.raw_at(i) = i;
    }

    std::size_t v = 0;
    for(auto i(data.begin()), ie(data.end()); i!=ie; ++i)
    {
        for(auto j(i->begin()), je(i->end()); j!=je; ++j)
        {
            BOOST_CHECK_EQUAL(*j, v);
            ++v;
        }
    }

    std::size_t y = 0;
    for(auto i(data.begin()), ie(data.end()); i!=ie; ++i)
    {
        std::size_t x = 10;
        for(auto j(i->rbegin()), je(i->rend()); j!=je; ++j)
        {
            --x;
            const std::size_t v = 10 * y + x;
            BOOST_CHECK_EQUAL(*j, v);
        }
        ++y;
    }
    for(auto i(data.rbegin()), ie(data.rend()); i!=ie; ++i)
    {
        for(auto j(i->rbegin()), je(i->rend()); j!=je; ++j)
        {
            --v;
            BOOST_CHECK_EQUAL(*j, v);
        }
    }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(range_based_for, T)
{
    asd::FrameData<std::uint8_t, T> data(10, 10);

    for(std::uint8_t i=0; i<100; ++i)
    {
        data.raw_at(i) = i;
    }

    std::size_t v = 0;
    for(auto line : data)
    {
        for(auto pixel : line)
        {
            BOOST_CHECK_EQUAL(pixel, v);
            ++v;
        }
    }
}

boost::unit_test::test_suite*
init_unit_test_suite(int, char**)
{
    typedef boost::mpl::list<
        asd::container::vec,
        asd::container::deq,
        asd::container::boost_vec,
        asd::container::boost_deq,
        asd::container::boost_small_vec<100>,
        asd::container::boost_static_vec<200>
        > list;

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(access_interface, list));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(reset, list));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(iterator, list));

    boost::unit_test::framework::master_test_suite()
        .add(BOOST_TEST_CASE_TEMPLATE(range_based_for, list));
    return 0;
}


