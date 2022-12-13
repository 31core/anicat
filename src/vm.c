#include <stdlib.h>
#include <string.h>
#include <src/vm.h>

static size_t get_size_by_type(uint8_t type)
{
	if(type == VM_TYPE_VAL8 || type == VM_TYPE_MEM8)
	{
		return 1;
	}
	else if(type == VM_TYPE_VAL16 || type == VM_TYPE_MEM16)
	{
		return 2;
	}
	else if(type == VM_TYPE_VAL32 || type == VM_TYPE_MEM32)
	{
		return 4;
	}
	else if(type == VM_TYPE_VAL64 || type == VM_TYPE_MEM64)
	{
		return 8;
	}
	/* register */
	else
	{
		return 1;
	}
}

void vm_init(VM *vm)
{
	vm->code = 0;
	vm->c0 = 0;
	vm->stack = malloc(1024 * 1024);
}

void vm_insert_code(VM *vm, const void *code, size_t size)
{
	if(vm->code != NULL)
	{
		free(vm->code);
	}
	vm->code = malloc(size);
	memcpy(vm->code, code, size);
	vm->ip = START_ADDR;
}

void vm_memcpy(VM *vm, off_t addr, const void *data, size_t size)
{

}

#define GET_CODE (vm->code[vm->ip - START_ADDR])
void vm_run_instruction(VM *vm)
{
	for(;;)
	{
		uint8_t instr = GET_CODE;
		vm->ip++;
		if(instr == VM_INS_MOV)
		{
			uint8_t type = GET_CODE;
			vm->ip++;
			uint64_t tmp_data;
			if(type == VM_REG_C0)
			{
				tmp_data = vm->c0;
			}
			else
			{
				size_t size = get_size_by_type(type);
				memcpy(&tmp_data, &GET_CODE, size);
				vm->ip += size;
			}

			type = GET_CODE;
			vm->ip++;
			if(type == VM_REG_C0)
			{
				vm->c0 = tmp_data;
			}
			else if(type >= VM_TYPE_MEM8 && type <= VM_TYPE_MEM64)
			{
				size_t size = get_size_by_type(type);
				off_t addr;
				memcpy(&addr, &GET_CODE, 8);
				vm->ip += 8;
				vm_memcpy(vm, GET_CODE , &tmp_data, size);
			}
		}
		else if(instr == VM_INS_HAL)
		{
			return;
		}
	}
}
