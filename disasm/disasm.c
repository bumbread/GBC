#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>

// The structure stores instruction decoding and interpretation info
// immbytes: if 0, its a single-byte instruction
//           if 1, the instruction takes 1 byte operand
//           if 2, the instruction takes 1 word operand
//           anything else for invalid encoding
// the format string has to contain the fields for immbytes,
//   either $%04x for word operand, or $%02x for byte operand
struct decodeinfo typedef decodeinfo;
struct decodeinfo {
  int immbytes;
  char *fmt;  
};

#define invalid(n)        [n] = {3, ""}
#define opcode0(n, fmt)   [n] = {0, fmt}
#define opcode2(n, fmt)   [n] = {2, fmt}
#define opcode1(n, fmt)   [n] = {1, fmt}

decodeinfo main_instructions[] = {
  opcode0(0x00, "NOP"),
  opcode2(0x01, "LD BC, $%04x"),
  opcode0(0x02, "LD [BC], A"),
  opcode0(0x03, "INC BC"),
  opcode0(0x04, "INC B"),
  opcode0(0x05, "DEC B"),
  opcode1(0x06, "LD B, $%02x"),
  opcode0(0x07, "RLCA"),
  opcode2(0x08, "LD [$%04x], SP"),
  opcode0(0x09, "ADD HL, BC"),
  opcode0(0x0A, "LD A, [BC]"),
  opcode0(0x0B, "DEC BC"),
  opcode0(0x0C, "INC C"),
  opcode0(0x0D, "DEC C"),
  opcode1(0x0E, "LD C, $%02x"),
  opcode0(0x0F, "RRCA"),

  opcode0(0x10, "STOP 0"),
  opcode2(0x11, "LD DE, $%04x"),
  opcode0(0x12, "LD [DE], A"),
  opcode0(0x13, "INC DE"),
  opcode0(0x14, "INC D"),
  opcode0(0x15, "DEC D"),
  opcode1(0x16, "LD D, $%02x"),
  opcode0(0x17, "RLA"),
  opcode1(0x18, "JR $%02x"),
  opcode0(0x19, "ADD HL, DE"),
  opcode0(0x1A, "LD A, [DE]"),
  opcode0(0x1B, "DEC DE"),
  opcode0(0x1C, "INC E"),
  opcode0(0x1D, "DEC E"),
  opcode1(0x1E, "LD E, $%02x"),
  opcode0(0x1F, "RRA"),
  
  opcode1(0x20, "JRNZ $%02x"),
  opcode2(0x21, "LD HL, $%04x"),
  opcode0(0x22, "LD [HL++], A"),
  opcode0(0x23, "INC HL"),
  opcode0(0x24, "INC H"),
  opcode0(0x25, "DEC H"),
  opcode1(0x26, "LD H, $%02x"),
  opcode0(0x27, "DAA"),
  opcode1(0x28, "JRZ, $%02x"),
  opcode0(0x29, "ADD HL, HL"),
  opcode0(0x2A, "LD A, [HL++]"),
  opcode0(0x2B, "DEC HL"),
  opcode0(0x2C, "INC L"),
  opcode0(0x2D, "DEC L"),
  opcode1(0x2E, "LD L, $%02x"),
  opcode0(0x2F, "CPL"),
  
  opcode1(0x30, "JRNC $%02x"),
  opcode2(0x31, "LD SP, $%04x"),
  opcode0(0x32, "LD [HL--], A"),
  opcode0(0x33, "INC SP"),
  opcode0(0x34, "INC [HL]"),
  opcode0(0x35, "DEC [HL]"),
  opcode1(0x36, "LD [HL], $%02x"),
  opcode0(0x37, "SCF"),
  opcode1(0x38, "JRC, $%02x"),
  opcode0(0x39, "ADD HL, SP"),
  opcode0(0x3A, "LD A, [HL--]"),
  opcode0(0x3B, "DEC SP"),
  opcode0(0x3C, "INC A"),
  opcode0(0x3D, "DEC A"),
  opcode1(0x3E, "LD A, $%02x"),
  opcode0(0x3F, "CCF"),
  
  opcode0(0x40, "LD B, B"),
  opcode0(0x41, "LD B, C"),
  opcode0(0x42, "LD B, D"),
  opcode0(0x43, "LD B, E"),
  opcode0(0x44, "LD B, H"),
  opcode0(0x45, "LD B, L"),
  opcode0(0x46, "LD B, [HL]"),
  opcode0(0x47, "LD B, A"),
  opcode0(0x48, "LD C, B"),
  opcode0(0x49, "LD C, C"),
  opcode0(0x4A, "LD C, D"),
  opcode0(0x4B, "LD C, E"),
  opcode0(0x4C, "LD C, H"),
  opcode0(0x4D, "LD C, L"),
  opcode0(0x4E, "LD C, [HL]"),
  opcode0(0x4F, "LD C, A"),
 
  opcode0(0x50, "LD D, B"),
  opcode0(0x51, "LD D, C"),
  opcode0(0x52, "LD D, D"),
  opcode0(0x53, "LD D, E"),
  opcode0(0x54, "LD D, H"),
  opcode0(0x55, "LD D, L"),
  opcode0(0x56, "LD D, [HL]"),
  opcode0(0x57, "LD D, A"),
  opcode0(0x58, "LD E, B"),
  opcode0(0x59, "LD E, C"),
  opcode0(0x5A, "LD E, D"),
  opcode0(0x5B, "LD E, E"),
  opcode0(0x5C, "LD E, H"),
  opcode0(0x5D, "LD E, L"),
  opcode0(0x5E, "LD E, [HL]"),
  opcode0(0x5F, "LD E, A"),
  
  opcode0(0x60, "LD H, B"),
  opcode0(0x61, "LD H, C"),
  opcode0(0x62, "LD H, D"),
  opcode0(0x63, "LD H, E"),
  opcode0(0x64, "LD H, H"),
  opcode0(0x65, "LD H, L"),
  opcode0(0x66, "LD H, [HL]"),
  opcode0(0x67, "LD H, A"),
  opcode0(0x68, "LD L, B"),
  opcode0(0x69, "LD L, C"),
  opcode0(0x6A, "LD L, D"),
  opcode0(0x6B, "LD L, E"),
  opcode0(0x6C, "LD L, H"),
  opcode0(0x6D, "LD L, L"),
  opcode0(0x6E, "LD L, [HL]"),
  opcode0(0x6F, "LD L, A"),
  
  opcode0(0x70, "LD [HL], B"),
  opcode0(0x71, "LD [HL], C"),
  opcode0(0x72, "LD [HL], D"),
  opcode0(0x73, "LD [HL], E"),
  opcode0(0x74, "LD [HL], H"),
  opcode0(0x75, "LD [HL], L"),
  opcode0(0x76, "HALT"),
  opcode0(0x77, "LD [HL], A"),
  opcode0(0x78, "LD A, B"),
  opcode0(0x79, "LD A, C"),
  opcode0(0x7A, "LD A, D"),
  opcode0(0x7B, "LD A, E"),
  opcode0(0x7C, "LD A, H"),
  opcode0(0x7D, "LD A, L"),
  opcode0(0x7E, "LD A, [HL]"),
  opcode0(0x7F, "LD A, A"),
  
  opcode0(0x80, "ADD B"),
  opcode0(0x81, "ADD C"),
  opcode0(0x82, "ADD D"),
  opcode0(0x83, "ADD E"),
  opcode0(0x84, "ADD H"),
  opcode0(0x85, "ADD L"),
  opcode0(0x86, "ADD [HL]"),
  opcode0(0x87, "ADD A"),
  opcode0(0x88, "ADC B"),
  opcode0(0x89, "ADC C"),
  opcode0(0x8A, "ADC D"),
  opcode0(0x8B, "ADC E"),
  opcode0(0x8D, "ADC H"),
  opcode0(0x8E, "ADC L"),
  opcode0(0x8F, "ADC [HL]"),
  opcode0(0x8C, "ADC A"),

  opcode0(0x90, "SUB B"),
  opcode0(0x91, "SUB C"),
  opcode0(0x92, "SUB D"),
  opcode0(0x93, "SUB E"),
  opcode0(0x94, "SUB H"),
  opcode0(0x95, "SUB L"),
  opcode0(0x96, "SUB [HL]"),
  opcode0(0x97, "SUB A"),
  opcode0(0x98, "SBC B"),
  opcode0(0x99, "SBC C"),
  opcode0(0x9A, "SBC D"),
  opcode0(0x9B, "SBC E"),
  opcode0(0x9C, "SBC H"),
  opcode0(0x9D, "SBC L"),
  opcode0(0x9E, "SBC [HL]"),
  opcode0(0x9F, "SBC A"),
  
  opcode0(0xA0, "AND B"),
  opcode0(0xA1, "AND C"),
  opcode0(0xA2, "AND D"),
  opcode0(0xA3, "AND E"),
  opcode0(0xA4, "AND H"),
  opcode0(0xA5, "AND L"),
  opcode0(0xA6, "AND [HL]"),
  opcode0(0xA7, "AND A"),
  opcode0(0xA8, "XOR B"),
  opcode0(0xA9, "XOR C"),
  opcode0(0xAA, "XOR D"),
  opcode0(0xAB, "XOR E"),
  opcode0(0xAC, "XOR H"),
  opcode0(0xAD, "XOR L"),
  opcode0(0xAE, "XOR [HL]"),
  opcode0(0xAF, "XOR A"),
  
  opcode0(0xB0, "OR B"),
  opcode0(0xB1, "OR C"),
  opcode0(0xB2, "OR D"),
  opcode0(0xB3, "OR E"),
  opcode0(0xB4, "OR H"),
  opcode0(0xB5, "OR L"),
  opcode0(0xB6, "OR [HL]"),
  opcode0(0xB7, "OR A"),
  opcode0(0xB8, "CP B"),
  opcode0(0xB9, "CP C"),
  opcode0(0xBA, "CP D"),
  opcode0(0xBB, "CP E"),
  opcode0(0xBC, "CP H"),
  opcode0(0xBD, "CP L"),
  opcode0(0xBE, "CP [HL]"),
  opcode0(0xBF, "CP A"),
  
  opcode0(0xC0, "RETNZ"),
  opcode0(0xC1, "POP BC"),
  opcode2(0xC2, "JPNZ $%04x"),
  opcode2(0xC3, "JP $%04x"),
  opcode2(0xC4, "CALLNZ $%04x"),
  opcode0(0xC5, "PUSH BC"),
  opcode1(0xC6, "ADD $%02x"),
  opcode0(0xC7, "RST $00"),
  opcode0(0xC8, "RETZ"),
  opcode0(0xC9, "RET"),
  opcode2(0xCA, "JPZ $%04x"),
  invalid(0xCB),
  opcode2(0xCC, "CALLZ $%04x"),
  opcode2(0xCD, "CALL $%04x"),
  opcode1(0xCE, "ADC $%02x"),
  opcode0(0xCF, "RST $00"),
  
  opcode0(0xD0, "RETNC"),
  opcode0(0xD1, "POP DE"),
  opcode2(0xD2, "JPNC $%04x"),
  invalid(0xD3),
  opcode2(0xD4, "CALLNC $%04x"),
  opcode0(0xD5, "PUSH DE"),
  opcode1(0xD6, "SUB $%02x"),
  opcode0(0xD7, "RST $10"),
  opcode0(0xD8, "RETC"),
  opcode0(0xD9, "RETI"),
  opcode2(0xDA, "JPC $%04x"),
  invalid(0xDB),
  opcode2(0xDC, "CALLC $%04x"),
  invalid(0xDD),
  opcode1(0xDE, "SBC $%02x"),
  opcode0(0xDF, "RST $18"),
  
  opcode1(0xE0, "LDH [$FFFF + $%02x], A"),
  opcode0(0xE1, "POP HL"),
  opcode0(0xE2, "LD A, [$FFFF + C]"),
  invalid(0xE3),
  invalid(0xE4),
  opcode0(0xE5, "PUSH HL"),
  opcode1(0xE6, "AND $%02x"),
  opcode0(0xE7, "RST $20"),
  opcode1(0xE8, "ADD SP, $%02x"),
  opcode0(0xE9, "JP [HL]"),
  opcode2(0xEA, "LD [$%04x], A"),
  invalid(0xEB),
  invalid(0xEC),
  invalid(0xED),
  opcode1(0xEE, "XOR $%02x"),
  opcode0(0xEF, "RST $28"),
  
  opcode1(0xF0, "LDH A, [$FFFF + $%02x]"),
  opcode0(0xF1, "POP AF"),
  opcode0(0xF2, "LD A, [C]"),
  opcode0(0xF3, "DI"),
  invalid(0xF4),
  opcode0(0xF5, "PUSH AF"),
  opcode1(0xF6, "OR $%02x"),
  opcode0(0xF7, "RST $30"),
  opcode1(0xF8, "LD HL, SP+$%02x"),
  opcode0(0xF9, "LD SP, HL"),
  opcode2(0xFA, "LD A, [$%04x]"),
  opcode0(0xFB, "EI"),
  invalid(0xFC),
  invalid(0xFD),
  opcode1(0xFE, "XOR $%02x"),
  opcode0(0xFF, "RST $38"),
};

decodeinfo cb_instructions[] = {

  opcode0(0x00, "RLC B"),
  opcode0(0x01, "RLC C"),
  opcode0(0x02, "RLC D"),
  opcode0(0x03, "RLC E"),
  opcode0(0x04, "RLC H"),
  opcode0(0x05, "RLC L"),
  opcode0(0x06, "RLC [HL]"),
  opcode0(0x07, "RLC A"),
  opcode0(0x08, "RRC B"),
  opcode0(0x09, "RRC C"),
  opcode0(0x0A, "RRC D"),
  opcode0(0x0B, "RRC E"),
  opcode0(0x0C, "RRC H"),
  opcode0(0x0D, "RRC L"),
  opcode0(0x0E, "RRC [HL]"),
  opcode0(0x0F, "RRC A"),

  opcode0(0x10, "RL B"),
  opcode0(0x11, "RL C"),
  opcode0(0x12, "RL D"),
  opcode0(0x13, "RL E"),
  opcode0(0x14, "RL H"),
  opcode0(0x15, "RL L"),
  opcode0(0x16, "RL [HL]"),
  opcode0(0x17, "RL A"),
  opcode0(0x18, "RR B"),
  opcode0(0x19, "RR C"),
  opcode0(0x1A, "RR D"),
  opcode0(0x1B, "RR E"),
  opcode0(0x1C, "RR H"),
  opcode0(0x1D, "RR L"),
  opcode0(0x1E, "RR [HL]"),
  opcode0(0x1F, "RR A"),

  opcode0(0x20, "SLA B"),
  opcode0(0x21, "SLA C"),
  opcode0(0x22, "SLA D"),
  opcode0(0x23, "SLA E"),
  opcode0(0x24, "SLA H"),
  opcode0(0x25, "SLA L"),
  opcode0(0x26, "SLA [HL]"),
  opcode0(0x27, "SLA A"),
  opcode0(0x28, "SRA B"),
  opcode0(0x29, "SRA C"),
  opcode0(0x2A, "SRA D"),
  opcode0(0x2B, "SRA E"),
  opcode0(0x2C, "SRA H"),
  opcode0(0x2D, "SRA L"),
  opcode0(0x2E, "SRA [HL]"),
  opcode0(0x2F, "SRA A"),

  opcode0(0x30, "SWAP B"),
  opcode0(0x31, "SWAP C"),
  opcode0(0x32, "SWAP D"),
  opcode0(0x33, "SWAP E"),
  opcode0(0x34, "SWAP H"),
  opcode0(0x35, "SWAP L"),
  opcode0(0x36, "SWAP [HL]"),
  opcode0(0x37, "SWAP A"),
  opcode0(0x38, "SRL B"),
  opcode0(0x39, "SRL C"),
  opcode0(0x3A, "SRL D"),
  opcode0(0x3B, "SRL E"),
  opcode0(0x3C, "SRL H"),
  opcode0(0x3D, "SRL L"),
  opcode0(0x3E, "SRL [HL]"),
  opcode0(0x3F, "SRL A"),

  opcode0(0x40, "BIT 0, B"),
  opcode0(0x41, "BIT 0, C"),
  opcode0(0x42, "BIT 0, D"),
  opcode0(0x43, "BIT 0, E"),
  opcode0(0x44, "BIT 0, H"),
  opcode0(0x45, "BIT 0, L"),
  opcode0(0x46, "BIT 0, [HL]"),
  opcode0(0x47, "BIT 0, A"),
  opcode0(0x48, "BIT 1, B"),
  opcode0(0x49, "BIT 1, C"),
  opcode0(0x4A, "BIT 1, D"),
  opcode0(0x4B, "BIT 1, E"),
  opcode0(0x4C, "BIT 1, H"),
  opcode0(0x4D, "BIT 1, L"),
  opcode0(0x4E, "BIT 1, [HL]"),
  opcode0(0x4F, "BIT 1, A"),

  opcode0(0x50, "BIT 2, B"),
  opcode0(0x51, "BIT 2, C"),
  opcode0(0x52, "BIT 2, D"),
  opcode0(0x53, "BIT 2, E"),
  opcode0(0x54, "BIT 2, H"),
  opcode0(0x55, "BIT 2, L"),
  opcode0(0x56, "BIT 2, [HL]"),
  opcode0(0x57, "BIT 2, A"),
  opcode0(0x58, "BIT 3, B"),
  opcode0(0x59, "BIT 3, C"),
  opcode0(0x5A, "BIT 3, D"),
  opcode0(0x5B, "BIT 3, E"),
  opcode0(0x5C, "BIT 3, H"),
  opcode0(0x5D, "BIT 3, L"),
  opcode0(0x5E, "BIT 3, [HL]"),
  opcode0(0x5F, "BIT 3, A"),

  opcode0(0x60, "BIT 4, B"),
  opcode0(0x61, "BIT 4, C"),
  opcode0(0x62, "BIT 4, D"),
  opcode0(0x63, "BIT 4, E"),
  opcode0(0x64, "BIT 4, H"),
  opcode0(0x65, "BIT 4, L"),
  opcode0(0x66, "BIT 4, [HL]"),
  opcode0(0x67, "BIT 5, A"),
  opcode0(0x68, "BIT 5, B"),
  opcode0(0x69, "BIT 5, C"),
  opcode0(0x6A, "BIT 5, D"),
  opcode0(0x6B, "BIT 5, E"),
  opcode0(0x6C, "BIT 5, H"),
  opcode0(0x6D, "BIT 5, L"),
  opcode0(0x6E, "BIT 5, [HL]"),
  opcode0(0x6F, "BIT 5, A"),

  opcode0(0x70, "BIT 6, B"),
  opcode0(0x71, "BIT 6, C"),
  opcode0(0x72, "BIT 6, D"),
  opcode0(0x73, "BIT 6, E"),
  opcode0(0x74, "BIT 6, H"),
  opcode0(0x75, "BIT 6, L"),
  opcode0(0x76, "BIT 6, [HL]"),
  opcode0(0x77, "BIT 6, A"),
  opcode0(0x78, "BIT 7, B"),
  opcode0(0x79, "BIT 7, C"),
  opcode0(0x7A, "BIT 7, D"),
  opcode0(0x7B, "BIT 7, E"),
  opcode0(0x7C, "BIT 7, H"),
  opcode0(0x7D, "BIT 7, L"),
  opcode0(0x7E, "BIT 7, [HL]"),
  opcode0(0x7F, "BIT 7, A"),

  opcode0(0x80, "RES 0, B"),
  opcode0(0x81, "RES 0, C"),
  opcode0(0x82, "RES 0, D"),
  opcode0(0x83, "RES 0, E"),
  opcode0(0x84, "RES 0, H"),
  opcode0(0x85, "RES 0, L"),
  opcode0(0x86, "RES 0, [HL]"),
  opcode0(0x87, "RES 0, A"),
  opcode0(0x88, "RES 1, B"),
  opcode0(0x89, "RES 1, C"),
  opcode0(0x8A, "RES 1, D"),
  opcode0(0x8B, "RES 1, E"),
  opcode0(0x8C, "RES 1, H"),
  opcode0(0x8D, "RES 1, L"),
  opcode0(0x8E, "RES 1, [HL]"),
  opcode0(0x8F, "RES 1, A"),

  opcode0(0x90, "RES 2, B"),
  opcode0(0x91, "RES 2, C"),
  opcode0(0x92, "RES 2, D"),
  opcode0(0x93, "RES 2, E"),
  opcode0(0x94, "RES 2, H"),
  opcode0(0x95, "RES 2, L"),
  opcode0(0x96, "RES 2, [HL]"),
  opcode0(0x97, "RES 2, A"),
  opcode0(0x98, "RES 3, B"),
  opcode0(0x99, "RES 3, C"),
  opcode0(0x9A, "RES 3, D"),
  opcode0(0x9B, "RES 3, E"),
  opcode0(0x9C, "RES 3, H"),
  opcode0(0x9D, "RES 3, L"),
  opcode0(0x9E, "RES 3, [HL]"),
  opcode0(0x9F, "RES 3, A"),

  opcode0(0xA0, "RES 4, B"),
  opcode0(0xA1, "RES 4, C"),
  opcode0(0xA2, "RES 4, D"),
  opcode0(0xA3, "RES 4, E"),
  opcode0(0xA4, "RES 4, H"),
  opcode0(0xA5, "RES 4, L"),
  opcode0(0xA6, "RES 4, [HL]"),
  opcode0(0xA7, "RES 5, A"),
  opcode0(0xA8, "RES 5, B"),
  opcode0(0xA9, "RES 5, C"),
  opcode0(0xAA, "RES 5, D"),
  opcode0(0xAB, "RES 5, E"),
  opcode0(0xAC, "RES 5, H"),
  opcode0(0xAD, "RES 5, L"),
  opcode0(0xAE, "RES 5, [HL]"),
  opcode0(0xAF, "RES 5, A"),

  opcode0(0xB0, "RES 6, B"),
  opcode0(0xB1, "RES 6, C"),
  opcode0(0xB2, "RES 6, D"),
  opcode0(0xB3, "RES 6, E"),
  opcode0(0xB4, "RES 6, H"),
  opcode0(0xB5, "RES 6, L"),
  opcode0(0xB6, "RES 6, [HL]"),
  opcode0(0xB7, "RES 6, A"),
  opcode0(0xB8, "RES 7, B"),
  opcode0(0xB9, "RES 7, C"),
  opcode0(0xBA, "RES 7, D"),
  opcode0(0xBB, "RES 7, E"),
  opcode0(0xBC, "RES 7, H"),
  opcode0(0xBD, "RES 7, L"),
  opcode0(0xBE, "RES 7, [HL]"),
  opcode0(0xBF, "RES 7, A"),

  opcode0(0xC0, "SET 0, B"),
  opcode0(0xC1, "SET 0, C"),
  opcode0(0xC2, "SET 0, D"),
  opcode0(0xC3, "SET 0, E"),
  opcode0(0xC4, "SET 0, H"),
  opcode0(0xC5, "SET 0, L"),
  opcode0(0xC6, "SET 0, [HL]"),
  opcode0(0xC7, "SET 0, A"),
  opcode0(0xC8, "SET 1, B"),
  opcode0(0xC9, "SET 1, C"),
  opcode0(0xCA, "SET 1, D"),
  opcode0(0xCB, "SET 1, E"),
  opcode0(0xCC, "SET 1, H"),
  opcode0(0xCD, "SET 1, L"),
  opcode0(0xCE, "SET 1, [HL]"),
  opcode0(0xCF, "SET 1, A"),

  opcode0(0xD0, "SET 2, B"),
  opcode0(0xD1, "SET 2, C"),
  opcode0(0xD2, "SET 2, D"),
  opcode0(0xD3, "SET 2, E"),
  opcode0(0xD4, "SET 2, H"),
  opcode0(0xD5, "SET 2, L"),
  opcode0(0xD6, "SET 2, [HL]"),
  opcode0(0xD7, "SET 2, A"),
  opcode0(0xD8, "SET 3, B"),
  opcode0(0xD9, "SET 3, C"),
  opcode0(0xDA, "SET 3, D"),
  opcode0(0xDB, "SET 3, E"),
  opcode0(0xDC, "SET 3, H"),
  opcode0(0xDD, "SET 3, L"),
  opcode0(0xDE, "SET 3, [HL]"),
  opcode0(0xDF, "SET 3, A"),

  opcode0(0xE0, "SET 4, B"),
  opcode0(0xE1, "SET 4, C"),
  opcode0(0xE2, "SET 4, D"),
  opcode0(0xE3, "SET 4, E"),
  opcode0(0xE4, "SET 4, H"),
  opcode0(0xE5, "SET 4, L"),
  opcode0(0xE6, "SET 4, [HL]"),
  opcode0(0xE7, "SET 5, A"),
  opcode0(0xE8, "SET 5, B"),
  opcode0(0xE9, "SET 5, C"),
  opcode0(0xEA, "SET 5, D"),
  opcode0(0xEB, "SET 5, E"),
  opcode0(0xEC, "SET 5, H"),
  opcode0(0xED, "SET 5, L"),
  opcode0(0xEE, "SET 5, [HL]"),
  opcode0(0xEF, "SET 5, A"),

  opcode0(0xF0, "SET 6, B"),
  opcode0(0xF1, "SET 6, C"),
  opcode0(0xF2, "SET 6, D"),
  opcode0(0xF3, "SET 6, E"),
  opcode0(0xF4, "SET 6, H"),
  opcode0(0xF5, "SET 6, L"),
  opcode0(0xF6, "SET 6, [HL]"),
  opcode0(0xF7, "SET 6, A"),
  opcode0(0xF8, "SET 7, B"),
  opcode0(0xF9, "SET 7, C"),
  opcode0(0xFA, "SET 7, D"),
  opcode0(0xFB, "SET 7, E"),
  opcode0(0xFC, "SET 7, H"),
  opcode0(0xFD, "SET 7, L"),
  opcode0(0xFE, "SET 7, [HL]"),
  opcode0(0xFF, "SET 7, A"),
};

#undef opcode0
#undef invalid
#undef opcode2
#undef opcode1

int start_addr = 0;
int max_size = 0;
int offset = 0;
FILE *in;
FILE *out;

static void disassemble(void)
{
  for(;;) {
    int instruction_addr = start_addr + offset;

    int i = 0;
    int bytes[3];
    
    bytes[i] = fgetc(in);
    if(bytes[i] == EOF) break;
    i ++;
    

    decodeinfo info;
    if(bytes[0] == 0xCB) {
      int pi = i;
      bytes[i] = fgetc(in);
      if(bytes[i] == EOF) goto bad_encoding;
      i ++;

      info = cb_instructions[bytes[pi]];
    }
    else {
      info = main_instructions[bytes[0]];
    }

    if(info.immbytes == 3) { // undecodeable
      goto bad_encoding;
    }
    else if(info.immbytes == 2) {
      int pi = i;
      bytes[i] = fgetc(in);
      if(bytes[i] == EOF) goto bad_encoding;
      i ++;
      bytes[i] = fgetc(in);
      if(bytes[i] == EOF) goto bad_encoding;
      i ++;

      int word = bytes[pi] + (bytes[pi+1] << 8);
      fprintf(out, "%04x\t%02x%02x%02x\t", instruction_addr, bytes[0], bytes[1], bytes[2]);
      fprintf(out, info.fmt, word);
      fprintf(out, "\n");
    }
    else if(info.immbytes == 1) {
      int pi = i;
      bytes[i] = fgetc(in);
      if(bytes[i] == EOF) goto bad_encoding;
      i ++;
      
      int byte = bytes[pi];
      fprintf(out, "%04x\t%02x%02x\t", instruction_addr, bytes[0], bytes[1]);
      fprintf(out, info.fmt, byte);
      fprintf(out, "\n");
    }
    else {
      fprintf(out, "%04x\t%02x\t", instruction_addr, bytes[0]);
      fprintf(out, info.fmt);
      fprintf(out, "\n");
    }

    offset += i;

    continue;
    bad_encoding: {
      for(int bi = 0; bi != i; bi++) {
        fprintf(out, "%04x\t%02x\tDB $%02x\n", instruction_addr++, bytes[bi], bytes[bi]);
      }
      offset += i;
    }
  }
}

// Start is required to be non-empty string
// Returns -1 if `str` is not started with `start`
// Returns  0 if `str` is equal to `start`
// Returns the length of start if `str` starts with `start`
static int startcmp(char *str, char *start)
{
  char *p = str;
  char *q = start;

  while(*p != 0 && *p == *q) {++p; ++q; }

  if(*q == 0) {
    if(*p == 0) return 0;
    else        return (int)(q-start);
  }
  return -1;
}

static void mkarg(char **argv, int *argi, char *opt, char **value)
{
  char *op_value = argv[*argi];
  char *param = *value;

  int oplen = startcmp(op_value, opt);
  if(oplen == 0) {
    ++*argi;
    param = argv[*argi];
    ++*argi;
  }
  else if(oplen > 0) {
    param = op_value + oplen;
    ++*argi;
  }

  *value = param;
}

static int tohex(char c)
{
  if('0' <= c && c <= '9') return c - '0';
  if('a' <= c && c <= 'f') return c - 'a' + 10;
  if('A' <= c && c <= 'F') return c - 'A' + 10;
  fprintf(stderr, "The provided value is not a hex number!\n");
  exit(1);
}

static int parse16(char *str)
{
  int hex = 0;
  while(*str!=0) {
    hex = 16*hex + tohex(str[0]);
    ++str;
  }
  return hex;
}

#define ARG(opt, ref) if(argi >= argc) break; mkarg(argv, &argi, opt, ref)

int main(int argc, char **argv)
{
  if(argc <= 1) {
    fprintf(stderr, "No input filename provided\n"
                    "use disasm <filename> to disassmeble file\n"
                    "and disasm <file> -o <output_file> to name the\n"
                    "output file.\n");
    exit(1);
  }

  char *saddr = "0000";
  char *soffs = "0000";
  char *ssize = "FFFF";
  char *infn = 0;
  char *outfn = 0;
  for(int argi = 1; argi < argc; ++argi) {
    char *str = argv[argi];
    if(str[0] != '-') {
      if(infn != 0) {
        fprintf(stderr, "Don't input more than one input filename!\n"
                        "Hint: options start with '-'\n"
                        "Current:\t%s, Previous:\t%s\n", infn, str);
        exit(1);
      }
      infn = str;
    }
    else {
      ARG("-o", &outfn);
      ARG("-a", &saddr);
      ARG("-s", &soffs);
      ARG("-t", &ssize);
    }
  }

  // Todo(bumbread/2021-11-02): Figure out the analog filename on windows
  // and assign it conditionally, based on the current platform.
  if(outfn == 0) outfn = "/dev/stdout";
  if(infn == 0) infn = "/dev/stdin";

  in = fopen(infn, "rb");
  if(in == 0) {
    fprintf(stderr, "Unable to open file %s\n", infn);
    exit(1);
  }

  out = fopen(outfn, "w");
  if(out == 0) {
    fprintf(stderr, "Unable to open file %s\n", outfn);
    exit(1);
  }

  start_addr = parse16(saddr);
  max_size = parse16(ssize);

  int start_offs = parse16(soffs);
  for(int i = 0; i != start_offs; ++i) {
    int b = fgetc(in);
    if(b == EOF) {
      fprintf(stderr, "The offset you provided is outside of the file\n");
      exit(1);
    }
  }

  disassemble();
  fclose(out);
  fclose(in);
  return 0;
}


