/* smushplay - A simple LucasArts SMUSH video player
 *
 * smushplay is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include <SDL.h>
#include <stdio.h>
#include <cstring>

#include "graphicsman.h"

// TODO: aspect ratio correction option
// TODO: resize/scaling option

GraphicsManager::~GraphicsManager() {
	if ( _palette )
		SDL_FreePalette(_palette);
	if ( _texture )
		SDL_DestroyTexture(_texture);
	if ( _renderer )
		SDL_DestroyRenderer(_renderer);
}

bool GraphicsManager::init(SDL_Window *window, uint width, uint height, bool isHighColor) {
	_width = width;
	_height = height;
	_isHighColor = isHighColor;

	// Create renderer
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if ( !_renderer )
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetScale(_renderer, width, height);

	// Create texture
	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);
	if ( !_texture ) {
		return false;
	}

	return true;
}

void GraphicsManager::setPalette(const byte *ptr, uint start, uint count) {
	if ( count == 0 || !ptr || start + count > 256 )
		return;

	SDL_Color colors[256] = { 0 };
	for ( uint i = 0; i < count; i++ ) {
		colors[i].r = ptr[i * 3];
		colors[i].g = ptr[i * 3 + 1];
		colors[i].b = ptr[i * 3 + 2];
		colors[i].a = 255;
	}

	if ( _palette )
		SDL_FreePalette(_palette);

	_palette = SDL_AllocPalette(count);
	SDL_SetPaletteColors(_palette, colors, start, count);
}

void GraphicsManager::convertIndexToRGBA(uint32_t *rgbaData, const byte *paletteData, int width, int height) const {
	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
			uint8_t index = paletteData[y * width + x];
			SDL_Color color = _palette->colors[index];
			rgbaData[y * width + x] = (color.r << 24) | (color.g << 16) | (color.b << 8) | (color.a);
		}
	}
}

void GraphicsManager::blit(const byte *ptr, uint x, uint y, uint width, uint height, uint pitch) {
	if ( width == 0 || height == 0 )
		return;

	// Clip dimensions
	if ( x + width > (uint)_width )
		width = _width - x;
	if ( y + height > (uint)_height )
		height = _height - y;


	void *texPixels;
	int texPitch;
	SDL_LockTexture(_texture, NULL, &texPixels, &texPitch);

	if ( _isHighColor )
		SDL_ConvertPixels(width, height, SDL_PIXELFORMAT_RGB565, ptr + y * pitch + x, pitch, SDL_PIXELFORMAT_RGBA8888, texPixels, texPitch);
	else {
		convertIndexToRGBA((uint32_t *)texPixels, ptr + y * pitch + x, width, height);
	}

	SDL_UnlockTexture(_texture);
}

void GraphicsManager::update() {
	// Clear renderer
	SDL_RenderClear(_renderer);

	// Copy entire texture to renderer
	SDL_RenderCopy(_renderer, _texture, NULL, NULL);

	// Present renderer
	SDL_RenderPresent(_renderer);
}