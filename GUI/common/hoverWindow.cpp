#include "hoverWindow.h"
#include <QDebug>




HoverWindow::HoverWindow(QWidget *parent): QWidget(parent)
{
	
}

HoverWindow::~HoverWindow()
{
	
}

void HoverWindow::onHoverEvent(QMouseEvent* ev)
{

}


void HoverWindow::onHoverLeave()
{

}


void HoverWindow::enterEvent(QEvent* ev)
{
	if (!m_bHoverIn)
	{
		m_bHoverIn = true;
		onHoverEvent((QMouseEvent*)ev);
	}
}
void HoverWindow::leaveEvent(QEvent*)
{
	if (m_bHoverIn)
	{
		m_bHoverIn = false;
		onHoverLeave();
	}
}