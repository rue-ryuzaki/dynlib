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

#ifndef DYNLIB_DYNAMIC_LIB_H
#define DYNLIB_DYNAMIC_LIB_H

#ifdef _WIN32 // windows
#include <windows.h>
#elif defined __linux__ || defined __APPLE__ // linux || apple
#include <dlfcn.h>
#else
#error "Operation system not supported"
#endif // _WIN32

#include <functional>

// -----------------------------------------------------------------------------
// -- DynamicLib ---------------------------------------------------------------
// -----------------------------------------------------------------------------
class DynamicLib
{
public:
    DynamicLib();
    ~DynamicLib();

#ifdef _WIN32 // windows
    void    open(const char* name);
#elif defined __linux__ || defined __APPLE__ // linux || apple
    void    open(const char* name, int mode = RTLD_LAZY);
#else
#error "Operation system not supported"
#endif // _WIN32

    void    close();

    void*   function(const char* name);

    template <class T>
    inline std::function<T()> function_cast(const char* name)
    {
        return std::bind(reinterpret_cast<T(*)()>(function(name)));
    }

private:
#ifdef _WIN32 // windows
    HMODULE m_library;
#elif defined __linux__ || defined __APPLE__ // linux || apple
    void*   m_library;
#else
#error "Operation system not supported"
#endif // _WIN32

    DynamicLib(const DynamicLib&) = delete;
    DynamicLib& operator =(const DynamicLib&) = delete;
};

#endif // DYNLIB_DYNAMIC_LIB_H
