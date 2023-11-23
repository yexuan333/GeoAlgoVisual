#include "MainWindow.h"
#include "ConvexHull_GrahamScan.h"
#include <BRepBuilderAPI_MakePolygon.hxx>


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
}
void MainWindow::Run() {
    ui->openGLWidget->Context()->EraseAll(true);
    std::vector<gp_Pnt> points;
    points.reserve(50);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 100);

    for (int i = 0; i < 50; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        points.emplace_back(x, y, 0);
    }

    for (auto p : points) {
        auto ais = new AIS_Point(new Geom_CartesianPoint(p));
        ui->openGLWidget->Context()->Display(ais,true);
    }

    ConvexHull_GrahamScan op(points);
    if (op.compute()) {
        auto res = op.getResult();
        BRepBuilderAPI_MakePolygon polygon;
        for (std::size_t i = 0; i < res.size(); ++i) {
            auto p3 = gp_Pnt(res[i].X(), res[i].Y(), 0.0);
            polygon.Add(BRepBuilderAPI_MakeVertex(p3));
            AIS_TextLabel* label = new AIS_TextLabel();
            label->SetText(std::to_string(i).c_str());
            label->SetPosition(p3);
            ui->openGLWidget->Context()->Display(label,true);
        }
        auto ais_polygon = new AIS_Shape(polygon.Shape());
        ui->openGLWidget->Context()->Display(ais_polygon, true);
        
    }
    
}


void MainWindow::displayShape(const TopoDS_Shape &shape, bool update) {
  Handle(AIS_Shape) aShape = new AIS_Shape(shape);
  ui->openGLWidget->Context()->Display(aShape, AIS_Shaded, 0, update);
}

void MainWindow::removeAll() {
  ui->openGLWidget->Context()->RemoveAll(true);
  ui->openGLWidget->displayViewCube();
}

void MainWindow::setTopView() {
  Handle(Graphic3d_Camera) camera = new Graphic3d_Camera();
  camera->SetEye({0, 0, 100});
  camera->SetDirectionFromEye({0, 0, -1});
  ui->openGLWidget->View()->SetCamera(camera);
}