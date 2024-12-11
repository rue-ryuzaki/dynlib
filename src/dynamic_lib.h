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

#ifndef _DYNLIB_DYNAMIC_LIB_H_
#define _DYNLIB_DYNAMIC_LIB_H_

#include <functional>

namespace dynlib {
// -----------------------------------------------------------------------------
enum Mode {
    ModeLocal       = 0x0000,
    ModeLazy        = 0x0001,
    ModeNow         = 0x0002,
    ModeNoLoad      = 0x0004,
    ModeDeepBind    = 0x0008,
    ModeGlobal      = 0x0100,
    ModeNodelete    = 0x1000,
};

// -----------------------------------------------------------------------------
// -- DynamicLib ---------------------------------------------------------------
// -----------------------------------------------------------------------------
class DynamicLib
{
    DynamicLib(DynamicLib const&) = delete;
    DynamicLib& operator =(DynamicLib const&) = delete;

public:
    DynamicLib();
    ~DynamicLib();

    void
    open(char const* name,
            Mode mode = ModeLazy);

    bool
    is_open() const;

    void
    close();

    void*
    function(
            char const* name) const;

    template <class T, class... Args>
    inline std::function<T(Args...)>
    function_cast(
            char const* name) const
    {
        return reinterpret_cast<T(*)(Args...)>(function(name));
    }

private:
    // -- data ----------------------------------------------------------------
    void* m_library;
};
}  // namespace dynlib

#endif  // _DYNLIB_DYNAMIC_LIB_H_
