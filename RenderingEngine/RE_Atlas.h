#pragma once

#include "RE_Settings.h"
#include "RE_Renderer.h"
#include "cJSON.h"

class RE_Atlas;

class RE_AtlasPart
{
public:
    ~RE_AtlasPart();

    int RenderCopy(int idx, const SDL_Rect *dstRect, RE_Anchor anchor);
    int RenderCopyF(int idx, const SDL_FRect *dstRect, RE_Anchor anchor);
    int RenderCopyEx(
        int idx, const SDL_Rect *dstRect, RE_Anchor anchor,
        const double angle, Vec2 center, SDL_RendererFlip flip
    );
    int RenderCopyExF(
        int idx, const SDL_FRect *dstRect, RE_Anchor anchor,
        const double angle, Vec2 center, SDL_RendererFlip flip
    );

    int GetFrameCount();
    SDL_Texture *GetTexture();
    const SDL_Rect *GetSrcRect(int i);

protected:
    friend class RE_Atlas;

    RE_AtlasPart(RE_Atlas &atlas);

    RE_Atlas &m_atlas;
    std::string m_name;
    int *m_frames;
    int m_frameCount;

private:
};

class RE_Atlas
{
public:

    RE_Atlas(SDL_Renderer *renderer, const std::string &path);
    RE_Atlas(RE_Atlas const&) = delete;
    RE_Atlas& operator=(RE_Atlas const&) = delete;
    ~RE_Atlas();

    SDL_Texture *GetTexture();

    RE_AtlasPart *GetPart(const std::string &name);

protected:

    friend class RE_AtlasPart;

    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    RE_AtlasPart **m_parts;
    int m_partCount;

    SDL_Rect *m_rects;
    int m_rectCount;

private:

    void LoadRect(cJSON *jRect, int i);
    void LoadPart(cJSON *jPart, int i);
    void LoadGeometry(cJSON *jGeo);
    char *ParseJSON(cJSON *json);
};

inline int RE_AtlasPart::RenderCopy(
    int idx, const SDL_Rect *dstRect, RE_Anchor anchor)
{
    const SDL_Rect *srcRect = GetSrcRect(idx);
    return RE_RenderCopy(
        m_atlas.m_renderer, m_atlas.m_texture, srcRect, dstRect, anchor
    );
}

inline int RE_AtlasPart::RenderCopyF(
    int idx, const SDL_FRect *dstRect, RE_Anchor anchor)
{
    const SDL_Rect *srcRect = GetSrcRect(idx);
    return RE_RenderCopyF(
        m_atlas.m_renderer, m_atlas.m_texture, srcRect, dstRect, anchor
    );
}

inline int RE_AtlasPart::RenderCopyEx(
    int idx, const SDL_Rect *dstRect, RE_Anchor anchor,
    const double angle, Vec2 center, SDL_RendererFlip flip)
{
    const SDL_Rect *srcRect = GetSrcRect(idx);
    return RE_RenderCopyEx(
        m_atlas.m_renderer, m_atlas.m_texture, srcRect, dstRect, anchor,
        angle, center, flip
    );
}

inline int RE_AtlasPart::RenderCopyExF(
    int idx, const SDL_FRect *dstRect, RE_Anchor anchor,
    const double angle, Vec2 center, SDL_RendererFlip flip)
{
    const SDL_Rect *srcRect = GetSrcRect(idx);
    return RE_RenderCopyExF(
        m_atlas.m_renderer, m_atlas.m_texture, srcRect, dstRect, anchor,
        angle, center, flip
    );
}

inline int RE_AtlasPart::GetFrameCount()
{
    return m_frameCount;
}

inline SDL_Texture *RE_AtlasPart::GetTexture()
{
    return m_atlas.GetTexture();
}

inline const SDL_Rect *RE_AtlasPart::GetSrcRect(int i)
{
    int rectIndex = m_frames[i % m_frameCount];
    return &(m_atlas.m_rects[rectIndex]);
}

inline SDL_Texture *RE_Atlas::GetTexture()
{
    return m_texture;
}
