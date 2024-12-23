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

#ifndef _DYNLIB_LIB_EXTENSIONS_H_
#define _DYNLIB_LIB_EXTENSIONS_H_

#include <string>
#include <vector>

namespace dynlib {
#ifdef _WIN32  // windows
std::vector<std::string> const library_dynamic_extensions = { ".dll" };
std::vector<std::string> const library_static_extensions  = { ".lib" };
#elif defined __linux__  // linux
std::vector<std::string> const library_dynamic_extensions = { ".so" };
std::vector<std::string> const library_static_extensions  = { ".a" };
#elif defined __APPLE__  // apple
std::vector<std::string> const library_dynamic_extensions = { ".dylib", ".so" };
std::vector<std::string> const library_static_extensions  = { ".bundle", ".a" };
#else
std::vector<std::string> const library_dynamic_extensions = { };
std::vector<std::string> const library_static_extensions  = { };
#endif  // _WIN32
}  // namespace dynlib

#endif  // _DYNLIB_LIB_EXTENSIONS_H_
