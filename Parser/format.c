#include <regex.h>
#include <string.h>

#define LOOP_TIME 10

/* 正则表达式替换字符串 */
static int regex_sub(char *ret, const char *str)
{
	strcpy(ret, str);
	regex_t reg;
	regmatch_t pm;
	regcomp(&reg, "\\s+", REG_EXTENDED);
	/* 没有匹配到 */
	if(regexec(&reg, str, 1, &pm, 0) != 0)
	{
		regfree(&reg);
		return -1;
	}
	strcpy(&ret[pm.rm_so], &str[pm.rm_eo]);
	regfree(&reg);
	return 0;
}

/* 格式化代码 */
void format_code(char *ret, const char *code)
{
	char tmp_code[100];
	strcpy(tmp_code, code);
	for(int i = 0; i < LOOP_TIME; i++)
	{
		if(regex_sub(ret, tmp_code) == -1)
		{
			break;
		}
		strcpy(tmp_code, ret);
	}
}