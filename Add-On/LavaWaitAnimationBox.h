// license: public domain
// authors: jonas.sundstrom@kirilla.com
#ifndef _ANIMATIONBOX_H_
#define _ANIMATIONBOX_H_

#include <stdlib.h>

#include <Window.h>
#include <View.h>
#include <Box.h>
#include <Bitmap.h>

class AnimationBox : public BBox {
	public:
		AnimationBox(BRect a_rect, const char * a_name, uint32 a_resizing_mode, uint32 a_flags);
		~AnimationBox();
		
		void Start();
		virtual void Pulse();
		virtual void Draw(BRect a_draw);
		virtual void FrameMoved(BPoint a_point);
		virtual void FrameResized(float a_width, float a_height);
	protected:
		void CreateBitmap(void);
		void LightenBitmapHighColor(rgb_color *a_color);
		void DrawIntoBitmap(void);
		
		bool m_is_running;
		pattern m_pattern;
		
		BBitmap	*m_barberpole_bitmap;
		BView *m_barberpole_bitmap_view;
};

#endif
