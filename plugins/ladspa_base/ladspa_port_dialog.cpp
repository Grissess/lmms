/*
 * ladspa_port_dialog.cpp - dialog to test a LADSPA plugin
 *
 * Copyright (c) 2006-2007 Danny McRae <khjklujn/at/users.sourceforge.net>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include <QtGui/QLayout>
#include <QtGui/QTableWidget>

#include "ladspa_port_dialog.h"

#include "embed.h"
#include "engine.h"
#include "mixer.h"


ladspaPortDialog::ladspaPortDialog( const ladspa_key_t & _key ) :
	m_key( _key ),
	m_ladspa( engine::getLADSPAManager() )
{
	setWindowIcon( embed::getIconPixmap( "ports" ) );
	setWindowTitle( "Ports" );
	setModal( TRUE );
	
	QVBoxLayout * vlayout = new QVBoxLayout( this );
	vlayout->setSpacing( 0 );
	vlayout->setMargin( 0 );
	QWidget * settings = new QWidget( this );
	
	Uint16 pc = m_ladspa->getPortCount( m_key );
	
	QTableWidget * display = new QTableWidget( pc, 7, settings );
	
	QStringList ports;
	ports.append( tr( "Name" ) );
	ports.append( tr( "Rate" ) );
	ports.append( tr( "Direction" ) );
	ports.append( tr( "Type" ) );
	ports.append( tr( "Min < Default < Max" ) );
	ports.append( tr( "Logarithmic" ) );
	ports.append( tr( "SR Dependent" ) );
	
	QStringList port_nums;
	for(Uint16 row = 0; row < pc; row++)
	{
		port_nums.append( QString::number( row ) );
		Uint8 col = 0;
		display->item( row, col )->setText( m_ladspa->getPortName( 
							m_key, row ) );
		col++;
		
		if( m_ladspa->isPortAudio( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Audio" ) );
		}
		else
		{
			display->item( row, col )->setText( tr( "Control" ) );
		}
		col++;
		
		if( m_ladspa->isPortInput( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Input" ) );
		}
		else
		{
			display->item( row, col )->setText( tr( "Output" ) );
		}
		col++;
			
		if( m_ladspa->isPortToggled( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Toggled" ) );
		}
		else if( m_ladspa->isInteger( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Integer" ) );
		}
		else
		{
			display->item( row, col )->setText( tr( "Float" ) );
		}
		col++;
		
		float min = m_ladspa->getLowerBound( m_key, row );
		float max = m_ladspa->getUpperBound( m_key, row );
		float def = m_ladspa->getDefaultSetting( m_key, row );
		QString range = "";
		
		if( m_ladspa->areHintsSampleRateDependent( m_key, row ) )
		{
			if( min != NOHINT )
			{
				min *= engine::getMixer()->sampleRate();
			}
			if( max != NOHINT )
			{
				max *= engine::getMixer()->sampleRate();
			}
		}
		
		if( min == NOHINT )
		{
			range += "-Inf < ";
		}
		else if( m_ladspa->isInteger( m_key, row ) )
		{
			range += QString::number( static_cast<int>( min ) ) +
									" < ";
		}
		else
		{
			range += QString::number( min ) + " < ";
		}
		
		if( def == NOHINT )
		{
			range += "None < ";
		}
		else if( m_ladspa->isInteger( m_key, row ) )
		{
			range += QString::number( static_cast<int>( def ) ) + 
									" < ";
		}
		else
		{
			range += QString::number( def ) + " < ";
		}
		
		if( max == NOHINT )
		{
			range += "Inf";
		}
		else if( m_ladspa->isInteger( m_key, row ) )
		{
			range += QString::number( static_cast<int>( max ) );
		}
		else
		{
			range += QString::number( max );
		}
		
		if( m_ladspa->isPortOutput( m_key, row ) ||
			m_ladspa->isPortToggled( m_key, row ) )
		{
			range = "";
		}
		display->item( row, col )->setText( range );
		col++;
				
		if( m_ladspa->isLogarithmic( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Yes" ) );
		}
		col++;
		
		if( m_ladspa->areHintsSampleRateDependent( m_key, row ) )
		{
			display->item( row, col )->setText( tr( "Yes" ) );
		}
		col++;
	}


	vlayout->addWidget( settings );
	
	setFixedSize( display->width(), display->height() );

	show();
}



ladspaPortDialog::~ ladspaPortDialog()
{
}



#include  "ladspa_port_dialog.moc"

