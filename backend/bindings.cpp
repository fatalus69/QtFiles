#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "file_operations.h"
#include "utils.h"

namespace py = pybind11;

PYBIND11_MODULE(fileops, m) {
    m.def("list_files", &list_files, "List files in a directory");
    m.def("check_for_dir_contents", &check_for_dir_contents, "Check if a directory has Content");
    m.def("search_in_dir", &search_in_dir, "Find file in directory");
    
    py::class_<FileEntry>(m, "FileEntry")
        .def_readonly("filename", &FileEntry::filename)
        .def_readonly("is_directory", &FileEntry::is_directory)
        .def_readonly("filesize", &FileEntry::filesize);
    
    m.def("get_home_directory", &get_home_directory, "Get current users home directory");
}