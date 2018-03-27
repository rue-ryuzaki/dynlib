/*
 * MIT License
 *
 * Cross-platform project to work with shared (dynamic) libraries in C++
 *
 * Copyright (c) 2018 Golubchikov Mihail
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "dynamic_lib.h"
#include "lib_extensions.h"

#include <algorithm>
#include <string>

#ifdef _WIN32 // windows
#include <io.h>
#define access    _access_s
#elif defined __linux__ || defined __APPLE__ // linux || apple
#include <unistd.h>
#else
#error "Operation system not supported"
#endif // _WIN32

// -----------------------------------------------------------------------------
static bool fileExists(const std::string& file)
{
    return access(file.c_str(), 0) == 0;
}

// -----------------------------------------------------------------------------
static std::string extension(const std::string& str)
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
#ifdef _WIN32 // windows
void DynamicLib::open(const char* name)
#elif defined __linux__ || defined __APPLE__ // linux || apple
void DynamicLib::open(const char* name, int mode)
#else
#error "Operation system not supported"
#endif // _WIN32
{
    close();
    std::string file = std::string(name);
    std::string ext = extension(file);
    if (ext.empty()) {
        for (const std::string& ex : library_dynamic_extensions) {
            if (fileExists(file + ex)) {
                ext = ex;
                break;
            }
        }
        if (ext.empty()) {
            throw std::invalid_argument(
                        "library file with default extensions not found");
        }
        file += ext;
    } else {
        auto it = std::find(library_dynamic_extensions.begin(),
                            library_dynamic_extensions.end(), ext);
        if (it == library_dynamic_extensions.end()) {
            throw std::invalid_argument("unexpected library extension "
                                        "'" + ext + "'");
        }
    }
    if (!fileExists(file)) {
        throw std::invalid_argument("library file '" + file + "' not exist");
    }
    m_library =
#ifdef _WIN32 // windows
            LoadLibrary(file.c_str());
#elif defined __linux__ || defined __APPLE__ // linux || apple
            dlopen(file.c_str(), mode);
#else
#error "Operation system not supported"
#endif // _WIN32
}

// -----------------------------------------------------------------------------
void DynamicLib::close()
{
    if (m_library) {
#ifdef _WIN32 // windows
        FreeLibrary(m_library);
#elif defined __linux__ || defined __APPLE__ // linux || apple
        dlclose(m_library);
#else
#error "Operation system not supported"
#endif // _WIN32
        m_library = nullptr;
    }
}

// -----------------------------------------------------------------------------
void* DynamicLib::function(const char* name)
{
    if (m_library) {
        return
#ifdef _WIN32 // windows
            reinterpret_cast<void*>(GetProcAddress(m_library, name));
#elif defined __linux__ || defined __APPLE__ // linux || apple
            dlsym(m_library, name);
#else
#error "Operation system not supported"
#endif // _WIN32
    } else {
        return nullptr;// TODO
    }
}
