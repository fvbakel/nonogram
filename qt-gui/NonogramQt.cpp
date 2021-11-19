#include "NonogramQt.h"

#include <QtWidgets>

NonogramQt::NonogramQt(QWidget *parent)
   : QMainWindow(parent), image_label(new QLabel)
   , scroll_area(new QScrollArea)
{
    image_label->setBackgroundRole(QPalette::Base);
    image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image_label->setScaledContents(true);

    scroll_area->setBackgroundRole(QPalette::Dark);
    scroll_area->setWidget(image_label);
    scroll_area->setVisible(false);
    
    setCentralWidget(scroll_area);

    create_actions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    const QString message = tr("Select File -> Open...");
    statusBar()->showMessage(message);
}

void NonogramQt::parse_input_image() {
    m_finder    = new imgsolver::GridFinder(&m_current_image,m_ocr_detector);
    m_input     = m_finder->parse();
    m_nonogram  = new Nonogram(*m_input);
}
void NonogramQt::parse_input_txt() {
    m_finder    = nullptr;
    m_input     = nullptr;
    m_nonogram  = new Nonogram(m_current_file_name);
}

bool NonogramQt::loadFile(const QString &fileName)
{
    m_current_file_name = fileName.toStdString();
    m_current_image = cv::imread(m_current_file_name,cv::IMREAD_GRAYSCALE);
    if (!update_image()) {
        return false;
    }
    NonogramQt::prependToRecentFiles(fileName);

    parse_input_image();
    update_gui_after_load();

    if (!fit_to_window_act->isChecked())
        image_label->adjustSize();
        
    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\"")
        .arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);

    return true;
}

bool NonogramQt::update_image()
{
    QPixmap pixmap;
    if(m_current_image.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)m_current_image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, m_current_image.cols, m_current_image.rows, m_current_image.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        pixmap = QPixmap::fromImage(img);
    } else if(m_current_image.type()==CV_8UC3) {
         // 8-bits unsigned, NO. OF CHANNELS=3
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)m_current_image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, m_current_image.cols, m_current_image.rows, m_current_image.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        QMessageBox::information(this, tr("Nonogram Qt"),
            tr("Unable to display image."));
        return false;
    }

    image_label->setPixmap(pixmap);
    
    return true;
}

void NonogramQt::update_gui_after_load() {
    scroll_area->setVisible(true);
    fit_to_window_act->setEnabled(true);
    solve_act->setEnabled(true);
    solve_step_act->setEnabled(true);
    reset_act->setEnabled(true);
    
    update_zoom_actions();
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("png");
}

void NonogramQt::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void NonogramQt::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool NonogramQt::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void NonogramQt::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

QString NonogramQt::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void NonogramQt::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = NonogramQt::strippedName(recentFiles.at(i));
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void NonogramQt::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        loadFile(action->data().toString());
}

void NonogramQt::solve() {
    QString message;
    if (m_nonogram != nullptr) {
        m_nonogram->reset();
        if (m_loc_observer != nullptr) {
            m_nonogram->attach_observer(m_loc_observer);
        }
        m_nonogram->solve();
        make_solution_image();
        update_image();
        if (m_nonogram->is_solved()) {
            message = tr("Solved successfully");
        } else {
            message = tr("Unable to solve");
        }
    } else {
        message = tr("No puzzle is loaded!");
    }
    statusBar()->showMessage(message);
}

void NonogramQt::enable_step_by_step() {
    if (m_loc_observer == nullptr) {
        m_loc_observer = new Location_observer(this);
    }
}
void NonogramQt::disable_step_by_step() {
    if (m_loc_observer != nullptr) {
        m_nonogram->detach_observer(m_loc_observer);
        delete m_loc_observer;
        m_loc_observer = nullptr;
    }
}

void NonogramQt::change_step_by_step() {
    if (solve_step_act->isChecked()) {
        enable_step_by_step();
    } else {
        disable_step_by_step();
    }
}

void NonogramQt::reset() {
    QString file_name = QString(m_current_file_name.c_str());
    loadFile(file_name);
}

/*
Public method
*/
void NonogramQt::draw_location(Location *location) {
    _draw_location(location);
    update_image();
    image_label->repaint();
}

/*
Internal method
*/
void NonogramQt::_draw_location(Location *location) {
    int x_index = location->get_x();
    int y_index = location->get_y();
    enum color loc_color = location->get_color();
    cv::Vec3b *chosen = &white_col;
    if (loc_color == black) {
        chosen = &black_col;
    } else if (loc_color == no_color) {
        chosen = &no_color_col;
    }
    cv::Rect loc_rect;
    m_finder->get_location(x_index,y_index,loc_rect);
    cv::rectangle(m_current_image,loc_rect,*chosen,cv::FILLED);
}

void NonogramQt::make_solution_image() {

    int x_size = m_nonogram->get_x_size();
    int y_size = m_nonogram->get_y_size();

    for (int y_index = 0; y_index < y_size; y_index++) {
        for (int x_index = 0; x_index < x_size; x_index++) {
            Location *location = m_nonogram->get_Location(x_index, y_index);
            _draw_location(location);
        }
    }
}

void NonogramQt::zoom_in()
{
    scale_image(1.25);
}

void NonogramQt::zoom_out()
{
    scale_image(0.8);
}

void NonogramQt::normal_size()
{
    image_label->adjustSize();
    m_scale_factor = 1.0;
}

void NonogramQt::fit_to_window()
{
    bool fitToWindow = fit_to_window_act->isChecked();
    scroll_area->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normal_size();
    update_zoom_actions();
}

void NonogramQt::about()
{
    QMessageBox::about(this, tr("About Nonogram Qt"),
            tr("<p><b>Nonogram Qt</b> solves nonogram puzzles. "
               "</p>"));
}

void NonogramQt::create_actions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &NonogramQt::open);
    openAct->setShortcut(QKeySequence::Open);

    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &NonogramQt::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &NonogramQt::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = fileMenu->addSeparator();

    setRecentFilesVisible(NonogramQt::hasRecentFiles());

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *solveMenu = menuBar()->addMenu(tr("&Solve"));
    solve_act = solveMenu->addAction(tr("&Solve"), this, &NonogramQt::solve);
    solve_act->setShortcut(tr("Ctrl+S"));
    solve_act->setEnabled(false);
    
    solveMenu->addSeparator();

    solve_step_act = solveMenu->addAction(tr("&Step by step"), this, &NonogramQt::change_step_by_step);
    solve_step_act->setCheckable(true);
    solve_step_act->setEnabled(false);

    reset_act = solveMenu->addAction(tr("&Reset"), this, &NonogramQt::reset);
    reset_act->setShortcut(tr("Ctrl+R"));
    reset_act->setEnabled(false);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoom_in_act = viewMenu->addAction(tr("Zoom &In (25%)"), this, &NonogramQt::zoom_in);
    zoom_in_act->setShortcut(QKeySequence::ZoomIn);
    zoom_in_act->setEnabled(false);

    zoom_out_act = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &NonogramQt::zoom_out);
    zoom_out_act->setShortcut(QKeySequence::ZoomOut);
    zoom_out_act->setEnabled(false);

    normal_size_act = viewMenu->addAction(tr("&Normal Size"), this, &NonogramQt::normal_size);
    normal_size_act->setShortcut(tr("Ctrl+N"));
    normal_size_act->setEnabled(false);

    viewMenu->addSeparator();

    fit_to_window_act = viewMenu->addAction(tr("&Fit to Window"), this, &NonogramQt::fit_to_window);
    fit_to_window_act->setEnabled(false);
    fit_to_window_act->setCheckable(true);
    fit_to_window_act->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &NonogramQt::about);
}

void NonogramQt::update_zoom_actions()
{
    zoom_in_act->setEnabled(!fit_to_window_act->isChecked());
    zoom_out_act->setEnabled(!fit_to_window_act->isChecked());
    normal_size_act->setEnabled(!fit_to_window_act->isChecked());
}

void NonogramQt::scale_image(double factor)
{
    m_scale_factor *= factor;
    image_label->resize(m_scale_factor * image_label->pixmap(Qt::ReturnByValue).size());

    adjust_scroll_bar(scroll_area->horizontalScrollBar(), factor);
    adjust_scroll_bar(scroll_area->verticalScrollBar(), factor);

    zoom_in_act->setEnabled(m_scale_factor < 3.0);
    zoom_out_act->setEnabled(m_scale_factor > 0.333);
}

void NonogramQt::adjust_scroll_bar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}


NonogramQt::~NonogramQt() {
    if (m_nonogram != nullptr) {
        delete m_nonogram;
    }

    if (m_input != nullptr) {
        delete m_input;
    }

    if (m_finder != nullptr) {
        delete m_finder;
    }

    if (m_loc_observer != nullptr) {
        delete m_loc_observer;
    }
}