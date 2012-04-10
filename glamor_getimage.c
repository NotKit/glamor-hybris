/*
 * Copyright © 2009 Intel Corporation
 * Copyright © 1998 Keith Packard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Zhigang Gong <zhigang.gong@gmail.com>
 *
 */

#include "glamor_priv.h"


static Bool
_glamor_get_image(DrawablePtr drawable, int x, int y, int w, int h,
		  unsigned int format, unsigned long planeMask, char *d,
		  Bool fallback)
{
	PixmapPtr pixmap;
	struct glamor_pixmap_private *pixmap_priv;
	struct glamor_screen_private *glamor_priv;
	int x_off, y_off;
	GLenum tex_format, tex_type;
	int no_alpha, revert;
	glamor_pixmap_fbo *temp_fbo = NULL;
	glamor_gl_dispatch * dispatch;
	Bool ret = FALSE;
	int swap_rb;
	int stride;
	void *data;

	if (format != ZPixmap)
		goto fall_back;

	glamor_priv = glamor_get_screen_private(drawable->pScreen);
	pixmap = glamor_get_drawable_pixmap(drawable);
	glamor_get_drawable_deltas(drawable, pixmap, &x_off, &y_off);

	if (!glamor_set_planemask(pixmap, planeMask)) {
		glamor_fallback
		    ("Failedto set planemask  in glamor_solid.\n");
		goto fall_back;
	}
	glamor_priv = glamor_get_screen_private(drawable->pScreen);
	pixmap_priv = glamor_get_pixmap_private(pixmap);


	if (!GLAMOR_PIXMAP_PRIV_HAS_FBO(pixmap_priv))
		goto fall_back;
	stride = PixmapBytePad(w, drawable->depth);

	x += drawable->x + x_off;
	y += drawable->y + y_off;

	data = glamor_download_sub_pixmap_to_cpu(pixmap, x, y, w, h, stride,
						 d, 0, GLAMOR_ACCESS_RO);
	if (data != NULL) {
		ret = TRUE;
		assert(data == d);
	}
fall_back:
	if (ret == FALSE)
		miGetImage(drawable, x, y, w, h, format, planeMask, d);
	return TRUE;
}

void
glamor_get_image(DrawablePtr pDrawable, int x, int y, int w, int h,
		 unsigned int format, unsigned long planeMask, char *d)
{
	_glamor_get_image(pDrawable, x, y, w, h, format, planeMask, d, TRUE);
	return;
}

Bool
glamor_get_image_nf(DrawablePtr pDrawable, int x, int y, int w, int h,
		    unsigned int format, unsigned long planeMask, char *d)
{
	return _glamor_get_image(pDrawable, x, y, w, 
				 h, format, planeMask, d, FALSE);
}
