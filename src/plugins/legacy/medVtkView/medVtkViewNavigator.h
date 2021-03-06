#pragma once
/*=========================================================================

medInria

Copyright (c) INRIA 2013 - 2020. All rights reserved.
See LICENSE.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

=========================================================================*/

#include <medAbstractImageViewNavigator.h>
#include <medImageViewEnum.h>

#include <medVtkViewPluginExport.h>

class medAbstractImageView;

class medVtkViewNavigatorPrivate;
class MEDVTKVIEWPLUGIN_EXPORT medVtkViewNavigator : public medAbstractImageViewNavigator
{
    Q_OBJECT

public:
    medVtkViewNavigator(medAbstractView* parent);
    virtual ~medVtkViewNavigator();

    virtual QString  identifier() const;
    static QString  s_identifier();
    static bool registered();
    virtual QStringList handled(void) const;
    virtual QString description() const;
    virtual QList<medAbstractParameterL*> linkableParameters();
    virtual QList<medBoolParameterL*> mouseInteractionParameters();

    virtual medImageView::Orientation orientation() const ;

public slots:
    void updateWidgets();

    virtual void setOrientation(medImageView::Orientation orientation);

    virtual void setZoom (double zoom);
    virtual void setPan (const QVector2D &pan);

    virtual void moveToPosition(const QVector3D &position);

    void setCamera(const QVector3D &position,
                           const QVector3D &viewup,
                           const QVector3D &focal,
                           double parallelScale);

signals:
    void orientationChanged();

protected:
    virtual QWidget* buildToolBoxWidget();
    virtual QWidget* buildToolBarWidget();

    void cameraUp(double *coordinates) const;
    void cameraPosition(double *coordinates) const;
    void cameraFocalPoint(double *coordinates) const;
    /**
     * @brief cameraParallelScale return -1 if orientaion is not 3D.
     * @param parallelScale
     */
    void cameraParallelScale(double &parallelScale) const;

protected slots:
    void setCamera(QHash<QString, QVariant>);

    void setCameraUp(const QVector3D& viewup);
    void setCameraPosition(const QVector3D& position);
    void setCameraFocalPoint(const QVector3D& focal);
    void setCameraParallelScale(double parallelScale);

    /**
     * @brief setSlice is used to change the current slice
     * @param slice type int
    */
    bool setSlice(int slice);

    /**
     * @brief setRotation is used to rotate a 3D mesh
     * @param angle type double, and in degree
    */
    bool setRotationAngle(double angle);
    
    void setAxial(bool axial);
    void setCoronal(bool coronal);
    void setSagittal(bool sagittal);
    void set3d(bool o3d);

    void showAxes(bool);
    void showRuler(bool);
    void showAnnotations(bool);
    void showScalarBar(bool);
    void showAnnotatedCube(bool show);

    void enableZooming(bool);
    void enableSlicing(bool);
    void enablePanning(bool);
    void enableMeasuring(bool);

private slots:
    void changeOrientation(medImageView::Orientation orientation);

private:
    medVtkViewNavigatorPrivate *d;
};
