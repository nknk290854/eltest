typedef unsigned long uint64_t;
typedef long int64_t;
typedef unsigned int uint32_t;

void  uart0_puts(char*);
void  uart0_hex(uint64_t h);
void  uart0_byte(uint64_t h);

struct Context {
  uint64_t     x0;
  uint64_t     x1;
  uint64_t     x2;
  uint64_t     x3;
  uint64_t     x4;
  uint64_t     x5;
  uint64_t     x6;
  uint64_t     x7;
  uint64_t     x8;
  uint64_t     x9;
  uint64_t    x10;
  uint64_t    x11;
  uint64_t    x12;
  uint64_t    x13;
  uint64_t    x14;
  uint64_t    x15;
  uint64_t    x16;
  uint64_t    x17;
  uint64_t    x18;
  uint64_t    x19;
  uint64_t    x20;
  uint64_t    x21;
  uint64_t    x22;
  uint64_t    x23;
  uint64_t    x24;
  uint64_t    x25;
  uint64_t    x26;
  uint64_t    x27;
  uint64_t    x28;
  uint64_t    x29;
  uint64_t    x30;  // link register
  uint64_t    elr;  // exception link register
  unsigned int spsr; // saved program status register
  char dummy[12];
};

//------------------------------------------------------------------------------

unsigned int  get_esr_el3(){
  int esr;
  asm volatile ("mrs %0, esr_el3" : "=r"(esr));
  return esr;
}

unsigned long  get_far_el3(){
  long far;
  asm volatile ("mrs %0, far_el3" : "=r"(far));
  return far;
}

// from the current EL using the current SP0

void curr_el_sp0_sync_el3(struct Context *ctx) {
    uart0_puts("EL3 exception: SP0 Sync\n");
}


void curr_el_sp0_irq_el3(struct Context *ctx) {
    uart0_puts("EL3 exception: SP0 IRQ\n");
}


void curr_el_sp0_fiq_el3(struct Context *ctx) {
    uart0_puts("EL3 exception: SP0 FIQ\n");
}


void curr_el_sp0_serror_el3(struct Context *ctx) {
    uart0_puts("EL3 exception: SP0 Error\n");
}

// from the current EL using the current SP

void curr_el_spx_sync_el3(struct Context *ctx) {
  // let r = unsafe { &*ctx };
    //uart0_puts("EL3 exception: SPX Sync\nELR = 0x, ESR=0x");
    uart0_puts("EL3 exception: SPX Sync\n");
    //driver::uart::hex(r.elr);
    //    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nFAR = 0x");
    uart0_hex(get_far_el3());
    uart0_puts("\n***\n\n");
    uart0_puts("\nESR = 0x");
    uart0_hex(get_esr_el3());
    uart0_puts("\n");
}


void curr_el_spx_irq_el3(struct Context *_ctx) {
    uart0_puts("EL3 exception: SPX IRQ\n");
}


void curr_el_spx_fiq_el3(struct Context *_ctx) {
    uart0_puts("EL3 exception: SPX FIQ\n");
}


void curr_el_spx_serror_el3(struct Context *_ctx) {
  //let r = unsafe { &*ctx };
    uart0_puts("EL3 exception: SPX Error\nELR = 0x");
    //driver::uart::hex(r.elr);
    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nESR = 0x");
    //driver::uart::hex(get_esr_el3() as u64);
    uart0_puts("\n");
}

// from lower EL (AArch64)

void lower_el_aarch64_sync_el3(struct Context *_ctx) {

}


void lower_el_aarch64_irq_el3(struct Context *_ctx) {

}


void lower_el_aarch64_fiq_el3(struct Context *_ctx) {

}


void lower_el_aarch64_serror_el3(struct Context *_ctx) {

}

// from lower EL (AArch32)

void lower_el_aarch32_sync_el3(struct Context *_ctx) {

}


void lower_el_aarch32_irq_el3(struct Context *_ctx) {

}


void lower_el_aarch32_fiq_el3(struct Context *_ctx) {

}


void lower_el_aarch32_serror_el3(struct Context *_ctx) {

}

//------------------------------------------------------------------------------

int get_esr_el2(){
    int esr;
    asm volatile ("mrs %0, esr_el2" : "=r"(esr));
    return esr;
}

// from the current EL using the current SP0

void curr_el_sp0_sync_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SP0 Sync\n");
}


void curr_el_sp0_irq_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SP0 IRQ\n");
}


void curr_el_sp0_fiq_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SP0 FIQ\n");
}


void curr_el_sp0_serror_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SP0 Error\n");
}


void curr_el_spx_sync_el2(struct  Context *_ctx) {
  //let r = unsafe { &*ctx };
    uart0_puts("EL2 exception: SPX Sync\nELR = 0x");
    //driver::uart::hex(r.elr);
    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nESR = 0x");
    //driver::uart::hex(get_esr_el2() as u64);
    uart0_puts("\n");
}


void curr_el_spx_irq_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SPX IRQ\n");
}


void curr_el_spx_fiq_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SPX FIQ\n");
}


void curr_el_spx_serror_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: SPX Error\n");
}

// from lower EL (AArch64)

void lower_el_aarch64_sync_el2(struct Context *_ctx) {
  //let r = unsafe { &*ctx };
    uart0_puts("EL2 exception: Sync lower AArch64\nELR = ");
    //driver::uart::hex(r.elr);
    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nESR = 0x");
    //driver::uart::hex(get_esr_el2() as u64);
    uart0_puts("\n");
}


void lower_el_aarch64_irq_el2(struct Context *_ctx) {

}


void lower_el_aarch64_fiq_el2(struct Context *_ctx) {

}


void lower_el_aarch64_serror_el2(struct Context *_ctx) {
    uart0_puts("EL2 exception: Error lower AArch64\n");
}

// from lower EL (AArch32)

void lower_el_aarch32_sync_el2(struct Context *_ctx) {

}


void lower_el_aarch32_irq_el2(struct Context *_ctx) {

}


void lower_el_aarch32_fiq_el2(struct Context *_ctx) {

}


void lower_el_aarch32_serror_el2(struct Context *_ctx) {

}

//------------------------------------------------------------------------------

int get_esr_el1(){
    int esr;
    asm volatile("mrs %0, esr_el1" : "=r"(esr)); 
    return esr;
}

// from the current EL using the current SP0

void curr_el_sp0_sync_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SP0 Sync\n");
}


void curr_el_sp0_irq_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SP0 IRQ\n");
}


void curr_el_sp0_fiq_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SP0 FIQ\n");
}


void curr_el_sp0_serror_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SP0 Error\n");
}


void curr_el_spx_sync_el1(struct Context *_ctx) {
  //let r = unsafe { &*ctx };
    uart0_puts("EL1 exception: SPX Sync\nELR = 0x");
    //driver::uart::hex(r.elr);
    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nESR = 0x");
    //driver::uart::hex(get_esr_el1() as u64);
    uart0_puts("\n");
}


void curr_el_spx_irq_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SPX IRQ\n");
}


void curr_el_spx_fiq_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SPX FIQ\n");
}


void curr_el_spx_serror_el1(struct Context *_ctx) {
    uart0_puts("EL1 exception: SPX Error\n");
}

// from lower EL (AArch64)

void lower_el_aarch64_sync_el1(struct Context *_ctx) {
    //let r = unsafe { &*ctx };
    uart0_puts("EL1 exception: lower EL Sync\nELR = 0x");
    //driver::uart::hex(r.elr);
    uart0_puts("\nSPSR = 0x");
    //driver::uart::hex(r.spsr as u64);
    uart0_puts("\nESR = 0x");
    //driver::uart::hex(get_esr_el1() as u64);
    uart0_puts("\n");
}


void lower_el_aarch64_irq_el1(struct Context *_ctx) {

}


void lower_el_aarch64_fiq_el1(struct Context *_ctx) {

}


void lower_el_aarch64_serror_el1(struct Context *_ctx) {

}

// from lower EL (AArch32)

void lower_el_aarch32_sync_el1(struct Context *_ctx) {

}


void lower_el_aarch32_irq_el1(struct Context *_ctx) {

}


void lower_el_aarch32_fiq_el1(struct Context *_ctx) {

}


void lower_el_aarch32_serror_el1(struct Context *_ctx) {

}
