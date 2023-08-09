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
					// Constructor
					//
					//.def( py::init([](const char* id, int lookup_index, directive::matching_type mtype) {
					//		return directive::instance((new std::string(id))->c_str(), lookup_index, mtype);
					//	}), py::arg("id"), py::arg("lookup_index"), py::arg("mtype") = directive::match_any )

					// Functions
					//
					.def( "to_string", &directive::instance::to_string )
					.def( "__repr__", &directive::instance::to_string )
					.def( "__str__", &directive::instance::to_string )

					.def( "__invert__", [](const directive::instance& rhs) { return ~rhs; } )
					.def( "__neg__", [](const directive::instance& rhs) { return -rhs; } )
					// .def( "__pos__", [](const directive::instance& rhs) { return rhs; } )
					.def( "popcnt", [](const directive::instance& rhs) { return __popcnt(rhs); } )
					.def( "bsf", [](const directive::instance& rhs) { return __bsf(rhs); } )
					.def( "bsr", [](const directive::instance& rhs) { return __bsr(rhs); } )
					.def( "mask", [](const directive::instance& rhs) { return __mask(rhs); } )
					.def( "bcnt", [](const directive::instance& rhs) { return __bcnt(rhs); } )

					// vs2022 Error C2678
					// .def( py::self + py::self )
					// .def( py::self - py::self )
					// .def( py::self | py::self )
					// .def( py::self & py::self )
					// .def( py::self ^ py::self )
					// .def( py::self * py::self )
					// .def( py::self * int64_t() )
					// .def( py::self / py::self )
					// .def( py::self % py::self )
					
					// End
					//
					;

#define IMPLEMENT_OPERATOR( name, ... ) 																				\
		.def( name, [ ] ( const directive::instance& lhs, const directive::instance& rhs ) { return __VA_ARGS__; } )	\
		.def( name, [ ] ( const directive::instance& lhs, int64_t rhs ) { return __VA_ARGS__; } )						\
		.def( name, [ ] ( const directive::instance& lhs, uint64_t rhs ) { return __VA_ARGS__; } )
					
				( *this )
					// Functions
					//
					IMPLEMENT_OPERATOR( "rotr", __rotr(lhs, rhs) )
					IMPLEMENT_OPERATOR( "rotl", __rotl(lhs, rhs) )

					IMPLEMENT_OPERATOR( "mulhi", mulhi(lhs, rhs))
					IMPLEMENT_OPERATOR( "umulhi", umulhi(lhs, rhs) )
					IMPLEMENT_OPERATOR( "umul", umul(lhs, rhs) )
					IMPLEMENT_OPERATOR( "udiv", udiv(lhs, rhs) )
					IMPLEMENT_OPERATOR( "urem", urem(lhs, rhs) )

					IMPLEMENT_OPERATOR( "ucast", __ucast(lhs, rhs) )
					IMPLEMENT_OPERATOR( "cast", __cast(lhs, rhs) )

					IMPLEMENT_OPERATOR( "bt", __bt(lhs, rhs) )
					IMPLEMENT_OPERATOR( "if_", __if(lhs, rhs) )
					IMPLEMENT_OPERATOR( "max", __max(lhs, rhs) )
					IMPLEMENT_OPERATOR( "min", __min(lhs, rhs) )
					IMPLEMENT_OPERATOR( "umax", __umax(lhs, rhs) )
					IMPLEMENT_OPERATOR( "umin", __umin(lhs, rhs) )

					IMPLEMENT_OPERATOR( "ugreat", __ugreat(lhs, rhs) )
					IMPLEMENT_OPERATOR( "ugreat_eq", __ugreat_eq(lhs, rhs) )
					IMPLEMENT_OPERATOR( "uequal", __uequal(lhs, rhs) )
					IMPLEMENT_OPERATOR( "unot_equal", __unot_equal(lhs, rhs) )
					IMPLEMENT_OPERATOR( "uless_eq", __uless_eq(lhs, rhs) )
					IMPLEMENT_OPERATOR( "uless", __uless(lhs, rhs) )

					IMPLEMENT_OPERATOR( "__add__", lhs + rhs )
					IMPLEMENT_OPERATOR( "__sub__", lhs - rhs )
					IMPLEMENT_OPERATOR( "__mul__", lhs * rhs )
					IMPLEMENT_OPERATOR( "__truediv__", lhs / rhs )
					IMPLEMENT_OPERATOR( "__mod__", lhs % rhs )
					IMPLEMENT_OPERATOR( "__and__", lhs & rhs )
					IMPLEMENT_OPERATOR( "__or__", lhs | rhs )
					IMPLEMENT_OPERATOR( "__xor__", lhs ^ rhs )
					IMPLEMENT_OPERATOR( "__lshift__", lhs << rhs )
					IMPLEMENT_OPERATOR( "__rshift__", lhs >> rhs )
					IMPLEMENT_OPERATOR( "__gt__", lhs > rhs )
					IMPLEMENT_OPERATOR( "__ge__", lhs >= rhs )
					IMPLEMENT_OPERATOR( "__lt__", lhs < rhs )
					IMPLEMENT_OPERATOR( "__le__", lhs <= rhs )
					IMPLEMENT_OPERATOR( "__eq__", lhs == rhs )
					IMPLEMENT_OPERATOR( "__ne__", lhs != rhs )
					
					// End
					//
					;
#undef IMPLEMENT_OPERATOR

//#define IMPLEMENT_IOPERATOR( name, ... ) 																		\
//		.def( name, [ ] ( directive::instance& lhs, const directive::instance& rhs ) { return __VA_ARGS__; } )	\
//		.def( name, [ ] ( directive::instance& lhs, int64_t rhs ) { return __VA_ARGS__; } )						\
//		.def( name, [ ] ( directive::instance& lhs, uint64_t rhs ) { return __VA_ARGS__; } )
//
//				(*this)
//					// Functions
//					//
//					IMPLEMENT_IOPERATOR( "__iadd__", lhs += rhs )
//					IMPLEMENT_IOPERATOR( "__isub__", lhs -= rhs )
//					IMPLEMENT_IOPERATOR( "__imul__", lhs *= rhs )
//					IMPLEMENT_IOPERATOR( "__itruediv__", lhs /= rhs )
//					IMPLEMENT_IOPERATOR( "__imod__", lhs %= rhs )
//					IMPLEMENT_IOPERATOR( "__ilshift__", lhs <<= rhs )
//					IMPLEMENT_IOPERATOR( "__irshift__", lhs >>= rhs )
//					IMPLEMENT_IOPERATOR( "__iand__", lhs &= rhs )
//					IMPLEMENT_IOPERATOR( "__ixor__", lhs ^= rhs )
//					IMPLEMENT_IOPERATOR( "__ior__", lhs |= rhs )
//
//					// End
//					//
//					;
//#undef IMPLEMENT_IOPERATOR


#define IMPLEMENT_ROPERATOR( name, ... ) 															\
		.def( name, [ ] ( const directive::instance& rhs, int64_t lhs ) { return __VA_ARGS__; } )	\
		.def( name, [ ] ( const directive::instance& rhs, uint64_t lhs ) { return __VA_ARGS__; } )

				( *this )
					// Functions
					//
					IMPLEMENT_ROPERATOR( "__radd__", lhs + rhs )
					IMPLEMENT_ROPERATOR( "__rsub__", lhs - rhs )
					IMPLEMENT_ROPERATOR( "__rmul__", lhs * rhs )
					IMPLEMENT_ROPERATOR( "__rtruediv__", lhs / rhs )
					IMPLEMENT_ROPERATOR( "__rmod__", lhs % rhs )
					IMPLEMENT_ROPERATOR( "__rand__", lhs & rhs )
					IMPLEMENT_ROPERATOR( "__ror__", lhs | rhs )
					IMPLEMENT_ROPERATOR( "__rxor__", lhs ^ rhs )
					IMPLEMENT_ROPERATOR( "__rlshift__", lhs << rhs )
					IMPLEMENT_ROPERATOR( "__rrshift__", lhs >> rhs )

					// End
					//
					;
#undef IMPLEMENT_ROPERATOR
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
