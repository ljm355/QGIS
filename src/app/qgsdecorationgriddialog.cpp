/***************************************************************************
                         qgsdecorationgriddialog.cpp
                         ----------------------
    begin                : May 10, 2012
    copyright            : (C) 2012 by Etienne Tourigny
    email                : etourigny.dev at gmail dot com

 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsdecorationgriddialog.h"

#include "qgsdecorationgrid.h"

#include "qgslogger.h"
#include "qgshelp.h"
#include "qgsstyle.h"
#include "qgssymbol.h"
#include "qgssymbolselectordialog.h"
#include "qgisapp.h"
#include "qgsguiutils.h"
#include "qgssettings.h"

QgsDecorationGridDialog::QgsDecorationGridDialog( QgsDecorationGrid &deco, QWidget *parent )
  : QDialog( parent )
  , mDeco( deco )
  , mLineSymbol( nullptr )
  , mMarkerSymbol( nullptr )
{
  setupUi( this );

  QgsSettings settings;
  //  restoreGeometry( settings.value( "/Windows/DecorationGrid/geometry" ).toByteArray() );

  grpEnable->setChecked( mDeco.enabled() );

  // mXMinLineEdit->setValidator( new QDoubleValidator( mXMinLineEdit ) );

  mGridTypeComboBox->insertItem( QgsDecorationGrid::Line, tr( "Line" ) );
  // mGridTypeComboBox->insertItem( QgsDecorationGrid::Cross, tr( "Cross" ) );
  mGridTypeComboBox->insertItem( QgsDecorationGrid::Marker, tr( "Marker" ) );

  // mAnnotationPositionComboBox->insertItem( QgsDecorationGrid::InsideMapFrame, tr( "Inside frame" ) );
  // mAnnotationPositionComboBox->insertItem( QgsDecorationGrid::OutsideMapFrame, tr( "Outside frame" ) );

  mAnnotationDirectionComboBox->insertItem( QgsDecorationGrid::Horizontal,
      tr( "Horizontal" ) );
  mAnnotationDirectionComboBox->insertItem( QgsDecorationGrid::Vertical,
      tr( "Vertical" ) );
  mAnnotationDirectionComboBox->insertItem( QgsDecorationGrid::HorizontalAndVertical,
      tr( "Horizontal and Vertical" ) );
  mAnnotationDirectionComboBox->insertItem( QgsDecorationGrid::BoundaryDirection,
      tr( "Boundary direction" ) );

  updateGuiElements();

  connect( buttonBox->button( QDialogButtonBox::Apply ), &QAbstractButton::clicked, this, &QgsDecorationGridDialog::apply );

}

void QgsDecorationGridDialog::updateGuiElements()
{
  // blockAllSignals( true );

  grpEnable->setChecked( mDeco.enabled() );

  mIntervalXEdit->setText( QString::number( mDeco.gridIntervalX() ) );
  mIntervalYEdit->setText( QString::number( mDeco.gridIntervalY() ) );
  mOffsetXEdit->setText( QString::number( mDeco.gridOffsetX() ) );
  mOffsetYEdit->setText( QString::number( mDeco.gridOffsetY() ) );

  mGridTypeComboBox->setCurrentIndex( static_cast< int >( mDeco.gridStyle() ) );
  mDrawAnnotationCheckBox->setChecked( mDeco.showGridAnnotation() );
  mAnnotationDirectionComboBox->setCurrentIndex( static_cast< int >( mDeco.gridAnnotationDirection() ) );
  mCoordinatePrecisionSpinBox->setValue( mDeco.gridAnnotationPrecision() );

  mDistanceToMapFrameSpinBox->setValue( mDeco.annotationFrameDistance() );
  // QPen gridPen = mDeco.gridPen();
  // mLineWidthSpinBox->setValue( gridPen.widthF() );
  // mLineColorButton->setColor( gridPen.color() );

  if ( mLineSymbol )
    delete mLineSymbol;
  if ( mDeco.lineSymbol() )
  {
    mLineSymbol = static_cast<QgsLineSymbol *>( mDeco.lineSymbol()->clone() );
    QIcon icon = QgsSymbolLayerUtils::symbolPreviewIcon( mLineSymbol, mLineSymbolButton->iconSize() );
    mLineSymbolButton->setIcon( icon );
  }
  if ( mMarkerSymbol )
    delete mMarkerSymbol;
  if ( mDeco.markerSymbol() )
  {
    mMarkerSymbol = static_cast<QgsMarkerSymbol *>( mDeco.markerSymbol()->clone() );
    QIcon icon = QgsSymbolLayerUtils::symbolPreviewIcon( mMarkerSymbol, mMarkerSymbolButton->iconSize() );
    mMarkerSymbolButton->setIcon( icon );
  }

  updateInterval( false );

  // blockAllSignals( false );
}

void QgsDecorationGridDialog::updateDecoFromGui()
{
  mDeco.setDirty( false );
  mDeco.setEnabled( grpEnable->isChecked() );

  mDeco.setGridIntervalX( mIntervalXEdit->text().toDouble() );
  mDeco.setGridIntervalY( mIntervalYEdit->text().toDouble() );
  mDeco.setGridOffsetX( mOffsetXEdit->text().toDouble() );
  mDeco.setGridOffsetY( mOffsetYEdit->text().toDouble() );
  if ( mGridTypeComboBox->currentText() == tr( "Marker" ) )
  {
    mDeco.setGridStyle( QgsDecorationGrid::Marker );
  }
  else if ( mGridTypeComboBox->currentText() == tr( "Line" ) )
  {
    mDeco.setGridStyle( QgsDecorationGrid::Line );
  }
  mDeco.setAnnotationFrameDistance( mDistanceToMapFrameSpinBox->value() );
  // if ( mAnnotationPositionComboBox->currentText() == tr( "Inside frame" ) )
  // {
  //   mDeco.setGridAnnotationPosition( QgsDecorationGrid::InsideMapFrame );
  // }
  // else
  // {
  //   mDeco.setGridAnnotationPosition( QgsDecorationGrid::OutsideMapFrame );
  // }
  mDeco.setShowGridAnnotation( mDrawAnnotationCheckBox->isChecked() );
  QString text = mAnnotationDirectionComboBox->currentText();
  if ( text == tr( "Horizontal" ) )
  {
    mDeco.setGridAnnotationDirection( QgsDecorationGrid::Horizontal );
  }
  else if ( text == tr( "Vertical" ) )
  {
    mDeco.setGridAnnotationDirection( QgsDecorationGrid::Vertical );
  }
  else if ( text == tr( "Horizontal and Vertical" ) )
  {
    mDeco.setGridAnnotationDirection( QgsDecorationGrid::HorizontalAndVertical );
  }
  else //BoundaryDirection
  {
    mDeco.setGridAnnotationDirection( QgsDecorationGrid::BoundaryDirection );
  }
  mDeco.setGridAnnotationPrecision( mCoordinatePrecisionSpinBox->value() );
  if ( mLineSymbol )
  {
    mDeco.setLineSymbol( mLineSymbol );
    mLineSymbol = mDeco.lineSymbol()->clone();
  }
  if ( mMarkerSymbol )
  {
    mDeco.setMarkerSymbol( mMarkerSymbol );
    mMarkerSymbol = mDeco.markerSymbol()->clone();
  }
}

QgsDecorationGridDialog::~QgsDecorationGridDialog()
{
  QgsSettings settings;
  settings.setValue( QStringLiteral( "/Windows/DecorationGrid/geometry" ), saveGeometry() );
  if ( mLineSymbol )
    delete mLineSymbol;
  if ( mMarkerSymbol )
    delete mMarkerSymbol;
}

void QgsDecorationGridDialog::on_buttonBox_helpRequested()
{
  QgsHelp::openHelp( QStringLiteral( "introduction/general_tools.html#grid" ) );
}

void QgsDecorationGridDialog::on_buttonBox_accepted()
{
  updateDecoFromGui();
  // mDeco.update();
  accept();
}

void QgsDecorationGridDialog::apply()
{
  updateDecoFromGui();
  mDeco.update();
  //accept();
}

void QgsDecorationGridDialog::on_buttonBox_rejected()
{
  reject();
}

void QgsDecorationGridDialog::on_mGridTypeComboBox_currentIndexChanged( int index )
{
  mLineSymbolButton->setEnabled( index == QgsDecorationGrid::Line );
  // mCrossWidthSpinBox->setEnabled( index == QgsDecorationGrid::Cross );
  mMarkerSymbolButton->setEnabled( index == QgsDecorationGrid::Marker );
}


void QgsDecorationGridDialog::on_mLineSymbolButton_clicked()
{
  if ( ! mLineSymbol )
    return;

  QgsLineSymbol *lineSymbol = mLineSymbol->clone();
  QgsSymbolSelectorDialog dlg( lineSymbol, QgsStyle::defaultStyle(), nullptr, this );
  if ( dlg.exec() == QDialog::Rejected )
  {
    delete lineSymbol;
  }
  else
  {
    delete mLineSymbol;
    mLineSymbol = lineSymbol;
    if ( mLineSymbol )
    {
      QIcon icon = QgsSymbolLayerUtils::symbolPreviewIcon( mLineSymbol, mLineSymbolButton->iconSize() );
      mLineSymbolButton->setIcon( icon );
    }
  }
}

void QgsDecorationGridDialog::on_mMarkerSymbolButton_clicked()
{
  if ( ! mMarkerSymbol )
    return;

  QgsMarkerSymbol *markerSymbol = mMarkerSymbol->clone();
  QgsSymbolSelectorDialog dlg( markerSymbol, QgsStyle::defaultStyle(), nullptr, this );
  if ( dlg.exec() == QDialog::Rejected )
  {
    delete markerSymbol;
  }
  else
  {
    delete mMarkerSymbol;
    mMarkerSymbol = markerSymbol;
    if ( mMarkerSymbol )
    {
      QIcon icon = QgsSymbolLayerUtils::symbolPreviewIcon( mMarkerSymbol, mMarkerSymbolButton->iconSize() );
      mMarkerSymbolButton->setIcon( icon );
    }
  }
}

void QgsDecorationGridDialog::on_mPbtnUpdateFromExtents_clicked()
{
  updateInterval( true );
}

void QgsDecorationGridDialog::on_mPbtnUpdateFromLayer_clicked()
{
  double values[4];
  if ( mDeco.getIntervalFromCurrentLayer( values ) )
  {
    mIntervalXEdit->setText( QString::number( values[0] ) );
    mIntervalYEdit->setText( QString::number( values[1] ) );
    mOffsetXEdit->setText( QString::number( values[2] ) );
    mOffsetYEdit->setText( QString::number( values[3] ) );
    if ( values[0] >= 1 )
      mCoordinatePrecisionSpinBox->setValue( 0 );
    else
      mCoordinatePrecisionSpinBox->setValue( 3 );
  }
}

void QgsDecorationGridDialog::on_mAnnotationFontButton_clicked()
{
  bool ok;
  QFont newFont = QgsGuiUtils::getFont( ok, mDeco.gridAnnotationFont() );
  if ( ok )
  {
    mDeco.setGridAnnotationFont( newFont );
  }
}

void QgsDecorationGridDialog::updateInterval( bool force )
{
  if ( force || mDeco.isDirty() )
  {
    double values[4];
    if ( mDeco.getIntervalFromExtent( values, true ) )
    {
      mIntervalXEdit->setText( QString::number( values[0] ) );
      mIntervalYEdit->setText( QString::number( values[1] ) );
      mOffsetXEdit->setText( QString::number( values[2] ) );
      mOffsetYEdit->setText( QString::number( values[3] ) );
      // also update coord. precision
      // if interval >= 1, set precision=0 because we have a rounded value
      // else set it to previous default of 3
      if ( values[0] >= 1 )
        mCoordinatePrecisionSpinBox->setValue( 0 );
      else
        mCoordinatePrecisionSpinBox->setValue( 3 );
    }
  }
}
