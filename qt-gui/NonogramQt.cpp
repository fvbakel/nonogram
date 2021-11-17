#include "NonogramQt.h"

#include <QtWidgets>

#include <imgsolver/GridFinder.h>
#include <solvercore/Nonogram.h>

NonogramQt::NonogramQt(QWidget *parent)
   : QMainWindow(parent), imageLabel(new QLabel)
   , scrollArea(new QScrollArea)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    const QString message = tr("Select File -> Open...");
    statusBar()->showMessage(message);
}

bool NonogramQt::loadFile(const QString &fileName)
{
    m_current_file_name = fileName.toStdString();
    m_current_image = cv::imread(m_current_file_name);
    updateImage();

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\"")
        .arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);

    return true;
}

void NonogramQt::updateImage()
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
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(m_current_image.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)m_current_image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, m_current_image.cols, m_current_image.rows, m_current_image.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage or QPixmap.";
        return;
    }

    imageLabel->setPixmap(pixmap);
 
    
    scrollArea->setVisible(true);
    fitToWindowAct->setEnabled(true);
    solveAct->setEnabled(true);
    solveStepAct->setEnabled(true);
    
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
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

void NonogramQt::solve()
{
    QMessageBox::about(this, tr("Nonogram Qt"),
            tr("solve Not implemented yet!"));
}

void NonogramQt::solve_step_by_step()
{
    QMessageBox::about(this, tr("Nonogram Qt"),
            tr("solve_step_by_step Not implemented yet!"));
}

void NonogramQt::zoomIn()
{
    scaleImage(1.25);
}

void NonogramQt::zoomOut()
{
    scaleImage(0.8);
}

void NonogramQt::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void NonogramQt::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void NonogramQt::about()
{
    QMessageBox::about(this, tr("About Nonogram Qt"),
            tr("<p><b>Nonogram Qt</b> solves nonogram puzzles. "
               "</p>"));
}

void NonogramQt::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &NonogramQt::open);
    openAct->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *solveMenu = menuBar()->addMenu(tr("&Solve"));
    solveAct = solveMenu->addAction(tr("&Solve"), this, &NonogramQt::solve);
    solveAct->setShortcut(tr("Ctrl+S"));
    solveAct->setEnabled(false);
    solveStepAct = solveMenu->addAction(tr("&Solve step by step"), this, &NonogramQt::solve_step_by_step);
    solveStepAct->setShortcut(tr("Ctrl+D"));
    solveStepAct->setEnabled(false);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &NonogramQt::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &NonogramQt::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &NonogramQt::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+N"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &NonogramQt::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &NonogramQt::about);
}

void NonogramQt::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void NonogramQt::scaleImage(double factor)
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void NonogramQt::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
