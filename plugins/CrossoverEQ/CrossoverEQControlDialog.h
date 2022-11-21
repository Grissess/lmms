/*
 * CrossoverEQControlDialog.h - A native 4-band Crossover Equalizer 
 * good for simulating tonestacks or simple peakless (flat-band) equalization
 *
 * Copyright (c) 2014 Vesa Kivimäki <contact/dot/diizy/at/nbl/dot/fi>
 * Copyright (c) 2006-2014 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#ifndef CROSSOVEREQ_CONTROL_DIALOG_H
#define CROSSOVEREQ_CONTROL_DIALOG_H

#include <QPixmap>
#include "EffectControlDialog.h"

namespace lmms
{


class CrossoverEQControls;


namespace gui
{

class CrossoverEQControlDialog : public EffectControlDialog
{
	Q_OBJECT
public:
	CrossoverEQControlDialog( CrossoverEQControls * controls );
	~CrossoverEQControlDialog() override = default;
	
private:
	QPixmap m_fader_bg;
	QPixmap m_fader_empty;
	QPixmap m_fader_knob;
};


} // namespace gui

} // namespace lmms

#endif
