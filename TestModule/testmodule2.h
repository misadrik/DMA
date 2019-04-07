#include <asm/page.h>
#define virt_to_pfn(kaddr)  (__pa(kaddr) >> PAGE_SHIFT)
//#define virt_to_pfn(v)          (PFN_DOWN(__pa(v)))