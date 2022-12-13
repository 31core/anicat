#include <stdint.h>

#define VM_INS_MOV 0x01
#define VM_REG_R0 0x20
#define VM_REG_R1 0x21
#define VM_REG_R2 0x22
#define VM_REG_R3 0x23
#define VM_REG_IP 0x24
#define VM_TYPE_REG 0x25
#define VM_TYPE_VAL 0x26

struct vm
{
	void *stack;
	void **heap;
};

struct instruction
{
	uint8_t type;
};

typedef struct vm VM;
