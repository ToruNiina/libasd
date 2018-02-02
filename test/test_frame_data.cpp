#define BOOST_TEST_MODULE "test_frame_data"
#include <libasd/frame_data.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(access_interface)
{
    {
        asd::FrameData<std::uint8_t> data(10, 10);

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
        asd::FrameData<std::uint8_t> data(10, 10);

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

BOOST_AUTO_TEST_CASE(reset)
{
    asd::FrameData<std::uint8_t> data(10, 10);
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

BOOST_AUTO_TEST_CASE(iterator)
{
    asd::FrameData<std::uint8_t> data(10, 10);

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

BOOST_AUTO_TEST_CASE(range_based_for)
{
    asd::FrameData<std::uint8_t> data(10, 10);

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
