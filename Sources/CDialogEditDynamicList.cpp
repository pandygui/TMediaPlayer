
#include "CDialogEditDynamicList.hpp"
#include "CDynamicPlayList.hpp"
#include "CApplication.hpp"
#include <QStandardItemModel>
#include <QMessageBox>


/**
 * Constructeur de la boite de dialogue d'�dition des listes de lecture dynamiques.
 *
 * \param playList    Pointeur sur la liste � modifier, ou NULL pour une nouvelle liste.
 * \param application Pointeur sur l'application.
 */

CDialogEditDynamicList::CDialogEditDynamicList(CDynamicPlayList * playList, CApplication * application) :
    QDialog       (application),
    m_uiWidget    (new Ui::DialogEditDynamicPlayList()),
    m_application (application),
    m_playList    (playList)
{
    Q_CHECK_PTR(application);

    setAttribute(Qt::WA_DeleteOnClose);
    m_uiWidget->setupUi(this);

    if (m_playList)
    {
        m_uiWidget->editName->setText(m_playList->getName());
    }

    connect(m_uiWidget->buttonBox, SIGNAL(accepted()), this, SLOT(onSave()));
    connect(m_uiWidget->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}


CDialogEditDynamicList::~CDialogEditDynamicList()
{
    delete m_uiWidget;
}


/// \todo Impl�mentation
void CDialogEditDynamicList::onSave(void)
{
    QString name = m_uiWidget->editName->text();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, QString(), tr("You need to choose a name for the playlist."));
        return;
    }

    if (!m_playList)
    {
        m_playList = new CDynamicPlayList(m_application);
    }

    m_playList->setName(name);
    //...

    if (!m_playList->updateDatabase())
    {
        delete m_playList;
        m_playList = NULL;
        return;
    }

    m_application->addPlayList(m_playList);

    close();
}