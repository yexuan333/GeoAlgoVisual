#include "RpBevel.h"

RpBevel::RpBevel(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

#ifndef _DEBUG
  ui->openGLWidget->View()->ChangeRenderingParams().ToShowStats = false;
#endif // !_DEBUG
  setTopView();
  connect(ui->actionRun, &QAction::triggered, this, &RpBevel::Run);
  ui->textBrowser->document()->setMaximumBlockCount(9001);
  ui->textBrowser->viewport()->setAutoFillBackground(false);
  ui->textBrowser->setTextColor(QColor(150, 150, 150));
}
void RpBevel::Run() {
	// rb_tree<int> tree;
	// int data[] = { 12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17 };
	// int max_len = sizeof(data) / sizeof(int);
	// ui->textBrowser->append("test_rb_tree: insert");
	// for (auto i = 0; i < max_len; ++i)
	// {
	// 	auto debug = data[i];
	// 	tree.insert(debug);
	// }
	// ui->textBrowser->append("test_rb_tree: dump");
	// tree.dump();
	// ui->textBrowser->append("test_rb_tree: height");
	// //ui->textBrowser->setText("height: " + std::to_string(tree.height()));

	// for (auto i = 0; i < max_len; ++i)
	// {
	// 	auto debug = data[i];
	// 	tree.erase(debug);
	// 	//ui->textBrowser->setText("test_rb_tree: height: " + std::to_string(tree.height()) + " erase: " + std::to_string(debug));
	// 	ui->textBrowser->append("test_rb_tree: dump");
	// 	tree.check_rb_feature_5();
	// 	tree.dump();
	// }
}


void RpBevel::displayShape(const TopoDS_Shape &shape, bool update) {
  Handle(AIS_Shape) aShape = new AIS_Shape(shape);
  ui->openGLWidget->Context()->Display(aShape, AIS_Shaded, 0, update);
}

void RpBevel::removeAll() {
  ui->openGLWidget->Context()->RemoveAll(true);
  ui->openGLWidget->displayViewCube();
}

void RpBevel::setTopView() {
  Handle(Graphic3d_Camera) camera = new Graphic3d_Camera();
  camera->SetEye({0, 0, 100});
  camera->SetDirectionFromEye({0, 0, -1});
  ui->openGLWidget->View()->SetCamera(camera);
}