#ifndef SINGLE_SOURCE_COMPILE

/*
 * name_label.cpp - implementation of class nameLabel, a label which
 *                  is renamable by double-clicking it
 *
 * Copyright (c) 2004-2007 Tobias Doerffel <tobydox/at/users.sourceforge.net>
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


#include <QtGui/QFileDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>


#include "name_label.h"
#include "rename_dialog.h"
#include "bb_editor.h"
#include "bb_track.h"
#include "gui_templates.h"
#include "config_mgr.h"
#include "engine.h"



nameLabel::nameLabel( const QString & _initial_name, QWidget * _parent ) :
	QLabel( _initial_name, _parent ),
	m_pixmap(),
	m_pixmapFile( "" )
{
}



nameLabel::~nameLabel()
{
}




void nameLabel::setPixmap( const QPixmap & _pixmap )
{
	m_pixmap = _pixmap;
}




void nameLabel::setPixmapFile( const QString & _file )
{
	QPixmap new_pixmap;
	if( QFileInfo( _file ).isRelative() )
	{
		new_pixmap = QPixmap( configManager::inst()->trackIconsDir() +
									_file );
	}
	else
	{
		new_pixmap = QPixmap( _file );
	}
	if( new_pixmap.isNull() )
	{
		return;
	}
	m_pixmap = new_pixmap;
	m_pixmapFile = _file;
	emit( pixmapChanged() );
	update();
}




void nameLabel::selectPixmap( void )
{
	QFileDialog ofd( NULL, tr( "Select icon" ) );

	QString dir;
	if( m_pixmapFile != "" )
	{
		QString f = m_pixmapFile;
		if( QFileInfo( f ).isRelative() )
		{
			f = configManager::inst()->trackIconsDir() + f;
		}
		dir = QFileInfo( f ).absolutePath();
	}
	else
	{
		dir = configManager::inst()->trackIconsDir();
	}
	// change dir to position of previously opened file
	ofd.setDirectory( dir );
	// use default QFileDialog::ExistingFile

	// set filters
	QStringList types;
	types << tr( "All images (*.png *.jpg *.jpeg *.gif *.bmp)" );
	ofd.setFilters( types );
	if( m_pixmapFile != "" )
	{
		// select previously opened file
		ofd.selectFile( QFileInfo( m_pixmapFile ).fileName() );
	}

	if ( ofd.exec () == QDialog::Accepted
		&& !ofd.selectedFiles().isEmpty()
	)
	{
		QString pf = ofd.selectedFiles()[0];
		if( !QFileInfo( pf ).isRelative() )
		{
			pf = pf.replace( configManager::inst()->trackIconsDir(),
									"" );
		}
		setPixmapFile( pf );
	}
}




void nameLabel::rename( void )
{
	QString txt = text();
	renameDialog rename_dlg( txt );
	rename_dlg.exec();
	if( txt != text() )
	{
		setText( txt );
		emit nameChanged( txt );
		emit nameChanged();
	}
}




void nameLabel::mousePressEvent( QMouseEvent * _me )
{

	if( _me->button() == Qt::RightButton )
	{
		QSize s( m_pixmap.width(), m_pixmap.height() );
		s.scale( width(), height(), Qt::KeepAspectRatio );
		if( _me->x() > 4 + s.width() )
		{
			rename();
		}
		else
		{
			selectPixmap();
		}
	}
	else
	{
		emit clicked();
		QLabel::mousePressEvent( _me );
	}
}




void nameLabel::mouseDoubleClickEvent( QMouseEvent * _me )
{
	QSize s( m_pixmap.width(), m_pixmap.height() );
	s.scale( width(), height(), Qt::KeepAspectRatio );
	if( _me->x() > 4 + s.width() )
	{
		rename();
	}
	else
	{
		selectPixmap();
	}
}




void nameLabel::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.fillRect( rect(), palette().color( backgroundRole() ) );
	p.setFont( pointSize<8>( p.font() ) );

	int x = 4;
	if( m_pixmap.isNull() == FALSE )
	{
		QPixmap pm = m_pixmap;
		if( pm.height() > height() )
		{
			pm = pm.scaledToHeight( height(),
						Qt::SmoothTransformation );
		}
		p.drawPixmap( x, ( height() - pm.height() ) / 2, pm );
		x += 4 + pm.width();
	}

	p.setPen( QColor( 16, 16, 16 ) );
	p.drawText( x+1, height() / 2 + p.fontMetrics().height() / 2 - 3,
								text() );

	p.setPen( QColor( 0, 224, 0 ) );
	bbTrack * bbt = bbTrack::findBBTrack(
					engine::getBBEditor()->currentBB() );
	if( bbt != NULL && bbt->getTrackSettingsWidget() ==
			dynamic_cast<trackSettingsWidget *>( parentWidget() ) )
	{
		p.setPen( QColor( 255, 255, 255 ) );
	}
	p.drawText( x, height() / 2 + p.fontMetrics().height() / 2 - 4,
								text() );

}




#include "name_label.moc"


#endif
