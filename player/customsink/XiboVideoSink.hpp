#pragma once

#include <gst/video/video-info.h>
#include <cairomm/surface.h>

#include "control/media/player/VideoWindow.hpp"

G_BEGIN_DECLS

#define GST_TYPE_XIBOVIDEOSINK \
  (gst_xibovideosink_get_type())
#define GST_XIBOVIDEOSINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_XIBOVIDEOSINK, XiboVideoSink))
#define GST_XIBOVIDEOSINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_XIBOVIDEOSINK, XiboVideoSinkClass))
#define GST_IS_XIBOVIDEOSINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_XIBOVIDEOSINK))
#define GST_IS_XIBOVIDEOSINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_XIBOVIDEOSINK))

struct XiboVideoSink
{
    GstVideoSink base;

    GstPad* sinkpad;
    GstVideoInfo info;

    VideoWindow* handler = nullptr;
    GstVideoFrame frame;
    bool frameMapped = false;
    Cairo::RefPtr<Cairo::ImageSurface> surface;
};

struct XiboVideoSinkClass
{
    GstVideoSinkClass base;
};

gboolean pluginInit(GstPlugin* plugin);
GType gst_xibovideosink_get_type(void);
void gst_xibovideosink_set_handler(XiboVideoSink* sink, VideoWindow* handler);

G_END_DECLS
