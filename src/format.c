#include <regex.h>
#include <string.h>

#define LOOP_TIME 50

/* 正则表达式替换字符串 */
static int regex_sub(char *ret, const char *str, const char *pattern, const char *target)
{
	strcpy(ret, str);
	regex_t reg;
	regmatch_t pm;
	regcomp(&reg, pattern, REG_EXTENDED);
	/* 没有匹配到 */
	if(regexec(&reg, str, 1, &pm, 0) != 0)
	{
		regfree(&reg);
		return -1;
	}
	strcpy(&ret[pm.rm_so], target);
	strcpy(&ret[pm.rm_so + strlen(target)], &str[pm.rm_eo]);
	regfree(&reg);
	return 0;
}

/* 格式化代码 */
void format_code(char *ret, const char *code)
{
	/* 正则表达式匹配字符 */
	char *pattern[] = {"\\s*=\\s*", "\\s*\\+\\s*", "\\s*-\\s*", "\\s*\\*\\s*", "\\s*/\\s*",\
	"\\s*==\\s*", "\\s*!=\\s*", "\\s*<=\\s*", "\\s*>=\\s*", "\\s*:\\s*", "\\s*->\\s*",\
	"\\s*\\(\\s*", "\\s*\\)\\s*", "\\s*,\\s*", "\\s+"};
	/* 替换成的字符 */
	char *target[] = {"=", "+", "-", "*", "/", "==", "!=", "<=", ">=", ":", "->", "(", ")",
	",", " "};
	char tmp_code[100];
	strcpy(tmp_code, code);
	for(int index = 0; index < 15; index++)
	{
		for(int i = 0; i < LOOP_TIME; i++)
		{
			if(regex_sub(ret, tmp_code, pattern[index], target[index]) == -1)
			{
				break;
			}
			strcpy(tmp_code, ret);
		}
	}
}