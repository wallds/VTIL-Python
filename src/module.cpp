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
// | module.cpp              | https://github.com/vtil-project/VTIL-Core      |
// |                         | https://github.com/pybind/pybind11             |
// |                         | https://github.com/aquynh/capstone/            |
// |                         | https://github.com/keystone-engine/keystone/   |
// |--------------------------------------------------------------------------|
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "architecture/arch/identifier.hpp"
#include "architecture/arch/instruction_desc.hpp"
#include "architecture/arch/instruction_set.hpp"
#include "architecture/arch/operands.hpp"
#include "architecture/arch/register_desc.hpp"
#include "architecture/routine/basic_block.hpp"
#include "architecture/routine/call_convention.hpp"
#include "architecture/routine/instruction.hpp"
#include "architecture/routine/routine.hpp"
#include "architecture/misc/debug.hpp"
#include "architecture/trace/tracer.hpp"
#include "architecture/trace/cached_tracer.hpp"
#include "architecture/symex/variable.hpp"

#include "common/util/fnv64.hpp"
// #include "common/util/fnv128.hpp"
#include "common/math/operators.hpp"
#include "common/math/bitwise.hpp"

#include "compiler/common/interface.hpp"

#include "symex/expressions/unique_identifier.hpp"
#include "symex/expressions/expression.hpp"
#include "symex/directive.hpp"

#include "external/arm64_reg.hpp"
#include "external/x86_reg.hpp"

#include "optimizer/auxiliaries.hpp"

using namespace vtil::python;
namespace py = pybind11;

struct dummy_arch {};
struct dummy_common {};
struct dummy_symbolic {};
struct dummy_math {};

PYBIND11_MODULE(vtil, m) {
	// Hook error function
	vtil::logger::error_hook = [ ] ( const std::string& msg )
	{
		throw std::runtime_error( msg );
	};
	py::class_<dummy_arch> arch(m, "arch");
	py::class_<dummy_common> common(m, "common");
	py::class_<dummy_symbolic> symbolic(m, "symbolic");
	py::class_<dummy_math> math(m, "math");

	// VTIL Arch
	//
	{
		arch.def_readonly_static( "size", &arch::size );
		arch.def_readonly_static( "bit_count", &arch::bit_count );
	}
	// VTIL Common
	//
	{
		/* Utility */
		fnv64_hash_py( common, "fnv64" );
		// fnv128_hash_py( common, "fnv128" );
	}

	// VTIL SymEx
	//
	{
		/* Expressions */
		unique_identifier_py( symbolic, "uid" );
		expression_reference_py( symbolic, "expression_reference" );
		expression_py( symbolic, "expression" );
		directive_py( symbolic, "directive" );
	}
	{
		operator_id_py( math, "operators" );
		bit_vector_py( math, "bit_vector" );
	}
	{
		/* Architecture */
		operand_py( m, "operand" );

		m.def("make_uint", [](uint64_t value, bitcnt_t bit_count) { return operand(value, bit_count); },
			py::arg("value"), py::arg("bit_count") = sizeof(uint64_t) * 8);
		m.def("make_int", [](int64_t value, bitcnt_t bit_count) { return operand(value, bit_count); },
			py::arg("value"), py::arg("bit_count") = sizeof(int64_t) * 8);

		architecture_identifier_py( m, "architecture_identifier" );
		m.attr("architecture_default") = vtil::architecture_default;
		instruction_desc_py( m, "instruction_desc" );
		register_desc_py( m, "register_desc" );
		{
			m.attr("register_virtual") = register_virtual;
			m.attr("register_physical") = register_physical;
			m.attr("register_local") = register_local;
			m.attr("register_flags") = register_flags;
			m.attr("register_stack_pointer") = register_stack_pointer;
			m.attr("register_image_base") = register_image_base;
			m.attr("register_volatile") = register_volatile;
			m.attr("register_readonly") = register_readonly;
			m.attr("register_undefined") = register_undefined;
			m.attr("register_internal") = register_internal;
			m.attr("register_special") = register_special;
		}
		ins_py( m, "ins" );
		{
			m.attr("UNDEFINED") = vtil::UNDEFINED;
			m.attr("REG_IMGBASE") = vtil::REG_IMGBASE;
			m.attr("REG_FLAGS") = vtil::REG_FLAGS;
			m.attr("REG_SP") = vtil::REG_SP;
			m.attr("invalid_vip") = vtil::invalid_vip;
			m.def("make_undefined", &vtil::make_undefined);
		}

		/* Instruction Stream */
		instruction_py( m, "instruction" );
		basic_block_py( m, "basic_block" );
		call_convention_py( m, "call_convention" );
		routine_py( m, "routine" );

		/* Miscellaneous */
		debug_py( m, "debug" );

		/* SymEx Integration */
		variable_py( symbolic, "variable" );

		/* Value Tracing */
		tracer_py( m, "tracer" );
		cached_tracer_py( m, "cached_tracer" );

	}

	// External
	//
	{
		// arm64_reg_py( m, "arm64_reg" );
		x86_reg_py( m, "x86_reg" );
	}

	// Optimizer
	//
	{
		auto optimizer = optimizer_py( m, "optimizer" );
		pass_interface_py(optimizer, "pass_interface");
	}
	
#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif

}
