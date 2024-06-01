#include <stdio.h>

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
} Operation;

typedef struct {
	char operator;
	int operand;
} Instruction;

typedef struct {
	int mem[MEMORY_CAPACITY];
	int stack[STACK_CAPACITY];
} Machine;

Machine g_machine = { 0 };

void lex_file(FILE* f, Instruction* out_program) {
	char c;
	size_t pc = 0; // stands for Personal Computer, of course
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
			g_machine.stack[sp] = pc;
			pc++;
			break;
		case ']':
			out_program[pc].operator = JMPR; 
			jmp_ip = g_machine.stack[sp];
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

void interpret_program(Instruction* program) {
	size_t ip = 0; // stands for Internet Protocol, of course
	size_t sp = 0;
	size_t ptr = 0;

	while (program[ip].operator != END) {
		char op = program[ip].operator;
		switch (op)
		{
		case PLUS:
			g_machine.mem[ptr]++;
			break;
		case MINUS:
			g_machine.mem[ptr]--;
			break;
		case PRINT:
			putchar(g_machine.mem[ptr]);
			break;
		case MOVEL:
			ptr--;
			break;
		case MOVER:
			ptr++;
			break;
		case JMPL:
			if (!g_machine.mem[ptr])
				ip = program[ip].operand;
			break;
		case JMPR:
			if (g_machine.mem[ptr])
				ip = program[ip].operand;
			break;
		case INPUT:
			printf("Program asked for input: ");
			float input = 0.0f;
			if (scanf("%f", &input) != 1) {
				printf("Input was not in a valid format. (floats only)\n");
				exit(2);
			}
			g_machine.mem[ptr] = input;
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
	Instruction program[PROGRAM_CAPACITY] = {0};
	FILE* f = fopen("hw.bf", "r");
	if (f == NULL) {
		printf("Can't open file\n");
		return 1;
	}
	lex_file(f, &program);
	interpret_program(&program);

	return 0;
}
