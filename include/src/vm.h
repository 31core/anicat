#include <stdint.h>

#define START_ADDR 0x10000

#define VM_INS_MOV	0x01
#define VM_INS_IN	0x02
#define VM_INS_OUT	0x03
#define VM_INS_JMP	0x04
#define VM_INS_CMP	0x05
#define VM_INS_HAL	0x20

#define VM_REG_C0	0x20
#define VM_REG_IP	0x24
#define VM_TYPE_REG	0x25
#define VM_TYPE_VAL8	0x26
#define VM_TYPE_VAL16	0x27
#define VM_TYPE_VAL32	0x28
#define VM_TYPE_VAL64	0x29
#define VM_TYPE_MEM8	0x30
#define VM_TYPE_MEM16	0x3a
#define VM_TYPE_MEM32	0x3f
#define VM_TYPE_MEM64	0x3c

struct vm
{
	uint64_t c0, ip;
	uint8_t *stack;
	uint8_t **heap, *code;
};

typedef struct vm VM;

void vm_init(VM *);
void vm_run_instruction(VM *);
void vm_insert_code(VM *, const void *, size_t);
