#include "Image.h"

Image::Image(Scene &scene, RE_AtlasPart *atlasPart, int index) :
    UIObject(scene), m_texture(nullptr), m_borders(nullptr), m_index(0), m_srcRect()
{
    m_name = "Image";

    m_srcRect = *atlasPart->GetSrcRect(index);
    m_texture = atlasPart->GetTexture();
}