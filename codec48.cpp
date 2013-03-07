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


#include <stdio.h>
#include <string.h>
#include "codec48.h"
#include "util.h"

Codec48Decoder::Codec48Decoder(int width, int height) {
	_width = width;
	_height = height;
	_frameSize = _width * _height;
}

Codec48Decoder::~Codec48Decoder() {
}

bool Codec48Decoder::decode(byte *dst, const byte *src) {
	const byte *gfxData = src + 0x10;

	if (src[12] & (1 << 3))
		gfxData += 0x8080;

	// seq number seems to be at src + 2

	switch (src[0]) {
	case 0:
		// Raw frame
		memcpy(dst, gfxData, _frameSize);
		break;
	case 1:
		// Probably raw frame, 1/4 size
		// Never used in MotS
		break;
	case 2:
		// Blast object
		bompDecodeLine(dst, gfxData, _frameSize);
		break;
	case 3:
		// Delta encoding; looks similar to codec 47's
		printf("STUB: Implement codec 48 frame type 3\n");
		break;
	default:
		printf("Unknown codec 48 frame type %d\n", src[0]);
		break;
	}

	return true;
}

void Codec48Decoder::bompDecodeLine(byte *dst, const byte *src, int len) {
	while (len > 0) {
		byte code = *src++;
		byte num = (code >> 1) + 1;

		if (num > len)
			num = len;

		len -= num;

		if (code & 1) {
			byte color = *src++;
			memset(dst, color, num);
		} else {
			memcpy(dst, src, num);
			src += num;
		}

		dst += num;
	}
}