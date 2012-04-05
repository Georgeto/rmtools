#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rimy3d.h"

MainWindow::MainWindow(QWidget * a_pParent) :
    QMainWindow( a_pParent ),
    m_pUi( new Ui::MainWindow )
{
    m_pUi->setupUi( this );
    connect( Rimy3D::getInstance(), SIGNAL( onSaveSettings( QSettings & ) ), this, SLOT( saveSettings( QSettings & ) ) );
    connect( Rimy3D::getInstance(), SIGNAL( onLoadSettings( QSettings & ) ), this, SLOT( loadSettings( QSettings & ) ) );
    Rimy3D::setMainWindow( this );
    Rimy3D::loadSettings();
    updateLanguage();
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}

void MainWindow::loadSettings( QSettings & a_Settings )
{
    a_Settings.beginGroup( "MainWindow" );
    resize( a_Settings.value( "size", size() ).toSize() );
    move( a_Settings.value( "pos", pos() ).toPoint() );
    m_RecentFiles.clear();
    for ( int i = a_Settings.value( "recentfilecount", 0 ).toInt(); i--; )
        m_RecentFiles.enqueue( a_Settings.value( "recentfile" + QString::number( i ) ).toString() );
    a_Settings.endGroup();
    updateRecentFiles();
}

void MainWindow::saveSettings( QSettings & a_Settings )
{
    a_Settings.beginGroup( "MainWindow" );
    a_Settings.setValue( "size", size() );
    a_Settings.setValue( "pos", pos() );
    int iQueueSize = m_RecentFiles.size();
    a_Settings.setValue( "recentfilecount", iQueueSize );
    for ( QList< QString >::iterator it = m_RecentFiles.begin(); iQueueSize--; ++it )
        a_Settings.setValue( "recentfile" + QString::number( iQueueSize ), *it );
    a_Settings.endGroup();
}

void MainWindow::changeEvent( QEvent * a_pEvent )
{
    if ( a_pEvent->type() == QEvent::LanguageChange )
        updateLanguage();
    QMainWindow::changeEvent( a_pEvent );
}

void MainWindow::closeEvent( QCloseEvent * a_pEvent )
{
    Rimy3D::saveSettings();
    QMainWindow::closeEvent( a_pEvent );
}

void MainWindow::open( QString a_strFileName )
{
    if ( a_strFileName == "" )
        return;
#ifdef Q_OS_WIN32
    a_strFileName.replace( '/', '\\' );
#endif
    m_RecentFiles.removeOne( a_strFileName );
    if ( m_SceneInfo.openSceneFile( a_strFileName ) )
        m_RecentFiles.enqueue( a_strFileName );
    updateRecentFiles();
}

void MainWindow::updateLanguage( void )
{
    char const * const arrIconFileNames[ Rimy3D::ELanguage_Count ] =
    {
        "gb.png",
        "de.png"
    };
    m_pUi->menuLanguage->setIcon( QIcon( QString( ":/flags/icons/" ).append( arrIconFileNames[ Rimy3D::getLanguage() ] ) ) );
    m_pUi->retranslateUi( this );
}

void MainWindow::updateRecentFiles( void )
{
    enum { ERecentFileCountMax = 5 };
    QAction * arrActions[ ERecentFileCountMax ];
    arrActions[ 0 ] = m_pUi->actionRecent1;
    arrActions[ 1 ] = m_pUi->actionRecent2;
    arrActions[ 2 ] = m_pUi->actionRecent3;
    arrActions[ 3 ] = m_pUi->actionRecent4;
    arrActions[ 4 ] = m_pUi->actionRecent5;
    int iRecentFileCount = m_RecentFiles.count();
    for ( int i = ERecentFileCountMax; i-- > iRecentFileCount; arrActions[ i ]->setVisible( false ) );
    for ( QList< QString >::iterator it = m_RecentFiles.begin(); iRecentFileCount--; ++it )
    {
        arrActions[ iRecentFileCount ]->setVisible( true );
        arrActions[ iRecentFileCount ]->setText( *it );
    }
Q_OS_WIN32
}

void MainWindow::on_actionEnglish_triggered( void )
{
    Rimy3D::setLanguage( Rimy3D::ELanguage_English );
}

void MainWindow::on_actionGerman_triggered( void )
{
    Rimy3D::setLanguage( Rimy3D::ELanguage_German );
}

void MainWindow::on_actionOpen_triggered()
{
    QString strFilter = tr( "All known types" ).append( " (*.obj *.3db *.gmax *.ase *.asc *.xact);;"
                        "Wavefront OBJ format (*.obj);;"
                        "Baltram's 3D format (*.3db);;"
                        "GMax Scene (*.gmax);;"
                        "3ds Max ASCII Scene (*.ase);;"
                        "Gothic ASCII Scene (*.asc);;"
                        "Gothic 3 Motion Actor (*.xact);;" );
    open( QFileDialog::getOpenFileName( this, tr( "Open" ), m_SceneInfo.getCurrentDir(), strFilter ) );
}

void MainWindow::on_actionRecent1_triggered()
{
    open( m_pUi->actionRecent1->text() );
}

void MainWindow::on_actionRecent2_triggered()
{
    open( m_pUi->actionRecent2->text() );
}

void MainWindow::on_actionRecent3_triggered()
{
    open( m_pUi->actionRecent3->text() );
}

void MainWindow::on_actionRecent4_triggered()
{
    open( m_pUi->actionRecent4->text() );
}

void MainWindow::on_actionRecent5_triggered()
{
    open( m_pUi->actionRecent5->text() );
}