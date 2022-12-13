#include <stdlib.h>
#include <src/vm.h>

void vm_init(VM *vm)
{
	vm->stack = malloc(1024 * 1024);
}
