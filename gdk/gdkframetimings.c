/* GDK - The GIMP Drawing Kit
 * Copyright (C) 2012 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gdkframetimings.h"

struct _GdkFrameTimings
{
  guint ref_count;

  gint64 frame_counter;
  guint64 cookie;
  gint64 frame_time;
  gint64 drawn_time;
  gint64 presentation_time;
  gint64 refresh_interval;
  gint64 predicted_presentation_time;

#ifdef G_ENABLE_DEBUG
  gint64 layout_start_time;
  gint64 paint_start_time;
  gint64 frame_end_time;
#endif /* G_ENABLE_DEBUG */

  guint complete : 1;
  guint slept_before : 1;
};

G_DEFINE_BOXED_TYPE (GdkFrameTimings, gdk_frame_timings,
                     gdk_frame_timings_ref,
                     gdk_frame_timings_unref)

GdkFrameTimings *
gdk_frame_timings_new (gint64 frame_counter)
{
  GdkFrameTimings *timings;

  timings = g_slice_new0 (GdkFrameTimings);
  timings->ref_count = 1;
  timings->frame_counter = frame_counter;

  return timings;
}

GdkFrameTimings *
gdk_frame_timings_ref (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, NULL);

  timings->ref_count++;

  return timings;
}

void
gdk_frame_timings_unref (GdkFrameTimings *timings)
{
  g_return_if_fail (timings != NULL);
  g_return_if_fail (timings->ref_count > 0);

  timings->ref_count--;
  if (timings->ref_count == 0)
    {
      g_slice_free (GdkFrameTimings, timings);
    }
}

gint64
gdk_frame_timings_get_frame_counter (GdkFrameTimings *timings)
{
  return timings->frame_counter;
}

guint64
gdk_frame_timings_get_cookie (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->cookie;
}

void
gdk_frame_timings_set_cookie (GdkFrameTimings *timings,
                              guint64          cookie)
{
  g_return_if_fail (timings != NULL);

  timings->cookie = cookie;
}

gboolean
gdk_frame_timings_get_complete (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, FALSE);

  return timings->complete;
}

void
gdk_frame_timings_set_complete (GdkFrameTimings *timings,
                                gboolean         complete)
{
  g_return_if_fail (timings != NULL);

  timings->complete = complete;
}

gboolean
gdk_frame_timings_get_slept_before (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, FALSE);

  return timings->slept_before;
}

void
gdk_frame_timings_set_slept_before (GdkFrameTimings *timings,
                                    gboolean         slept_before)
{
  g_return_if_fail (timings != NULL);

  timings->slept_before = slept_before;
}

gint64
gdk_frame_timings_get_frame_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->frame_time;
}

void
gdk_frame_timings_set_frame_time (GdkFrameTimings *timings,
                                  gint64           frame_time)
{
  g_return_if_fail (timings != NULL);

  timings->frame_time = frame_time;
}

gint64
gdk_frame_timings_get_drawn_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->drawn_time;
}

void
gdk_frame_timings_set_drawn_time (GdkFrameTimings *timings,
                                  gint64           drawn_time)
{
  g_return_if_fail (timings != NULL);

  timings->drawn_time = drawn_time;
}

gint64
gdk_frame_timings_get_presentation_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->presentation_time;
}

void
gdk_frame_timings_set_presentation_time (GdkFrameTimings *timings,
                                         gint64           presentation_time)
{
  g_return_if_fail (timings != NULL);

  timings->presentation_time = presentation_time;
}

gint64
gdk_frame_timings_get_predicted_presentation_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->predicted_presentation_time;
}

void
gdk_frame_timings_set_predicted_presentation_time (GdkFrameTimings *timings,
                                                   gint64           predicted_presentation_time)
{
  g_return_if_fail (timings != NULL);

  timings->predicted_presentation_time = predicted_presentation_time;
}

gint64
gdk_frame_timings_get_refresh_interval (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->refresh_interval;
}

void
gdk_frame_timings_set_refresh_interval (GdkFrameTimings *timings,
                                        gint64           refresh_interval)
{
  g_return_if_fail (timings != NULL);

  timings->refresh_interval = refresh_interval;
}

#ifdef G_ENABLE_DEBUG
gint64
_gdk_frame_timings_get_layout_start_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->layout_start_time;
}

void
_gdk_frame_timings_set_layout_start_time (GdkFrameTimings *timings,
                                          gint64           layout_start_time)
{
  g_return_if_fail (timings != NULL);

  timings->layout_start_time = layout_start_time;
}

gint64
_gdk_frame_timings_get_paint_start_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->paint_start_time;
}

void
_gdk_frame_timings_set_paint_start_time (GdkFrameTimings *timings,
                                         gint64           paint_start_time)
{
  g_return_if_fail (timings != NULL);

  timings->paint_start_time = paint_start_time;
}

gint64
_gdk_frame_timings_get_frame_end_time (GdkFrameTimings *timings)
{
  g_return_val_if_fail (timings != NULL, 0);

  return timings->frame_end_time;
}

void
_gdk_frame_timings_set_frame_end_time (GdkFrameTimings *timings,
                                       gint64           frame_end_time)
{
  g_return_if_fail (timings != NULL);

  timings->frame_end_time = frame_end_time;
}

#endif /* G_ENABLE_DEBUG */