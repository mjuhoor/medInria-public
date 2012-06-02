/* medViewContainerMulti.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Mar 17 11:01:46 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Dec 20 11:26:25 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 56
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "medViewContainer_p.h"
#include "medViewContainerMulti.h"
#include "medViewPool.h"

#include <dtkCore/dtkAbstractView.h>

#include <medAbstractView.h>
#include <medViewManager.h>

medViewContainerSingle2::~medViewContainerSingle2()
{
    d->layout->setContentsMargins(0, 0, 0, 0);
}

void medViewContainerSingle2::setView (dtkAbstractView *view)
{
//    Never replace an existing view: we are in multi, we always add a new one.
//    if (d->view==view)
//        return;
//
//    if (d->view) {
//        this->onViewClosing();
//    }

    if (d->view)
    //if we already have a view, we should transfer the request to the root.
    {
        if (medViewContainerMulti* container =
                qobject_cast<medViewContainerMulti*>(parentContainer()))
        {
            qDebug() << "let set the parent's container" << container;
            container->setView( view );
            return;
        }
        qWarning() << "should find parent container, we're in a multi... ";
        return;
    }
//    qDebug() << "setting the view" << view << "in container" << this;
    d->view = view;

    if (view) {
        d->layout->addWidget(view->widget(), 0, 0);
        // d->pool->appendView (view); // only difference with medViewContainerSingle: do not add the view to the pool
        connect (view, SIGNAL (closing()), this, SLOT (onViewClosing()));
    }
    this->setFocus(Qt::MouseFocusReason);
}

bool medViewContainerSingle2::isLeaf(void) const
{
    return true;
}

void medViewContainerSingle2::onViewClosing (void)
{
    qDebug()<<"closing single2";
    if (d->view) {
        d->layout->removeWidget(d->view->widget());
        //d->view->widget()->hide();

        disconnect (d->view, SIGNAL (closing()), this, SLOT (onViewClosing()));
        // d->pool->removeView (d->view); // do not remove it from the pool

        d->view->close();
        d->view = NULL;
    }

    // qDebug() << this << __func__;
    // qDebug() << "isRoot:    " << this->isRoot();
    // qDebug() << "isLeaf:    " << this->isLeaf();
    // qDebug() << "isEmpty:   " << this->isEmpty();
    // qDebug() << "isCurrent: " << this->isCurrent();
}

void medViewContainerSingle2::onViewFocused (bool value)
{
    if ( !value )
        return;

    if ( !this->isEmpty() )
    {
        qobject_cast<medViewContainerMulti*>( parentContainer() )->setCurrent( this );
    }

    if (dtkAbstractView *view = d->view)
    {
        emit focused(view);
    }

    this->update();
}

class medViewContainerMultiPrivate
{
public:
    QList< dtkAbstractView* >  views;
};

medViewContainerMulti::medViewContainerMulti (QWidget *parent) : medViewContainer (parent), d2 (new medViewContainerMultiPrivate)
{
}

medViewContainerMulti::~medViewContainerMulti()
{
    foreach ( QObject* obj, d->layout->children())
    {
        obj->deleteLater();
    }

    delete d2;
    d2 = NULL;
}


void medViewContainerMulti::split(int rows, int cols)
{
    Q_UNUSED(rows);
    Q_UNUSED(cols);

    // Don't split a multi view container

    return;
}

dtkAbstractView *medViewContainerMulti::view(void) const
{
    return NULL;
}

QList<dtkAbstractView*> medViewContainerMulti::views (void) const
{
    return d2->views;
}

void medViewContainerMulti::setView(dtkAbstractView *view)
{
    if (!view)
        return;

//    if (d2->views.contains (view))
//        return;

    qDebug() << "setting the view" << view << "from multi container" << this;
    QList<QWidget *> content;
    for(int i = 0; i < d->layout->rowCount() ; i++) {
        for(int j = 0; j < d->layout->columnCount() ; j++) {
            if(QLayoutItem *item = d->layout->itemAtPosition(i, j)) {
                content << item->widget();
                d->layout->removeItem(item);
            }
        }
    }

    // retrieve the list of containers before creating a new one
    // so it is not in the list
    QList <medViewContainer *> containers = this->childContainers();

    medViewContainer *container = new medViewContainerSingle2(this);
    container->setAcceptDrops(false);
    container->setView(view);

    foreach (medViewContainer *cont, containers) {
        if (cont->isLeaf()) {
            connect (container, SIGNAL (clicked()), cont, SLOT (onContainerClicked()), Qt::UniqueConnection);
            connect (cont, SIGNAL (clicked()), container, SLOT (onContainerClicked()), Qt::UniqueConnection);
        }
    }

    content << container;
    this->layout(content);

    d2->views << view;

    if (medAbstractView *medView = qobject_cast<medAbstractView*> (view))
        d->pool->appendView (medView);

    connect (view, SIGNAL (closing()),         this, SLOT (onViewClosing()));
    connect (view, SIGNAL (fullScreen(bool)),  this, SLOT (onViewFullScreen(bool)));
    connect (view, SIGNAL (changeDaddy(bool)),
             this, SLOT (onDaddyChanged(bool)));

    this->setCurrent( container );
    emit viewAdded (view);
    //the focus is given in single2, no need to give it to the root container.
//    container->setFocus(Qt::MouseFocusReason);
}

void medViewContainerMulti::layout(QList<QWidget *> content)
{
    int row = 0;
    int col = 0, colmax = 0;

    for(int i = 0; i < content.count()-1; i++) {

        if(((col+1)*(row+1)) <= content.count()-1) {

            qreal rratio = qMin(((qreal)this->height()/(qreal)(row+2)), ((qreal)this->width()/(qreal)(col+1)));
            qreal cratio = qMin(((qreal)this->height()/(qreal)(row+1)), ((qreal)this->width()/(qreal)(col+2)));

            if(rratio > cratio) {
                row++;
                col = 0;
            } else {
                col++;
            }

            colmax = qMax(col, colmax);
        }
    }

    int layout_row = 0;
    int layout_col = 0;

    for(int i = 0; i < content.size(); i++) {

        d->layout->addWidget(content.at(i), layout_row, layout_col);

        if(layout_col == colmax) {
            layout_row++;
            layout_col = 0;
        } else {
            layout_col++;
        }
    }
}

void medViewContainerMulti::onViewClosing (void)
{
//    qDebug()<<"containerMulti closing a view";
    if (dtkAbstractView *view =
        qobject_cast<dtkAbstractView *>(this->sender())) {

        // needed for selecting another container as current afterwards
        QWidget * predContainer   = NULL;
        QWidget * succContainer   = NULL;
        bool      closedItemFound = false;

        QWidget * closedContainer =
            qobject_cast< QWidget * >( view->widget()->parent() );
        QList<QWidget *> content;
        for (int i = 0; i < d->layout->rowCount(); i++) {
            for (int j = 0; j < d->layout->columnCount(); j++) {
                QLayoutItem * item = d->layout->itemAtPosition(i, j);
                if ( item != NULL ) {
                    QWidget * container = item->widget();
                    if ( container == closedContainer ) {
                        container->hide();
                        closedItemFound = true;
                    }
                    else {
                        content << container; // keep the container in layout
                        container->show(); // in case view was hidden

                        // remember the predecessor resp. successor of
                        // the closed container
                        if ( closedItemFound ) {
                            if ( succContainer == NULL )
                                succContainer = container;
                        }
                        else
                            predContainer = container;
                    }

                    d->layout->removeItem(item);
                }
            }
        }

        disconnect (view, SIGNAL (closing()),
                    this, SLOT (onViewClosing()));
        disconnect (view, SIGNAL (fullScreen(bool)),
                    this, SLOT (onViewFullScreen(bool)));
        disconnect (view, SIGNAL (changeDaddy(bool)),
                    this, SLOT (onDaddyChanged(bool)));

        if (medAbstractView *medView = qobject_cast<medAbstractView*> (view))
            d->pool->removeView (medView);

        d2->views.removeOne (view);

        emit viewRemoved (view);

        // view->close(); // the container will close the view once deleted

        //tests showed it's safe to call directly delete here: deleteLater
        //doesn't work: container is still a child of "this" for some time afterward.
        //delete closedContainer;
        // BEGIN FIXME
        closedContainer->setParent (NULL);
        closedContainer->deleteLater();
        // END FIXME

        this->layout (content);
        //Choose the new current container based on who is next or previous
        medViewContainer * current =
            qobject_cast< medViewContainer * >( succContainer );
        if ( current == NULL )
            current = qobject_cast< medViewContainer * >( predContainer );
        if ( current == NULL )
            current = this;
//        qDebug() << "multi:"<< this;
//        qDebug() << "pred" << predContainer;
//        qDebug() << "succ" << succContainer;
//        qDebug() << "new current" << current;
        this->setCurrent(current);
        current->setFocus();

        this->update();

        // qDebug() << this << __func__;
        // qDebug() << "isRoot:    " << this->isRoot();
        // qDebug() << "isLeaf:    " << this->isLeaf();
        // qDebug() << "isEmpty:   " << this->isEmpty();
        // qDebug() << "isCurrent: " << this->isCurrent();
    }
}

void medViewContainerMulti::onViewFullScreen (bool value)
{
    if (dtkAbstractView *view = qobject_cast<dtkAbstractView *>(this->sender())) {
        if (value) {
            for(int i = 0; i < d->layout->rowCount() ; i++) {
                for(int j = 0; j < d->layout->columnCount() ; j++) {
                    if(QLayoutItem *item = d->layout->itemAtPosition(i, j)) {
                        if(item->widget()!=view->widget()->parent())
                            item->widget()->hide();
                    }
                }
            }
        }
    else {
            for(int i = 0; i < d->layout->rowCount() ; i++) {
                for(int j = 0; j < d->layout->columnCount() ; j++) {
                    if(QLayoutItem *item = d->layout->itemAtPosition(i, j)) {
                        if(item->widget()!=view->widget()->parent())
                            item->widget()->show();
                    }
                }
            }
      }
  }
}
