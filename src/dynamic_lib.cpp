/*
 * MIT License
 *
 * Cross-platform project to work with shared (dynamic) libraries in C++
 *
 * Copyright (c) 2018-2024 Golubchikov Mihail
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "dynlib/dynamic_lib.h"
#include "lib_extensions.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#ifdef _WIN32  // windows
# include <io.h>
# include <windows.h>
# define access    _access_s

namespace dynlib {
# define DYNLIB_OPEN(x,y) LoadLibrary(x)
# define DYNLIB_CLOSE(x) FreeLibrary(x)
# define DYNLIB_GET_FUNC(x,y) GetProcAddress(x,y)
}  // namespace dynlib
#elif defined __linux__ || defined __APPLE__  // linux || apple
# include <dlfcn.h>
# include <unistd.h>

namespace dynlib {
# define DYNLIB_OPEN(x,y) dlopen(x,y)
# define DYNLIB_CLOSE(x) dlclose(x)
# define DYNLIB_GET_FUNC(x,y) dlsym(x,y)
}  // namespace dynlib
#else
# warning "Operation system not supported"
# define DYNLIB_OPEN(x,y) nullptr
# define DYNLIB_CLOSE(x)
# define DYNLIB_GET_FUNC(x,y) nullptr
#endif  // _WIN32

namespace dynlib {
namespace detail {
// -----------------------------------------------------------------------------
inline bool
_file_exists(
        std::string const& file)
{
    return access(file.c_str(), 0) == 0;
}

// -----------------------------------------------------------------------------
inline std::string
_extension(
        std::string const& str)
{
    std::string file = str;
    size_t pos = file.rfind('/');
    if (pos != std::string::npos) {
        file = file.substr(pos, std::string::npos);
    }
    pos = file.rfind('.');
    return ((pos == std::string::npos) ? std::string()
                                       : file.substr(pos, std::string::npos));
}
}  // namespace detail

// -----------------------------------------------------------------------------
// -- DynamicLib ---------------------------------------------------------------
// -----------------------------------------------------------------------------
DynamicLib::DynamicLib()
    : m_library(nullptr)
{
}

// -----------------------------------------------------------------------------
DynamicLib::~DynamicLib()
{
    close();
}

// -----------------------------------------------------------------------------
void
DynamicLib::open(
        char const* name,
        int mode)
{
    close();
    std::string file = std::string(name);
    std::string ext = detail::_extension(file);
    if (ext.empty()) {
        for (auto const& ex : library_dynamic_extensions) {
            if (detail::_file_exists(file + ex)) {
                ext = ex;
                break;
            }
        }
        if (ext.empty()) {
            throw std::invalid_argument(
                        "library file with default extensions not found");
        }
        file += ext;
    } else if (std::find(library_dynamic_extensions.begin(),
                         library_dynamic_extensions.end(), ext) == library_dynamic_extensions.end()) {
        throw std::invalid_argument("unexpected library extension "
                                    "'" + ext + "'");
    }
    if (!detail::_file_exists(file)) {
        throw std::invalid_argument("library file '" + file + "' doesn't exist");
    }
    m_library = DYNLIB_OPEN(file.c_str(), mode);
}

// -----------------------------------------------------------------------------
bool
DynamicLib::is_open() const
{
    return m_library != nullptr;
}

// -----------------------------------------------------------------------------
void
DynamicLib::close()
{
    if (m_library) {
        DYNLIB_CLOSE(m_library);
        m_library = nullptr;
    }
}

// -----------------------------------------------------------------------------
void*
DynamicLib::function(
        char const* name) const
{
    if (m_library) {
        return reinterpret_cast<void*>(DYNLIB_GET_FUNC(m_library, name));
    } else {
        return nullptr;
    }
}
}  // namespace dynlib
