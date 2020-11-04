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

#ifndef _HPSJAM_CLIENTDLG_H_
#define	_HPSJAM_CLIENTDLG_H_

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>

class HpsJamConnect;
class HpsJamMixer;
class HpsJamLyrics;
class HpsJamChat;
class HpsJamConfig;
class HpsJamStats;

class HpsJamClient : public QWidget {
	Q_OBJECT;
public:
	HpsJamClient();
	QGridLayout gl;
	QStackedWidget w_stack;
	QPushButton b_connect;
	QPushButton b_mixer;
	QPushButton b_lyrics;
	QPushButton b_chat;
	QPushButton b_config;
	QPushButton b_stats;

	HpsJamConnect *w_connect;
	HpsJamMixer *w_mixer;
	HpsJamLyrics *w_lyrics;
	HpsJamChat *w_chat;
	HpsJamConfig *w_config;
	HpsJamStats *w_stats;

public slots:
	void handle_connect();
	void handle_mixer();
	void handle_lyrics();
	void handle_chat();
	void handle_config();
	void handle_stats();
};

#endif		/* _HPSJAM_CLIENTDLG_H_ */