#include <stdio.h>

// I have no idea what's going on... but I did it!

#define PROGRAM_CAPACITY 512
#define MEMORY_CAPACITY 512
#define STACK_CAPACITY 512

typedef enum {
	PLUS,
	MINUS,
	PRINT,
	MOVEL,
	MOVER,
	INPUT,
	JMPL,
	JMPR,
	END,
} OP;

typedef struct {
	char operator;
	int operand;
} INS;

int mem[MEMORY_CAPACITY] = { 0 };
int stack[STACK_CAPACITY] = { 0 };

void lex_file(FILE* f, INS* out_program) {
	char c;
	size_t pc = 0; // Personal Computer
	size_t sp = 0;
	size_t jmp_ip = 0;

	// NOTE: This stuff should probably check if it didn't overflow the program...
	while ((c = getc(f)) != EOF) {
		switch (c)
		{
		case '+': out_program[pc++].operator = PLUS; break;
		case '-': out_program[pc++].operator = MINUS; break;
		case '.': out_program[pc++].operator = PRINT; break;
		case '>': out_program[pc++].operator = MOVER; break;
		case '<': out_program[pc++].operator = MOVEL; break;
		case ',': out_program[pc++].operator = INPUT; break;
		case '[': 
			out_program[pc].operator = JMPL;
			stack[sp] = pc;
			pc++;
			break;
		case ']':
			out_program[pc].operator = JMPR; 
			jmp_ip = stack[sp];
			out_program[pc].operand = jmp_ip;
			out_program[jmp_ip].operand = pc;
			pc++;
			break;
		default:
			break;
		}
	}
	out_program[pc].operator = END;
}

void interpret_program(INS* program) {
	size_t ip = 0; // Internet Protocol
	size_t sp = 0;
	size_t ptr = 0;

	// TODO: Implement the `INPUT` instruction
	while (program[ip].operator != END) {
		char op = program[ip].operator;
		switch (op)
		{
		case PLUS:
			mem[ptr]++;
			break;
		case MINUS:
			mem[ptr]--;
			break;
		case PRINT:
			putchar(mem[ptr]);
			break;
		case MOVEL:
			ptr--;
			break;
		case MOVER:
			ptr++;
			break;
		case JMPL:
			if (!mem[ptr])
				ip = program[ip].operand;
			break;
		case JMPR:
			if (mem[ptr])
				ip = program[ip].operand;
			break;
		default:
			printf("Unexpected operation\n");
			exit(1);
			break;
		}
		ip++;
	}
}

int main() {
	INS program[PROGRAM_CAPACITY] = {0};
	FILE* f = fopen("hw.bf", "r");
	if (f == NULL) {
		printf("Can't open file\n");
		return;
	}
	lex_file(f, &program);
	interpret_program(&program);
}
