#include <stdio.h>
#include <err.h>
// NOTE: Figure out how this includes
#include <X11/Xlib.h>

int main(int argc, char const *argv[])
{
  Display *myDisplay;

  if (!(myDisplay = XOpenDisplay(0)))
  {
    errx(1, "cannot open display '%s'", XDisplayName(0));
  }
  else
  {
    printf("Display found\n");
  }

  printf("\n");
  return 0;
}
