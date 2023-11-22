#pragma once


#include "OcctQtViewer.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QComboBox>
#include <QDateTime>
#include <QFileDialog>
#include <QMutex>
#include <QPushButton>
#include <QScreen>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QSurfaceFormat>
#include <QtWidgets/QMainWindow>

#include <QRandomGenerator>
#include <ctime>
#include <STEPControl_Reader.hxx>
#include "rbTree.h"

class RpBevel : public QMainWindow {
  Q_OBJECT

public:
  /// @brief
  /// @param parent
  RpBevel(QWidget *parent = nullptr);

private:
  /// @brief
  void removeAll();

  /// @brief
  void displayShape(const TopoDS_Shape &shape, bool update);

  void setTopView();

private Q_SLOTS:
    void Run();

public:
  /// @brief
  /// @tparam ...Args
  /// @param format
  /// @param ...args
  template <typename... Args>
  void WriteLine(const std::string &format, Args... args) const {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_s <= 0)
      return;
    size_t size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    std::string msg(buf.get(), buf.get() + size - 1);
    std::cout << QDateTime::currentDateTime()
                     .toString("[yyyy-MM-dd HH:mm:ss:zzz] ")
                     .toStdString()
              << msg << std::endl;
  }

private:
  Ui::MainWindow *ui;
};
