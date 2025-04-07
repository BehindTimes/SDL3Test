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
	m_forcecapture(false)
{
}

U3Button::~U3Button()
{
	if (m_texDefault)
	{
		SDL_DestroyTexture(m_texDefault);
	}
	if (m_texPushed)
	{
		SDL_DestroyTexture(m_texPushed);
	}
	if (m_texDisabled)
	{
		SDL_DestroyTexture(m_texDisabled);
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

void U3Button::render(SDL_Renderer* renderer, int blockSize, int x, int y, short adjustX, short adjustY)
{
	if (!m_visible)
	{
		return;
	}
	float mult = (float)blockSize / 64.0f;    // blkSiz normally 16, but buttons are 4x

	m_renderRect.x = (float)x * 4 + adjustX;
	m_renderRect.y = (float)y * 4 + adjustY;
	m_renderRect.h = (float)mult * m_height;
	m_renderRect.w = (float)mult * m_width;

	if (m_showPushed)
	{
		SDL_RenderTexture(renderer, m_texPushed, NULL, &m_renderRect);
	}
	else
	{
		SDL_RenderTexture(renderer, m_texDefault, NULL, &m_renderRect);
	}
}

void U3Button::setRect(SDL_Renderer* renderer, SDL_Texture* buttonImage, int x, int y, int width, int height, bool has_clicked, bool has_disabled)
{
	SDL_FRect myRect;

	if (!buttonImage)
	{
		return;
	}

	myRect.x = (float)x * 4;
	myRect.y = (float)y * 4;
	myRect.w = (float)width * 4;
	myRect.h = (float)height * 4;

	m_width = myRect.w;
	m_height = myRect.h;

	m_texDefault = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

	SDL_SetRenderTarget(renderer, m_texDefault);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, buttonImage, &myRect, NULL);

	if (has_clicked)
	{
		myRect.x = (float)x * 4;
		myRect.y = (float)y * 4 + (height * 4);
		myRect.w = (float)width * 4;
		myRect.h = (float)height * 4;

		m_texPushed = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetRenderTarget(renderer, m_texPushed);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, buttonImage, &myRect, NULL);
	}
	if (has_disabled)
	{
		myRect.x = (float)x * 4;
		myRect.y = (float)y * 4 + (height * 8);
		myRect.w = (float)width * 4;
		myRect.h = (float)height * 4;

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

void U3Button::SetButtonCallback(std::function<void()> func)
{
	m_callbackFunction = func;
}

void U3Button::setMouseCapture(int blockSize, int capture, float mouse_x, float mouse_y, float x, float y, short adjustX, short adjustY)
{
	if (!m_visible)
	{
		return;
	}
	SDL_FRect myRect;
	float mult = (float)blockSize / 64.0f;    // blkSiz normally 16, but buttons are 4x
	m_showPushed = false;
	bool bCallback = false;

	myRect.x = (float)x * 4 + adjustX;
	myRect.y = (float)y * 4 + adjustY;
	myRect.h = (float)mult * m_height;
	myRect.w = (float)mult * m_width;

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
			m_callbackFunction();
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
		m_callbackFunction();
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
			m_callbackFunction();
		}
	}
}