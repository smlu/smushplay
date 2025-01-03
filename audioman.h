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

// Inspired by the ScummVM code of the same name (GPLv2+)

#ifndef AUDIOMAN_H
#define AUDIOMAN_H

#include <SDL.h>
#include <SDL_thread.h>
#include <map>
#include "types.h"

class AudioManager;
class AudioStream;
class RateConverter;

/**
 * An AudioHandle instances corresponds to a specific sound
 * being played via the AudioManager. It can be used to control that
 * sound (pause it, stop it, etc.).
 * @see The AudioManager class
 */
class AudioHandle {
	friend class AudioManager;

public:
	inline AudioHandle() : _id(0xFFFFFFFF) {}

protected:
	uint32 _id;
};

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	enum {
		kMaxChannelVolume = 0xFF,
		kMaxAudioManVolume = 0x100
	};

	bool init();
	void play(AudioStream *stream);
	void play(AudioStream *stream, AudioHandle &handle, byte volume = kMaxChannelVolume, int8 balance = 0);
	void stop(const AudioHandle &handle);
	void stopAll();

	void setVolume(const AudioHandle &handle, byte volume);
	byte getVolume(const AudioHandle &handle);
	void setBalance(const AudioHandle &handle, int8 balance);
	int8 getBalance(const AudioHandle &handle);

private:
	void callbackHandler(byte *samples, int len);
	static void sdlCallback(void *manager, byte *samples, int len);

	SDL_AudioSpec _spec;
	SDL_mutex *_mutex;

	struct Channel {
	public:
		Channel(AudioStream *stream, uint destFreq, byte volume, int8 balance);
		~Channel();

		bool endOfStream() const;
		bool endOfData() const;
		void mixInt16(int16 *samples, uint length);
		void mix(SDL_AudioFormat format, void *samples, uint length);

		void setVolume(byte volume);
		byte getVolume() const { return _volume; }
		void setBalance(int8 balance);
		int8 getBalance() const { return _balance; }

	protected:
		AudioStream *_stream;
		RateConverter *_converter;
		int8 _balance;
		byte _volume;
		uint16 _leftVolume, _rightVolume;

	private:
		void updateChannelVolumes();
	};

	typedef std::map<uint, Channel *> ChannelMap;
	ChannelMap _channels;
	uint _channelSeed;
};

#endif
