// Copyright (c) 2020 Daniel (@L33T) and contributors of the VTIL Project
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of VTIL Project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

// Furthermore, the following pieces of software have additional copyrights
// licenses, and/or restrictions:
//
// |--------------------------------------------------------------------------|
// | File name               | Link for further information                   |
// |-------------------------|------------------------------------------------|
// | operators.hpp           | https://github.com/vtil-project/VTIL-Core      |
// |                         | https://github.com/pybind/pybind11             |
// |--------------------------------------------------------------------------|
//
#pragma once

#include <vtil/vtil>
#include <pybind11/pybind11.h>

using namespace vtil;
namespace py = pybind11;

namespace vtil::python
{
    class operator_id_py : public py::enum_<math::operator_id>
    {
    public:
        operator_id_py(const handle &scope, const char *name)
            : enum_(scope, name)
        {
            (*this)
                // Values
                //

                .value("invalid", math::operator_id::invalid) // = <Invalid>

                // ------------------ Bitwise Operators ------------------ //

                // Bitwise modifiers:
                //
                .value("bitwise_not", math::operator_id::bitwise_not) // ~RHS

                // Basic bitwise operations:
                //
                .value("bitwise_and", math::operator_id::bitwise_and) // LHS&(RHS&...)
                .value("bitwise_or", math::operator_id::bitwise_or)   // LHS|(RHS|...)
                .value("bitwise_xor", math::operator_id::bitwise_xor) // LHS^(RHS^...)

                // Distributing bitwise operations:
                //
                .value("shift_right", math::operator_id::shift_right)   // LHS>>(RHS+...)
                .value("shift_left", math::operator_id::shift_left)     // LHS<<(RHS+...)
                .value("rotate_right", math::operator_id::rotate_right) // LHS>](RHS+...)
                .value("rotate_left", math::operator_id::rotate_left)   // LHS[<(RHS+...)

                // ---------------- Arithmetic Operators ----------------- //

                // Arithmetic modifiers:
                //
                .value("negate", math::operator_id::negate) // -RHS

                // Basic arithmetic operations:
                //
                .value("add", math::operator_id::add)           // LHS+(RHS+...)
                .value("subtract", math::operator_id::subtract) // LHS-(RHS+...)

                // Distributing arithmetic operations:
                //
                .value("multiply_high", math::operator_id::multiply_high) // HI(LHS*RHS)
                .value("multiply", math::operator_id::multiply)           // LHS*(RHS*...)
                .value("divide", math::operator_id::divide)               // LHS/(RHS*...)
                .value("remainder", math::operator_id::remainder)         // LHS%RHS

                .value("umultiply_high", math::operator_id::umultiply_high) // < Unsigned variants of above >
                .value("umultiply", math::operator_id::umultiply)           //
                .value("udivide", math::operator_id::udivide)               //
                .value("uremainder", math::operator_id::uremainder)         //

                // ----------------- Special Operators ----------------- //
                .value("ucast", math::operator_id::ucast)             // uintRHS_t(LHS)
                .value("cast", math::operator_id::cast)               // intRHS_t(LHS)
                .value("popcnt", math::operator_id::popcnt)           // POPCNT(RHS)
                .value("bitscan_fwd", math::operator_id::bitscan_fwd) // BitScanForward(RHS)
                .value("bitscan_rev", math::operator_id::bitscan_rev) // BitScanReverse(RHS)
                .value("bit_test", math::operator_id::bit_test)       // [LHS>>RHS]&1
                .value("mask", math::operator_id::mask)               // RHS.mask()
                .value("bit_count", math::operator_id::bit_count)     // RHS.bitcount()
                .value("value_if", math::operator_id::value_if)       // LHS&1 ? RHS : 0

                .value("max_value", math::operator_id::max_value) // LHS>=RHS ? LHS : RHS
                .value("min_value", math::operator_id::min_value) // LHS<=RHS ? LHS : RHS

                .value("umax_value", math::operator_id::umax_value) // < Unsigned variants of above >
                .value("umin_value", math::operator_id::umin_value) //

                .value("greater", math::operator_id::greater)       // LHS > RHS
                .value("greater_eq", math::operator_id::greater_eq) // LHS >= RHS
                .value("equal", math::operator_id::equal)           // LHS == RHS
                .value("not_equal", math::operator_id::not_equal)   // LHS != RHS
                .value("less_eq", math::operator_id::less_eq)       // LHS <= RHS
                .value("less", math::operator_id::less)             // LHS < RHS

                .value("ugreater", math::operator_id::ugreater)       // < Unsigned variants of above > [Note: equal and not_equal are always unsigned.]
                .value("ugreater_eq", math::operator_id::ugreater_eq) //
                .value("uequal", math::operator_id::uequal)           //
                .value("unot_equal", math::operator_id::unot_equal)   //
                .value("uless_eq", math::operator_id::uless_eq)       //
                .value("uless", math::operator_id::uless)             //
                .value("max", math::operator_id::max)
                // End
                //
                ;
        }
    };
}
