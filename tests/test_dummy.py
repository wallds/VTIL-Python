import pytest

import sys
sys.path.append(".") 

from pyvtil import *


def test_dummy():
    block = vtil.basic_block(0)
    block.vemits("cpuid")
    print('test_dummy called.')
    vtil.debug.dump(block.owner)
    assert 1 == 1

def test_Optimization_dead_code_elimination_pass():
    block = vtil.basic_block(0x1337)
    block.mov(vtil.x86_reg.RAX, 1)
    block.mov(vtil.x86_reg.RAX, 2)
    block.vexit(0)
    vtil.optimizer.dead_code_elimination_pass(block.owner)
    vtil.debug.dump(block.owner)
    assert block.size() == 2

def test_3():
    print('Test_3 called.')
    assert 2==1+1