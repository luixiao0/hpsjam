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

#include "hpsjam.h"

#include "clientdlg.h"
#include "mixerdlg.h"
#include "lyricsdlg.h"
#include "configdlg.h"
#include "chatdlg.h"
#include "connectdlg.h"
#include "statsdlg.h"

HpsJamClient :: HpsJamClient() : gl(this), b_connect(tr("CONN&ECT")),
    b_mixer(tr("&MIXER")), b_lyrics(tr("&LYRICS")), b_chat(tr("CH&AT")),
    b_config(tr("CON&FIG")), b_stats(tr("&STATS"))
{
	setWindowTitle(HPSJAM_WINDOW_TITLE " Client");

	connect(&b_connect, SIGNAL(released()), this, SLOT(handle_connect()));
	connect(&b_mixer, SIGNAL(released()), this, SLOT(handle_mixer()));
	connect(&b_lyrics, SIGNAL(released()), this, SLOT(handle_lyrics()));
	connect(&b_chat, SIGNAL(released()), this, SLOT(handle_chat()));
	connect(&b_config, SIGNAL(released()), this, SLOT(handle_config()));
	connect(&b_stats, SIGNAL(released()), this, SLOT(handle_stats()));

	gl.addWidget(&b_connect, 0,0);
	gl.addWidget(&b_mixer, 0,1);
	gl.addWidget(&b_lyrics, 0,2);
	gl.addWidget(&b_chat, 0,3);
	gl.addWidget(&b_config, 0,4);
	gl.addWidget(&b_stats, 0,5);
	gl.setColumnStretch(6,1);
	gl.addWidget(&w_stack, 1,0,1,7);
	gl.setRowStretch(1,1);
	gl.setColumnStretch(6,1);

	w_connect = new HpsJamConnect();
	w_mixer = new HpsJamMixer();
	w_lyrics = new HpsJamLyrics();
	w_chat = new HpsJamChat();
	w_config = new HpsJamConfig();
	w_stats = new HpsJamStats();

	w_stack.addWidget(w_connect);
	w_stack.addWidget(w_mixer);
	w_stack.addWidget(w_lyrics);
	w_stack.addWidget(w_chat);
	w_stack.addWidget(w_config);
	w_stack.addWidget(w_stats);
}

void
HpsJamClient :: handle_connect()
{
	w_stack.setCurrentWidget(w_connect);
}

void
HpsJamClient :: handle_mixer()
{
	w_stack.setCurrentWidget(w_mixer);
}

void
HpsJamClient :: handle_lyrics()
{
	w_stack.setCurrentWidget(w_lyrics);
}

void
HpsJamClient :: handle_chat()
{
	w_stack.setCurrentWidget(w_chat);
}

void
HpsJamClient :: handle_config()
{
	w_stack.setCurrentWidget(w_config);
}

void
HpsJamClient :: handle_stats()
{
	w_stack.setCurrentWidget(w_stats);
}