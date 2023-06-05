#pragma once

#include "RE_Settings.h"
#include "RE_Utils.h"

enum class RE_Anchor : uint32_t
{
    NORTH_WEST, NORTH_EAST, NORTH,
    EAST, CENTER, WEST,
    SOUTH_WEST, SOUTH, SOUTH_EAST
};

int RE_RenderCopy(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_Rect *dstRect, RE_Anchor anchor
);

int RE_RenderCopyF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, RE_Anchor anchor
);

int RE_RenderCopyEx(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_Rect *dstRect, RE_Anchor anchor,
    const double angle, Vec2 center, SDL_RendererFlip flip
);

int RE_RenderCopyExF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, RE_Anchor anchor,
    const double angle, Vec2 center, SDL_RendererFlip flip
);

int RE_RenderCopyBordersF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, RE_Anchor anchor,
    int l, int r, int t, int b
);
