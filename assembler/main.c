#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum Inst {
	// just an error
	INST_ERROR,
	
	INST_LD,
	
	INST_LDI,
	INST_LDD,
	INST_INC,
	INST_DEC,
	INST_ADD,
	INST_SUB,
	INST_POP,
	INST_RET,
	
	INST_PUSH,
	INST_SWAP,
	INST_STOP,
	INST_HALT,
	
	// not a real instruction
	INST_LABEL,
	
	INST_MAX_INST
} Inst;

enum {
	// 8bit registers
	REG_A,
	REG_F,
	REG_B,
	REG_C,
	REG_D,
	REG_E,
	REG_H,
	REG_L,
	
	// 16bit registers
	REG_AF,
	REG_BC,
	REG_DE,
	REG_HL,
	
	// special 16bit registers
	REG_SP,
	REG_PC
};

static void assemble(const char* input);

static const char* parse_mnemonic(const char* input, Inst* out_mnemonic);
static const char* skip_whitespace(const char* input);
static bool is_ident(char ch);

static char* emitter;

int main(int argc, char* argv[]) {
	char* out = emitter = malloc(1 << 16);
	
	assemble("L0: ldi a,a");
	
	// TODO(NeGate): Implement file output
	free(out);
	return 0;
}

void assemble(const char* input) {
	while (*input) {
		const char* start = input;
		
		Inst mnemonic;
		input = parse_mnemonic(input, &mnemonic);
		
		if (mnemonic == INST_LABEL) {
			printf("DEFINE LABEL: '%.*s'\n", (int)(input - start), start);
			
			input = skip_whitespace(input + 1); // skip colon and whitespace
		} else if (mnemonic == INST_ERROR) {
			printf("error: unknown mnemonic '%.*s'\n", (int)(input - start), start);
			break;
		} else {
			printf("'%.*s' : %d\n", (int)(input - start), start, mnemonic);
		}
	}
}

static const char* parse_mnemonic(const char* input, Inst* out_mnemonic) {
	const char* end = input;
	while (*end && is_ident(*end)) end++;
	size_t length = end - input;
	
	// it's a label
	if (*end == ':') {
		*out_mnemonic = INST_LABEL;
		return end;
	}
	
	Inst mnemonic = INST_ERROR;
	
	// NOTE(NeGate): We can get away with 4-byte load on 3-byte compare
	// because there's always at least a null terminator afterwards.
#define STR_MATCH2(a, b) (*((uint16_t*) a) == __builtin_bswap16(b))
#define STR_MATCH3(a, b) ((*((uint32_t*) a) & 0xFFFFFF) == (__builtin_bswap32(b) >> 8))
#define STR_MATCH4(a, b) (*((uint32_t*) a) == __builtin_bswap32(b))
	switch (length) {
		case 2: {
			if      (STR_MATCH2(input, 'ld')) mnemonic = INST_LD;
			break;
		}
		break;
		case 3: {
			if      (STR_MATCH3(input, 'ldi')) mnemonic = INST_LDI;
			else if (STR_MATCH3(input, 'ldd')) mnemonic = INST_LDD;
			else if (STR_MATCH3(input, 'add')) mnemonic = INST_ADD;
			else if (STR_MATCH3(input, 'sub')) mnemonic = INST_SUB;
			else if (STR_MATCH3(input, 'pop')) mnemonic = INST_POP;
			else if (STR_MATCH3(input, 'ret')) mnemonic = INST_RET;
			break;
		}
		case 4: {
			if      (STR_MATCH4(input, 'stop')) mnemonic = INST_STOP;
			else if (STR_MATCH4(input, 'halt')) mnemonic = INST_HALT;
			else if (STR_MATCH4(input, 'swap')) mnemonic = INST_SWAP;
			else if (STR_MATCH4(input, 'push')) mnemonic = INST_PUSH;
			break;
		}
		default: 
		break;
	}
#undef STR_MATCH4
#undef STR_MATCH3
#undef STR_MATCH2
	
	*out_mnemonic = mnemonic;
	return skip_whitespace(end);
	
}

static bool is_ident(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '.');
}

static const char* skip_whitespace(const char* input) {
	while (*input == ' ' || *input == '\t' || *input == '\r' || *input == '\n') input++;
	
	return input;
}
