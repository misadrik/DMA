#include <asm/page.h>
#define virt_to_pfn(kaddr) (__pa(kaddr)>>PAGE_SHIFT)