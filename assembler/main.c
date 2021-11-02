#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum OperandType {
	OPERAND_NUL,
	OPERAND_REG,
	OPERAND_MEM,
	OPERAND_IMM
} OperandType;

typedef enum TokenType {
	TOKEN_EOF, // end-of-file
	TOKEN_EOL, // end-of-line
	
	TOKEN_IDENT,
	TOKEN_LABEL_DEF,
	TOKEN_NUMBER,
	TOKEN_REGISTER,
	TOKEN_COLON,
	
	TOKEN_LD,
	TOKEN_LDI,
	TOKEN_LDD,
	TOKEN_INC,
	TOKEN_DEC,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_POP,
	TOKEN_RET,
	TOKEN_PUSH,
	TOKEN_SWAP,
	TOKEN_STOP,
	TOKEN_HALT,
	
	TOKEN_B,
	TOKEN_C,
	TOKEN_D,
	TOKEN_E,
	TOKEN_H,
	TOKEN_L,
	TOKEN_A,
	
	// TODO(NeGate): Figure this out! the ordering is weird
	TOKEN_F,
	TOKEN_HL,
	TOKEN_AF,
	TOKEN_BC,
	TOKEN_DE,
	TOKEN_SP,
	TOKEN_PC,
	
	TOKEN_FIRST_INST = TOKEN_LD,
	TOKEN_LAST_INST = TOKEN_HALT
} TokenType;

typedef struct Token {
	TokenType type;
	const char* start;
	const char* end;
} Token;

// This is the descriptor used for immediate instructions,
// they require a pseudo-reg value along with the opcode.
//
// R - immediate pseudo-reg
// C - does it have a CB prefix?
// O - opcode
// S - immediate size (0 for 1byte, 1 for 2bytes)
//
// 1RRRSCOO = Immediates
// 10000COO = Normal op
// 00000000 = Error
#define INST_DESCI(opcode, rx, cb) (((rx) << 4) | ((cb) << 2) | ((opcode) << 0))
#define INST_DESC(opcode, cb) (((cb) << 2) | ((opcode) << 0))

#define MAKE_PATTERN(op1, op2) ((op2 << 2) | ((op1) << 0))

#define INST_ENTRY(name, op1, op2) \
[(TOKEN_ ## name) - TOKEN_FIRST_INST][MAKE_PATTERN(op1, op2)]

static const uint8_t INST_DESCRIPTORS[TOKEN_LAST_INST - TOKEN_FIRST_INST][16] = {
	// inc
	INST_ENTRY(INC, OPERAND_REG, OPERAND_NUL) = INST_DESC(0b00, 0)
};

static const char* lexer_at;
static char* emitter;

static bool assemble();
int main(int argc, char* argv[]) {
	char* out = emitter = malloc(1 << 16);
	
	lexer_at = "L0:\ninc a\ninc b\ninc c\ninc d\ninc e\ninc h\n";
	assemble();
	
	// TODO(NeGate): Implement file output
	free(out);
	return 0;
}

static Token read_token();
static bool assemble() {
	while (true) {
		Token t = read_token();
		
		if (t.type == TOKEN_EOF) break;
		else if (t.type == TOKEN_EOL) continue;
		else if (t.type == TOKEN_LABEL_DEF) {
			// TODO(NeGate): Do label defines
		} else if (t.type >= TOKEN_FIRST_INST && t.type <= TOKEN_LAST_INST) {
			// needed to index into the instruction descriptor table
			int inst = t.type - TOKEN_FIRST_INST;
			
			((void)inst);
			((void)INST_DESCRIPTORS);
			
			// TODO(NeGate): Parse and identify the operands
			// TODO(NeGate): Index into the INST_DESCRIPTORS array to get the 
			// descriptor
			// TODO(NeGate): Emit the correct bytes for the pattern
		} else {
			abort();
		}
	}
	
	return true;
}

static bool is_ident(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '.');
}

// NOTE(NeGate): We can get away with 4-byte load on 3-byte compare
// because there's always at least a null terminator afterwards.
#define STR_MATCH1(a, b) (*((uint8_t*) a) == (b))
#define STR_MATCH2(a, b) (*((uint16_t*) a) == __builtin_bswap16(b))
#define STR_MATCH3(a, b) ((*((uint32_t*) a) & 0xFFFFFF) == (__builtin_bswap32(b) >> 8))
#define STR_MATCH4(a, b) (*((uint32_t*) a) == __builtin_bswap32(b))
static Token read_token() {
	// branchless space skip
	lexer_at += (*lexer_at == ' ');
	
	redo_lex:
	switch (*lexer_at) {
		case ' ':
		goto redo_lex;
		case '\0':
		return (Token) { .type = TOKEN_EOF };
		case '\n':
		return (Token) { .type = TOKEN_EOL };
		case '.':
		case 'a' ... 'z':
		case 'A' ... 'Z': {
			const char* start = lexer_at;
			while (is_ident(*lexer_at)) lexer_at++;
			
			if (*lexer_at == ':') {
				lexer_at += 1;
				return (Token) { .type = TOKEN_LABEL_DEF, .start = start, .end = lexer_at - 1 };
			}
			
			int type = TOKEN_IDENT;
			// separated by length
			switch (lexer_at - start) {
				case 1: {
					if      (STR_MATCH1(lexer_at, 'a')) type = TOKEN_A;
					else if (STR_MATCH1(lexer_at, 'b')) type = TOKEN_B;
					else if (STR_MATCH1(lexer_at, 'c')) type = TOKEN_C;
					else if (STR_MATCH1(lexer_at, 'd')) type = TOKEN_D;
					else if (STR_MATCH1(lexer_at, 'e')) type = TOKEN_E;
					else if (STR_MATCH1(lexer_at, 'f')) type = TOKEN_F;
					else if (STR_MATCH1(lexer_at, 'h')) type = TOKEN_H;
					else if (STR_MATCH1(lexer_at, 'l')) type = TOKEN_L;
					break;
				}
				case 2: {
					if      (STR_MATCH2(lexer_at, 'ld')) type = TOKEN_LD;
					else if (STR_MATCH2(lexer_at, 'af')) type = TOKEN_AF;
					else if (STR_MATCH2(lexer_at, 'bc')) type = TOKEN_BC;
					else if (STR_MATCH2(lexer_at, 'de')) type = TOKEN_DE;
					else if (STR_MATCH2(lexer_at, 'sp')) type = TOKEN_SP;
					else if (STR_MATCH2(lexer_at, 'pc')) type = TOKEN_PC;
					break;
				}
				break;
				case 3: {
					if      (STR_MATCH3(lexer_at, 'ldi')) type = TOKEN_LDI;
					else if (STR_MATCH3(lexer_at, 'ldd')) type = TOKEN_LDD;
					else if (STR_MATCH3(lexer_at, 'inc')) type = TOKEN_INC;
					else if (STR_MATCH3(lexer_at, 'dec')) type = TOKEN_DEC;
					else if (STR_MATCH3(lexer_at, 'add')) type = TOKEN_ADD;
					else if (STR_MATCH3(lexer_at, 'sub')) type = TOKEN_SUB;
					else if (STR_MATCH3(lexer_at, 'pop')) type = TOKEN_POP;
					else if (STR_MATCH3(lexer_at, 'ret')) type = TOKEN_RET;
					break;
				}
				case 4: {
					if      (STR_MATCH4(lexer_at, 'stop')) type = TOKEN_STOP;
					else if (STR_MATCH4(lexer_at, 'halt')) type = TOKEN_HALT;
					else if (STR_MATCH4(lexer_at, 'swap')) type = TOKEN_SWAP;
					else if (STR_MATCH4(lexer_at, 'push')) type = TOKEN_PUSH;
					break;
				}
				default: break;
			}
			
			return (Token) { .type = type, .start = start, .end = lexer_at };
		}
		case '0' ... '9': {
			const char* start = lexer_at;
			while (*lexer_at >= '0' && *lexer_at <= '9') lexer_at++;
			
			return (Token) { .type = TOKEN_NUMBER, .start = start, .end = lexer_at };
		}
		default:
		printf("Unknown character '%c'\n", *lexer_at);
		lexer_at++;
		goto redo_lex;
	}
}
#undef STR_MATCH4
#undef STR_MATCH3
#undef STR_MATCH2
#undef STR_MATCH1

