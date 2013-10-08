//
// Copyright (c) 2008-2013 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "ArrayPtr.h"
#include "Resource.h"

namespace Urho3D
{

class Font;
class Graphics;
class Image;
class Texture;

static const int FONT_TEXTURE_MIN_SIZE = 128;
static const int FONT_TEXTURE_MAX_SIZE = 2048;
static const int FONT_DPI = 96;

/// %Font glyph description.
struct FontGlyph
{
    /// Construct.
    FontGlyph();
    
    /// X position in texture.
    short x_;
    /// Y position in texture.
    short y_;
    /// Width.
    short width_;
    /// Height.
    short height_;
    /// Glyph X offset from origin.
    short offsetX_;
    /// Glyph Y offset from origin.
    short offsetY_;
    /// Horizontal advance.
    short advanceX_;
    /// Page.
    unsigned page_;
    /// Kerning information.
    HashMap<unsigned, unsigned> kerning_;
};

/// %Font file type.
enum FONT_TYPE
{
    FONT_NONE = 0,
    FONT_TTF,
    FONT_BITMAP,
    MAX_FONT_TYPES
};

/// %Font face description.
class URHO3D_API FontFace : public RefCounted
{
public:
    /// Construct.
    FontFace(Font* font, int pointSize);
    /// Destruct.
    virtual ~FontFace();
    
    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize) = 0;
    /// Return pointer to the glyph structure corresponding to a character. Return null if glyph not found.
    const FontGlyph* GetGlyph(unsigned c) const;
    /// Return the kerning for a character and the next character.
    short GetKerning(unsigned c, unsigned d) const;
    /// Return true when one of the texture has a data loss.
    bool IsDataLost() const;
    /// Load font face texture from image resource.
    SharedPtr<Texture> LoadFaceTexture(SharedPtr<Image> image);
    /// Return total texture size.
    unsigned GetTotalTextureSize() const;

    /// Font.
    WeakPtr<Font> font_;
    /// Point size.
    int pointSize_;
    /// Texture.
    Vector<SharedPtr<Texture> > textures_;
    /// Glyphs.
    Vector<FontGlyph> glyphs_;
    /// Row height.
    int rowHeight_;
    /// Glyph index mapping.
    HashMap<unsigned, unsigned> glyphMapping_;
    /// Kerning flag.
    bool hasKerning_;
};

/// Ture type font face description.
class URHO3D_API FontFaceTTF : public FontFace
{
public:
    /// Construct.
    FontFaceTTF(Font* font, int pointSize);
    /// Destruct.
    virtual ~FontFaceTTF();

    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize);
};

/// Bitmap font face description.
class URHO3D_API FontFaceBitmap : public FontFace
{
public:
    /// Construct.
    FontFaceBitmap(Font* font, int pointSize);
    /// Destruct.
    virtual ~FontFaceBitmap();

    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize);
};

/// %Font resource.
class URHO3D_API Font : public Resource
{
    OBJECT(Font);
    
public:
    /// Construct.
    Font(Context* context);
    /// Destruct.
    virtual ~Font();
    /// Register object factory.
    static void RegisterObject(Context* context);
    /// Load resource. Return true if successful.
    virtual bool Load(Deserializer& source);
    /// Return font face. Pack and render to a texture if not rendered yet. Return null on error.
    const FontFace* GetFace(int pointSize);
    
private:
    /// Return True-type font face. Called internally. Return null on error.
    const FontFace* GetFaceTTF(int pointSize);
    /// Return bitmap font face. Called internally. Return null on error.
    const FontFace* GetFaceBitmap(int pointSize);
    
    /// Created faces.
    HashMap<int, SharedPtr<FontFace> > faces_;
    /// Font data.
    SharedArrayPtr<unsigned char> fontData_;
    /// Size of font data.
    unsigned fontDataSize_;
    /// Font type.
    FONT_TYPE fontType_;
};

}
