#pragma once

#include <medCoreLegacyExport.h>

#include <dtkCoreSupport/dtkSmartPointer.h>
#include <dtkCoreSupport/dtkAbstractDataReader.h>

class medAbstractData;
class medAbstractDataReaderPrivate;

class MEDCORELEGACY_EXPORT medAbstractDataReader : public dtkAbstractDataReader
{
    Q_OBJECT

public:
    medAbstractDataReader(void);
    medAbstractDataReader(const medAbstractDataReader& other);
    virtual ~medAbstractDataReader(void);

    int getNumberOfData() const;
    QVector<dtkSmartPointer<medAbstractData>> getData() const;

signals:
    void finishedReadingData(int dataNumber);

protected:
    QVector<dtkSmartPointer<medAbstractData>> m_data;
};
