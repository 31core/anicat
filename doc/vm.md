## Virtual Machine
### Instructions Format
`[operation] [value type] [value/register] ...`

All of these are defined in `include/src/vm.h`

For example:
* operation: `VM_INS_`
* register: `VM_REG_`
* value type" `VM_TYPE_`
#### instructions list:
|operation|value 1|value 2|description|
|---------|-------|-------|-----------|
|move     |source |target |move value |
|halt     |-      |-      |halt       |
