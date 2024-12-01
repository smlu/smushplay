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

#ifndef GRAPHICSMAN_H
#define GRAPHICSMAN_H

#include "types.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class GraphicsManager {
public:
	GraphicsManager() = default;
	~GraphicsManager();
	bool init(SDL_Window *window, uint width, uint height, bool highColor);
	void blit(const byte *ptr, uint x, uint y, uint width, uint height, uint pitch);
	void update();
	void setPalette(const byte *ptr, uint start, uint count);

private:
	void convertIndexToRGBA(uint32_t *rgbaData, const byte *paletteData, int width, int height) const;
	SDL_Renderer *_renderer = nullptr;
	SDL_Texture *_texture   = nullptr;
	SDL_Palette *_palette   = nullptr;
	int _width  = 0;
	int _height = 0;
	bool _isHighColor = false;
};

#endif
