#include "medAbstractDataReader.h"

#include <medAbstractData.h>
#include <dtkCoreSupport/dtkAbstractDataReader.h>

medAbstractDataReader::medAbstractDataReader()
    : dtkAbstractDataReader()
{
}

medAbstractDataReader::medAbstractDataReader(const medAbstractDataReader& other)
    : dtkAbstractDataReader(other)
{
}

medAbstractDataReader::~medAbstractDataReader()
{
}

int medAbstractDataReader::getNumberOfData() const
{
    return m_data.size();
}

QVector<dtkSmartPointer<medAbstractData>> medAbstractDataReader::getData() const
{
    return m_data;
}
