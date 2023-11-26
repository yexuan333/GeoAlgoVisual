#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

#ifndef _DEBUG
  ui->openGLWidget->View()->ChangeRenderingParams().ToShowStats = false;
#endif // !_DEBUG
  setTopView();
  connect(ui->actionRun, &QAction::triggered, this, &MainWindow::Run);
  ui->textBrowser->document()->setMaximumBlockCount(9001);
  ui->textBrowser->viewport()->setAutoFillBackground(false);
  ui->textBrowser->setTextColor(QColor(150, 150, 150));

  m_logStream = new LogStream(std::cout, ui->textBrowser);
  ui->openGLWidget->displayViewCube();
}
void MainWindow::Run() {
    ui->openGLWidget->Context()->EraseAll(true);

    QThread* thread = new QThread;

    std::vector<gp_Pnt> points;
    //points.reserve(50);
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dis(0, 100);

    //for (int i = 0; i < 4; ++i) {
    //    double x = dis(gen);
    //    double y = dis(gen);
    //    points.emplace_back(x, y, 0);
    //}
    points.push_back(gp_Pnt(0, 0, 0));
    points.push_back(gp_Pnt(0, 0, 0));
    points.push_back(gp_Pnt(10, 10, 0));
    points.push_back(gp_Pnt(20, 0, 0));
    points.push_back(gp_Pnt(10, 5, 10));
    ConvexHull3D_V* worker = new ConvexHull3D_V(ui->openGLWidget->Context(), points);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &ConvexHull3D_V::process);
    connect(worker, &ConvexHull3D_V::finished, thread, &QThread::quit);
    connect(worker, &ConvexHull3D_V::finished, worker, &ConvexHull3D_V::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(worker, &ConvexHull3D_V::updateView, this, &MainWindow::on_updateView);
    connect(worker, &ConvexHull3D_V::clearView, this, &MainWindow::on_clearView);

    thread->start();
}

//void MainWindow::Run() {
//    ui->openGLWidget->Context()->EraseAll(true);
//    
//    QThread* thread = new QThread;
//
//    std::vector<gp_Pnt> points;
//    points.reserve(50);
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> dis(0, 100);
//
//    for (int i = 0; i < 50; ++i) {
//        double x = dis(gen);
//        double y = dis(gen);
//        points.emplace_back(x, y, 0);
//    }
//    ConvexHull_GrahamScan_V* worker = new ConvexHull_GrahamScan_V(ui->openGLWidget->Context(), points);
//    worker->moveToThread(thread);
//
//    connect(thread, &QThread::started, worker, &ConvexHull_GrahamScan_V::process);
//    connect(worker, &ConvexHull_GrahamScan_V::finished, thread, &QThread::quit);
//    connect(worker, &ConvexHull_GrahamScan_V::finished, worker, &ConvexHull_GrahamScan_V::deleteLater);
//    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
//    connect(worker, &ConvexHull_GrahamScan_V::updateView, this, &MainWindow::on_updateView);
//    connect(worker, &ConvexHull_GrahamScan_V::clearView, this, &MainWindow::on_clearView);
//
//    thread->start();
//}
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