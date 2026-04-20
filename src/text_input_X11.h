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
    const int dyt = 12;
    const int offset = 4;
    const int i = 5;

    int cx = XTextWidth(XQueryFont(dpy, XGContextFromGC(gc)),
                buf, cursor);
    
    XClearArea(dpy, win, 0, dyt*(i + 1) + offset + 1, 1000, dyt - 1, true);
    XDrawString(dpy, win, gc, 0, dyt * (i + 2)+offset, buf, strlen(buf));
    XDrawLine(dpy, win, gc, cx, dyt*(i + 2) + offset, cx, dyt*(i + 1) + offset);
}

static void setup_window(Display *dpy, Window win, GC gc, const char *buf, int cursor)
{
    const char* hlp[5];
    const int dyt = 12;
    const int offset = 4;
    
    hlp[0] = "0..9   note played on keyboard, 0 is lowest                < . >  tempo up / reset / down";
    hlp[1] = "( )    numbers in parentheses are stacked to chords        d h    note length up / down";
    hlp[2] = "+ = -  octave up / reset / down                            / \\    velocity up / down";
    hlp[3] = "t = g  semitone up / reset / down                          p      pause";
    
    hlp[4] = "Enter Pattern (ESC: Cancel, Enter: Accept):";
    
    int cx = XTextWidth(XQueryFont(dpy, XGContextFromGC(gc)),
                buf, cursor);

    int sep_line_x = 56*dyt/2;
    
    int i = 0;
    for (i = 0;i < 4;i++) {
        XDrawString(dpy, win, gc, 10, dyt * (i + 1), hlp[i], strlen(hlp[i]));
    }
    
    XDrawLine(dpy, win, gc, sep_line_x, 0, sep_line_x, dyt*(i) + offset);
    
    i++;
    
    XDrawString(dpy, win, gc, 0, dyt * (i + 1), hlp[4], strlen(hlp[4]));
    
    XDrawLine(dpy, win, gc, 0, dyt*(i + 1) + offset - 2, 1000, dyt*(i + 1) + offset-2);
    XDrawLine(dpy, win, gc, 0, dyt*(i + 1) + offset, 1000, dyt*(i + 1) + offset);
    XDrawString(dpy, win, gc, 0, dyt * (i + 2) + offset, buf, strlen(buf));
    XDrawLine(dpy, win, gc, 0, dyt*(i + 2) + offset, 1000, dyt*(i + 2) + offset);

    XDrawLine(dpy, win, gc, cx, dyt*(i + 2) + offset, cx, dyt*(i + 1) + offset);
}

static char* x11_text_input(const char *initial) {
    setlocale(LC_ALL, "");

    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) return NULL;

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    int w = 600, h = 95;

    XColor bg_color, fg_color;
    Colormap colormap;
    char grey[] = "#383838";
    char white[] = "#C0C0C0";
    
    colormap = DefaultColormap(dpy, 0);
    XParseColor(dpy, colormap, grey, &bg_color);
    XAllocColor(dpy, colormap, &bg_color);
    XParseColor(dpy, colormap, white, &fg_color);
    XAllocColor(dpy, colormap, &fg_color);


    Window win = XCreateSimpleWindow(
        dpy, root,
        200, 200, w, h, 1,
        // WhitePixel(dpy, screen),
        // BlackPixel(dpy, screen)
        fg_color.pixel,
        bg_color.pixel
    );

    XStoreName(dpy, win, "QMidiArp Edit Pattern");    
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
    XSetForeground(dpy, gc, fg_color.pixel);
    
    int running = 1;

    while (running) {
        XEvent ev;
        XNextEvent(dpy, &ev);

        if (ev.type == Expose) {
            setup_window(dpy, win, gc, buf, cursor);
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
                /* Treat like Escape */
                buf[0] = 0;
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
