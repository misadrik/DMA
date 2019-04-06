#include <asm/page.h>
#define virt_to_pfn(v)          (PFN_DOWN(__pa(v)))