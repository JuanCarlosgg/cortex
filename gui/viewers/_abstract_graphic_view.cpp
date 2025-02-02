//
// Created by robolab on 12/06/20.
//

#include <dsr/gui/viewers/_abstract_graphic_view.h>


using namespace DSR ;



AbstractGraphicViewer::AbstractGraphicViewer(QWidget* parent) :  QGraphicsView(parent)
{
	scene.setItemIndexMethod(QGraphicsScene::NoIndex);
	scene.setSceneRect(-5000,-5000, 10000, 10000);
	this->setScene(&scene);
	this->setCacheMode(QGraphicsView::CacheBackground);
	this->setViewport(new QOpenGLWidget());
	this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	this->setRenderHint(QPainter::Antialiasing);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setMinimumSize(400, 400);
    this->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
	this->setMouseTracking(true);
	this->viewport()->setMouseTracking(true);
    this->adjustSize();
}

//////////////////////////////////////////////////////////////////////////////////////
///// EVENTS
//////////////////////////////////////////////////////////////////////////////////////

void AbstractGraphicViewer::wheelEvent(QWheelEvent* event)
{
	qreal factor;
	if (event->angleDelta().y() > 0)
	{
		factor = 1.1;

	}
	else
	{
		factor = 0.9;

	}
	auto view_pos = event->position().toPoint();
	auto scene_pos = this->mapToScene(view_pos);
	this->centerOn(scene_pos);
	this->scale(factor, factor);
	auto delta = this->mapToScene(view_pos) - this->mapToScene(this->viewport()->rect().center());
	this->centerOn(scene_pos - delta);
    //qInfo()<< "wheel_event" << event->angleDelta().y() << factor << event->position().toPoint();
}

void AbstractGraphicViewer::resizeEvent(QResizeEvent *e)
{
//	qDebug() << "resize_graph_view" << x() << y()<<e->size();
	QGraphicsView::resizeEvent(e);
}


void AbstractGraphicViewer::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_pan = true;
		_panStartX = event->position().x();
		_panStartY = event->position().y();
		setCursor(Qt::ClosedHandCursor);
		event->accept();
		return;
	}
	QGraphicsView::mousePressEvent(event);
}

void AbstractGraphicViewer::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_pan = false;
		setCursor(Qt::ArrowCursor);
		event->accept();
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void AbstractGraphicViewer::mouseMoveEvent(QMouseEvent *event)
{
	if (_pan)
	{
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->position().x() - _panStartX));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->position().y() - _panStartY));
		_panStartX = event->position().x();
		_panStartY = event->position().y();
		event->accept();

	}
	QGraphicsView::mouseMoveEvent(event);
}

void AbstractGraphicViewer::showEvent(QShowEvent* event)
{
	QGraphicsView::showEvent(event);
	auto adjusted = scene.itemsBoundingRect().adjusted(-100,-100,100,100);
	scene.setSceneRect(adjusted);
	// FitInView is called two times because of this bug: https://bugreports.qt.io/browse/QTBUG-1047
	bool updateState = updatesEnabled();
	setUpdatesEnabled(false);
	fitInView(adjusted, Qt::KeepAspectRatio);
	QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	fitInView(adjusted, Qt::KeepAspectRatio);
	setUpdatesEnabled(updateState);
}
