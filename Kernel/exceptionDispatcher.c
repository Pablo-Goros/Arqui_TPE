// exceptionDispatcher.c
#include <naiveConsole.h>
#include <stdint.h>


#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6



extern void _sti();

static void zero_division();
static void invalid_opcode();

// Se rellena el contexto con tu macro pushState → en la pila queda el snapshot
extern void dumpRegisters(void);

void exceptionDispatcher(uint64_t exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
    if (exception == INVALID_OPCODE)
        invalid_opcode();
}

static void zero_division() {
    _sti();
    ncPrint("Zero Division Error");
}

static void invalid_opcode() {
    _sti();
    ncPrint("Invalid Opcode");
}