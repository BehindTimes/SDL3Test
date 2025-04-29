#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"
#include <SDL3/SDL.h>
#include <iostream>

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Misc> m_misc;

void U3Graphics::renderDiorama()
{
    SDL_FRect fromRect{};
    SDL_FRect fromRect1{};
    SDL_FRect outRect{};
    SDL_FRect toRect{};
    SDL_FRect toRect1{};

    char ht[64] = { 0, 1, 2, 4, 6, 4, 4, 4, 1, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                   6, 0, 4, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 4, 6, 6, 4, 4 };

    short minSize = (short)((m_blockSize * 22) / m_misc->m_mapSize);

    short value;

    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_resources->m_renderer, m_texMap);
    SDL_RenderClear(m_resources->m_renderer);
    ////////////////////////////////////////////////////////////////
    
    outRect.w = minSize;
    outRect.h = minSize;

    SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 128);

    for (short y = 0; y < m_misc->m_mapSize; y++)
    {
        outRect.y = (float)(y * minSize);
        for (short x = 0; x < m_misc->m_mapSize; x++)
        {
            value = m_misc->GetXYVal(x, y) / 4;
            short top = ht[value];
            int minival = value;
            int multval = value / 16;
            value += (unsigned char)(16 * multval);
            //value = m_resources->GetRealTile(value);
            outRect.x = (float)(x * minSize);

			if (value < 192)
			{
				// if it's an I and not up against the edge of the map,
				if (value == 78 && x > 0 && x < m_misc->m_mapSize - 1 && y > 0 && y < m_misc->m_mapSize - 1) // letter I or Door
				{
					// and the item left of it is not a letter
					unsigned char left = m_misc->GetXYVal(x - 1, y) / 4;
					minival = left;
					multval = left / 16;
					left += (unsigned char)(16 * multval);

					bool isLetter = false;
					if (left < 69 || left > 106 || left == 78)
					{
						// and the item right of it is not a letter
						unsigned char right = m_misc->GetXYVal(x + 1, y) / 4;
						minival = right;
						multval = right / 16;
						right += (unsigned char)(16 * multval);
						if (right < 69 || right > 106 || right == 78)
						{
							// and the item above it is not a letter, turn it into a door.
							unsigned char above = m_misc->GetXYVal(x, y - 1) / 4;
							minival = above;
							multval = above / 16;
							above += (unsigned char)(16 * multval);
							if (above < 69 || above > 106)
							{
								for (short index = x - 1; index > 0; --index)
								{
									unsigned char templet = m_misc->GetXYVal(index, y) / 4;
									minival = templet;
									multval = templet / 16;
									templet += (unsigned char)(16 * multval);
									if (templet != 78)
									{
										if (templet >= 69 && templet <= 106)
										{
											isLetter = true;
										}
										break;
									}
								}
								if (!isLetter)
								{
									value = 94;    // 0x5C normally for I
								}
							}
						}
					}
				}

                outRect.y -= top;
                outRect.x -= top;
                bool shade = false;

                if (x > 0 && y > 0 && top < 3)
                {
                    if ((ht[m_misc->GetXYVal(x - 1, y) / 4] > 3) || (ht[m_misc->GetXYVal(x, y - 1) / 4] > 3) ||
                        (ht[m_misc->GetXYVal(x - 1, y - 1) / 4] > 3))
                    {
                        shade = true;
                    }
                }

                fromRect.x = 0;
                fromRect.y = 0;
                fromRect.w = (float)TEXTURE_SIZE_X;
                fromRect.h = ((float)TEXTURE_SIZE_Y / (float)minSize);
                fromRect1.x = 0;
                fromRect1.y = 0;
                fromRect1.w = ((float)TEXTURE_SIZE_X / (float)minSize);;
                fromRect1.h = (float)TEXTURE_SIZE_Y;

                toRect.h = 1.0f;
                toRect.w = minSize;

                toRect1.w = 1;
                toRect1.h = minSize;

                if (top > 0)
                {
                    for (int tempVal = 0; tempVal <= top; tempVal++)
                    {
                        toRect.x = outRect.x + tempVal;
                        toRect.y = (outRect.y + outRect.h) + tempVal;

                        toRect1.x = outRect.x + tempVal + outRect.w;
                        toRect1.y = (outRect.y + tempVal);
                        //SDL_RenderFillRect(m_resources->m_renderer, &toRect);
                        //SDL_RenderFillRect(m_resources->m_renderer, &toRect1);
                        SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], &fromRect, &toRect);
                        SDL_RenderFillRect(m_resources->m_renderer, &toRect);
                        SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], &fromRect1, &toRect1);
                        SDL_RenderFillRect(m_resources->m_renderer, &toRect1);
                    }
                }

				SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], NULL, &outRect);

                if (shade)
                {
                    
                    SDL_RenderFillRect(m_resources->m_renderer, &outRect);      
                }

                outRect.y += top;
                outRect.x += top;
			}
		}
	}
    SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_NONE);
    
    /////////////////////////////////////////////////////////////

    SDL_SetRenderTarget(m_resources->m_renderer, NULL);
    m_forceRedraw = false;
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void U3Graphics::DrawDioramaMap()
{
    SDL_FRect theRect{};

    if (m_forceRedraw)
    {
        m_fading = true;
        renderDiorama();
    }

    if (!m_texMap)
    {
        return;
    }

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_RenderTexture(m_resources->m_renderer, m_texMap, NULL, &theRect);

    float minSize = (m_blockSize * 22.0f) / m_misc->m_mapSize;

    theRect.x = (float)minSize * m_misc->m_xpos + m_blockSize;
    theRect.y = (float)minSize * m_misc->m_ypos + m_blockSize;
    theRect.w = (float)minSize;
    theRect.h = (float)minSize;

    float ratio = 0;
    if (m_fading)
    {
        ratio = (DelayScroll - m_blinkElapsed) / (float)DelayScroll;
    }
    else
    {
        ratio = (m_blinkElapsed) / (float)DelayScroll;
    }

    m_resources->adjustRect(theRect);

    SDL_SetRenderDrawColor(m_resources->m_renderer, (Uint8)(255 * ratio), (Uint8)(255 * ratio), (Uint8)(255 * ratio), 255);
    SDL_RenderFillRect(m_resources->m_renderer, &theRect);
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}
