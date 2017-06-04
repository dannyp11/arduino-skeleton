/*
 * InfixCalculator.c
 *
 *  Created on: Jun 3, 2017
 *      Author: dat
 */

#include "PostfixCalculator.h"
#include "AVRString.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char g_postfixcalc_supported_ops[] = "+-*/()";

// get length of stack
static uint8_t stackLen(const STACK_TYPE * stack)
{
	uint8_t retVal = 0;
	while (stack[retVal] != STACK_I32_STOPENTRY)
	{
		++retVal;
	}

	return retVal;
}

static void pushStack(STACK_TYPE * stack, STACK_TYPE num)
{
	stack[stackLen(stack) + 1] = STACK_I32_STOPENTRY;
	stack[stackLen(stack)] = num;
}

static STACK_TYPE peekStack(STACK_TYPE * stack)
{
	if (stackLen(stack))
	{
		return stack[stackLen(stack) - 1];
	}

	return STACK_I32_STOPENTRY;
}

static STACK_TYPE popStack(STACK_TYPE * stack)
{
	STACK_TYPE retVal = peekStack(stack);

	if (stackLen(stack))
	{
		stack[stackLen(stack) - 1] = STACK_I32_STOPENTRY;
	}

	return retVal;
}

// valid only when input only contains digit, g_postfixcalc_supported_ops, or space
uint8_t isValidPostfixInput(const char * input)
{
	uint8_t retVal = 1;
	uint8_t i;

	for (i = 0; i < strlen(input); ++i)
	{
		uint8_t isValidChar = (isdigit(input[i]) != 0);
		isValidChar |= (strchr(g_postfixcalc_supported_ops, input[i]) != NULL);
		isValidChar |= (isspace(input[i]) != 0);

		retVal &= isValidChar;
	}

	return retVal;
}

// algo followed: http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=9EC30C66159F8F0C4A7986DB9750D6A5?doi=10.1.1.161.38&rep=rep1&type=pdf
STACK_TYPE PostfixCalculatorCompute(const char * input)
{
	STACK_TYPE retVal = (isValidPostfixInput(input)) ? 0 : STACK_I32_STOPENTRY;
	if (retVal)
		return retVal;

	STACK_TYPE stack[50] = { STACK_I32_STOPENTRY }; // stack len determined by getting earliest INT32_MIN

	char buffer[STRING_MAXLEN];
	strcpy(buffer, input);

	char * token = strtok(buffer, " ");
	while (token)
	{
		long num;
		char op = token[0];

		if (sscanf(token, "%ld", &num) == 1)
		{
			// it's digit
			pushStack(stack, (STACK_TYPE) num);
		}
		else if (strchr(g_postfixcalc_supported_ops, op))
		{
			STACK_TYPE operand2 = popStack(stack);
			STACK_TYPE operand1 = popStack(stack);

			if (operand1 == STACK_I32_STOPENTRY
					|| operand2 == STACK_I32_STOPENTRY)
			{
				retVal = STACK_I32_STOPENTRY;
				break;
			}

			// main calculation part
			if (op == '+')
			{
				pushStack(stack, operand1 + operand2);
			}
			else if (op == '-')
			{
				pushStack(stack, operand1 - operand2);
			}
			else if (op == '*')
			{
				pushStack(stack, operand1 * operand2);
			}
			else if (op == '/')
			{
				if (operand2 != 0)
				{
					pushStack(stack, operand1 / operand2);
				}
				else
				{
					retVal = STACK_I32_STOPENTRY;
				}
			}
			else
			{
				retVal = STACK_I32_STOPENTRY;
				break;
			}
		}

		token = strtok(NULL, " ");
	}

	if (retVal)
		return retVal;

	retVal = popStack(stack);
	if (stackLen(stack))
	{
		retVal = STACK_I32_STOPENTRY;
	}

	return retVal;
}
