#include <stdio.h>
#include "utils.h"

int main(void)
{
 int result=0;

 print_welcome();

 result=add(10,20);
 printf("10+20=%d\n",result);

  return 0;
}
