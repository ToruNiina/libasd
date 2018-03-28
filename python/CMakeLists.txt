set(PYBIND11_CPP_STANDARD -std=c++11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

pybind11_add_module(libasd SHARED libasd_py.cpp)
