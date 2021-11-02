#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>

int addr = 0;
FILE *in;
FILE *out;

static char *rtostr(int r)
{
  switch(r) {
    case 0: return "B";
    case 1: return "C";
    case 2: return "D";
    case 3: return "E";
    case 4: return "H";
    case 5: return "L";
    case 6: return "(HL)"; // technically incorrect, but removes extra special casing
    case 7: return "A";
  }
  return "";
}

static char *rrtostr(int rr)
{
  switch(rr) {
    case 0: return "BC";
    case 1: return "DE";
    case 2: return "HL";
    case 3: return "SP";
  }
  return "";
}

// Note(bumbread): same thing, but
// for pus/pop instructions that use
// AF instead of SP for the index 3
static char *rr2tostr(int rr)
{
  switch(rr) {
    case 0: return "BC";
    case 1: return "DE";
    case 2: return "HL";
    case 3: return "AF";
  }
  return "";
}

static char *mrrtostr(int rr)
{
  switch(rr) {
    case 0: return "(BC)";
    case 1: return "(DE)";
    case 2: return "(HL+)";
    case 3: return "(HL-)";
  }
  return "";
}

#define BYTE(n) int n = fgetc(in); if(n == EOF) exit(1); fprintf(out, "%02x", n); addr++
#define WORD(n) BYTE(b0); BYTE(b1); int n = b0+(b1<<8)

static void disasm_prefix_cb(void)
{
  BYTE(byte);
  int low3 = byte&0x07;
  int hi3 = (byte>>3)&0x07;
  int opcode = byte>>6;
  switch(opcode) {
    case 0: { 
      switch(hi3) {
        case 0: { fprintf(out, "\tRLC %s", rtostr(low3)); } break;
        case 1: { fprintf(out, "\tRRC %s", rtostr(low3)); } break;
        case 2: { fprintf(out, "\tRL %s", rtostr(low3)); } break;
        case 3: { fprintf(out, "\tRR %s", rtostr(low3)); } break;
        case 4: { fprintf(out, "\tSLA %s", rtostr(low3)); } break;
        case 5: { fprintf(out, "\tSRA %s", rtostr(low3)); } break;
        case 6: { fprintf(out, "\tSWAP %s", rtostr(low3)); } break;
        case 7: { fprintf(out, "\tSRL %s", rtostr(low3)); } break;
      }
    } break;
    case 1: {
      fprintf(out, "\tBIT %d, %s", hi3, rtostr(low3));
    } break;
    case 2: {
      fprintf(out, "\tRES %d, %s", hi3, rtostr(low3));
    } break;
    case 3: {
      fprintf(out, "\tSET %d, %s", hi3, rtostr(low3));
    } break;
  }
}

static void disassemble(void)
{
  for(;;) {
    int start_addr = addr;

    int byte = fgetc(in);
    if(byte == EOF) break;
    addr++;
  
    fprintf(out, "%04x\t", start_addr);
    fprintf(out, "%02x", byte);
    
    int opcode = byte>>6;
    int low3 = byte&0x07;
    int low4 = byte&0x0f;
    int hi3 = (byte>>3)&0x07;
    int hi2 = (byte>>4)&0x03;
    switch(opcode) {
      case 0: {
        switch(low3) {
          case 0: {
            switch(hi3) {
              case 0: { fprintf(out, "\tNOP");} break;
              case 1: { WORD(a16); fprintf(out, "\tLD (%04x), SP", a16); } break;
              case 2: { fprintf(out, "\tSTOP 0");} break;
              case 3: { BYTE(r8); fprintf(out, "\tJR %02x", r8);} break;
              case 4: { BYTE(r8); fprintf(out, "\tJR NZ, %02x", r8); } break;
              case 5: { BYTE(r8); fprintf(out, "\tJR Z, %02x", r8); } break; 
              case 6: { BYTE(r8); fprintf(out, "\tJR NC, %02x", r8); } break; 
              case 7: { BYTE(r8); fprintf(out, "\tJR C, %02x", r8); } break; 
            }
          } break;
  
          case 1: {
            if((hi3&1) == 0) {
              WORD(d16);
              fprintf(out, "\tLD %s, %04x", mrrtostr(hi2), d16);
            }
            else {
              fprintf(out, "\tADD HL, %s", rrtostr(hi2));
            }
          } break;
  
          case 2: {
            if((hi3 & 1) == 0) {
              fprintf(out, "\tLD %s, A", mrrtostr(hi2));
            }
            else {
              fprintf(out, "\tLD A, %s", mrrtostr(hi2));
            } 
          } break;
  
          case 3: {
            if((hi3 & 1) == 0) {
              fprintf(out, "\tINC %s", rrtostr(hi2));
            }
            else {
              fprintf(out, "\tDEC %s", rrtostr(hi2));
            }
          } break;
  
          case 4: fprintf(out, "\tINC %s", rtostr(hi3)); break;
          case 5: fprintf(out, "\tDEC %s", rtostr(hi3)); break;
  
          case 6: { 
            BYTE(n);
            fprintf(out, "\tLD ");
            fprintf(out, "%s, %02x", rtostr(hi3), n);
          } break;
          
          case 7: {
            switch(hi3) {
              case 0: fprintf(out, "\tRLCA"); break;
              case 1: fprintf(out, "\tRRCA"); break;
              case 2: fprintf(out, "\tRLA"); break;
              case 3: fprintf(out, "\tRRA"); break;
              case 4: fprintf(out, "\tDAA"); break;
              case 5: fprintf(out, "\tCPL"); break;
              case 6: fprintf(out, "\tSCF"); break;
              case 7: fprintf(out, "\tCCF"); break;
            }
          } break;
        }
      } break;
      case 1: {
        if(hi3 == 6 && low3 == 6) {
          fprintf(out, "\tHALT");
        }
        else {
          fprintf(out, "\tLD %s, %s", rtostr(hi3), rtostr(low3));
        }
      } break;
      case 2: {
        switch(hi3) {
          case 0: { fprintf(out, "\tADD A, %s", rtostr(low3)); } break;
          case 1: { fprintf(out, "\tADC A, %s", rtostr(low3)); } break;
          case 2: { fprintf(out, "\tSUB A, %s", rtostr(low3)); } break;
          case 3: { fprintf(out, "\tSBC A, %s", rtostr(low3)); } break;
          case 4: { fprintf(out, "\tAND A, %s", rtostr(low3)); } break;
          case 5: { fprintf(out, "\tXOR A, %s", rtostr(low3)); } break;
          case 6: { fprintf(out, "\tOR A, %s",  rtostr(low3)); } break;
          case 7: { fprintf(out, "\tCP A, %s",  rtostr(low3)); } break;
        }
      } break;
      case 3: {
        switch(low4) {
          case 1: fprintf(out, "POP %s", rr2tostr(hi2)); break;
          case 5: fprintf(out, "PUSH %s", rr2tostr(hi2));
          case 0: {
            switch(hi2) {
              case 0: { fprintf(out, "\tRET NZ"); } break;
              case 1: { fprintf(out, "\tRET NC"); } break;
              case 2: { BYTE(a8); fprintf(out, "\tLDH ($FF00+%02x), A", a8); } break;
              case 3: { BYTE(a8); fprintf(out, "\tLDH ($FF00+%02x), A", a8); } break;
            }
          } break;
          case 8: {
            switch(hi2) {
              case 0: { fprintf(out, "\tRET Z"); } break;
              case 1: { fprintf(out, "\tRET C"); } break;
              case 2: { BYTE(r8); fprintf(out, "\tADD SP, %02x", r8); } break;
              case 3: { BYTE(r8); fprintf(out, "\tLD HL, SP+%02x", r8); } break;
            }
          } break;
          case 2: {
            switch(hi2) {
              case 0: { WORD(a16); fprintf(out, "\tJP NZ, %04x", a16); } break;
              case 1: { WORD(a16); fprintf(out, "\tJP NC, %04x", a16); } break;
              case 2: { BYTE(a8);  fprintf(out, "\tLDH ($FF00+%02x), A", a8); } break;
              case 3: { BYTE(a8);  fprintf(out, "\tLDH A, ($FF00+%02x)", a8); } break;
            }
          } break;
          case 10: {
            switch(hi2) {
              case 0: { WORD(a16); fprintf(out, "\tJP Z, %04x", a16); } break;
              case 1: { WORD(a16); fprintf(out, "\tJP C, %04x", a16); } break;
              case 2: { WORD(a16); fprintf(out, "\tLD (%04x), A", a16); } break;
              case 3: { WORD(a16); fprintf(out, "\tLD A, (%04x)", a16); } break;
            }
          } break;
          case 3: {
            if(hi2 == 0)      { WORD(a16); fprintf(out, "\tJP %04x", a16); }
            else if(hi2 == 3) { fprintf(out, "\tDI"); }
            else              { exit(1); } 
          } break;
          case 11: {
            if(hi2 == 0)      { disasm_prefix_cb();  }
            else if(hi2 == 3) { fprintf(out, "\tEI"); }
            else              { exit(1); }
          } break;
          case 4: {
            if(hi2 == 0)      { WORD(a16); fprintf(out, "\tCALL NZ, %04x", a16); }
            else if(hi2 == 1) { WORD(a16); fprintf(out, "\tCALL NC, %04x", a16); }
            else              { exit(1); }
          } break;
          case 12: {
            if(hi2 == 0)      { WORD(a16); fprintf(out, "\tCALL Z, %04x", a16); }
            else if(hi2 == 1) { WORD(a16); fprintf(out, "\tCALL C, %04x", a16); }
            else              { exit(1); }
          } break;
          case 13: {
            if(hi2 == 0) { WORD(a16); fprintf(out, "\tCALL %04x", a16); }
            else         { exit(1); }
          } break;
          case 6: {
            switch(hi2) {
              case 0: { BYTE(d8); fprintf(out, "\tADD A, %02x", d8); } break;
              case 1: { BYTE(d8); fprintf(out, "\tSUB A, %02x", d8); } break;
              case 2: { BYTE(d8); fprintf(out, "\tAND A, %02x", d8); } break;
              case 3: { BYTE(d8); fprintf(out, "\tOR A, %02x", d8); } break;
            }
          } break;
          case 14: {
            switch(hi2) {
              case 0: { BYTE(d8); fprintf(out, "\tADC A, %02x", d8); } break;
              case 1: { BYTE(d8); fprintf(out, "\tSBC A, %02x", d8); } break;
              case 2: { BYTE(d8); fprintf(out, "\tXOR A, %02x", d8); } break;
              case 3: { BYTE(d8); fprintf(out, "\tCP A, %02x", d8); } break;
            }
          } break;
          case 7: {
            switch(hi2) {
              case 0: { fprintf(out, "\tRST 00H"); } break;
              case 1: { fprintf(out, "\tRST 10H"); } break;
              case 2: { fprintf(out, "\tRST 20H"); } break;
              case 3: { fprintf(out, "\tRST 30H"); } break;
            }
          } break;
          case 15: {
            switch(hi2) {
              case 0: { fprintf(out, "\tRST 08H"); } break;
              case 1: { fprintf(out, "\tRST 18H"); } break;
              case 2: { fprintf(out, "\tRST 28H"); } break;
              case 3: { fprintf(out, "\tRST 38H"); } break;
            }
          } break;
          case 9: {
            switch(hi2) {
              case 0: { fprintf(out, "\tRET"); } break;
              case 1: { fprintf(out, "\tRETI"); } break;
              case 2: { fprintf(out, "\tJP (HL)"); } break;
              case 3: { fprintf(out, "\tLD SP, HL"); } break;
            }
          } break;
        }
      } break;
    }
  
    fprintf(out, "\n");
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

  addr = parse16(saddr);
  
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


