/* GStreamer
 * Copyright (C) 2022 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_IMAGE_BLUR_H_
#define _GST_IMAGE_BLUR_H_

#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>

#include <opencv2/core.hpp>

G_BEGIN_DECLS

#define GST_TYPE_IMAGE_BLUR   (gst_image_blur_get_type())
#define GST_IMAGE_BLUR(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_IMAGE_BLUR,GstImageBlur))
#define GST_IMAGE_BLUR_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_IMAGE_BLUR,GstImageBlurClass))
#define GST_IS_IMAGE_BLUR(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_IMAGE_BLUR))
#define GST_IS_IMAGE_BLUR_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_IMAGE_BLUR))

typedef struct _GstImageBlur GstImageBlur;
typedef struct _GstImageBlurClass GstImageBlurClass;

struct _GstImageBlur
{
  GstVideoFilter base_imageblur;

  cv::Mat cv_in_image;
  cv::Mat cv_out_image;

  int in_width;
  int in_height;
  int in_cv_type;

  int out_width;
  int out_height;
  int out_cv_type;

};

struct _GstImageBlurClass
{
  GstVideoFilterClass base_imageblur_class;
};

GType gst_image_blur_get_type (void);

G_END_DECLS

#endif
