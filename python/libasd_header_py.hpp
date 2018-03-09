#ifndef LIBASD_HEADER_PY_HPP
#define LIBASD_HEADER_PY_HPP
#include <pybind11/pybind11.h>
#include <libasd/read_header.hpp>

namespace py = pybind11;

void add_header_enums(py::module& mod);
void add_header_classes(py::module& mod);
void add_header_io_funcs(py::module& mod);


#endif// LIBASD_HEADER_PY_HPP
