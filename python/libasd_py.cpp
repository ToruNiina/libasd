#include "libasd_header_py.hpp"


PYBIND11_MODULE(libasd, mod) {
    mod.doc() = "libasd -- library to read and write .asd format file";

    add_header_enums(mod);
    add_header_classes(mod);
    add_header_io_funcs(mod);
}
