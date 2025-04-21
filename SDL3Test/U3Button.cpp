#include "U3Button.h"

U3Button::U3Button() :
	m_texDefault(nullptr),
	m_texPushed(nullptr),
	m_texDisabled(nullptr),
	m_width(0),
	m_height(0),
	m_captureMouse(false),
	m_showPushed(false),
	m_callbackFunction(nullptr),
	m_visible(false),
	m_forcecapture(false),
	m_renderRect(0),
	m_id(0),
	m_hasFocus(false),
	m_x(0),
	m_y(0)
{
}

U3Button::~U3Button()
{
	if (m_texDefault)
	{
		SDL_DestroyTexture(m_texDefault);
		m_texDefault = NULL;
	}
	if (m_texPushed)
	{
		SDL_DestroyTexture(m_texPushed);
		m_texPushed = NULL;
	}
	if (m_texDisabled)
	{
		SDL_DestroyTexture(m_texDisabled);
		m_texDisabled = NULL;
	}
}

void U3Button::resizeButton(int blockSize, SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font)
{
	if (m_texDefault)
	{
		SDL_DestroyTexture(m_texDefault);
		m_texDefault = NULL;
	}
	if (m_texPushed)
	{
		SDL_DestroyTexture(m_texPushed);
		m_texPushed = NULL;
	}
	if (m_texDisabled)
	{
		SDL_DestroyTexture(m_texDisabled);
		m_texDisabled = NULL;
	}
	CreateTextButton(blockSize, renderer, engine_surface, font, m_text, m_x, m_y);
}

void U3Button::CreateTextButton(int blockSize, SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font, std::string strText, int x, int y)
{
	m_x = x;
	m_y = y;
	float offset = 0;
	float offsety = 0;

	TTF_Text* text_obj = NULL;
	text_obj = TTF_CreateText(engine_surface, font, strText.c_str(), 0);
	if (text_obj)
	{
		int w, h;
		TTF_GetTextSize(text_obj, &w, &h);

		if (h > blockSize)
		{
			offsety = (h - blockSize) / 2.0f;
			h = blockSize;
		}
		if (w  + 4 < h)
		{
			offset = (h - w) / 2.0f;
			w = h - 4;
		}

		m_texDefault = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + 4, h);
		SDL_SetTextureScaleMode(m_texDefault, SDL_SCALEMODE_NEAREST);

		SDL_SetRenderTarget(renderer, m_texDefault);

		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
		SDL_RenderFillRect(renderer, NULL);
		TTF_SetTextColor(text_obj, 0, 0, 0, 255);
		TTF_DrawRendererText(text_obj, 2 + offset, (float)((int)(-1.0f * offsety)));
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_SetRenderTarget(renderer, NULL);

		m_texPushed = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + 4, h);

		SDL_SetRenderTarget(renderer, m_texPushed);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
		SDL_RenderFillRect(renderer, NULL);
		TTF_SetTextColor(text_obj, 255,255, 255, 255);
		TTF_DrawRendererText(text_obj, 2 + offset, 0);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderRect(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_SetRenderTarget(renderer, NULL);

		m_width = (float)w;
		m_height = (float)h;

		m_visible = true;

		TTF_DestroyText(text_obj);
		text_obj = NULL;
		m_text = strText;
	}
}

void U3Button::updateLocationCentered(SDL_FRect fillRect)
{
	m_renderRect.x = fillRect.x;
	m_renderRect.y = fillRect.y;
	m_renderRect.h = fillRect.h;
	m_renderRect.w = fillRect.w;

	float wRatio = fillRect.w / m_width;
	float hRatio = fillRect.h / m_height;

	if (wRatio < hRatio)
	{
		m_renderRect.h = m_height * wRatio;
	}
	else
	{
		m_renderRect.w = m_width * hRatio;
	}
	float startX = (fillRect.w - m_renderRect.w) / 2.0f + fillRect.x;
	float startY = (fillRect.h - m_renderRect.h) / 2.0f + fillRect.y;
	m_renderRect.x = startX;
	m_renderRect.y = startY;
}

void U3Button::renderCentered(SDL_Renderer* renderer)
{
	if (!m_visible || m_width <= 0 || m_height <= 0)
	{
		return;
	}

	if (m_showPushed)
	{
		SDL_RenderTexture(renderer, m_texPushed, NULL, &m_renderRect);
	}
	else
	{
		SDL_RenderTexture(renderer, m_texDefault, NULL, &m_renderRect);
	}
}

void U3Button::render(SDL_Renderer* renderer, int blockSize, int x, int y)
{
	if (!m_visible)
	{
		return;
	}
	//float mult = (float)blockSize / 64.0f;    // blkSiz normally 16, but buttons are 4x

	m_renderRect.x = (float)x;
	m_renderRect.y = (float)y;
	m_renderRect.h = (float)m_height;
	m_renderRect.w = (float)m_width;

	if (m_showPushed)
	{
		SDL_RenderTexture(renderer, m_texPushed, NULL, &m_renderRect);
	}
	else
	{
		SDL_RenderTexture(renderer, m_texDefault, NULL, &m_renderRect);
	}
}

void U3Button::render(SDL_Renderer* renderer, int blockSize, int x, int y, short adjustX, short adjustY)
{
	if (!m_visible)
	{
		return;
	}
	//float mult = (float)blockSize / 64.0f;    // blkSiz normally 16, but buttons are 4x

	m_renderRect.x = (float)x * 4 + adjustX;
	m_renderRect.y = (float)y * 4 + adjustY;
	m_renderRect.h = (float)m_height;
	m_renderRect.w = (float)m_width;

	if (m_showPushed)
	{
		SDL_RenderTexture(renderer, m_texPushed, NULL, &m_renderRect);
	}
	else
	{
		SDL_RenderTexture(renderer, m_texDefault, NULL, &m_renderRect);
	}
}

void U3Button::setRect(SDL_Renderer* renderer, SDL_Texture* buttonImage, int blockSize, int x, int y, int width, int height, bool has_clicked, bool has_disabled)
{
	SDL_FRect myRect;

	if (!buttonImage)
	{
		return;
	}

	myRect.x = (float)x * 4;
	myRect.y = (float)y * 4;
	myRect.w = (float)width;
	myRect.h = (float)height;

	float mult = blockSize / 64.0f;

	m_width = myRect.w * mult;
	m_height = myRect.h * mult;

	if (m_texDefault)
	{
		SDL_DestroyTexture(m_texDefault);
		m_texDefault = NULL;
	}
	if (m_texPushed)
	{
		SDL_DestroyTexture(m_texPushed);
		m_texPushed = NULL;
	}
	if (m_texDisabled)
	{
		SDL_DestroyTexture(m_texDisabled);
		m_texDisabled = NULL;
	}

	m_texDefault = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

	SDL_SetRenderTarget(renderer, m_texDefault);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, buttonImage, &myRect, NULL);

	if (has_clicked)
	{
		myRect.x = (float)x * 4;
		myRect.y = (float)y * 4 + (height);
		myRect.w = (float)width;
		myRect.h = (float)height;

		m_texPushed = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetRenderTarget(renderer, m_texPushed);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, buttonImage, &myRect, NULL);
	}
	if (has_disabled)
	{
		myRect.x = (float)x * 4;
		myRect.y = (float)y * 4 + (height * 2);
		myRect.w = (float)width;
		myRect.h = (float)height;

		m_texDisabled = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetRenderTarget(renderer, m_texDisabled);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, buttonImage, &myRect, NULL);
	}

	SDL_SetRenderTarget(renderer, NULL);
}

void U3Button::setVisible(bool visible)
{
	m_visible = visible;
	if (!m_visible)
	{
		m_captureMouse = false;
	}
}

void U3Button::SetButtonCallback(std::function<void(int)> func, int id)
{
	m_callbackFunction = func;
	m_id = id;
}

void U3Button::setMouseCapture(int blockSize, int capture, float mouse_x, float mouse_y, float x, float y, short adjustX, short adjustY)
{
	if (!m_visible)
	{
		return;
	}
	SDL_FRect myRect;
	//float mult = (float)blockSize / 64.0f;    // blkSiz normally 16, but buttons are 4x
	m_showPushed = false;
	bool bCallback = false;

	myRect.x = (float)x + adjustX;
	myRect.y = (float)y + adjustY;
	myRect.h = (float)m_height;
	myRect.w = (float)m_width;

	switch (capture)
	{
	case 1: // mouse down
		if ((mouse_x >= myRect.x && mouse_x <= (myRect.x + myRect.w)) &&
			(mouse_y >= myRect.y && mouse_y <= (myRect.y + myRect.h)))
		{
			m_captureMouse = true;
			m_showPushed = true;
		}
		else
		{
			m_captureMouse = false;
		}
		break;
	case 2: // mouse up
		if ((mouse_x >= myRect.x && mouse_x <= (myRect.x + myRect.w)) &&
			(mouse_y >= myRect.y && mouse_y <= (myRect.y + myRect.h)) &&
			m_captureMouse)
		{
			bCallback = true;
		}
		m_captureMouse = false;
		m_showPushed = false;
		break;
	default:
		if ((mouse_x >= myRect.x && mouse_x <= (myRect.x + myRect.w)) &&
			(mouse_y >= myRect.y && mouse_y <= (myRect.y + myRect.h)) &&
			m_captureMouse)
		{
			m_showPushed = true;
		}
		else
		{
			m_showPushed = false;
		}
		break;
	}

	if (bCallback)
	{
		if (m_callbackFunction)
		{
			m_callbackFunction(m_id);
		}
	}
}

void U3Button::forceCapture()
{
	m_captureMouse = true;
	m_showPushed = true;

	m_forcecapture = true;
}

void U3Button::click()
{
	m_forcecapture = false;

	if (m_callbackFunction)
	{
		m_callbackFunction(3);
	}
}

void U3Button::setMouseCapture(int capture, float mouse_x, float mouse_y)
{
	if (m_forcecapture)
	{
		return;
	}
	bool bCallback = false;
	switch (capture)
	{
	case 1: // mouse down
		if ((mouse_x >= m_renderRect.x && mouse_x <= (m_renderRect.x + m_renderRect.w)) &&
			(mouse_y >= m_renderRect.y && mouse_y <= (m_renderRect.y + m_renderRect.h)))
		{
			m_captureMouse = true;
			m_showPushed = true;
		}
		else
		{
			m_captureMouse = false;
		}
		break;
	case 2: // mouse up
		if ((mouse_x >= m_renderRect.x && mouse_x <= (m_renderRect.x + m_renderRect.w)) &&
			(mouse_y >= m_renderRect.y && mouse_y <= (m_renderRect.y + m_renderRect.h)) &&
			m_captureMouse)
		{
			bCallback = true;
		}
		m_captureMouse = false;
		m_showPushed = false;
		break;
	default:
		if ((mouse_x >= m_renderRect.x && mouse_x <= (m_renderRect.x + m_renderRect.w)) &&
			(mouse_y >= m_renderRect.y && mouse_y <= (m_renderRect.y + m_renderRect.h)) &&
			m_captureMouse)
		{
			m_showPushed = true;
		}
		else
		{
			m_showPushed = false;
		}
		break;
	}

	if (bCallback)
	{
		if (m_callbackFunction)
		{
			m_callbackFunction(m_id);
		}
	}
}
