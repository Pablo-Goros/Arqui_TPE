#include "exceptionDispatcher.h"

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(uint64_t exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
    if (exception == INVALID_OPCODE)
        invalid_opcode();
}

static void zero_division() {
    _sti();
    vd_put_string("Zero Division Exception\n", STDERR);
    
}

static void invalid_opcode() {
    _sti();
    vd_put_string("Invalid Opcode Exception\n", STDERR);
}