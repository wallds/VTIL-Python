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
// | instruction_set.hpp     | https://github.com/vtil-project/VTIL-Core      |
// |                         | https://github.com/pybind/pybind11             |
// |--------------------------------------------------------------------------|
//
#pragma once

#include <vtil/vtil>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

using namespace vtil;
namespace py = pybind11;

struct dummy_ins {};
namespace vtil::python
{
	class ins_py : public py::class_<dummy_ins>
	{
		public:
		ins_py( const handle& scope, const char* name )
			: class_( scope, name )
		{
			( *this )
				// Properties
				//
                //  -- Data/Memory instructions
				.def_readonly_static( "mov", &ins::mov )
				.def_readonly_static( "movsx", &ins::movsx )
				.def_readonly_static( "str", &ins::str )
				.def_readonly_static( "ldd", &ins::ldd )
                //    -- Arithmetic instructions
				.def_readonly_static( "neg", &ins::neg )
				.def_readonly_static( "add", &ins::add )
				.def_readonly_static( "sub", &ins::sub )
				.def_readonly_static( "mul", &ins::mul )
				.def_readonly_static( "imul", &ins::imul )
				.def_readonly_static( "mulhi", &ins::mulhi )
				.def_readonly_static( "imulhi", &ins::imulhi )
				.def_readonly_static( "div", &ins::div )
				.def_readonly_static( "idiv", &ins::idiv )
				.def_readonly_static( "rem", &ins::rem )
				.def_readonly_static( "irem", &ins::irem )
                //  -- Bitwise instructions
				.def_readonly_static( "popcnt", &ins::popcnt )
				.def_readonly_static( "bsf", &ins::bsf )
				.def_readonly_static( "bsr", &ins::bsr )
				.def_readonly_static( "bnot", &ins::bnot )
				.def_readonly_static( "bshr", &ins::bshr )
				.def_readonly_static( "bshl", &ins::bshl )
				.def_readonly_static( "bxor", &ins::bxor )
				.def_readonly_static( "bor", &ins::bor )
				.def_readonly_static( "band", &ins::band )
				.def_readonly_static( "bror", &ins::bror )
				.def_readonly_static( "brol", &ins::brol )
                //  -- Conditional instructions
				.def_readonly_static( "tg",  &ins::tg )
				.def_readonly_static( "tge", &ins::tge )
				.def_readonly_static( "te",  &ins::te )
				.def_readonly_static( "tne", &ins::tne )
				.def_readonly_static( "tle", &ins::tle )
				.def_readonly_static( "tl", &ins::tl )
				.def_readonly_static( "tug", &ins::tug )
				.def_readonly_static( "tuge", &ins::tuge )
				.def_readonly_static( "tule", &ins::tule )
				.def_readonly_static( "tul", &ins::tul )
				.def_readonly_static( "ifs", &ins::ifs )
                //  -- Control flow instructions
				.def_readonly_static( "js", &ins::js )
				.def_readonly_static( "jmp", &ins::jmp )
				.def_readonly_static( "vexit", &ins::vexit )
				.def_readonly_static( "vxcall", &ins::vxcall )
                //    -- Special instructions
				.def_readonly_static( "nop", &ins::nop )
				.def_readonly_static( "sfence", &ins::sfence )
				.def_readonly_static( "lfence", &ins::lfence )
				.def_readonly_static( "vemit", &ins::vemit )
				.def_readonly_static( "vpinr", &ins::vpinr )
				.def_readonly_static( "vpinw", &ins::vpinw )
				.def_readonly_static( "vpinrm", &ins::vpinrm )
				.def_readonly_static( "vpinwm", &ins::vpinwm )
				// End
				//
				;
		}
	};
}
