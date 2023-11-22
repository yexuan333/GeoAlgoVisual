// Copyright (c) 2021 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software
// library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE

#ifdef _WIN32
#include <windows.h>
#endif

#include <OpenGl_Context.hxx>

#ifndef _OcctQtViewer_HeaderFile
#define _OcctQtViewer_HeaderFile

#include <QOpenGLWidget>
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMainWindow>
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>

#include <AIS_Shape.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <Message.hxx>
#include <OpenGl_FrameBuffer.hxx>
#include <OpenGl_GraphicDriver.hxx>

class AIS_ViewCube;

//! OCCT 3D View.
class OcctQtViewer : public QOpenGLWidget, public AIS_ViewController {
  Q_OBJECT
public:
  enum CurrentAction3d {
    CurAction3d_Nothing,
    CurAction3d_DynamicZooming,
    CurAction3d_WindowZooming,
    CurAction3d_DynamicPanning,
    CurAction3d_GlobalPanning,
    CurAction3d_DynamicRotation,
    CurAction3d_Sketcher
  };
  //! Main constructor.
  OcctQtViewer(QWidget *theParent = nullptr);

  //! Destructor.
  virtual ~OcctQtViewer();

  //! Return Viewer.
  const Handle(V3d_Viewer) & Viewer() const { return myViewer; }

  //! Return View.
  const Handle(V3d_View) & View() const { return myView; }

  //! Return AIS context.
  const Handle(AIS_InteractiveContext) & Context() const { return myContext; }

  //! Return OpenGL info.
  const QString &getGlInfo() const { return myGlInfo; }

  //! Minial widget size.
  virtual QSize minimumSizeHint() const override { return QSize(200, 200); }

  //! Default widget size.
  virtual QSize sizeHint() const override { return QSize(720, 480); }

  void displayViewCube();

protected: // OpenGL events
  virtual void initializeGL() override;
  virtual void paintGL() override;
  // virtual void resizeGL(int , int ) override;

protected: // user input events
  virtual void closeEvent(QCloseEvent *theEvent) override;
  virtual void keyPressEvent(QKeyEvent *theEvent) override;
  virtual void mousePressEvent(QMouseEvent *theEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent *theEvent) override;
  virtual void mouseMoveEvent(QMouseEvent *theEvent) override;
  virtual void wheelEvent(QWheelEvent *theEvent) override;

private:
  //! Dump OpenGL info.
  void dumpGlInfo(bool theIsBasic, bool theToPrint);

  //! Request widget paintGL() event.
  void updateView();

  //! Handle view redraw.
  virtual void handleViewRedraw(const Handle(AIS_InteractiveContext) & theCtx,
                                const Handle(V3d_View) & theView) override;

  qreal screenScale() const;

private:
  Handle(V3d_Viewer) myViewer;
  Handle(V3d_View) myView;
  Handle(AIS_InteractiveContext) myContext;
  Handle(AIS_ViewCube) myViewCube;

  QString myGlInfo;
  bool myIsCoreProfile;
  CurrentAction3d myCurrentMode;
};

#endif // _OcctQtViewer_HeaderFile
