set(PYBIND11_CPP_STANDARD -std=c++11)

pybind11_add_module(libasd SHARED libasd_py.cpp)
