#ifndef SINGLE_SOURCE_COMPILE

/*
 * tab_widget.cpp - tabwidget for LMMS
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QWheelEvent>

#include "tab_widget.h"
#include "gui_templates.h"



tabWidget::tabWidget( const QString & _caption, QWidget * _parent ) :
	QWidget( _parent ),
	specialBgHandlingWidget( QColor( 96, 96, 96 ) ),
	m_activeTab( 0 ),
	m_caption( _caption )
{
	setFont( pointSize<7>( font() ) );

	setAutoFillBackground( TRUE );
	QColor bg_color = QApplication::palette().color( QPalette::Active,
							QPalette::Background ).
								dark( 132 );
	QPalette pal = palette();
	pal.setColor( QPalette::Background, bg_color );
	setPalette( pal );
}




tabWidget::~tabWidget()
{
}




void tabWidget::addTab( QWidget * _w, const QString & _name, int _idx )
{
	widgetDesc d = { _w, _name, fontMetrics().width( _name ) + 10 } ;
	if( _idx < 0/* || m_widgets.contains( _idx ) == TRUE*/ )
	{
		while( m_widgets.contains( ++_idx ) == TRUE )
		{
		}
	}
	m_widgets[_idx] = d;
	_w->setFixedSize( width() - 4, height() - 14 );
	_w->move( 2, 12 );
	_w->hide();

	if( m_widgets.contains( m_activeTab ) )
	{
		// make sure new tab doesn't overlap current widget
		m_widgets[m_activeTab].w->show();
		m_widgets[m_activeTab].w->raise();
	}
}




void tabWidget::setActiveTab( int _idx )
{
	if( m_widgets.contains( _idx ) )
	{
		int old_active = m_activeTab;
		m_activeTab = _idx;
		m_widgets[m_activeTab].w->raise();
		m_widgets[m_activeTab].w->show();
		if( old_active != _idx && m_widgets.contains( old_active ) )
		{
			m_widgets[old_active].w->hide();
		}
		update();
	}
}




void tabWidget::mousePressEvent( QMouseEvent * _me )
{
	if( _me->y() > 1 && _me->y() < 13 )
	{
		int cx = ( ( m_caption == "" ) ? 4 : 14 ) +
					fontMetrics().width( m_caption );
		for( widgetStack::iterator it = m_widgets.begin();
						it != m_widgets.end(); ++it )
		{
			if( _me->x() >= cx &&
					_me->x() <= cx + ( *it ).nwidth )
			{
				setActiveTab( it.key() );
				update();
				return;
			}
			cx += ( *it ).nwidth;
		}
	}
}




void tabWidget::resizeEvent( QResizeEvent * )
{
	for( widgetStack::iterator it = m_widgets.begin();
						it != m_widgets.end(); ++it )
	{
		( *it ).w->setFixedSize( width() - 4, height() - 14 );
	}
}





void tabWidget::paintEvent( QPaintEvent * _pe )
{
	QPainter p( this );

	p.fillRect( 0, 0, width() - 1, height() - 1, QColor( 96, 96, 96 ) );

	const int c = 0;
	bool big_tab_captions = ( m_caption == "" );
	int add = big_tab_captions ? 1 : 0;

	p.setPen( QColor( 64, 64, 64 ) );
	p.drawRect( 0, 0, width() - 1 + c, height() - 1 + c );

	p.setPen( QColor( 160, 160, 160 ) );
	p.drawLine( width() - 1, 0, width() - 1, height() - 1 );
	p.drawLine( 0, height() - 1, width() - 1, height() - 1 );

	p.setPen( QColor( 0, 0, 0 ) );
	p.drawRect( 1, 1, width() - 3 + c, height() - 3 + c );

	p.fillRect( 2, 2, width() - 4, 9 + add, QColor( 30, 45, 60 ) );
	p.drawLine( 2, 11 + add, width() - 3, 11 + add );

	if( !big_tab_captions )
	{
		p.setPen( QColor( 255, 255, 255 ) );
		p.setFont( font() );
		p.drawText( 5, 10, m_caption );
	}

	int cx = ( big_tab_captions ? 4 : 14 ) +
					fontMetrics().width( m_caption );

	QColor cap_col( 160, 160, 160 );
	if( big_tab_captions )
	{
		p.setFont( pointSize<7>( p.font() ) );
		cap_col = QColor( 224, 224, 224 );
	}
	else
	{
		p.setFont( pointSize<6>( p.font() ) );
	}
	p.setPen( cap_col );


	for( widgetStack::iterator it = m_widgets.begin();
						it != m_widgets.end(); ++it )
	{
		if( it.key() == m_activeTab )
		{
			p.setPen( QColor( 32, 48, 64 ) );
			p.fillRect( cx, 2, ( *it ).nwidth - 6, 9, cap_col );
		}
		p.drawText( cx + 3, 9 + add, ( *it ).name );
		p.setPen( cap_col );
		cx += ( *it ).nwidth;
	}
}




void tabWidget::wheelEvent( QWheelEvent * _we )
{
	_we->accept();
	int dir = ( _we->delta() < 0 ) ? 1 : -1;
	int tab = m_activeTab;
	while( tab > -1 && static_cast<int>( tab ) < m_widgets.count() )
	{
		tab += dir;
		if( m_widgets.contains( tab ) )
		{
			break;
		}
	}
	setActiveTab( tab );
}




#include "tab_widget.moc"


#endif
