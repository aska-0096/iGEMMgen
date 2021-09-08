from abc import ABC, abstractmethod
from python.codegen.gpu_instruct import SMEM_instr_caller
from python.codegen.gpu_reg_block import reg_block
from python.codegen.kernel_driver import base_config
from ..codegen.config_parser import config_content_t
from ..codegen.mc import mc_base_t, mc_asm_printer_t
from .kernel_constructor import *


class direct_navi_config(base_config):
    def __init__(self, config_content: config_content_t):
        super().__init__(config_content, '-navi')
        
        self.read_size = config_content.get_section('direct-navi')[0]['read_size']

class conv_direct_navi(kernel_constructor):

    def get_kernel_name(self):
        return 'conv_direct_navi'

    class kernel_karg_t(karg_file_t):
        '''Define kernel arguments. Used in _set_kernel_karg_t'''
        def __init__(self, k_ptr:kernel_constructor, mc) -> None:
            super().__init__(mc)
            pb_arg = k_ptr._pb_kernel_arg
            self.in_ptr  = pb_arg('in_ptr',  arg_kind.GlobBuffer, arg_type.F32)
            self.out_ptr = pb_arg('out_ptr', arg_kind.GlobBuffer, arg_type.F32)
            self.wei_ptr = pb_arg('wei_ptr', arg_kind.GlobBuffer, arg_type.F32)
            self.N = pb_arg('N', arg_kind.value, arg_type.I32)
            self.C = pb_arg('C', arg_kind.value, arg_type.I32)
            self.H = pb_arg('H', arg_kind.value, arg_type.I32)
            self.W = pb_arg('W', arg_kind.value, arg_type.I32)
            self.K = pb_arg('K', arg_kind.value, arg_type.I32)
            self.X = pb_arg('Y', arg_kind.value, arg_type.I32)
            self.Y = pb_arg('X', arg_kind.value, arg_type.I32)
            self.G = pb_arg('G', arg_kind.value, arg_type.I32)

    class _sgpr(sgpr_file_t):
        def __init__(self, mc):
            super().__init__(mc)
            add = self.add 
            self.karg_ptr = add('karg_ptr', 2)
            self.group_id = add('group_id', 1)
            self.gid_off = add('gid_off', 1)
            self.in_buff_ptr = add('in_buff_ptr', 4)
            self.out_buff_ptr = add('out_buff_ptr', 4)
            self.wei_buff_ptr = add('wei_buff_ptr', 4)
            self.N = add('N', 1)
            self.C = add('C', 1)
            self.H = add('H', 1)
            self.W = add('W', 1)
            self.K = add('K', 1)
            self.X = add('X', 1)
            self.Y = add('Y', 1)
            self.G = add('G', 1)
    
    class _vgpr(vgpr_file_t):
        def __init__(self, mc):
            super().__init__(mc)
            sseq = self._sq
            add = self.add 
            self.tid = add('tid', sseq(1))
            self.in_off = add('in_off', sseq(1))
            self.in_off = add('in_off', sseq(1))

    def _get_LDS_usage(self):
        return 0

    def __init__(self, mc_asm_printer: mc_asm_printer_t, **kwargs):
        super().__init__(mc_asm_printer, **kwargs)
                
        
    def _emit_kernel_body(self):
        s = self.sgpr
        v = self.vgpr
        karg = self.kargs
        ic = self.instr_ctrl.instructions_caller
        
        cl = self._emit

        cl(f"s_load_dwordx2  {s.in_buff_ptr[0, 1]},   {s.karg_ptr[0, 1]},    0+{karg.in_ptr()}")
        cl(f"s_load_dwordx2  {s.out_buff_ptr[0, 1]},   {s.karg_ptr[0, 1]},    0+{karg.out_ptr()}")
        cl(f"s_load_dwordx2  {s.wei_buff_ptr[0, 1]},   {s.karg_ptr[0, 1]},    0+{karg.wei_ptr()}")

        cl(f"s_load_dwordx4  {s.H[0, 3]},   {s.karg_ptr[0, 1]},    0+{karg.H()}")
        cl(f"s_load_dwordx2  {s.Y[0, 1]},   {s.karg_ptr[0, 1]},    0+{karg.Y()}")

        #def fill_buff_desc(desc_reg, size, cl):
        #    cl(f"s_mov_b32 {desc_reg[2]}, {size}")
        #    cl(f"s_mov_b32 {desc_reg[3]}, {0x00027000}")
        
        input_buffer_size = filter_buffer_size = output_buffer_size = 100
        
        #fill_buff_desc()

        cl(f"s_mov_b32  {s.in_buff_ptr[2]}, {input_buffer_size}")
        cl(f"s_mov_b32 {s.in_buff_ptr[3]}, {0x00027000}")

        cl(f"s_mov_b32 {s.out_buff_ptr[2]}, {filter_buffer_size}")
        cl(f"s_mov_b32 {s.out_buff_ptr[3]}, {0x00027000}")

        cl(f"s_mov_b32 {s.wei_buff_ptr[2]}, {output_buffer_size}")
        cl(f"s_mov_b32 {s.wei_buff_ptr[3]}, {0x00027000}")

        input_buffer_size = filter_buffer_size = output_buffer_size = 101

        ic.s_mov_b32(s.in_buff_ptr[2], input_buffer_size)
        ic.s_mov_b32(s.in_buff_ptr[3], '0x00027000')

        ic.s_mov_b32(s.out_buff_ptr[2], output_buffer_size)
        ic.s_mov_b32(s.out_buff_ptr[3], 0x00027000)

        ic.s_mov_b32(s.wei_buff_ptr[2], filter_buffer_size)
        ic.s_mov_b32(s.wei_buff_ptr[3], 0x00027000)

        self.instr_ctrl._emmit_all(self._emit)

        cl(f"s_load_dwordx2  {s.in_buff_ptr[0:1]},   {s.karg_ptr[0:1]},    0+{karg.in_ptr()}")
        cl(f"s_load_dwordx2  {s.out_buff_ptr[0:1]},   {s.karg_ptr[0:1]},    0+{karg.out_ptr()}")
        cl(f"s_load_dwordx2  {s.wei_buff_ptr[0:1]},   {s.karg_ptr[0:1]},    0+{karg.wei_ptr()}")

        cl(f"s_load_dwordx4  {s.H[0:3]},   {s.karg_ptr[0:1]},    0+{karg.H()}")
        cl(f"s_load_dwordx2  {s.Y[0:1]},   {s.karg_ptr[0:1]},    0+{karg.Y()}")

        def fill_buff_desc(desc_reg:reg_block, size:int, cl):
            cl(f"s_mov_b32 {desc_reg[2]}, {size}")
            cl(f"s_mov_b32 {desc_reg[3]}, {0x00027000}")
        
        input_buffer_size = 25
        filter_buffer_size = 50
        output_buffer_size = 100
        
        fill_buff_desc(s.in_buff_ptr, input_buffer_size, cl)
        fill_buff_desc(s.out_buff_ptr, filter_buffer_size, cl)
        fill_buff_desc(s.wei_buff_ptr, output_buffer_size, cl)

        cl(f"s_mov_b32 {s.in_buff_ptr[2:2]}, {input_buffer_size}")
        cl(f"s_mov_b32 {s.in_buff_ptr[3:3]}, {0x00027000}")

        cl(f"s_mov_b32 {s.out_buff_ptr[2:2]}, {filter_buffer_size}")
        cl(f"s_mov_b32 {s.out_buff_ptr[3:3]}, {0x00027000}")

        cl(f"s_mov_b32 {s.wei_buff_ptr[2:2]}, {output_buffer_size}")
        cl(f"s_mov_b32 {s.wei_buff_ptr[3:3]}, {0x00027000}")

        H1O = s.add('H1O', 1, 4)
        H2O = s.add('H2O', 4, 4)

        H4O = s.add_no_pos('H30',4)
        H31O = s.add_no_pos('H310',1)

        block = s.add_block('block', [H4O, H31O])

        fill_buff_desc(H2O, 15, cl)


    def _set_kernel_karg_t(self) -> None:
        '''Should be called before get_amdgpu_metadata_list in kernel_constructor.__init__.
        Defined in kernel class to make overwrited kernel_karg_t trackable by IDE.'''
        self.kargs=self.kernel_karg_t(self, self.mc)
    
    def _gpr_init(self, mc :mc_asm_printer_t) -> None:
        '''Should be called before get_amdgpu_metadata_list in kernel_constructor.__init__.
        Defined in kernel class to make overwrited sgpr and vgpr trackable by IDE.'''
        
        self.sgpr = self._sgpr(mc)
        self.vgpr = self._vgpr(mc)
        self.agpr = self._agpr(mc)
