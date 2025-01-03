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

#include <cstdio>
#include <SDL.h>

#include "audioman.h"
#include "graphicsman.h"
#include "smushvideo.h"

void printUsage(const char *appName) {
	printf("Usage: %s <video>\n", appName);
}

#define SMUSHPLAY_VERSION "0.0.1"

int main(int argc, char **argv) {
	printf("\nsmushplay " SMUSHPLAY_VERSION " - SMUSH v1/v2 Player\n");
	printf("Plays LucasArts SMUSH videos\n");
	printf("Written by Matthew Hoops (clone2727)\n");
	printf("Based on ScummVM and ResidualVM's SMUSH player\n");
	printf("See COPYING for the license\n\n");

	if ( argc < 2 ) {
		printUsage(argv[0]);
		return 0;
	}

	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
		fprintf(stderr, "Failed to initialize SDL\n");
		return 1;
	}

	atexit(SDL_Quit);

	// Initialize audio here
	AudioManager audio;
	if ( !audio.init() ) {
		fprintf(stderr, "Failed to initialize SDL audio\n");
		return 1;
	}

	SMUSHVideo video(audio);
	if ( !video.load(argv[1]) ) {
		fprintf(stderr, "Failed to play file '%s'\n", argv[1]);
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("smushplay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, video.getWidth(), video.getHeight(), SDL_WINDOW_SHOWN);
	if ( !window ) {
		fprintf(stderr, "Failed to initialize SDL screen\n");
		return 1;
	}

	GraphicsManager gfx;
	if ( !gfx.init(window, video.getWidth(), video.getHeight(), video.isHighColor()) ) {
		fprintf(stderr, "Failed to initialize SDL screen\n");
		SDL_DestroyWindow(window);
		return 1;
	}

	// Finally, play the damned thing
	video.play(gfx);

	SDL_DestroyWindow(window);
	return 0;
}
