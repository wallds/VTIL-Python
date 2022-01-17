from pyvtil import *

def main():
    block = vtil.basic_block(0)
    t0, zf = block.tmp(1, 1)
    t1, t2 = block.tmp(64, 64)
    sp = vtil.register_desc.SP

    block.mov(t1, vtil.register_desc.IMGBASE)
    block.ldd(t2, t1, 0)
    block.mov(vtil.x86_reg.RAX, 100)
    block.te(zf, t2, 0)
    block.js(zf, 0x1000, 0x2000)
    
    block2 = block.fork(0x1000)
    block2.vexit(0)
    block3 = block.fork(0x2000)
    block3.vexit(0)

    tracer = vtil.tracer()
    
    flag = vtil.optimizer.aux.branch_analysis_flags()
    exp = tracer.trace(vtil.variable(block.end(), t0))
    branch = vtil.optimizer.aux.analyze_branch(block, tracer, flag)
    
    vtil.debug.dump(block.owner)
    # vtil.optimizer.apply_all(block.owner)
    # vtil.optimizer.apply_all(block, False)
    # vtil.optimizer.apply_all(block.owner)
    vtil.debug.dump(block.owner)


    print('x0: {}'.format(exp))
    pass

if __name__ == '__main__':
    main()
