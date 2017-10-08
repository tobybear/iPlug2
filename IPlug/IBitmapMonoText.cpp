#include "IBitmapMonoText.h"

void DrawBitmapedText(IGraphics& graphics,
                      IBitmap& bitmap,
                      IRECT& rect,
                      IText& text,
                      IChannelBlend* pBlend,
                      const char* str,
                      bool vCenter,
                      bool multiline,
                      int charWidth,
                      int charHeight,
                      int charOffset)
{
  if (CSTR_NOT_EMPTY(str))
  {
    int stringLength = (int) strlen(str);

    int basicYOffset, basicXOffset;

    if (vCenter)
      basicYOffset = rect.T + ((rect.H() - charHeight) / 2);
    else
      basicYOffset = rect.T;
    
    if (text.mAlign == IText::kAlignCenter)
      basicXOffset = rect.L + ((rect.W() - (stringLength * charWidth)) / 2);
    else if (text.mAlign == IText::kAlignNear)
      basicXOffset = rect.L;
    else if (text.mAlign == IText::kAlignFar)
      basicXOffset = rect.R - (stringLength * charWidth);

    int widthAsOneLine = charWidth * stringLength;

    int nLines;
    int stridx = 0;

    int nCharsThatFitIntoLine;

    if(multiline)
    {
      if (widthAsOneLine > rect.W())
      {
        nCharsThatFitIntoLine = rect.W() / charWidth;
        nLines = (widthAsOneLine / rect.W()) + 1;
      }
      else // line is shorter than width of rect
      {
        nCharsThatFitIntoLine = stringLength;
        nLines = 1;
      }
    }
    else
    {
      nCharsThatFitIntoLine = rect.W() / charWidth;
      nLines = 1;
    }

    for(int line=0; line<nLines; line++)
    {
      int yOffset = basicYOffset + line * charHeight;

      for(int linepos=0; linepos<nCharsThatFitIntoLine; linepos++)
      {
        if (str[stridx] == '\0') return;

        int frameOffset = (int) str[stridx++] - 31; // calculate which frame to look up

        int xOffset = (linepos * (charWidth + charOffset)) + basicXOffset;    // calculate xOffset for character we're drawing
        IRECT charRect = IRECT(xOffset, yOffset, xOffset + charWidth, yOffset + charHeight);
        graphics.DrawBitmap(bitmap, charRect, frameOffset, pBlend);
      }
    }
  }
}

void IBitmapTextControl::SetTextFromPlug(char* str)
{
  if (strcmp(mStr.Get(), str))
  {
    SetDirty(false);
    mStr.Set(str);
  }
}

bool IBitmapTextControl::Draw(IGraphics& graphics)
{
  char* cStr = mStr.Get();
  if (CSTR_NOT_EMPTY(cStr))
  {
    DrawBitmapedText(graphics, mTextBitmap, mRECT, mText, &mBlend, cStr, mVCentre, mMultiLine, mCharWidth, mCharHeight, mCharOffset);
  }
  return true;
}
