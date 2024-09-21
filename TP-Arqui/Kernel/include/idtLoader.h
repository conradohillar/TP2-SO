//******************************************************************************
// Archivo: idtLoader.h
//******************************************************************************

#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

#include <stdint.h>
#include <defs.h>
#include <interrupts.h>
#include <syscallsDispatcher.h>


//******************************************************************************
// DECLARACIÓN DE PROTOTIPOS
//******************************************************************************

void setup_IDT_entry(int index, uint64_t offset);
void load_idt();




#endif // _IDTLOADER_H_
