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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstimageblur
 *
 * The imageblur element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! imageblur ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>

#include <opencv2/imgproc.hpp>

#include "gstimageblur.h"

GST_DEBUG_CATEGORY_STATIC (gst_image_blur_debug_category);
#define GST_CAT_DEFAULT gst_image_blur_debug_category

/* prototypes */


static void gst_image_blur_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_image_blur_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_image_blur_dispose (GObject * object);
static void gst_image_blur_finalize (GObject * object);

static gboolean gst_image_blur_start (GstBaseTransform * trans);
static gboolean gst_image_blur_stop (GstBaseTransform * trans);
static gboolean gst_image_blur_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_image_blur_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_image_blur_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME: add/remove formats you can handle */
#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")

/* FIXME: add/remove formats you can handle */
#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstImageBlur, gst_image_blur, GST_TYPE_VIDEO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_image_blur_debug_category, "imageblur", 0,
  "debug category for imageblur element"));

static void
gst_image_blur_class_init (GstImageBlurClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstVideoFilterClass *video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_image_blur_set_property;
  gobject_class->get_property = gst_image_blur_get_property;
  gobject_class->dispose = gst_image_blur_dispose;
  gobject_class->finalize = gst_image_blur_finalize;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_image_blur_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_image_blur_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_image_blur_set_info);
  // video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_image_blur_transform_frame);
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_image_blur_transform_frame_ip);

}

static void
gst_image_blur_init (GstImageBlur *imageblur)
{
}

void
gst_image_blur_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (object);

  GST_DEBUG_OBJECT (imageblur, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_image_blur_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (object);

  GST_DEBUG_OBJECT (imageblur, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_image_blur_dispose (GObject * object)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (object);

  GST_DEBUG_OBJECT (imageblur, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_image_blur_parent_class)->dispose (object);
}

void
gst_image_blur_finalize (GObject * object)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (object);

  GST_DEBUG_OBJECT (imageblur, "finalize");

  /* clean up object here */
  imageblur->cv_in_image.release ();
  imageblur->cv_out_image.release ();


  G_OBJECT_CLASS (gst_image_blur_parent_class)->finalize (object);
}

static gboolean
gst_image_blur_start (GstBaseTransform * trans)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (trans);

  GST_DEBUG_OBJECT (imageblur, "start");

  return TRUE;
}

static gboolean
gst_image_blur_stop (GstBaseTransform * trans)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (trans);

  GST_DEBUG_OBJECT (imageblur, "stop");

  return TRUE;
}

static gboolean
gst_image_blur_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (filter);

  GST_DEBUG_OBJECT (imageblur, "set_info");

  imageblur->in_width = GST_VIDEO_INFO_WIDTH (in_info);
  imageblur->in_height = GST_VIDEO_INFO_HEIGHT (in_info);
  imageblur->in_cv_type = CV_8UC4;

  imageblur->out_width = GST_VIDEO_INFO_WIDTH (out_info);
  imageblur->out_height = GST_VIDEO_INFO_HEIGHT (out_info);
  imageblur->out_cv_type = CV_8UC4;


  int in_width = imageblur->in_width;
  int in_height = imageblur->in_height;
  int in_cv_type = imageblur->in_cv_type;
  imageblur->cv_in_image.create(cv::Size (in_width, in_height), in_cv_type);

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_image_blur_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (filter);
  
  gst_printerrln("Printing from custom video filter");
  imageblur->cv_in_image.data = (unsigned char *) inframe->data[0];
  imageblur->cv_in_image.datastart = (unsigned char *) inframe->data[0];

  gst_printerrln("input image width: %d", imageblur->in_width);
  gst_printerrln("input image height: %d", imageblur->in_height);

  GST_DEBUG_OBJECT (imageblur, "transform_frame");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_image_blur_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  GstImageBlur *imageblur = GST_IMAGE_BLUR (filter);

  GST_DEBUG_OBJECT (imageblur, "transform_frame_ip");

  // gst_printerrln("Printing from gst_image_blur_transform_frame_ip");
  // gst_printerrln("input image width: %d", imageblur->in_width);
  // gst_printerrln("input image height: %d", imageblur->in_height);

  imageblur->cv_in_image.data = (unsigned char *) frame->data[0];
  imageblur->cv_in_image.datastart = (unsigned char *) frame->data[0];

  blur(imageblur->cv_in_image, imageblur->cv_in_image, cv::Size(11,11));

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "imageblur", GST_RANK_NONE,
      GST_TYPE_IMAGE_BLUR);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    imageblur,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

