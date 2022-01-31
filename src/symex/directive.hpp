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
// | directive.hpp           | https://github.com/vtil-project/VTIL-Core      |
// |                         | https://github.com/pybind/pybind11             |
// |--------------------------------------------------------------------------|
//
#pragma once

#include <vtil/vtil>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

using namespace vtil::symbolic;
namespace py = pybind11;

struct dummy_directive {};

namespace vtil::python
{
	class directive_py : public py::class_<dummy_directive>
	{
		class instance_py : public py::class_<directive::instance>
		{
		public:
		instance_py( const handle& scope, const char* name )
			: class_( scope, name )
			{
				( *this )
					.def( "to_string", &directive::instance::to_string )
					.def( "__repr__", &directive::instance::to_string )
					.def( "__str__", &directive::instance::to_string )
					//.def( py::self + py::self )
					.def( py::self - py::self )
					// .def( py::self | py::self )
					// .def( py::self & py::self )
					// .def( py::self ^ py::self )
					.def( py::self * py::self )
					.def( py::self * int64_t() )
					// .def( py::self / py::self )
					// .def( py::self % py::self )

					;
			}
		};

		public:
		directive_py( const handle& scope, const char* name )
			: class_( scope, name )
		{
			py::enum_<directive::matching_type>( ( *this ), "matching_type" )
				.value( "match_any", directive::matching_type::match_any )
				.value( "match_variable", directive::matching_type::match_variable )
				.value( "match_constant", directive::matching_type::match_constant )
				.value( "match_expression", directive::matching_type::match_expression )
                .value( "match_non_expression", directive::matching_type::match_non_expression )
				.value( "match_non_constant", directive::matching_type::match_non_constant )

				;
			
            instance_py( (*this), "instance" );

			( *this )
				.def_readonly_static( "A", &directive::A )
				.def_readonly_static( "B", &directive::B )
				.def_readonly_static( "C", &directive::C )
				.def_readonly_static( "D", &directive::D )
				.def_readonly_static( "E", &directive::E )
				.def_readonly_static( "F", &directive::F )
				.def_readonly_static( "G", &directive::G )

				.def_readonly_static( "V", &directive::V )
				.def_readonly_static( "U", &directive::U )
				.def_readonly_static( "Q", &directive::Q )
				.def_readonly_static( "W", &directive::W )
				.def_readonly_static( "X", &directive::X )
				// End
				//
				;
		}
	};
}
