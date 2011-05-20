/* medViewerConfigurationFiltering.cpp --- 
 * 
 */

//TODO memory management :
/*
 * Current memory usage: 1112 Required memory need: 1207
****** TRY_FREE_MEM_BEGIN:  1112  to reach:  475
Not possible to free any items, current cache count is:  0 items
****** TRY_FREE_MEM_END:  1112
 *
 */

//TODO save result in a file or database

//TODO launch process in a thread to prevent freezing the user interface

#include "medViewerConfigurationFiltering.h"
#include <medGui/medViewContainerFiltering.h>

#include <medGui/medViewerToolBoxView.h>

#include <medGui/medToolBoxFiltering.h>

#include <medGui/medViewContainer.h>
#include <medGui/medViewContainerMulti.h>
#include <medGui/medStackedViewContainers.h>
#include <medGui/medToolBoxFilteringCustom.h>

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

class medViewerConfigurationFilteringPrivate
{
public:
    medViewerToolBoxView   *viewToolBox;
    medToolBoxFiltering *filteringToolBox;
    dtkAbstractData* filterOutput;
    QList<dtkAbstractView *> views;
};

medViewerConfigurationFiltering::medViewerConfigurationFiltering(QWidget *parent) : medViewerConfiguration(parent), d(new medViewerConfigurationFilteringPrivate)
{
  setLayoutToolBoxVisibility(false);
	
  d->viewToolBox = new medViewerToolBoxView(parent);
  
  this->addToolBox( d->viewToolBox );
  
  d->filteringToolBox = new medToolBoxFiltering(parent);
  
  this->addToolBox( d->filteringToolBox );    
  
  
  connect(d->filteringToolBox, SIGNAL(addToolBox(medToolBox *)), this, SLOT(addToolBox(medToolBox *)));
  connect(d->filteringToolBox, SIGNAL(removeToolBox(medToolBox *)), this, SLOT(removeToolBox(medToolBox *)));
  connect(d->filteringToolBox,SIGNAL(dataSelected(dtkAbstractData *)),this,SIGNAL(inputDataChanged(dtkAbstractData *)));
  connect(d->filteringToolBox,SIGNAL(processFinished()),this,SLOT(onProcessSuccess()));

  //this->setViewLayoutType (-1);
}

medViewerConfigurationFiltering::~medViewerConfigurationFiltering(void)
{
    delete d;
    d = NULL;
}

void medViewerConfigurationFiltering::setupViewContainerStack()
{
        if (!this->stackedViewContainers()->count())
        {
            medViewContainerFiltering *filteringContainer = new medViewContainerFiltering(this->stackedViewContainers());

            connect(this,SIGNAL(inputDataChanged(dtkAbstractData *)),filteringContainer,SLOT(updateInput(dtkAbstractData *)));

            connect(this,SIGNAL(outputDataChanged(dtkAbstractData *)),filteringContainer,SLOT(updateOutput(dtkAbstractData *)));

            this->stackedViewContainers()->addContainer("Filtering",filteringContainer);

            setCurrentViewContainer("Filtering");
        }
}

void medViewerConfigurationFiltering::patientChanged(int patientId)
{
   d->filteringToolBox->clear();
}

void medViewerConfigurationFiltering::onProcessSuccess()
{
	d->filterOutput = d->filteringToolBox->customToolbox()->processOutput();
	if(!d->filterOutput)
		return;

	emit outputDataChanged(d->filterOutput);
}

QString medViewerConfigurationFiltering::description(void) const
{
    return "Filtering";
}

medViewerConfiguration *createMedViewerConfigurationFiltering(void)
{
    return new medViewerConfigurationFiltering;
}
