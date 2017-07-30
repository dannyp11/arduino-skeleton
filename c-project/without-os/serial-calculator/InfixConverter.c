/*
 * InfixConverter.c
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#include "InfixConverter.h"
#include "AVRString.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static const char g_infix_supported_ops[] = "+-*/()";

/**
 * Compare precedence between a and b
 *
 * + - < / *
 * + = -, * = /
 *
 * @param a
 * @param b
 * @return	0 on same
 * 			1 if a >b
 * 			-1 if a < b
 * 			-2 if error
 */
static int8_t comparePriority(char a, char b)
{
	int8_t retVal = -2;

	uint8_t a_pm = (a == '+') || (a == '-');
	uint8_t b_pm = (b == '+') || (b == '-');
	uint8_t a_md = (a == '*') || (a == '/');
	uint8_t b_md = (b == '*') || (b == '/');

	if (a_pm && b_pm)
	{
		retVal = 0;
	}
	else if (a_pm && b_md)
	{
		retVal = -1;
	}
	else if (a_md && b_pm)
	{
		retVal = 1;
	}

	return retVal;
}

/**
 * append " %c" to str
 *
 * @param str
 * @param c
 */
static void stringPushBackChar(char * str, char c)
{
	sprintf(str, "%s %c", str, c);
}

/**
 * appen " %u" to str
 *
 * @param str
 * @param num
 */
static void stringPushBackInt(char * str, long num)
{
	sprintf(str, "%s %ld", str, num);
}

/*
 * Make sure buffer is large enough
 *
 * return 2 on unsupported char
 * 			0 on success
 */
uint8_t addSpaces(char * buffer)
{
	// count special chars
	uint8_t i;
	char buffer2[STRING_MAXLEN];
	uint8_t index2 = 0;
	for (i = 0; i < strlen(buffer); ++i)
	{
		if (strchr(g_infix_supported_ops, buffer[i]))
		{
			buffer2[index2++] = ' ';
			buffer2[index2++] = buffer[i];
			buffer2[index2++] = ' ';
		}
		else
		{
			buffer2[index2++] = buffer[i];
		}
	}
	buffer2[index2] = '\0';
	char * buffer3 = buffer2;
	AVRStrinStripExtraSpace(buffer3);
	AVRStringTrimWhiteSpace(buffer3);

	strcpy(buffer, buffer3);

	return 0;
}

/**
 * get top of op_stack
 *
 * @param stack - op stack
 * @return
 */
static char getStackTop(const char * stack)
{
	if (strlen(stack))
	{
		return stack[strlen(stack) - 1];
	}

	return '\0';
}

/**
 * pop the top of op_stack
 *
 * @param stack
 */
static char popStack(char * stack)
{
	char retVal = getStackTop(stack);

	if (strlen(stack))
	{
		stack[strlen(stack) - 1] = '\0';
	}

	return retVal;
}

/**
 * push c to top of op stack
 *
 * @param stack
 * @param c
 */
void pushStack(char * stack, char c)
{
	stack[strlen(stack) + 1] = '\0';
	stack[strlen(stack)] = c;
}

/*
 * Algorithm:
 *
 * 1.	Print operands as they arrive.
 * 2.	If the stack is empty or contains a left parenthesis on top, push the incoming operator onto the stack.
 * 3.	If the incoming symbol is a left parenthesis, push it on the stack.
 * 4.	If the incoming symbol is a right parenthesis, pop the stack and print the operators until you see a left parenthesis. Discard the pair of parentheses.
 * 5.	If the incoming symbol has higher precedence than the top of the stack, push it on the stack.
 * 6.	If the incoming symbol has equal or lower precedence than the symbol on the top of the stack, pop the stack and print the top operator. Then test the incoming operator against the new top of stack.
 * 7.	At the end of the expression, pop and print all operators on the stack. (No parentheses should remain.)
 */
uint8_t InfixConverterConvertString(const char * input, char * output)
{
	uint8_t retVal = 0;

	// refine input & cpy to output
	output[0] = '\0';
	char output_tmp[STRING_MAXLEN];
	strcpy(output_tmp, input);
	AVRStringRemoveChar(output_tmp, ' '); // rm all spaces

	// test for unsupported char
	unsigned i;
	for (i = 0; i < strlen(output_tmp); ++i)
	{
		if (!strchr(g_infix_supported_ops, output_tmp[i])
				&& !isdigit(output_tmp[i]))
		{
			retVal = 2;
		}
	}
	if (retVal)
		return retVal;

	retVal = addSpaces(output_tmp);
	if (retVal)
		return retVal;

	// parse each token, can be "123" or "+"
	char op_stack[20] =
	{ '\0' };
	char * token = strtok(output_tmp, " ");

	while (token)
	{
		char incomingSymbol = token[0];

		// 1.	Print operands as they arrive.
		long num;
		if (sscanf(token, "%ld", &num) == 1)
		{
			stringPushBackInt(output, num);
		}
		// error checking, if token isn't number, it must exist in g_infix_supported_ops
		else if (!strchr(g_infix_supported_ops, incomingSymbol))
		{
			retVal = 2;
		}

		// 2. If the stack is empty or contains a left parenthesis on top, push the incoming operator onto the stack.
		else if (!getStackTop(op_stack) || getStackTop(op_stack) == '(')
		{
			pushStack(op_stack, incomingSymbol);
		}

		// 3. If the incoming symbol is a left parenthesis, push it on the stack.
		else if (incomingSymbol == '(')
		{
			pushStack(op_stack, incomingSymbol);
		}

		// 4. If the incoming symbol is a right parenthesis, pop the stack and print the operators
		// 		until you see a left parenthesis. Discard the pair of parentheses.
		else if (incomingSymbol == ')')
		{
			// ensure ( exists in stack
			if (strchr(op_stack, '('))
			{
				while (getStackTop(op_stack) != '(')
				{
					stringPushBackChar(output, popStack(op_stack));
				}

				// discard '('
				popStack(op_stack);
			}
			else
			{
				retVal = 1;
			}
		}

		// 5. If the incoming symbol has higher precedence than the top of the stack, push it on the stack.
		else if (comparePriority(incomingSymbol, getStackTop(op_stack)) == 1)
		{
			pushStack(op_stack, incomingSymbol);
		}

		// 6. If the incoming symbol has equal or lower precedence than the symbol on the top of the stack,
		// pop the stack and print the top operator. Then test the incoming operator against the new top of stack.
		else if (comparePriority(incomingSymbol, getStackTop(op_stack)) <=0)
		{
			stringPushBackChar(output, popStack(op_stack));

			while ( strlen(op_stack) && comparePriority(incomingSymbol, getStackTop(op_stack)) <= 0)
			{
				stringPushBackChar(output, popStack(op_stack));
			}

			pushStack(op_stack, incomingSymbol);
		}

		else
		{
			retVal = 1;
		}

		if (retVal)
			break;

		token = strtok(NULL, " ");
	}

	// push the rest of op stack
	if (strlen(op_stack))
	{
		if (!strchr(op_stack, '(')) // check if op stack still contains '('
		{
			while (getStackTop(op_stack))
			{
				stringPushBackChar(output, popStack(op_stack));
			}
		}
		else
		{
			retVal = 1;
		}
	}

	if (retVal)
	{
		output[0] = '\0';
	}
	else
	{
		strcpy(output_tmp, AVRStringTrimWhiteSpace(output));
		strcpy(output, output_tmp);
	}

	return retVal;
}

#ifdef __cplusplus
}
#endif
