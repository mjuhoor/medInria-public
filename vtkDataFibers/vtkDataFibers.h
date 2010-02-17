// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef VTKDATAFIBERS_H
#define VTKDATAFIBERS_H

#include <dtkCore/dtkAbstractData.h>

#include "vtkDataFibersPluginExport.h"

class vtkDataFibersPrivate;

class VTKDATAFIBERSPLUGIN_EXPORT vtkDataFibers : public dtkAbstractData
{
    Q_OBJECT

public:
             vtkDataFibers(void);
    virtual ~vtkDataFibers(void);

    virtual QString description(void) const;

    static bool registered(void);

public:
    bool read(QString file);
    bool write(QString file);
	
    void *data(void);
	
    void setData(void *data);
	
private:
    vtkDataFibersPrivate *d;
};

dtkAbstractData *createVtkDataFibers(void);

#endif
