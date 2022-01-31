import threading
from pyvtil import *


class simple_nop_remove_pass(vtil.optimizer.pass_interface):
    def __init__(self, parallel=True) -> None:
        self.lock = threading.Lock()
        super().__init__(parallel)

    def pass_(self, blk: vtil.basic_block, xblock: bool):
        self.lock.acquire()
        delete_list = []
        it = blk.begin()
        while it != blk.end():
            if it.get().base == vtil.ins.nop:
                delete_list.append(it)
            it = it.next()
        for it in delete_list:
            blk.erase(it)
        self.lock.release()
        count = len(delete_list)
        return count


def main():
    rtn = vtil.routine()
    block, inserted = rtn.create_block(0x1000)
    t0, t1 = block.tmp(1, 64)

    block.mov(t0, vtil.REG_FLAGS.select(1, 11))
    block.nop()
    block.mov(t1, 12345)
    block.nop()
    block.js(t0, 0x2000, 0x3000)

    block2 = block.fork(0x2000)
    block2.nop()
    block2.mov(vtil.x86_reg.RAX, 0)
    block2.nop()
    block2.vexit(0)

    block3 = block.fork(0x3000)
    block3.nop()
    block3.mov(vtil.x86_reg.RAX, 1)
    block3.nop()
    block3.vexit(0)

    print('Before:')
    vtil.debug.dump(rtn)
    simple = simple_nop_remove_pass()
    simple(rtn)
    print('After:')
    vtil.debug.dump(rtn)


if __name__ == "__main__":
    main()
