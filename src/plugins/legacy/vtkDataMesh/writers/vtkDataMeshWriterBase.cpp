/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/
#include "vtkDataMeshWriterBase.h"

#include <dtkCoreSupport/dtkAbstractData>

#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkType.h>

const QString vtkDataMeshWriterBase::metaDataFieldPrefix = "medMetaData::";

vtkDataMeshWriterBase::vtkDataMeshWriterBase() : dtkAbstractDataWriter()
{
}

bool vtkDataMeshWriterBase::canWrite(const QString& /*path*/)
{
    bool result = false;

    if (this->data())
    {
        result = dynamic_cast<vtkMetaDataSet*>((vtkObject*)(this->data()->data()));
    }

    return result;
}

void vtkDataMeshWriterBase::addMetaDataAsFieldData(vtkMetaDataSet* dataSet)
{
    for(QString key : data()->metaDataList())
    {
        vtkSmartPointer<vtkStringArray> metaDataArray = vtkSmartPointer<vtkStringArray>::New();
        QString arrayName = QString(metaDataFieldPrefix) + key;
        metaDataArray->SetName(arrayName.toStdString().c_str());

        for(QString value : data()->metaDataValues(key))
        {
            metaDataArray->InsertNextValue(value.toStdString().c_str());
        }

        dataSet->GetDataSet()->GetFieldData()->AddArray(metaDataArray);
    }
}

void vtkDataMeshWriterBase::clearMetaDataFieldData(vtkMetaDataSet* dataSet)
{
    vtkFieldData* fieldData = dataSet->GetDataSet()->GetFieldData();
    vtkSmartPointer<vtkFieldData> newFieldData = vtkSmartPointer<vtkFieldData>::New();

    for (int i = 0; i < fieldData->GetNumberOfArrays(); i++)
    {
        QString arrayName = fieldData->GetArrayName(i);

        if (!arrayName.startsWith(metaDataFieldPrefix))
        {
            newFieldData->AddArray(fieldData->GetAbstractArray(i));
        }
    }

    dataSet->GetDataSet()->SetFieldData(newFieldData);
}
/*
void vtkDataMeshWriterBase::inspectAllFieldDataInMetaData(vtkMetaDataSet* metaData, bool replaceNaNs)
{
    vtkDataSet* dataSet = metaData->GetDataSet();
    if (dataSet)
    {
        replaceNaNsFromFieldData(dataSet->GetPointData(), replaceNaNs);
        replaceNaNsFromFieldData(dataSet->GetCellData(), replaceNaNs);
        replaceNaNsFromFieldData(dataSet->GetFieldData(), replaceNaNs);
    }
}

void vtkDataMeshWriterBase::replaceNaNsFromFieldData(vtkFieldData* fieldData, bool replaceNaNs)
{
    for (int i = 0; i < fieldData->GetNumberOfArrays(); ++i)
    {
        vtkAbstractArray* array = fieldData->GetArray(i);
        if (array && array->GetDataType() == VTK_DOUBLE)
        {
            replaceNaNsFromArray(array, replaceNaNs);
        }
    }
}

void vtkDataMeshWriterBase::replaceNaNsFromArray(vtkAbstractArray* abstractArray, bool replaceNaNs)
{
    vtkDoubleArray* array = vtkDoubleArray::SafeDownCast(abstractArray);
    if (array)
    {
        double* data = static_cast<double*>(array->GetVoidPointer(0));
        vtkIdType nbComponents = array->GetNumberOfComponents();

        for (vtkIdType i = 0; i < array->GetNumberOfTuples(); ++i)
        {
            for (vtkIdType j = 0; j < nbComponents; ++j)
            {
                double* value = data + i * nbComponents + j;
                if (replaceNaNs && vtkMath::IsNan(*value))
                {
                    *value = std::numeric_limits<double>::max();
                }
                else if (!replaceNaNs && *value == std::numeric_limits<double>::max())
                {
                    *value = vtkMath::Nan();
                }
            }
        }
    }
}
*/
