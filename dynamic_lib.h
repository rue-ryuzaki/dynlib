/*
 * MIT License
 *
 * Cross-platform project to work with shared (dynamic) libraries in C++
 *
 * Copyright (c) 2018-2021 Golubchikov Mihail
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

#ifndef DYNLIB_DYNAMIC_LIB_H_
#define DYNLIB_DYNAMIC_LIB_H_

#ifdef _WIN32 // windows
#include <windows.h>
#define DYNLIB_DEFAULT_MODE 1
typedef HMODULE DynlibPtr;
#elif defined __linux__ || defined __APPLE__ // linux || apple
#include <dlfcn.h>
#define DYNLIB_DEFAULT_MODE RTLD_LAZY
typedef void* DynlibPtr;
#else
#error "Operation system not supported"
#endif // _WIN32

#include <functional>

namespace dynlib {
// -----------------------------------------------------------------------------
// -- DynamicLib ---------------------------------------------------------------
// -----------------------------------------------------------------------------
class DynamicLib
{
public:
    DynamicLib();
    ~DynamicLib();

    void    open(char const* name, int mode = DYNLIB_DEFAULT_MODE);

    void    close();

    void*   function(char const* name);

    template <class T>
    inline std::function<T()> function_cast(char const* name)
    {
        return std::bind(reinterpret_cast<T(*)()>(function(name)));
    }

private:
    DynlibPtr   m_library;

    DynamicLib(DynamicLib const&) = delete;
    DynamicLib& operator =(DynamicLib const&) = delete;
};
} // dynlib

#endif // DYNLIB_DYNAMIC_LIB_H_
