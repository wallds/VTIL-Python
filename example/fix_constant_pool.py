from pyvtil import *

rtn = vtil.routine()
block, inserted = rtn.create_block(0x0)
t0, t1 = block.tmp(64, 64)

block.mov(t1, vtil.REG_IMGBASE)
block.add(t1, 0x1240)
block.ldd(t0, t1, 0)
block.vpinr(t0)
block.vexit(0)

vtil.optimizer.stack_pinning_pass(block)
vtil.optimizer.istack_ref_substitution_pass(block)
vtil.debug.dump(rtn)

reloc_base = vtil.symbolic.variable(block.begin(), vtil.REG_SP).to_expression()


def exp_eval(uid):
    var = uid.get_variable()
    if var.is_register() and var.reg().is_image_base():
        return 0
    if var.is_memory() and ((var.mem().base - reloc_base) == 0):
        # TODO:
        assert False
        return 0  # -base
    return None


tracer = vtil.cached_tracer()

fake = {
    0x1240 : 0x1234567800110022
}

it = block.begin()
while it != block.end():
    ins = it.get()
    if ins.base != vtil.ins.ldd:
        it = it.next()
        continue
    base, off = ins.memory_location()
    if base.is_stack_pointer():
        it = it.next()
        continue

    exp = tracer(vtil.symbolic.variable(it, base))
    print(exp)
    res = exp.evaluate(exp_eval)

    if res.is_known():
        rva = res.get_uint64() + off
        if rva in fake and ins.access_size() == 64:  # test
            value = fake[rva]
            ins.base = vtil.ins.mov
            ins.operands = [ins.operands[0],
                            vtil.make_uint(value, ins.access_size())]
    it = it.next()

vtil.debug.dump(rtn)

exp = tracer.trace(vtil.symbolic.variable(block.end().prev(), t0))
print('t0: {}'.format(exp.simplify(True)))
