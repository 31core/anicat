## Virtual Machine
### Instructions Format
`[operation] [value type] [value/register] ...`

All of these are defined in `include/src/vm.h`

For example:
* operation: `VM_INS_`
* register: `VM_REG_`
* value type" `VM_TYPE_`
#### instructions list:
Accepted value types:
|Type|Description|
|----|-----------|
|r   |register   |
|v   |value      |
|m   |memory     |

|Operation|Value 1       |Value 2     |Description|
|---------|--------------|------------|-----------|
|mov      |source[r,v,m] |target[r,m] |move value |
|hal      |-             |-           |halt       |
|jmp      |address[v]    |-           |jump to    |
|add      |source[r,m]   |value[r,v,m]|add        |
|sub      |source[r,m]   |value[r,v,m]|subtract   |
|mul      |source[r,m]   |value[r,v,m]|multiply   |
|div      |source[r,m]   |value[r,v,m]|divide     |
