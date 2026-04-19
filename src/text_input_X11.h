/*!
 * @file text_input_X11.h
 * @brief Helper function for robtk LV2 UIs. It implements an X11 
 *        text input window used by the Arp Plugin
 *
 *
 *      Copyright 2009 - 2026 <qmidiarp-devel@lists.sourceforge.net>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 256


static void refresh(Display *dpy, Window win, GC gc, const char *buf, int cursor)
{
    XClearWindow(dpy, win);
    
    XDrawString(dpy, win, gc, 10, 30, buf, strlen(buf));

    int cx = 10 + XTextWidth(
        XQueryFont(dpy, XGContextFromGC(gc)),
        buf, cursor);

    XDrawLine(dpy, win, gc, cx, 10, cx, 40);
}

static char* x11_text_input(const char *initial) {
    setlocale(LC_ALL, "");

    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) return NULL;

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    int w = 500, h = 60;

    Window win = XCreateSimpleWindow(
        dpy, root,
        200, 200, w, h, 1,
        WhitePixel(dpy, screen),
        BlackPixel(dpy, screen)
    );

    XStoreName(dpy, win, "QMidiArp Pattern");    
    Atom wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wm_delete, 1);    
    
    XSelectInput(dpy, win,
        ExposureMask | KeyPressMask | StructureNotifyMask);

    XMapWindow(dpy, win);

    /* --- Input method (UTF-8 support) --- */
    XIM xim = XOpenIM(dpy, NULL, NULL, NULL);
    XIC xic = NULL;

    if (xim) {
        xic = XCreateIC(xim,
            XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow, win,
            XNFocusWindow, win,
            NULL);
    }

    /* --- Text buffer --- */
    char buf[BUF_SIZE];
    strncpy(buf, initial ? initial : "", BUF_SIZE-1);
    buf[BUF_SIZE-1] = 0;

    int cursor = strlen(buf);

    GC gc = DefaultGC(dpy, screen);
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));

    int running = 1;

    while (running) {
        XEvent ev;
        XNextEvent(dpy, &ev);

        if (ev.type == Expose) {
            refresh(dpy, win, gc, buf, cursor);            
       }

        else if (ev.type == KeyPress) {
            char text[32];
            KeySym ks;
            Status status;

            int len = 0;

            if (xic) {
                len = Xutf8LookupString(
                    xic, &ev.xkey,
                    text, sizeof(text)-1,
                    &ks, &status);
            } else {
                len = XLookupString(
                    &ev.xkey, text, sizeof(text)-1,
                    &ks, NULL);
            }

            text[len] = 0;

            switch (ks) {
            case XK_Return:
                running = 0;
                break;

            case XK_Escape:
                buf[0] = 0;
                running = 0;
                break;

            case XK_BackSpace:
                if (cursor > 0) {
                    memmove(&buf[cursor-1],
                            &buf[cursor],
                            strlen(buf) - cursor + 1);
                    cursor--;
                }
                break;

            case XK_Left:
                if (cursor > 0) cursor--;
                break;

            case XK_Right:
                if (cursor < (int)strlen(buf)) cursor++;
                break;

            default:
                if (len > 0 && strlen(buf) + len < BUF_SIZE-1) {
                    memmove(buf + cursor + len,
                            buf + cursor,
                            strlen(buf) - cursor + 1);

                    memcpy(buf + cursor, text, len);
                    cursor += len;
                }
                break;
            }
            refresh(dpy, win, gc, buf, cursor);
        }
        else if (ev.type == ClientMessage) {
            if ((Atom)ev.xclient.data.l[0] == wm_delete) {
                /* Treat like Enter */
                running = 0;
            }
        }
    }

    if (xic) XDestroyIC(xic);
    if (xim) XCloseIM(xim);

    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    if (strlen(buf) == 0)
        return NULL;

    return strdup(buf);
}
