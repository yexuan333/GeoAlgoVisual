#pragma once
#include <vector>
#include <Adaptor3d_Curve.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Point.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_Shape.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepFill.hxx>
#include <BRepGProp.hxx>
#include <BRepLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepTools.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <CPnts_AbscissaPoint.hxx>
#include <Extrema_ExtPC.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_QuasiUniformDeflection.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeLine.hxx>
#include <GC_MakeSegment.hxx>
#include <GProp_GProps.hxx>
#include <Geom2dAPI_Interpolate.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GeomAPI.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomConvert.hxx>
#include <GeomFill.hxx>
#include <GeomLib.hxx>
#include <GeomLib_IsPlanarSurface.hxx>
#include <GeomTools.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_OffsetCurve.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_SurfaceOfLinearExtrusion.hxx>
#include <Geom_Transformation.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Prs3d_PointAspect.hxx>
#include <ShapeAnalysis_Curve.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <ShapeFix_Shell.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeConstruct.hxx>
#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Version.hxx>
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>

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
#include <QThread>
#include <QtWidgets/QMainWindow>
#include <QRandomGenerator>
#include <Qobject>
#include <Qtimer>

#include <algorithm>
#include <iostream>
#include <math.h>
#include <numeric>
#include <unordered_map>

#ifndef DllExport
#define DllExport Standard_EXPORT
#endif