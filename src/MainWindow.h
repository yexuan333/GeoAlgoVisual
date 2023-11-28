#pragma once

#include "OcctQtViewer.h"
#include "ui_MainWindow.h"



#include <ctime>
#include "pch.h"
#include "LogStream.h"
#include "GeoAlgo_V.h"
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
    void connectAlgoThread(GeoAlgo_V* algo, QThread* thread);
private Q_SLOTS:
    void GrahamScanRun();
    void on_updateView();
    void on_clearView();
private:
  Ui::MainWindow *ui;
  LogStream* m_logStream = nullptr;
};