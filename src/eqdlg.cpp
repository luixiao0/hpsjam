/*-
 * Copyright (c) 2019-2020 Hans Petter Selasky. All rights reserved.
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

#include <QMessageBox>

#include "hpsjam.h"
#include "eqdlg.h"
#include "clientdlg.h"

HpsJamEqualizer :: HpsJamEqualizer() : gl(this),
    gl_spec(&g_spec), gl_control(&g_control)
{
	setWindowTitle(QString("HPS JAM Equalizer"));

	g_spec.setTitle(tr("Filter specification"));
	g_control.setTitle(tr("Control"));

	b_defaults.setText(tr("Defaults"));
	b_disable.setText(tr("Disable"));
	b_apply.setText(tr("Apply"));
	b_close.setText(tr("Close"));

	gl_spec.addWidget(&edit, 0,0);

	gl_control.addWidget(&b_defaults, 0,0);
	gl_control.addWidget(&b_disable, 0,1);
	gl_control.addWidget(&b_apply, 0,2);
	gl_control.addWidget(&b_close, 0,3);

	gl.addWidget(&g_spec, 0,0);
	gl.addWidget(&g_control, 1,0);
	gl.setRowStretch(0,1);

	connect(&b_defaults, SIGNAL(released()), this, SLOT(handle_defaults()));
	connect(&b_disable, SIGNAL(released()), this, SLOT(handle_disable()));
	connect(&b_apply, SIGNAL(released()), this, SLOT(handle_apply()));
	connect(&b_close, SIGNAL(released()), this, SLOT(handle_close()));

	handle_disable();
}

void
HpsJamEqualizer :: handle_defaults()
{
	edit.setText(QString(
	    "filtersize 2.0ms\n"
	    "normalize\n"
	    "20 1\n"
	    "25 1\n"
	    "31.5 1\n"
	    "40 1\n"
	    "50 1\n"
	    "63 1\n"
	    "80 1\n"
	    "100 1\n"
	    "125 1\n"
	    "160 1\n"
	    "200 1\n"
	    "250 1\n"
	    "315 1\n"
	    "400 1\n"
	    "500 1\n"
	    "630 1\n"
	    "800 1\n"
	    "1000 1\n"
	    "1250 1\n"
	    "1600 1\n"
	    "2000 1\n"
	    "2500 1\n"
	    "3150 1\n"
	    "4000 1\n"
	    "5000 1\n"
	    "6300 1\n"
	    "8000 1\n"
	    "10000 1\n"
	    "12500 1\n"
	    "16000 1\n"
	    "20000 1\n"
     ));
}

void
HpsJamEqualizer :: handle_disable()
{
	edit.setText(QString("filtersize 0.0ms\n"));
}

void
HpsJamEqualizer :: handle_apply()
{
	ssize_t len = edit.toPlainText().toLatin1().length();
	if (len > 255) {
		QMessageBox::information(this, tr("EQUALIZER"),
                    tr("Number of characters %1 in\n"
		       "filter specification is greater than 255!\n").arg(len));
	}
}

void
HpsJamEqualizer :: handle_close()
{
	hide();
}