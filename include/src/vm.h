#define INS_MOV 0x01
#define INS_REG_R0 0x20
#define INS_REG_R1 0x21
#define INS_REG_R2 0x22
#define INS_REG_R3 0x23
#define INS_REG_IP 0x24
#define INS_REG 0x25
#define INS_REG_VAL 0x26

struct vm
{
	void *stack;
	void **heap;
};

typedef struct vm VM;
