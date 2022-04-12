#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
// NOTE: Figure out how this includes
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(argc, argv)
int argc;
char **argv;
{
  char hello[] = "Hello World!";
  char hi[] = "hi!";

  Display *myDisplay;
  Window myWindow;

  GC myGC;

  XEvent myEvent;
  KeySym myKey;

  XSizeHints myHint;

  int myScreen;
  unsigned long myForeground, myBackground;
  int i;
  char text[10];
  int done;

  // Opens default display
  // Basically entry point for X11 client, not a physicall display
  if (!(myDisplay = XOpenDisplay(NULL)))
  {
    errx(1, "cannot open display '%s'", XDisplayName(0));
  }

  myScreen = DefaultScreen(myDisplay);
  myForeground = BlackPixel(myDisplay, myScreen);
  myBackground = WhitePixel(myDisplay, myScreen);
  myHint.x = 200;
  myHint.y = 100;
  myHint.width = 350;
  myHint.height = 250;
  myHint.flags = PPosition | PSize;

  myWindow = XCreateSimpleWindow(myDisplay, DefaultRootWindow(myDisplay), myHint.x, myHint.y,
                                 myHint.width, myHint.height, 5, myForeground, myBackground);

  /* window manager properties (yes, use of StdProp is obsolete) */
  XSetStandardProperties(myDisplay, myWindow, hello, hello,
                         None, argv, argc, &myHint);

  /* graphics context */
  myGC = XCreateGC(myDisplay, myWindow, 0, 0);
  XSetBackground(myDisplay, myGC, myBackground);
  XSetForeground(myDisplay, myGC, myForeground);

  /* allow receiving mouse events */
  XSelectInput(myDisplay, myWindow,
               ButtonPressMask | KeyPressMask | ExposureMask);

  /* show up window */
  XMapRaised(myDisplay, myWindow);

  /* event loop */
  done = 0;
  while (done == 0)
  {

    /* fetch event */
    XNextEvent(myDisplay, &myEvent);

    switch (myEvent.type)
    {

    case Expose:
      /* Window was showed. */
      if (myEvent.xexpose.count == 0)
        XDrawImageString(myEvent.xexpose.display,
                         myEvent.xexpose.window,
                         myGC,
                         50, 50,
                         hello, strlen(hello));
      break;
    case MappingNotify:
      /* Modifier key was up/down. */
      XRefreshKeyboardMapping(&myEvent.xmapping);
      break;
    case ButtonPress:
      /* Mouse button was pressed. */
      XDrawImageString(myEvent.xbutton.display,
                       myEvent.xbutton.window,
                       myGC,
                       myEvent.xbutton.x, myEvent.xbutton.y,
                       hi, strlen(hi));
      break;
    case KeyPress:
      /* Key input. */
      i = XLookupString(&myEvent.xkey, text, 10, &myKey, 0);
      if (i == 1 && text[0] == 'q')
        done = 1;
      break;
    }
  }

  /* finalization */
  XFreeGC(myDisplay, myGC);
  XDestroyWindow(myDisplay, myWindow);
  XCloseDisplay(myDisplay);
  return 0;
}
