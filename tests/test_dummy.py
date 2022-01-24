import pytest

import sys
sys.path.append(".") 

from pyvtil import *


if vtil.arch.size == 4:
    ZAX = vtil.x86_reg.EAX
    ZBX = vtil.x86_reg.EBX
    ZCX = vtil.x86_reg.ECX
else:
    ZAX = vtil.x86_reg.RAX
    ZBX = vtil.x86_reg.RBX
    ZCX = vtil.x86_reg.RCX

# https://github.com/vtil-project/VTIL-Core/blob/master/VTIL-Tests/dummy.cpp


def test_dummy():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0)
    block.vemits("cpuid")
    print('test_dummy called.')
    vtil.debug.dump(block.owner)
    assert 1 == 1


def test_expression_hash():
    const_a = vtil.symbolic.expression(123)
    const_b = (vtil.symbolic.expression(123)+1-1).simplify(True)
    assert const_a.hash() == const_b.hash()

    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1234)
    block.push(0)
    variable_a = vtil.symbolic.variable(block.begin(), vtil.REG_FLAGS)
    variable_b = vtil.symbolic.variable(block.begin(), vtil.REG_FLAGS)
    assert variable_a.hash() == variable_b.hash()
    # simple shift_right
    exp_a = vtil.symbolic.expression(123, 32) >> vtil.symbolic.expression(6, 8)
    exp_b = vtil.symbolic.expression(
        123, 32) >> vtil.symbolic.expression(6, 32)
    exp_a = exp_a.simplify(True)
    exp_b = exp_b.simplify(True)
    assert exp_a.hash() == exp_b.hash()
    # advanced shift_right
    # eax@6:1
    temp = block.tmp(64)
    temp_6 = temp.select(1, 6)
    exp_a = vtil.symbolic.variable(block.begin(), temp_6).to_expression()
    exp_a.resize(vtil.arch.bit_count)
    exp_a = exp_a.simplify(True)
    # eax >> 6 & 1
    exp_b = vtil.symbolic.variable(block.begin(), temp).to_expression()
    exp_b >>= vtil.symbolic.expression(6, 8)
    exp_b &= vtil.symbolic.expression(1, 8)
    exp_b = exp_b.simplify(True)
    assert exp_a.hash() == exp_b.hash()


def test_optimization_vtil_file():
    pass


def test_optimization_stack_pinning_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 0)
    block.shift_sp(-vtil.arch.size)
    block.mov(ZAX, 0)
    block.push(ZAX)
    block.pop(ZAX)
    block.mov(ZAX, 8)
    block.shift_sp(vtil.arch.size)
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
    block.mov(ZAX, vtil.REG_SP)
    block.sub(ZAX, vtil.arch.size)
    block.store(ZAX, 0, 1)
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
    assert ins.operands[1].imm().ival == -vtil.arch.size
    assert ins.operands[2].imm().ival == 1


def test_optimization_stack_propagation_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.store(vtil.REG_SP, 0, 0x1234)
    block.ldd(ZAX, vtil.REG_SP, 0)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.stack_propagation_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)

    ins = block[1]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == ZAX
    assert ins.operands[1].imm().ival == 0x1234


def test_optimization_dead_code_elimination_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 1)
    block.mov(ZAX, 2)
    block.vexit(0)
    vtil.optimizer.dead_code_elimination_pass(block.owner)
    vtil.debug.dump(block.owner)
    assert block.size() == 2


def test_optimization_mov_propagation_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 1)
    block.mov(ZBX, ZAX)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.mov_propagation_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)

    ins = block[1]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == ZBX
    assert ins.operands[1].imm().ival == 1


def test_optimization_register_renaming_pass():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 1)
    block.mov(ZBX, ZAX)
    block.store(vtil.REG_SP, 0, ZBX)
    block.mov(ZAX, 1)
    block.mov(ZBX, 1)
    block.vexit(0)

    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.register_renaming_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)

    ins = block[0]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == ZBX
    assert ins.operands[1].imm().ival == 1


def test_optimization_symbolic_rewrite_pass_force():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 1)
    block.mov(ZBX, ZAX)
    block.store(vtil.REG_SP, 0, ZBX)
    block.mov(ZAX, 1)
    block.mov(ZBX, 1)
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


def test_optimization_dead_code_elimination_pass_2():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 0)
    # simple single block
    block.push(ZAX)
    block.push(ZBX)
    block.shift_sp(vtil.arch.size*2)
    block.mov(ZAX, 0)
    block.vexit(0)
    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.dead_code_elimination_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    assert block.size() == 2
    block = None
    rtn = None
    # with jmp
    rtn = vtil.routine()
    block1, inserted = rtn.create_block(0x1337)
    block1.push(ZAX)
    block1.push(ZBX)
    block1.jmp(0x2000)
    block2 = block1.fork(0x2000)
    block2.shift_sp(vtil.arch.size*2)
    block2.mov(ZAX, 0)
    block2.vexit(0)
    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.dead_code_elimination_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    assert block1.size() == 1
    # with te jmp
    block1 = block2 = None
    rtn = None
    rtn = vtil.routine()
    block1, inserted = rtn.create_block(0x1337)
    block1.push(ZAX)
    block1.push(ZBX) # DCE me
    block1.te(ZCX, ZCX, 0xAABB)
    block1.js(ZCX, 0x2000, 0x3000)
    block2 = block1.fork(0x2000)
    block2.shift_sp(vtil.arch.size*2)
    block2.add(ZAX, 1)
    block2.vexit(0)
    block3 = block1.fork(0x3000)
    block3.ldd(ZAX, vtil.REG_SP, block3.sp_offset+vtil.arch.size)
    block3.shift_sp(vtil.arch.size*2)
    block3.add(ZAX, 1)
    block3.vexit(0)
    print(':: Before:')
    vtil.debug.dump(rtn)
    vtil.optimizer.dead_code_elimination_pass(rtn)
    print(':: After:')
    vtil.debug.dump(rtn)
    # revert VTIL-Core#60 auxiliaries.cpp
    assert block1.size() == 3


def test_simplification():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1337)
    block.mov(ZAX, 0)
    for i in range(2):
        block.add(ZAX, 13)
        block.nop()
        block.sub(ZAX, 12)
        block.nop()
        block.add(ZAX, 14)
        block.mov(ZAX, ZAX)
        block.sub(ZAX, ZAX)
        block.bxor(ZAX, i)
        block.push(ZAX)
    block.vpinr(ZAX)
    block.vexit(0)
    vtil.debug.dump(rtn)
    vtil.optimizer.collective_cross_pass(rtn)
    vtil.debug.dump(rtn)
    ins = block[0]
    assert ins.base == vtil.ins.mov
    assert len(ins.operands) == 2
    assert ins.operands[0].reg().local_id == ZAX
    assert ins.operands[1].imm().ival == 1

    ins = block[1]
    assert ins.base == vtil.ins.str
    assert len(ins.operands) == 3
    assert str(ins.operands[0].reg()) == "$sp"
    assert ins.operands[1].imm().ival == -vtil.arch.size
    assert ins.operands[2].imm().ival == 0

    ins = block[2]
    assert ins.base == vtil.ins.str
    assert len(ins.operands) == 3
    assert str(ins.operands[0].reg()) == "$sp"
    assert ins.operands[1].imm().ival == -vtil.arch.size*2
    assert ins.operands[2].imm().ival == 1

    ins = block[3]
    assert ins.base == vtil.ins.vpinr
    assert len(ins.operands) == 1
    assert ins.operands[0].reg().local_id == ZAX

    ins = block[4]
    assert ins.base == vtil.ins.vexit
    assert len(ins.operands) == 1
    assert ins.operands[0].imm().ival == 0


def test_3():
    print('Test_3 called.')
    assert 2 == 1+1
