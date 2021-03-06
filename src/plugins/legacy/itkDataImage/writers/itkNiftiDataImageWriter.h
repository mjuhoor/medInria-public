#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkDataImageWriterBase.h>
#include <itkDataImagePluginExport.h>

class ITKDATAIMAGEPLUGIN_EXPORT itkNiftiDataImageWriter: public itkDataImageWriterBase
{
public:
    itkNiftiDataImageWriter();
    virtual ~itkNiftiDataImageWriter();

    QString identifier()  const override;
    QString description() const override;
    QStringList handled() const override;
    QStringList supportedFileExtensions() const override;

    static bool registered();

    static dtkAbstractDataWriter * create();
};
