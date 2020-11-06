/*-
 * Copyright (c) 2020 Hans Petter Selasky. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_HPSJAM_PEER_H_
#define	_HPSJAM_PEER_H_

#include <QString>
#include <QByteArray>
#include <QMutex>

#include "audiobuffer.h"
#include "equalizer.h"
#include "socket.h"
#include "protocol.h"

#include <stdbool.h>

class hpsjam_server_peer : QObject {
	Q_OBJECT;
public:
	QMutex lock;
	struct hpsjam_socket_address address;
	struct hpsjam_input_packetizer input_pkt;
	class hpsjam_output_packetizer output_pkt;
	class hpsjam_audio_buffer in_audio[2];
	class hpsjam_audio_level out_level[2];
	float tmp_audio[2][HPSJAM_SAMPLE_RATE / 1000];
	float out_audio[2][HPSJAM_SAMPLE_RATE / 1000];

	QString name;
	QByteArray icon;
	uint8_t bits[256];
	float gain;
	float pan;
	float out_peak;
	uint8_t output_fmt;
	bool valid;

	void init() {
		address.clear();
		input_pkt.init();
		output_pkt.init();
		in_audio[0].clear();
		in_audio[1].clear();
		out_level[0].clear();
		out_level[1].clear();
		memset(out_audio, 0, sizeof(out_audio));
		name = QString();
		icon = QByteArray();
		output_fmt = 0;
		gain = 1.0f;
		pan = 0.0f;
		out_peak = 0.0f;
		valid = false;
	};

	void audio_export();
	void audio_import();

	hpsjam_server_peer() {
		init();
		connect(&output_pkt, SIGNAL(pendingWatchdog()), this, SLOT(handle_pending_watchdog()));
		connect(&output_pkt, SIGNAL(pendingTimeout()), this, SLOT(handle_pending_timeout()));
	};
public slots:
	void handle_pending_watchdog();
	void handle_pending_timeout();
};

class hpsjam_client_peer : QObject {
	Q_OBJECT;
public:
	QMutex lock;
	struct hpsjam_socket_address address;
	struct hpsjam_input_packetizer input_pkt;
	class hpsjam_output_packetizer output_pkt;
	class hpsjam_audio_buffer in_audio[2];
	class hpsjam_audio_buffer out_audio[2];
	class hpsjam_audio_level in_level[2];
	class hpsjam_equalizer eq;
	float mon_gain;
	float mon_pan;
	float in_gain;
	float in_pan;
	float in_peak;
	float out_peak;
	uint8_t bits;
	uint8_t out_format;
	bool valid[256];

	void init() {
		address.clear();
		memset(valid, 0, sizeof(valid));
		input_pkt.init();
		output_pkt.init();
		in_audio[0].clear();
		in_audio[1].clear();
		out_audio[0].clear();
		out_audio[1].clear();
		in_level[0].clear();
		in_level[1].clear();
		in_gain = 1.0f;
		mon_gain = 0.0f;
		mon_pan = 0.0f;
		in_pan = 0.0f;
		in_peak = 0.0;
		out_peak = 0.0;
		out_format = HPSJAM_TYPE_END;
		bits = 0;
	};
	hpsjam_client_peer() {
		init();

		connect(&output_pkt, SIGNAL(pendingWatchdog()), this, SLOT(handle_pending_watchdog()));
	};
	void sound_process(float *, float *, size_t);
	void tick();
public slots:
	void handle_pending_watchdog();
};

extern void hpsjam_peer_receive(const struct hpsjam_socket_address &,
    const union hpsjam_frame &);
extern void hpsjam_server_tick();

#endif		/* _HPSJAM_PEER_H_ */
