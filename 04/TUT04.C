/*
  PMode tutorials in C and Asm
  Copyright (C) 2000 Alexei A. Frounze
  The programs and sources come under the GPL 
  (GNU General Public License), for more information
  read the file gnu-gpl.txt (originally named COPYING).
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#include "pm.h"
#include "pm_defs.h"
#include "isr_wrap.h"

DESCR_SEG gdt[5];			/* GDT */
GDTR gdtr;				/* GDTR */

DESCR_INT idt[0x20];			/* IDT */
IDTR idtr;				/* IDTR */

word old_CS, old_DS, old_SS;

void setup_GDT_entry (DESCR_SEG *item,
                      dword base, dword limit, byte access, byte attribs) {
  item->base_l = base & 0xFFFF;
  item->base_m = (base >> 16) & 0xFF;
  item->base_h = base >> 24;
  item->limit = limit & 0xFFFF;
  item->attribs = attribs | ((limit >> 16) & 0x0F);
  item->access = access;
}

void setup_IDT_entry (DESCR_INT *item,
                      word selector, dword offset, byte access, byte param_cnt) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->param_cnt = param_cnt;
}

void setup_GDT() {
  /* 0x00 -- null descriptor */
  setup_GDT_entry (&gdt[0], 0, 0, 0, 0);

  /* 0x08 -- code segment descriptor */
  setup_GDT_entry (&gdt[1], ((dword)_CS)<<4, 0xFFFF, ACS_CODE, 0);

  /* 0x10 -- data segment descriptor */
  setup_GDT_entry (&gdt[2], ((dword)_DS)<<4, 0xFFFF, ACS_DATA, 0);

  /* 0x18 -- stack segment descriptor */
  setup_GDT_entry (&gdt[3], ((dword)_SS)<<4, 0xFFFF, ACS_STACK, 0);

  /* 0x20 -- text video mode segment descriptor */
  setup_GDT_entry (&gdt[4], 0xB8000L, 0xFFFF, ACS_DATA, 0);

  /* setting up the GDTR register */
  gdtr.base = ((dword)_DS)<<4;
  gdtr.base += (word)&gdt;
  gdtr.limit = sizeof(gdt)-1;
  lgdt (&gdtr);
}

void setup_IDT() {
  /* setting up the exception handlers */
  setup_IDT_entry (&idt[0x00], 0x08, (word)&isr_00_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x01], 0x08, (word)&isr_01_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x02], 0x08, (word)&isr_02_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x03], 0x08, (word)&isr_03_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x04], 0x08, (word)&isr_04_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x05], 0x08, (word)&isr_05_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x06], 0x08, (word)&isr_06_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x07], 0x08, (word)&isr_07_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x08], 0x08, (word)&isr_08_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x09], 0x08, (word)&isr_09_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0A], 0x08, (word)&isr_0A_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0B], 0x08, (word)&isr_0B_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0C], 0x08, (word)&isr_0C_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0D], 0x08, (word)&isr_0D_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0E], 0x08, (word)&isr_0E_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x0F], 0x08, (word)&isr_0F_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x10], 0x08, (word)&isr_10_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x11], 0x08, (word)&isr_11_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x12], 0x08, (word)&isr_12_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x13], 0x08, (word)&isr_13_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x14], 0x08, (word)&isr_14_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x15], 0x08, (word)&isr_15_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x16], 0x08, (word)&isr_16_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x17], 0x08, (word)&isr_17_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x18], 0x08, (word)&isr_18_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x19], 0x08, (word)&isr_19_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1A], 0x08, (word)&isr_1A_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1B], 0x08, (word)&isr_1B_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1C], 0x08, (word)&isr_1C_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1D], 0x08, (word)&isr_1D_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1E], 0x08, (word)&isr_1E_wrapper, ACS_INT, 0);
  setup_IDT_entry (&idt[0x1F], 0x08, (word)&isr_1F_wrapper, ACS_INT, 0);

  /* setting up the IDTR register */
  idtr.base = ((dword)_DS)<<4;
  idtr.base += (word)&idt;
  idtr.limit = sizeof(idt)-1;
  lidt (&idtr);
}

void setup_PMode() {
  /* disable interrupts so that IRQs don't cause exceptions */
  disable();

  /* disable NMIs as well */
  outportb (0x70, inportb(0x70) | 0x80);

  /* setup GDT */
  setup_GDT();

  /* setup IDT */
  setup_IDT();

  /* saving real mode segment addresses */
  old_CS = _CS;
  old_DS = _DS;
  old_SS = _SS;

  /* WOW!!! This switches us to PMode just setting up CR0.PM bit to 1 */
  write_cr0 (read_cr0() | 1L);

  /* loading segment registers with PMode selectors */
  update_cs (0x08);
  _ES = _DS = 0x10;
  _SS = 0x18;
}

void shut_down() {
  /* get out of PMode clearing CR0.PM bit to 0 */
  write_cr0 (read_cr0() & 0xFFFFFFFEL);

  /* restoring real mode segment values */
  update_cs (old_CS);
  _ES = _DS = old_DS;
  _SS = old_SS;

  idtr.base = 0;
  idtr.limit = 0x3FF;
  lidt (&idtr);

  /* enable NMIs */
  outportb (0x70, inportb(0x70) & 0x7F);

  /* enabling interrupts */
  enable();
}

void exc_handler (word exc_no, word cs, dword ip, word error) {
  shut_down();

  textbackground (RED); textcolor (WHITE);
  clreol();
  printf ("exception no: %02XH\n", exc_no);
  clreol();
  printf ("at address  : %04XH:%08XH\n", cs, ip);
  if (exc_has_error[exc_no]) {
    clreol();
    printf ("error code  : %04XH [ Index:%04XH, Type:%d ]\n",
            error, error >> 3, error & 7);
  }

  exit (0);
}

void my_exit() {
  printf ("\nWe're back...\n");
  printf ("\nPMode Tutorial by Alexei A. Frounze (c) 2000\n");
  printf ("E-mail  : alexfru@chat.ru\n");
  printf ("Homepage: http://alexfru.chat.ru\n");
  printf ("Mirror  : http://members.xoom.com/alexfru\n");
  printf ("PMode...: http://welcome.to/pmode\n");
}

int main() {
  int zero=0;

  clrscr();
  printf ("Welcome to the 4th PMode tutorial!\n\n\n");
  atexit (my_exit);

  if (read_msw() & 1) {
    printf ("The CPU is already in PMode.\nAborting...");
    return 0;
  };

  /* setting up pmode */
  setup_PMode();

  /* try to uncomment one of the following lines to see an exception */

/*
  _DS = 0x80;
*/

/**/
  __emit__ (0xCD, 0x20);
/**/

/*
  zero = 1 / zero;
*/

  /* going back to real mode */
  shut_down();

  printf ("\n\nWe're back...\n", zero);
  return 0;
}
