/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 2/15/19.
 * Contact with:wk707060335@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef PROTYPE_LDOSGWIDGET_H
#define PROTYPE_LDOSGWIDGET_H

//Osg
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>
//Qt
#include <QtCore/QTimer>
#include <QtCore/QFileInfo>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QOpenGLWidget>

#include "Common.h"
/**
 * @brief The OSGWidget class is the bridge between OSG and Qt.
 * It renders the whole osg scene
 * and communicate with the main window(user input)
 * through signal&slot
 */
class OSGWidget : public QOpenGLWidget {
Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param parent parent window pointer
     * @param f windows flags
     */
    explicit OSGWidget(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    ~OSGWidget() final = default;

    void init();

    void readDataFromFile(const QFileInfo &file_path);

    void loadDSM(const std::string &file_path);

    void loadBuildings(const std::string &file_path);

    void activeTraceRefresh(bool is_active);

    void activeTestRefresh(bool is_active);

    //!inherit from QOpenGLWidget
    virtual void keyPressEvent(QKeyEvent *event);

    Q_DISABLE_COPY(OSGWidget);
protected:
    //!inherit from QOpenGLWidget
    virtual void paintEvent(QPaintEvent *paintEvent);

    //!inherit from QOpenGLWidget
    virtual void paintGL();

    //!inherit from QOpenGLWidget
    virtual void onResize(int width, int height);

    //!inherit from QOpenGLWidget
    virtual void resizeGL(int width, int height);

    //!inherit from QOpenGLWidget
    virtual void mouseMoveEvent(QMouseEvent *event);

    //!inherit from QOpenGLWidget
    virtual void mousePressEvent(QMouseEvent *event);

    //!inherit from QOpenGLWidget
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //!inherit from QOpenGLWidget
    virtual void wheelEvent(QWheelEvent *event);

    //!inherit from QOpenGLWidget
    void mouseDoubleClickEvent(QMouseEvent *event);

    //!inherit from QOpenGLWidget
    virtual bool event(QEvent *event);

    //!inherit from QOpenGLWidget
    virtual void timerEvent(QTimerEvent *);
private:
    //! init all node
    void initSceneGraph();

    //! init the camera
    void initCamera();

    //! init some help node
    void initHelperNode();

    void initTrace();

    void beginTraceRefresh();

    void finishTraceRefresh();

	void setBuildingColor(const osg::Vec3d& color);

    //! create hud node
    osg::Camera *createHUD();

	osg::Geode *createSphere(const osg::Vec4& color, double radius);

    /**
     * @brief calculate the bounding box of a node
     **/
    osg::Geode *calculateBBoxForModel(osg::Node *node) const;

    /**
     * @brief calculate the color of a point
     **/
    osg::Vec3d calculateColorForPoint(const osg::Vec3d &point) const;

    //! Getter of osg event queue
    osgGA::EventQueue *getEventQueue() const;

    Result intersectionCheck();

    void postResult(const Result &);

    void pointCloudMatch(Result &);

    //! Ref of OSG Graphics Window
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _graphicsWindow;
    //! Ref of OSG Viewer
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    //! root node of the scene
    osg::ref_ptr<osg::Switch> root_node_;
    osg::ref_ptr<osg::Switch> text_node_;

    std::vector<osg::Vec3d> trace_vec_;
	Point ref_position;
    Point cur_position;
    Array cur_points;

    bool is_testing_;
    QScopedPointer<QTimer> update_timer_;
    QScopedPointer<QUdpSocket> udp_socket_;
	osg::ref_ptr<osg::Geometry> current_buildin_geom;

    //! some manipulators
    osg::ref_ptr<osgGA::TrackballManipulator> _trackballMani;
    osg::ref_ptr<osgGA::TerrainManipulator> _terrainMani;
public slots:

    //! Zoom Viewer to whole display
    void home();

    //! Center On, set to terrianManipulator if 0,0,0 input
    void trackballCenterOn(double x, double y, double z);

    void updateScene();

    void updateUAVPose(Point);

	void updateRefPose(Point);

    void updatePointCloud();
};


#endif //PROTYPE_LDOSGWIDGET_H