#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <vtkUnstructuredGridAlgorithm.h>

class vtkLimitVectorsToVOI: public vtkUnstructuredGridAlgorithm
{

public:
    static vtkLimitVectorsToVOI *New();
    vtkTypeMacro(vtkLimitVectorsToVOI, vtkUnstructuredGridAlgorithm)
    
    void PrintSelf (ostream& os, vtkIndent indent){}


    /** Set the VOI */
    void SetVOI (const int&, const int&, const int&,
                 const int&, const int&, const int&);

protected:
    vtkLimitVectorsToVOI();
    ~vtkLimitVectorsToVOI(){}

    // Usual data generation method
    virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

    //----------------------------------------------------------------------------
    virtual int RequestInformation(
            vtkInformation* vtkNotUsed(request),
            vtkInformationVector** vtkNotUsed(inputVector),
            vtkInformationVector* vtkNotUsed(outputVector))
    {
        // do nothing let subclasses handle it
        return 1;
    }


private:
    vtkLimitVectorsToVOI (const vtkLimitVectorsToVOI&);
    void operator=(const vtkLimitVectorsToVOI&);

    int m_XMin;
    int m_XMax;
    int m_YMin;
    int m_YMax;
    int m_ZMin;
    int m_ZMax;

};
