import pytest

import sys
sys.path.append(".") 

from pyvtil import *

# https://github.com/vtil-project/VTIL-Core/blob/master/VTIL-Tests/dummy.cpp
def test_dummy():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0)
    block.vemits("cpuid")
    print('test_dummy called.')
    vtil.debug.dump(block.owner)
    assert 1 == 1

def test_optimization_stack_pinning_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 0)
    block.shift_sp(-8)
    block.mov(vtil.x86_reg.RAX, 0)
    block.push(vtil.x86_reg.RAX)
    block.pop(vtil.x86_reg.RAX)
    block.mov(vtil.x86_reg.RAX, 8)
    block.shift_sp(8)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.stack_pinning_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    assert block.size() == 6
    assert block.sp_offset == 0


def test_optimization_istack_ref_substitution_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, vtil.REG_SP)
    block.sub(vtil.x86_reg.RAX, 8)
    block.store(vtil.x86_reg.RAX, 0, 1)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.istack_ref_substitution_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    ins = block[2]
    assert ins.base == vtil.ins.str
    assert len(ins.operands) == 3
    assert str(ins.operands[0].reg()) == "$sp"
    assert ins.operands[1].imm().ival == -8
    assert ins.operands[2].imm().ival == 1


def test_optimization_stack_propagation_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.store(vtil.REG_SP, 0, 0x1234)
    block.ldd(vtil.x86_reg.RAX, vtil.REG_SP, 0)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.stack_propagation_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    ins = block[1]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == vtil.x86_reg.RAX
    assert ins.operands[1].imm().ival == 0x1234

def test_optimization_mov_propagation_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RBX, vtil.x86_reg.RAX)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.mov_propagation_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    ins = block[1]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == vtil.x86_reg.RBX
    assert ins.operands[1].imm().ival == 1

def test_optimization_register_renaming_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RBX, vtil.x86_reg.RAX)
    block.store(vtil.REG_SP, 0, vtil.x86_reg.RBX)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RBX, 1)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.register_renaming_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    ins = block[0]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == vtil.x86_reg.RBX
    assert ins.operands[1].imm().ival == 1

def test_optimization_symbolic_rewrite_pass_force():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RBX, vtil.x86_reg.RAX)
    block.store(vtil.REG_SP, 0, vtil.x86_reg.RBX)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RBX, 1)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.symbolic_rewrite_pass_force(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    
    assert block.size() == 4

    ins = block[0]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[1].imm().ival == 1

    ins = block[1]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[1].imm().ival == 1

    ins = block[2]
    assert ins.base == vtil.ins.str
    assert len(ins.operands) == 3
    assert str(ins.operands[0].reg()) == "$sp"
    assert ins.operands[2].imm().ival == 1


def test_Optimization_dead_code_elimination_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RAX, 2)
    block.vexit(0)
    vtil.optimizer.dead_code_elimination_pass(block.owner)
    vtil.debug.dump(block.owner)
    assert block.size() == 2

def test_3():
    print('Test_3 called.')
    assert 2==1+1