#pragma once
#include <string>

#include <QMainWindow>
#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>

#include <imgsolver/GridFinder.h>
#include <solvercore/Nonogram.h>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class NonogramQt : public QMainWindow
{
    Q_OBJECT

public:
    NonogramQt(QWidget *parent = nullptr);
    ~NonogramQt();
    bool loadFile(const QString &);

private slots:
    void open();

    void solve();
    void solve_step_by_step();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

private:
    void createActions();
    void createMenus();
    void updateActions();
    bool updateImage();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void make_solution_image();

    double          scaleFactor         = 1;
    std::string     m_current_file_name;
    cv::Mat         m_current_image;

    Nonogram                *m_nonogram   = nullptr;
    NonogramInput           *m_input      = nullptr;
    imgsolver::GridFinder   *m_finder     = nullptr;
    //TODO: make configurable
    enum imgsolver::detector ocr_detector = imgsolver::detector::DNN;

    QLabel          *imageLabel;
    QScrollArea     *scrollArea;
    
    QAction         *solveAct;
    QAction         *solveStepAct;

    QAction         *zoomInAct;
    QAction         *zoomOutAct;
    QAction         *normalSizeAct;
    QAction         *fitToWindowAct;
};
