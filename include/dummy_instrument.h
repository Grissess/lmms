/*
 * dummy_instrument.h - instrument used as fallback if an instrument couldn't
 *                      be loaded
 *
 * Copyright (c) 2005-2006 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 * 
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */


#ifndef _DUMMY_INSTRUMENT_H
#define _DUMMY_INSTRUMENT_H

#include "instrument.h"


class dummyInstrument : public instrument
{
public:
	inline dummyInstrument( instrumentTrack * _channel_track ) :
		instrument( _channel_track, NULL )
	{
	}

	inline virtual ~dummyInstrument()
	{
	}


	inline virtual void saveSettings( QDomDocument &, QDomElement & )
	{
	}

	inline virtual void loadSettings( const QDomElement & )
	{
	}

	inline virtual QString nodeName( void ) const
	{
		return( "dummyinstrument" );
	}

} ;


#endif
