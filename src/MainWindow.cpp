#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

#ifndef _DEBUG
    ui->openGLWidget->View()->ChangeRenderingParams().ToShowStats = false;
#endif // !_DEBUG
    setTopView();
    connect(ui->actionGrahamScan, &QAction::triggered, this, &MainWindow::GrahamScanRun);
    connect(ui->actionIncremental, &QAction::triggered, this, &MainWindow::GrahamScanRun);

    ui->textBrowser->document()->setMaximumBlockCount(9001);
    ui->textBrowser->viewport()->setAutoFillBackground(false);
    ui->textBrowser->setTextColor(QColor(150, 150, 150));
  
    m_logStream = new LogStream(std::cout, ui->textBrowser);
    ui->openGLWidget->displayViewCube();
}

void MainWindow::connectAlgoThread(GeoAlgo_V* algo, QThread* thread) {
    connect(thread, &QThread::started, algo, &GeoAlgo_V::process);
    connect(algo, &GeoAlgo_V::finished, thread, &QThread::quit);
    connect(algo, &GeoAlgo_V::finished, algo, &GeoAlgo_V::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(algo, &GeoAlgo_V::updateView, this, &MainWindow::on_updateView);
    connect(algo, &GeoAlgo_V::clearView, this, &MainWindow::on_clearView);
}

void MainWindow::GrahamScanRun()
{
    on_clearView();
    auto thread = new QThread();
    auto algo = new ConvexHull_GrahamScan_V(ui->openGLWidget->Context());
    algo->moveToThread(thread);
    connectAlgoThread(algo, thread);
    thread->start();
}

void MainWindow::IncrementalRun() {
    on_clearView();
    auto thread = new QThread();
    auto algo = new ConvexHull3D_V(ui->openGLWidget->Context());
    algo->moveToThread(thread);
    connectAlgoThread(algo, thread);
    thread->start();
}

void MainWindow::on_updateView() {
    ui->openGLWidget->Context()->UpdateCurrent();
    repaint();
}

void MainWindow::on_clearView() {
    ui->openGLWidget->Context()->RemoveAll(true);
    ui->openGLWidget->displayViewCube();
    repaint();
}

void MainWindow::setTopView() {
    Handle(Graphic3d_Camera) camera = new Graphic3d_Camera();
    camera->SetEye({0, 0, 100});
    camera->SetDirectionFromEye({0, 0, -1});
    ui->openGLWidget->View()->SetCamera(camera);
}