#include "FrameBuffer.h"

FrameBuffer::
FrameBuffer():
   _id(0)
{
   glGenFramebuffers(1, &_id);
}

FrameBuffer::
~FrameBuffer()
{
   glDeleteFramebuffers(1, &_id);
}

void FrameBuffer::
bind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void FrameBuffer::
unbind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
