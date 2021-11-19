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
    void reset();
    void zoom_in();
    void zoom_out();
    void normal_size();
    void fit_to_window();
    void about();

private:
    void create_actions();
    void create_menus();
    void update_zoom_actions();
    bool update_image();
    void scale_image(double factor);
    void adjust_scroll_bar(QScrollBar *scroll_bar, double factor);
    void make_solution_image();
    void parse_input_image();
    void parse_input_txt();
    void update_gui_after_load();

    double          m_scale_factor         = 1;
    std::string     m_current_file_name;
    cv::Mat         m_current_image;

    Nonogram                *m_nonogram   = nullptr;
    NonogramInput           *m_input      = nullptr;
    imgsolver::GridFinder   *m_finder     = nullptr;
    //TODO: make configurable
    enum imgsolver::detector m_ocr_detector = imgsolver::detector::DNN;

    QLabel          *image_label;
    QScrollArea     *scroll_area;
    
    QAction         *solve_act;
    QAction         *solve_step_act;
    QAction         *reset_act;

    QAction         *zoom_in_act;
    QAction         *zoom_out_act;
    QAction         *normal_size_act;
    QAction         *fit_to_window_act;
};
