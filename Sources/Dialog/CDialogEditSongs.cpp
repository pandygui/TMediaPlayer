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

#include "CDialogEditSongs.hpp"
#include "CApplication.hpp"
#include "CSpecialSpinBox.hpp"
#include <QStandardItemModel>
#include <QPushButton>


/**
 * Construit la boite de dialogue pour modifier les informations de plusieurs morceaux.
 *
 * \param songItemList Liste des morceaux à modifier.
 * \param application  Pointeur sur l'application.
 */

CDialogEditSongs::CDialogEditSongs(QList<CSongTableItem *>& songItemList, CApplication * application) :
    QDialog             (application),
    m_uiWidget          (new Ui::DialogEditSongs()),
    m_editRating        (NULL),
    m_differentComments (false),
    m_differentLyrics   (false),
    m_songItemList      (songItemList)
{
    Q_CHECK_PTR(application);
    Q_ASSERT(songItemList.size() > 1);

    setAttribute(Qt::WA_DeleteOnClose);
    m_uiWidget->setupUi(this);


    // Synchronisation des champs
    connect(m_uiWidget->editTitle, SIGNAL(textEdited(const QString&)), m_uiWidget->editTitle_2, SLOT(setText(const QString&)));
    connect(m_uiWidget->editTitle_2, SIGNAL(textEdited(const QString&)), m_uiWidget->editTitle, SLOT(setText(const QString&)));

    connect(m_uiWidget->chTitle, SIGNAL(toggled(bool)), m_uiWidget->chTitle_2, SLOT(setChecked(bool)));
    connect(m_uiWidget->chTitle_2, SIGNAL(toggled(bool)), m_uiWidget->chTitle, SLOT(setChecked(bool)));

    connect(m_uiWidget->editArtist, SIGNAL(textEdited(const QString&)), m_uiWidget->editArtist_2, SLOT(setText(const QString&)));
    connect(m_uiWidget->editArtist_2, SIGNAL(textEdited(const QString&)), m_uiWidget->editArtist, SLOT(setText(const QString&)));

    connect(m_uiWidget->chArtist, SIGNAL(toggled(bool)), m_uiWidget->chArtist_2, SLOT(setChecked(bool)));
    connect(m_uiWidget->chArtist_2, SIGNAL(toggled(bool)), m_uiWidget->chArtist, SLOT(setChecked(bool)));

    connect(m_uiWidget->editAlbum, SIGNAL(textEdited(const QString&)), m_uiWidget->editAlbum_2, SLOT(setText(const QString&)));
    connect(m_uiWidget->editAlbum_2, SIGNAL(textEdited(const QString&)), m_uiWidget->editAlbum, SLOT(setText(const QString&)));

    connect(m_uiWidget->chAlbum, SIGNAL(toggled(bool)), m_uiWidget->chAlbum_2, SLOT(setChecked(bool)));
    connect(m_uiWidget->chAlbum_2, SIGNAL(toggled(bool)), m_uiWidget->chAlbum, SLOT(setChecked(bool)));

    connect(m_uiWidget->editAlbumArtist, SIGNAL(textEdited(const QString&)), m_uiWidget->editAlbumArtist_2, SLOT(setText(const QString&)));
    connect(m_uiWidget->editAlbumArtist_2, SIGNAL(textEdited(const QString&)), m_uiWidget->editAlbumArtist, SLOT(setText(const QString&)));

    connect(m_uiWidget->chAlbumArtist, SIGNAL(toggled(bool)), m_uiWidget->chAlbumArtist_2, SLOT(setChecked(bool)));
    connect(m_uiWidget->chAlbumArtist_2, SIGNAL(toggled(bool)), m_uiWidget->chAlbumArtist, SLOT(setChecked(bool)));

    connect(m_uiWidget->editComposer, SIGNAL(textEdited(const QString&)), m_uiWidget->editComposer_2, SLOT(setText(const QString&)));
    connect(m_uiWidget->editComposer_2, SIGNAL(textEdited(const QString&)), m_uiWidget->editComposer, SLOT(setText(const QString&)));

    connect(m_uiWidget->chComposer, SIGNAL(toggled(bool)), m_uiWidget->chComposer_2, SLOT(setChecked(bool)));
    connect(m_uiWidget->chComposer_2, SIGNAL(toggled(bool)), m_uiWidget->chComposer, SLOT(setChecked(bool)));


    // Liste des langues
    m_uiWidget->editLanguage->addItems(CSong::getLanguageList());

    
    // Recherche des données similaires pour tous les éléments
    QString songTitle;           bool songTitleSim           = true;
    QString songTitleSort;       bool songTitleSortSim       = true;
    QString songArtist;          bool songArtistSim          = true;
    QString songArtistSort;      bool songArtistSortSim      = true;
    QString songAlbum;           bool songAlbumSim           = true;
    QString songAlbumSort;       bool songAlbumSortSim       = true;
    QString songAlbumArtist;     bool songAlbumArtistSim     = true;
    QString songAlbumArtistSort; bool songAlbumArtistSortSim = true;
    QString songComposer;        bool songComposerSim        = true;
    QString songComposerSort;    bool songComposerSortSim    = true;
    QString songSubTitle;        bool songSubTitleSim        = true;
    QString songGrouping;        bool songGroupingSim        = true;
    QString songComments;        bool songCommentsSim        = true;
    QString songGenre;           bool songGenreSim           = true;
    QString songLyrics;          bool songLyricsSim          = true;
    QString songLyricist;        bool songLyricistSim        = true;

    int songYear        = 0; bool songYearSim        = true;
    int songTrackNumber = 0; bool songTrackNumberSim = true;
    int songTrackCount  = 0; bool songTrackCountSim  = true;
    int songDiscNumber  = 0; bool songDiscNumberSim  = true;
    int songDiscCount   = 0; bool songDiscCountSim   = true;
    int songBPM         = 0; bool songBPMSim         = true;
    int songRating      = 0; bool songRatingSim      = true;

    CSong::TLanguage songLanguage = CSong::LangUnknown;
    bool songLanguageSim = true;

    bool songEnabled     = false; bool songEnabledSim     = true;
    bool songSkipShuffle = false; bool songSkipShuffleSim = true;
    bool songCompilation = false; bool songCompilationSim = true;


    bool first = true;

    for (QList<CSongTableItem *>::const_iterator it = m_songItemList.begin(); it != m_songItemList.end(); ++it)
    {
        Q_CHECK_PTR(*it);
        CSong * song = (*it)->getSong();

        if (first)
        {
            songTitle           = song->getTitle();
            songTitleSort       = song->getTitleSort();
            songArtist          = song->getArtistName();
            songArtistSort      = song->getArtistNameSort();
            songAlbum           = song->getAlbumTitle();
            songAlbumSort       = song->getAlbumTitleSort();
            songAlbumArtist     = song->getAlbumArtist();
            songAlbumArtistSort = song->getAlbumArtistSort();
            songComposer        = song->getComposer();
            songComposerSort    = song->getComposerSort();
            songSubTitle        = song->getSubTitle();
            songGrouping        = song->getGrouping();
            songComments        = song->getComments();
            songGenre           = song->getGenre();
            songLyrics          = song->getLyrics();
            songLyricist        = song->getLyricist();

            songYear        = song->getYear();
            songTrackNumber = song->getTrackNumber();
            songTrackCount  = song->getTrackCount();
            songDiscNumber  = song->getDiscNumber();
            songDiscCount   = song->getDiscCount();
            songBPM         = song->getBPM();
            songRating      = song->getRating();

            songLanguage    = song->getLanguage();

            songEnabled     = song->isEnabled();
            songSkipShuffle = song->isSkipShuffle();
            songCompilation = song->isCompilation();

            first = false;
        }
        else
        {
            if (songTitleSim           && song->getTitle()           != songTitle          ) { songTitleSim           = false; songTitle          .clear(); }
            if (songTitleSortSim       && song->getTitleSort()       != songTitleSort      ) { songTitleSortSim       = false; songTitleSort      .clear(); }
            if (songArtistSim          && song->getArtistName()      != songArtist         ) { songArtistSim          = false; songArtist         .clear(); }
            if (songArtistSortSim      && song->getArtistNameSort()  != songArtistSort     ) { songArtistSortSim      = false; songArtistSort     .clear(); }
            if (songAlbumSim           && song->getAlbumTitle()      != songAlbum          ) { songAlbumSim           = false; songAlbum          .clear(); }
            if (songAlbumSortSim       && song->getAlbumTitleSort()  != songAlbumSort      ) { songAlbumSortSim       = false; songAlbumSort      .clear(); }
            if (songAlbumArtistSim     && song->getAlbumArtist()     != songAlbumArtist    ) { songAlbumArtistSim     = false; songAlbumArtist    .clear(); }
            if (songAlbumArtistSortSim && song->getAlbumArtistSort() != songAlbumArtistSort) { songAlbumArtistSortSim = false; songAlbumArtistSort.clear(); }
            if (songComposerSim        && song->getComposer()        != songComposer       ) { songComposerSim        = false; songComposer       .clear(); }
            if (songComposerSortSim    && song->getComposerSort()    != songComposerSort   ) { songComposerSortSim    = false; songComposerSort   .clear(); }
            if (songSubTitleSim        && song->getSubTitle()        != songSubTitle       ) { songSubTitleSim        = false; songSubTitle       .clear(); }
            if (songGroupingSim        && song->getGrouping()        != songGrouping       ) { songGroupingSim        = false; songGrouping       .clear(); }
            if (songCommentsSim        && song->getComments()        != songComments       ) { songCommentsSim        = false; songComments       .clear(); }
            if (songGenreSim           && song->getGenre()           != songGenre          ) { songGenreSim           = false; songGenre          .clear(); }
            if (songLyricsSim          && song->getLyrics()          != songLyrics         ) { songLyricsSim          = false; songLyrics         .clear(); }
            if (songLyricistSim        && song->getLyricist()        != songLyricist       ) { songLyricistSim        = false; songLyricist       .clear(); }

            if (songYearSim        && song->getYear()        != songYear        ) { songYearSim        = false; songYear        = 0; }
            if (songTrackNumberSim && song->getTrackNumber() != songTrackNumber ) { songTrackNumberSim = false; songTrackNumber = 0; }
            if (songTrackCountSim  && song->getTrackCount()  != songTrackCount  ) { songTrackCountSim  = false; songTrackCount  = 0; }
            if (songDiscNumberSim  && song->getDiscNumber()  != songDiscNumber  ) { songDiscNumberSim  = false; songDiscNumber  = 0; }
            if (songDiscCountSim   && song->getDiscCount()   != songDiscCount   ) { songDiscCountSim   = false; songDiscCount   = 0; }
            if (songBPMSim         && song->getBPM()         != songBPM         ) { songBPMSim         = false; songBPM         = 0; }
            if (songRatingSim      && song->getRating()      != songRating      ) { songRatingSim      = false; songRating      = 0; }

            if (songLanguageSim && song->getLanguage() != songLanguage)
            {
                songLanguageSim = false;
                songLanguage = CSong::LangUnknown;
            }

            if (songEnabledSim     && song->isEnabled()     != songEnabled    ) songEnabledSim     = false;
            if (songSkipShuffleSim && song->isSkipShuffle() != songSkipShuffle) songSkipShuffleSim = false;
            if (songCompilationSim && song->isCompilation() != songCompilation) songCompilationSim = false;
        }
    }

    const QString notSimText = tr("Different values");


    // Titre
    m_uiWidget->editTitle->setText(songTitle);
    m_uiWidget->editTitle_2->setText(songTitle);

    if (!songTitleSim)
    {
        m_uiWidget->editTitle->setPlaceholderText(notSimText);
        m_uiWidget->editTitle_2->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editTitle, SIGNAL(textEdited(const QString&)), this, SLOT(onTitleChange(const QString&)));
    connect(m_uiWidget->editTitle_2, SIGNAL(textEdited(const QString&)), this, SLOT(onTitleChange(const QString&)));
    connect(m_uiWidget->chTitle, SIGNAL(clicked(bool)), this, SLOT(onTitleChecked(bool)));
    connect(m_uiWidget->chTitle_2, SIGNAL(clicked(bool)), this, SLOT(onTitleChecked(bool)));

    // Titre pour le tri
    m_uiWidget->editTitleSort->setText(songTitleSort);

    if (!songTitleSortSim)
    {
        m_uiWidget->editTitleSort->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editTitleSort, SIGNAL(textEdited(const QString&)), this, SLOT(onTitleSortChange(const QString&)));
    connect(m_uiWidget->chTitleSort, SIGNAL(clicked(bool)), this, SLOT(onTitleSortChecked(bool)));

    // Artiste
    m_uiWidget->editArtist->setText(songArtist);
    m_uiWidget->editArtist_2->setText(songArtist);

    if (!songArtistSim)
    {
        m_uiWidget->editArtist->setPlaceholderText(notSimText);
        m_uiWidget->editArtist_2->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editArtist, SIGNAL(textEdited(const QString&)), this, SLOT(onArtistChange(const QString&)));
    connect(m_uiWidget->editArtist_2, SIGNAL(textEdited(const QString&)), this, SLOT(onArtistChange(const QString&)));
    connect(m_uiWidget->chArtist, SIGNAL(clicked(bool)), this, SLOT(onArtistChecked(bool)));
    connect(m_uiWidget->chArtist_2, SIGNAL(clicked(bool)), this, SLOT(onArtistChecked(bool)));

    // Artiste pour le tri
    m_uiWidget->editArtistSort->setText(songArtistSort);

    if (!songArtistSortSim)
    {
        m_uiWidget->editArtistSort->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editArtistSort, SIGNAL(textEdited(const QString&)), this, SLOT(onArtistSortChange(const QString&)));
    connect(m_uiWidget->chArtistSort, SIGNAL(clicked(bool)), this, SLOT(onArtistSortChecked(bool)));

    // Album
    m_uiWidget->editAlbum->setText(songAlbum);
    m_uiWidget->editAlbum_2->setText(songAlbum);

    if (!songAlbumSim)
    {
        m_uiWidget->editAlbum->setPlaceholderText(notSimText);
        m_uiWidget->editAlbum_2->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editAlbum, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumChange(const QString&)));
    connect(m_uiWidget->editAlbum_2, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumChange(const QString&)));
    connect(m_uiWidget->chAlbum, SIGNAL(clicked(bool)), this, SLOT(onAlbumChecked(bool)));
    connect(m_uiWidget->chAlbum_2, SIGNAL(clicked(bool)), this, SLOT(onAlbumChecked(bool)));

    // Album pour le tri
    m_uiWidget->editAlbumSort->setText(songAlbumSort);

    if (!songAlbumSortSim)
    {
        m_uiWidget->editAlbumSort->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editAlbumSort, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumSortChange(const QString&)));
    connect(m_uiWidget->chAlbumSort, SIGNAL(clicked(bool)), this, SLOT(onAlbumSortChecked(bool)));

    // Artiste de l'album
    m_uiWidget->editAlbumArtist->setText(songAlbumArtist);
    m_uiWidget->editAlbumArtist_2->setText(songAlbumArtist);

    if (!songAlbumArtistSim)
    {
        m_uiWidget->editAlbumArtist->setPlaceholderText(notSimText);
        m_uiWidget->editAlbumArtist_2->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editAlbumArtist, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumArtistChange(const QString&)));
    connect(m_uiWidget->editAlbumArtist_2, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumArtistChange(const QString&)));
    connect(m_uiWidget->chAlbumArtist, SIGNAL(clicked(bool)), this, SLOT(onAlbumArtistChecked(bool)));
    connect(m_uiWidget->chAlbumArtist_2, SIGNAL(clicked(bool)), this, SLOT(onAlbumArtistChecked(bool)));

    // Artiste de l'album pour le tri
    m_uiWidget->editAlbumArtistSort->setText(songAlbumArtistSort);

    if (!songAlbumArtistSortSim)
    {
        m_uiWidget->editAlbumArtistSort->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editAlbumArtistSort, SIGNAL(textEdited(const QString&)), this, SLOT(onAlbumArtistSortChange(const QString&)));
    connect(m_uiWidget->chAlbumArtistSort, SIGNAL(clicked(bool)), this, SLOT(onAlbumArtistSortChecked(bool)));

    // Compositeur
    m_uiWidget->editComposer->setText(songComposer);
    m_uiWidget->editComposer_2->setText(songComposer);

    if (!songComposerSim)
    {
        m_uiWidget->editComposer->setPlaceholderText(notSimText);
        m_uiWidget->editComposer_2->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editComposer, SIGNAL(textEdited(const QString&)), this, SLOT(onComposerChange(const QString&)));
    connect(m_uiWidget->editComposer_2, SIGNAL(textEdited(const QString&)), this, SLOT(onComposerChange(const QString&)));
    connect(m_uiWidget->chComposer, SIGNAL(clicked(bool)), this, SLOT(onComposerChecked(bool)));
    connect(m_uiWidget->chComposer_2, SIGNAL(clicked(bool)), this, SLOT(onComposerChecked(bool)));

    // Compositeur pour le tri
    m_uiWidget->editComposerSort->setText(songComposerSort);

    if (!songComposerSortSim)
    {
        m_uiWidget->editComposerSort->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editComposerSort, SIGNAL(textEdited(const QString&)), this, SLOT(onComposerSortChange(const QString&)));
    connect(m_uiWidget->chComposerSort, SIGNAL(clicked(bool)), this, SLOT(onComposerSortChecked(bool)));

    // Sous-titre
    m_uiWidget->editSubTitle->setText(songSubTitle);

    if (!songSubTitleSim)
    {
        m_uiWidget->editSubTitle->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editSubTitle, SIGNAL(textEdited(const QString&)), this, SLOT(onSubTitleChange(const QString&)));
    connect(m_uiWidget->chSubTitle, SIGNAL(clicked(bool)), this, SLOT(onSubTitleChecked(bool)));

    // Regroupement
    m_uiWidget->editGrouping->setText(songGrouping);

    if (!songGroupingSim)
    {
        m_uiWidget->editGrouping->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editGrouping, SIGNAL(textEdited(const QString&)), this, SLOT(onGroupingChange(const QString&)));
    connect(m_uiWidget->chGrouping, SIGNAL(clicked(bool)), this, SLOT(onGroupingChecked(bool)));

    // Commentaires
    m_uiWidget->editComments->setText(songComments);

    if (!songCommentsSim)
    {
        m_uiWidget->editComments->setText("<span style='color:grey;'>" + notSimText + "</span>");
        m_differentComments = true;
    }

    connect(m_uiWidget->editComments, SIGNAL(textChanged()), this, SLOT(onCommentsChange()));
    connect(m_uiWidget->chComments, SIGNAL(clicked(bool)), this, SLOT(onCommentsChecked(bool)));

    // Année
    m_uiWidget->editYear->setText(songYear > 0 ? QString::number(songYear) : QString());
    m_uiWidget->editYear->setValidator(new QIntValidator(0, 9999, this));

    if (!songYearSim)
    {
        m_uiWidget->editYear->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editYear, SIGNAL(textEdited(const QString&)), this, SLOT(onYearChange(const QString&)));
    connect(m_uiWidget->chYear, SIGNAL(clicked(bool)), this, SLOT(onYearChecked(bool)));

    // Numéro de piste
    m_uiWidget->editTrackNumber->setText(songTrackNumber > 0 ? QString::number(songTrackNumber) : QString());
    m_uiWidget->editTrackNumber->setValidator(new QIntValidator(0, 999, this));

    if (!songTrackNumberSim)
    {
        m_uiWidget->editTrackNumber->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editTrackNumber, SIGNAL(textEdited(const QString&)), this, SLOT(onTrackNumberChange(const QString&)));
    connect(m_uiWidget->chTrackNumber, SIGNAL(clicked(bool)), this, SLOT(onTrackNumberChecked(bool)));
    
    // Nombre de pistes
    m_uiWidget->editTrackCount->setText(songTrackCount > 0 ? QString::number(songTrackCount) : QString());
    m_uiWidget->editTrackCount->setValidator(new QIntValidator(0, 999, this));

    if (!songTrackCountSim)
    {
        m_uiWidget->editTrackCount->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editTrackCount, SIGNAL(textEdited(const QString&)), this, SLOT(onTrackCountChange(const QString&)));
    connect(m_uiWidget->chTrackCount, SIGNAL(clicked(bool)), this, SLOT(onTrackCountChecked(bool)));

    // Numéro de disque
    m_uiWidget->editDiscNumber->setText(songDiscNumber > 0 ? QString::number(songDiscNumber) : QString());
    m_uiWidget->editDiscNumber->setValidator(new QIntValidator(0, 999, this));

    if (!songDiscNumberSim)
    {
        m_uiWidget->editDiscNumber->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editDiscNumber, SIGNAL(textEdited(const QString&)), this, SLOT(onDiscNumberChange(const QString&)));
    connect(m_uiWidget->chDiscNumber, SIGNAL(clicked(bool)), this, SLOT(onDiscNumberChecked(bool)));
    
    // Nombre de disques
    m_uiWidget->editDiscCount->setText(songDiscCount > 0 ? QString::number(songDiscCount) : QString());
    m_uiWidget->editDiscCount->setValidator(new QIntValidator(0, 999, this));

    if (!songDiscCountSim)
    {
        m_uiWidget->editDiscCount->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editDiscCount, SIGNAL(textEdited(const QString&)), this, SLOT(onDiscCountChange(const QString&)));
    connect(m_uiWidget->chDiscCount, SIGNAL(clicked(bool)), this, SLOT(onDiscCountChecked(bool)));

    // BPM
    m_uiWidget->editBPM->setText(songBPM > 0 ? QString::number(songBPM) : QString());
    m_uiWidget->editBPM->setValidator(new QIntValidator(0, 999, this));

    if (!songBPMSim)
    {
        m_uiWidget->editBPM->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editBPM, SIGNAL(textEdited(const QString&)), this, SLOT(onBPMChange(const QString&)));
    connect(m_uiWidget->chBPM, SIGNAL(clicked(bool)), this, SLOT(onBPMChecked(bool)));

    // Genre
    QStringList genres = application->getGenreList();
    m_uiWidget->editGenre->addItems(genres);
    m_uiWidget->editGenre->lineEdit()->setText(songGenre);
    m_uiWidget->editGenre->setCurrentIndex(m_uiWidget->editGenre->findText(songGenre));

    if (!songGenreSim)
    {
        m_uiWidget->editGenre->lineEdit()->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editGenre, SIGNAL(editTextChanged(const QString&)), this, SLOT(onGenreChange(const QString&)));
    connect(m_uiWidget->editGenre, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onGenreChange(const QString&)));
    connect(m_uiWidget->chGenre, SIGNAL(clicked(bool)), this, SLOT(onGenreChecked(bool)));

    // Note
    m_editRating = new CSpecialSpinBox();
    m_editRating->setObjectName(QString::fromUtf8("editRating"));
    m_editRating->setMaximum(5);
    m_uiWidget->layoutInfos->addWidget(m_editRating, 17, 3, 1, 4);

    if (songRatingSim)
    {
        m_editRating->setValue(songRating);
    }
    else
    {
        m_editRating->setSpecialValue(10);
        m_editRating->setPlaceholderText(notSimText);
    }

    connect(m_editRating, SIGNAL(valueChanged(int)), this, SLOT(onRatingChange(int)));
    connect(m_uiWidget->chRating, SIGNAL(clicked(bool)), this, SLOT(onGenreChecked(bool)));

    // Paroles
    m_uiWidget->editLyrics->setText(songLyrics);

    if (!songLyricsSim)
    {
        m_uiWidget->editLyrics->setText("<span style='color:grey;'>" + notSimText + "</span>");
        m_differentLyrics = true;
    }

    connect(m_uiWidget->editLyrics, SIGNAL(textChanged()), this, SLOT(onLyricsChange()));
    connect(m_uiWidget->chLyrics, SIGNAL(clicked(bool)), this, SLOT(onLyricsChecked(bool)));

    // Langue
    if (songLanguageSim)
    {
        m_uiWidget->editLanguage->setCurrentIndex(songLanguage);
    }
    else
    {
        m_uiWidget->editLanguage->setCurrentIndex(-1);
    }

    connect(m_uiWidget->editLanguage, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onLanguageChange(const QString&)));
    connect(m_uiWidget->chLanguage, SIGNAL(clicked(bool)), this, SLOT(onLanguageChecked(bool)));

    // Parolier
    m_uiWidget->editLyricist->setText(songLyricist);

    if (!songLyricistSim)
    {
        m_uiWidget->editLyricist->setPlaceholderText(notSimText);
    }

    connect(m_uiWidget->editLyricist, SIGNAL(textEdited(const QString&)), this, SLOT(onLyricistChange(const QString&)));
    connect(m_uiWidget->chLyricist, SIGNAL(clicked(bool)), this, SLOT(onLyricistChecked(bool)));

    // Morceau coché
    if (songEnabledSim)
    {
        m_uiWidget->editEnabled->setChecked(songEnabled);
    }
    else
    {
        m_uiWidget->editEnabled->setCheckState(Qt::PartiallyChecked);
    }

    // Ne pas lire en mode aléatoire
    if (songSkipShuffleSim)
    {
        m_uiWidget->editSkipShuffle->setChecked(songSkipShuffle);
    }
    else
    {
        m_uiWidget->editSkipShuffle->setCheckState(Qt::PartiallyChecked);
    }

    // Compilation
    if (songCompilationSim)
    {
        m_uiWidget->editCompilation->setChecked(songCompilation);
    }
    else
    {
        m_uiWidget->editCompilation->setCheckState(Qt::PartiallyChecked);
    }


    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(onFocusChange(QWidget *, QWidget *)));


    // Connexions des signaux des boutons
    QPushButton * btnSave = m_uiWidget->buttonBox->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
    QPushButton * btnCancel = m_uiWidget->buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    QPushButton * btnApply = m_uiWidget->buttonBox->addButton(tr("Apply"), QDialogButtonBox::ApplyRole);

    connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(apply()));
}


/**
 * Détruit la boite de dialogue.
 */

CDialogEditSongs::~CDialogEditSongs()
{
    delete m_uiWidget;
}


/**
 * Enregistre les modifications effectuées sur les morceaux.
 */

void CDialogEditSongs::apply(void)
{
    // Modification de chaque morceau
    for (QList<CSongTableItem *>::const_iterator it = m_songItemList.begin(); it != m_songItemList.end(); ++it)
    {
        CSong * song = (*it)->getSong();

        if (m_uiWidget->chTitle->isChecked())
            song->setTitle(m_uiWidget->editTitle->text());

        if (m_uiWidget->chSubTitle->isChecked())
            song->setSubTitle(m_uiWidget->editSubTitle->text());

        if (m_uiWidget->chGrouping->isChecked())
            song->setGrouping(m_uiWidget->editGrouping->text());

        if (m_uiWidget->chArtist->isChecked())
            song->setArtistName(m_uiWidget->editArtist->text());

        if (m_uiWidget->chAlbum->isChecked())
            song->setAlbumTitle(m_uiWidget->editAlbum->text());

        if (m_uiWidget->chAlbumArtist->isChecked())
            song->setAlbumArtist(m_uiWidget->editAlbumArtist->text());

        if (m_uiWidget->chComposer->isChecked())
            song->setComposer(m_uiWidget->editComposer->text());

        if (m_uiWidget->chTitleSort->isChecked())
            song->setTitleSort(m_uiWidget->editTitleSort->text());

        if (m_uiWidget->chArtistSort->isChecked())
            song->setArtistNameSort(m_uiWidget->editArtistSort->text());

        if (m_uiWidget->chAlbumSort->isChecked())
            song->setAlbumTitleSort(m_uiWidget->editAlbumSort->text());

        if (m_uiWidget->chAlbumArtistSort->isChecked())
            song->setAlbumArtistSort(m_uiWidget->editAlbumArtistSort->text());

        if (m_uiWidget->chComposerSort->isChecked())
            song->setComposerSort(m_uiWidget->editComposerSort->text());

        if (m_uiWidget->chBPM->isChecked())
            song->setBPM(m_uiWidget->editBPM->text().toInt());

        if (m_uiWidget->chYear->isChecked())
            song->setYear(m_uiWidget->editYear->text().toInt());

        if (m_uiWidget->chTrackNumber->isChecked())
            song->setTrackNumber(m_uiWidget->editTrackNumber->text().toInt());

        if (m_uiWidget->chTrackCount->isChecked())
            song->setTrackCount(m_uiWidget->editTrackCount->text().toInt());

        if (m_uiWidget->chDiscNumber->isChecked())
            song->setDiscNumber(m_uiWidget->editDiscNumber->text().toInt());

        if (m_uiWidget->chDiscCount->isChecked())
            song->setDiscCount(m_uiWidget->editDiscCount->text().toInt());

        if (m_uiWidget->chComments->isChecked())
            song->setComments(m_uiWidget->editComments->toPlainText());

        if (m_uiWidget->chGenre->isChecked())
            song->setGenre(m_uiWidget->editGenre->currentText());

        if (m_uiWidget->chRating->isChecked())
            song->setRating(m_editRating->value());

        if (m_uiWidget->chLyrics->isChecked())
            song->setLyrics(m_uiWidget->editLyrics->toPlainText());

        if (m_uiWidget->chLanguage->isChecked())
            song->setLanguage(CSong::getLanguageFromInteger(m_uiWidget->editLanguage->currentIndex()));

        if (m_uiWidget->chLyricist->isChecked())
            song->setLyricist(m_uiWidget->editLyricist->text());

        if (m_uiWidget->editEnabled->checkState() != Qt::PartiallyChecked)
            song->setEnabled(m_uiWidget->editEnabled->isChecked());

        if (m_uiWidget->editSkipShuffle->checkState() != Qt::PartiallyChecked)
            song->setSkipShuffle(m_uiWidget->editSkipShuffle->isChecked());

        if (m_uiWidget->editCompilation->checkState() != Qt::PartiallyChecked)
            song->setCompilation(m_uiWidget->editCompilation->isChecked());

        song->writeTags();
        song->updateDatabase();
    }
}


/**
 * Enregistre les modifications effectuées sur les morceaux et ferme la boite de dialogue.
 */

void CDialogEditSongs::save(void)
{
    apply();
    close();
}


void CDialogEditSongs::onTitleChange(const QString& title)
{
    m_uiWidget->editTitle->setPlaceholderText(QString());
    m_uiWidget->editTitle_2->setPlaceholderText(QString());

    m_uiWidget->chTitle->setChecked(true);
    m_uiWidget->chTitle_2->setChecked(true);
}


void CDialogEditSongs::onTitleSortChange(const QString& title)
{
    m_uiWidget->editTitleSort->setPlaceholderText(QString());
    m_uiWidget->chTitleSort->setChecked(true);
}


void CDialogEditSongs::onArtistChange(const QString& artistName)
{
    m_uiWidget->editArtist->setPlaceholderText(QString());
    m_uiWidget->editArtist_2->setPlaceholderText(QString());

    m_uiWidget->chArtist->setChecked(true);
    m_uiWidget->chArtist_2->setChecked(true);
}


void CDialogEditSongs::onArtistSortChange(const QString& artistName)
{
    m_uiWidget->editArtistSort->setPlaceholderText(QString());
    m_uiWidget->chArtistSort->setChecked(true);
}


void CDialogEditSongs::onAlbumChange(const QString& albumTitle)
{
    m_uiWidget->editAlbum->setPlaceholderText(QString());
    m_uiWidget->editAlbum_2->setPlaceholderText(QString());

    m_uiWidget->chAlbum->setChecked(true);
    m_uiWidget->chAlbum_2->setChecked(true);
}


void CDialogEditSongs::onAlbumSortChange(const QString& albumTitle)
{
    m_uiWidget->editAlbumSort->setPlaceholderText(QString());
    m_uiWidget->chAlbumSort->setChecked(true);
}


void CDialogEditSongs::onAlbumArtistChange(const QString& albumTitle)
{
    m_uiWidget->editAlbumArtist->setPlaceholderText(QString());
    m_uiWidget->editAlbumArtist_2->setPlaceholderText(QString());

    m_uiWidget->chAlbumArtist->setChecked(true);
    m_uiWidget->chAlbumArtist_2->setChecked(true);
}


void CDialogEditSongs::onAlbumArtistSortChange(const QString& albumTitle)
{
    m_uiWidget->editAlbumArtistSort->setPlaceholderText(QString());
    m_uiWidget->chAlbumArtistSort->setChecked(true);
}


void CDialogEditSongs::onComposerChange(const QString& composer)
{
    m_uiWidget->editComposer->setPlaceholderText(QString());
    m_uiWidget->editComposer_2->setPlaceholderText(QString());

    m_uiWidget->chComposer->setChecked(true);
    m_uiWidget->chComposer_2->setChecked(true);
}


void CDialogEditSongs::onComposerSortChange(const QString& composer)
{
    m_uiWidget->editComposerSort->setPlaceholderText(QString());
    m_uiWidget->chComposerSort->setChecked(true);
}


void CDialogEditSongs::onSubTitleChange(const QString& subTitle)
{
    m_uiWidget->editSubTitle->setPlaceholderText(QString());
    m_uiWidget->chSubTitle->setChecked(true);
}


void CDialogEditSongs::onGroupingChange(const QString& grouping)
{
    m_uiWidget->editGrouping->setPlaceholderText(QString());
    m_uiWidget->chGrouping->setChecked(true);
}


void CDialogEditSongs::onCommentsChange(void)
{
    if (m_differentComments)
    {
        m_differentComments = false;
    }

    m_uiWidget->chComments->setChecked(true);
}


void CDialogEditSongs::onYearChange(const QString& year)
{
    m_uiWidget->editYear->setPlaceholderText(QString());
    m_uiWidget->chYear->setChecked(true);
}


void CDialogEditSongs::onTrackNumberChange(const QString& trackNumber)
{
    m_uiWidget->editTrackNumber->setPlaceholderText(QString());
    m_uiWidget->chTrackNumber->setChecked(true);
}


void CDialogEditSongs::onTrackCountChange(const QString& trackCount)
{
    m_uiWidget->editTrackCount->setPlaceholderText(QString());
    m_uiWidget->chTrackCount->setChecked(true);
}


void CDialogEditSongs::onDiscNumberChange(const QString& discNumber)
{
    m_uiWidget->editDiscNumber->setPlaceholderText(QString());
    m_uiWidget->chDiscNumber->setChecked(true);
}


void CDialogEditSongs::onDiscCountChange(const QString& discCount)
{
    m_uiWidget->editDiscCount->setPlaceholderText(QString());
    m_uiWidget->chDiscCount->setChecked(true);
}


void CDialogEditSongs::onBPMChange(const QString& bpm)
{
    m_uiWidget->editBPM->setPlaceholderText(QString());
    m_uiWidget->chBPM->setChecked(true);
}


void CDialogEditSongs::onGenreChange(const QString& genre)
{
    m_uiWidget->editGenre->lineEdit()->setPlaceholderText(QString());
    m_uiWidget->chGenre->setChecked(true);
}


void CDialogEditSongs::onRatingChange(int rating)
{
    m_editRating->setPlaceholderText(QString());
    m_uiWidget->chRating->setChecked(true);
}


void CDialogEditSongs::onLyricsChange(void)
{
    if (m_differentLyrics)
    {
        m_differentLyrics = false;
    }

    m_uiWidget->chLyrics->setChecked(true);
}


void CDialogEditSongs::onLanguageChange(const QString& language)
{
    m_uiWidget->chLanguage->setChecked(true);
}


void CDialogEditSongs::onTitleChecked(bool checked)
{
    m_uiWidget->editTitle->setPlaceholderText(QString());
    m_uiWidget->editTitle_2->setPlaceholderText(QString());
}


void CDialogEditSongs::onTitleSortChecked(bool checked)
{
    m_uiWidget->editTitleSort->setPlaceholderText(QString());
}


void CDialogEditSongs::onArtistChecked(bool checked)
{
    m_uiWidget->editArtist->setPlaceholderText(QString());
    m_uiWidget->editArtist_2->setPlaceholderText(QString());
}


void CDialogEditSongs::onArtistSortChecked(bool checked)
{
    m_uiWidget->editArtistSort->setPlaceholderText(QString());
}


void CDialogEditSongs::onAlbumChecked(bool checked)
{
    m_uiWidget->editAlbum->setPlaceholderText(QString());
    m_uiWidget->editAlbum_2->setPlaceholderText(QString());
}


void CDialogEditSongs::onAlbumSortChecked(bool checked)
{
    m_uiWidget->editAlbumSort->setPlaceholderText(QString());
}


void CDialogEditSongs::onAlbumArtistChecked(bool checked)
{
    m_uiWidget->editAlbumArtist->setPlaceholderText(QString());
    m_uiWidget->editAlbumArtist_2->setPlaceholderText(QString());
}


void CDialogEditSongs::onAlbumArtistSortChecked(bool checked)
{
    m_uiWidget->editAlbumArtistSort->setPlaceholderText(QString());
}


void CDialogEditSongs::onComposerChecked(bool checked)
{
    m_uiWidget->editComposer->setPlaceholderText(QString());
    m_uiWidget->editComposer_2->setPlaceholderText(QString());
}


void CDialogEditSongs::onComposerSortChecked(bool checked)
{
    m_uiWidget->editComposer->setPlaceholderText(QString());
}


void CDialogEditSongs::onSubTitleChecked(bool checked)
{
    m_uiWidget->editSubTitle->setPlaceholderText(QString());
}


void CDialogEditSongs::onGroupingChecked(bool checked)
{
    m_uiWidget->editGrouping->setPlaceholderText(QString());
}


void CDialogEditSongs::onCommentsChecked(bool checked)
{
    if (m_differentComments)
    {
        m_uiWidget->editComments->setText(QString());
        m_differentComments = false;
    }
}


void CDialogEditSongs::onYearChecked(bool checked)
{
    m_uiWidget->editYear->setPlaceholderText(QString());
}


void CDialogEditSongs::onTrackNumberChecked(bool checked)
{
    m_uiWidget->editTrackNumber->setPlaceholderText(QString());
}


void CDialogEditSongs::onTrackCountChecked(bool checked)
{
    m_uiWidget->editTrackCount->setPlaceholderText(QString());
}


void CDialogEditSongs::onDiscNumberChecked(bool checked)
{
    m_uiWidget->editDiscNumber->setPlaceholderText(QString());
}


void CDialogEditSongs::onDiscCountChecked(bool checked)
{
    m_uiWidget->editDiscCount->setPlaceholderText(QString());
}


void CDialogEditSongs::onBPMChecked(bool checked)
{
    m_uiWidget->editBPM->setPlaceholderText(QString());
}


void CDialogEditSongs::onGenreChecked(bool checked)
{
    m_uiWidget->editGenre->lineEdit()->setPlaceholderText(QString());
}


void CDialogEditSongs::onRatingChecked(bool checked)
{
    m_editRating->setPlaceholderText(QString());
}


void CDialogEditSongs::onLyricsChecked(bool checked)
{
    if (m_differentLyrics)
    {
        m_uiWidget->editLyrics->setText(QString());
        m_differentLyrics = false;
    }
}


void CDialogEditSongs::onLanguageChecked(bool checked)
{
    if (m_uiWidget->editLanguage->currentIndex() == -1)
    {
        m_uiWidget->editLanguage->setCurrentIndex(0);
    }
}


void CDialogEditSongs::onFocusChange(QWidget * old, QWidget * now)
{
    if (m_differentComments)
    {
        if (now == m_uiWidget->editComments)
        {
            m_uiWidget->editComments->setText(QString());
            m_differentComments = true;
            m_uiWidget->chComments->setChecked(false);
            return;
        }
        else if (old == m_uiWidget->editComments)
        {
            m_uiWidget->editComments->setText("<span style='color:grey;'>" + tr("Different values") + "</span>");
            m_differentComments = true;
            m_uiWidget->chComments->setChecked(false);
            return;
        }
    }

    if (m_differentLyrics)
    {
        if (now == m_uiWidget->editLyrics)
        {
            m_uiWidget->editLyrics->setText(QString());
            m_differentLyrics = true;
            m_uiWidget->chLyrics->setChecked(false);
            return;
        }
        else if (old == m_uiWidget->editLyrics)
        {
            m_uiWidget->editLyrics->setText("<span style='color:grey;'>" + tr("Different values") + "</span>");
            m_differentLyrics = true;
            m_uiWidget->chLyrics->setChecked(false);
            return;
        }
    }
}