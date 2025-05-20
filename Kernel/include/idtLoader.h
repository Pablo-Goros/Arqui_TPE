
#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

static void setup_IDT_entry(int index, uint64_t offset);

void load_idt();

extern void syscall_stub(void);


#endif // _IDTLOADER_H_
