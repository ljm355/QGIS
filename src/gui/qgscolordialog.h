/***************************************************************************
    qgscolordialog.h - color selection dialog

    ---------------------
    begin                : March 19, 2013
    copyright            : (C) 2013 by Larry Shaffer
    email                : larrys at dakcarto dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSCOLORDIALOG_H
#define QGSCOLORDIALOG_H

#include <QColorDialog>
#include "ui_qgscolordialog.h"
#include "qgis_gui.h"
#include "qgis.h"

class QColor;

/** \ingroup gui
 * \class QgsColorDialog
 * A custom QGIS dialog for selecting a color. Has many improvements over the standard Qt color picker dialog, including
 * hue wheel supports, color swatches, and a color sampler.
 * \since QGIS 2.5
 */

class GUI_EXPORT QgsColorDialog : public QDialog, private Ui::QgsColorDialogBase
{

    Q_OBJECT

  public:

    /** Create a new color picker dialog
     * \param parent parent widget
     * \param fl window flags
     * \param color initial color for dialog
     */
    QgsColorDialog( QWidget *parent SIP_TRANSFERTHIS = nullptr, Qt::WindowFlags fl = QgsGuiUtils::ModalDialogFlags,
                    const QColor &color = QColor() );

    /** Returns the current color for the dialog
     * \returns dialog color
     */
    QColor color() const;

    /** Sets the title for the color dialog
     * \param title title for dialog box
     */
    void setTitle( const QString &title );

    /** Sets whether opacity modification (transparency) is permitted
     * for the color dialog. Defaults to true.
     * \param allowOpacity set to false to disable opacity modification
     * \since QGIS 3.0
     */
    void setAllowOpacity( const bool allowOpacity );

    /** Return a color selection from a color dialog, with live updating of interim selections.
     * \param initialColor the initial color of the selection dialog.
     * \param updateObject the receiver object of the live updating.
     * \param updateSlot the receiver object's slot for live updating (e.g. SLOT( setValidColor( const QColor& ) ) ).
     * \param parent parent widget
     * \param title the title of the dialog.
     * \param allowOpacity set to true to allow modification of color opacity value (transparency)
     * \returns Selected color on accepted() or initialColor on rejected().
     * \see getColor
     */
    static QColor getLiveColor( const QColor &initialColor, QObject *updateObject, const char *updateSlot,
                                QWidget *parent = nullptr,
                                const QString &title = QString(),
                                const bool allowOpacity = true );

    /** Return a color selection from a color dialog.
     * \param initialColor the initial color of the selection dialog.
     * \param parent parent widget
     * \param title the title of the dialog.
     * \param allowOpacity set to true to allow modification of color opacity value (transparency)
     * \returns Selected color on accepted() or initialColor on rejected().
     * \see getLiveColor
     */
    static QColor getColor( const QColor &initialColor, QWidget *parent, const QString &title = QString(),
                            const bool allowOpacity = false );

  signals:

    /** Emitted when the dialog's color changes
     * \param color current color
     */
    void currentColorChanged( const QColor &color );

  public slots:

    /** Sets the current color for the dialog
     * \param color desired color
     */
    void setColor( const QColor &color );

  protected:

    void closeEvent( QCloseEvent *e ) override;

  private slots:

    void on_mButtonBox_accepted();
    void on_mButtonBox_rejected();
    void on_mButtonBox_clicked( QAbstractButton *button );
    void discardColor();

  private:

    QColor mPreviousColor;

    bool mAllowOpacity;

    /** Saves all dialog and widget settings
     */
    void saveSettings();

};

#endif // #ifndef QGSCOLORDIALOG_H
