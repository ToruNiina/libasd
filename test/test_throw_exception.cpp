#define BOOST_TEST_MODULE "test_throw_exception"
#include <libasd/exception_thrower.hpp>
#include <boost/test/included/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(message_thrown)
{
    try
    {
        asd::throw_exception<std::out_of_range>("hoge %", 100);
    }
    catch(std::out_of_range& oor)
    {
        const std::string what = oor.what();
        BOOST_CHECK_EQUAL(what, std::string("hoge 100"));
    }
    catch(...)
    {
        BOOST_CHECK(false);
    }
}
