#pragma once

#include "OcctQtViewer.h"
#include "ui_MainWindow.h"



#include <ctime>
#include "pch.h"
#include "LogStream.h"
#include "ConvexHull_GrahamScan_V.h"
#include "ConvexHull3D_V.h"


class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  /// @brief
  /// @param parent
  MainWindow(QWidget *parent = nullptr);

private:
    void setTopView();

private Q_SLOTS:
    void Run();
    void on_updateView();
    void on_clearView();
private:
  Ui::MainWindow *ui;
  LogStream* m_logStream = nullptr;
};
