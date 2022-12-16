#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <src/vm.h>

#define GET_CODE(vm) (vm->code[vm->ip - START_ADDR])

struct arg
{
	uint8_t type;
	uint64_t value;
	size_t size;
};


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
		return 0;
	}
}

static struct arg get_arg(VM *vm)
{
	struct arg arg;
	arg.type = GET_CODE(vm);
	vm->ip++;

	arg.size = get_size_by_type(arg.type);

	if(arg.type >= VM_TYPE_MEM8 && arg.type <= VM_TYPE_MEM64)
	{
		memcpy(&arg.value, &GET_CODE(vm), 8);
		vm->ip += 8;
	}
	else
	{
		memcpy(&arg.value, &GET_CODE(vm), arg.size);
		vm->ip += arg.size;
	}

	return arg;
}

static uint64_t get_value(const VM *vm, const struct arg *arg)
{
	uint64_t value;
	if(arg->type >= VM_TYPE_VAL8 && arg->type <= VM_TYPE_VAL64)
	{
		value = arg->value;
	}
	else if(arg->type >= VM_TYPE_MEM8 && arg->type <= VM_TYPE_MEM64)
	{
		//vm_memcpy(vm, source.value, &tmp_data, source.size);
	}
	else if(arg->type == VM_REG_C0)
	{
		value = vm->c0;
	}
	return value;
}

static void set_value(VM *vm, const struct arg *arg, uint64_t value)
{
	if(arg->type >= VM_TYPE_MEM8 && arg->type <= VM_TYPE_MEM64)
	{
		//vm_memcpy(vm, GET_CODE , &tmp_data, target.size);
	}
	else if(arg->type == VM_REG_C0)
	{
		vm->c0 = value;
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

void vm_run_instruction(VM *vm)
{
	for(;;)
	{
		uint8_t instr = GET_CODE(vm);
		vm->ip++;
		if(instr == VM_INS_MOV)
		{
			struct arg source = get_arg(vm);
			struct arg target = get_arg(vm);

			uint64_t tmp_data = get_value(vm, &source);
			set_value(vm, &target, tmp_data);
		}
		else if(instr == VM_INS_JMP)
		{
			struct arg addr = get_arg(vm);
			vm->ip = addr.value;
		}
		else if(instr >= VM_INS_ADD && instr <= VM_INS_DIV)
		{
			struct arg source = get_arg(vm);
			struct arg value = get_arg(vm);

			uint64_t tmp_data = get_value(vm, &source);
			uint64_t v = get_value(vm, &value);
			if(instr == VM_INS_ADD)
			{
				tmp_data += v;
			}
			else if(instr == VM_INS_SUB)
			{
				tmp_data -= v;
			}
			else if(instr == VM_INS_MUL)
			{
				tmp_data *= v;
			}
			else
			{
				tmp_data /= v;
			}
			set_value(vm, &source, tmp_data);
		}
		else if(instr == VM_INS_HAL)
		{
			return;
		}
		else
		{
			printf("Illegal instruction: 0x%x at 0x%lx\n", instr, vm->ip);
			return;
		}
	}
}
