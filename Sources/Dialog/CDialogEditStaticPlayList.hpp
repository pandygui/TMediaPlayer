/*
Copyright (C) 2012 Teddy Michel

This file is part of TMediaPlayer.

TMediaPlayer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TMediaPlayer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TMediaPlayer. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILE_C_DIALOG_EDIT_STATIC_PLAYLIST
#define FILE_C_DIALOG_EDIT_STATIC_PLAYLIST

#include <QDialog>
#include "ui_DialogEditStaticPlayList.h"


class CStaticPlayList;
class CApplication;
class CFolder;
class CSong;


/**
 * Boite de dialogue permettant la cr�ation ou la modification d'une liste de lecture statique.
 */

class CDialogEditStaticPlayList : public QDialog
{
    Q_OBJECT

public:

    CDialogEditStaticPlayList(CStaticPlayList * playList, CApplication * application, CFolder * folder = NULL, const QList<CSong *>& songs = QList<CSong *>());
    virtual ~CDialogEditStaticPlayList();

protected slots:

    void save(void);

private:
    
    Ui::DialogEditStaticPlayList * m_uiWidget;
    CStaticPlayList * m_playList; ///< Pointeur sur la liste de lecture.
    CApplication * m_application; ///< Pointeur sur l'application.
    CFolder * m_folder;       ///< Pointeur sur le dossier contenant la liste de lecture.
    QList<CSong *> m_songs;       ///< Liste de morceaux � ajouter � la liste.
};

#endif // FILE_C_DIALOG_EDIT_STATIC_PLAYLIST