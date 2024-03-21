#include "baseWindow.h"
#include <QStyle>
#include <QStyleOption>
#include <QDebug>
#include <QPainter>

static QList<BaseWindow*> m_transWindows;

QList<BaseWindow*> getAllTransWindows()
{
	return m_transWindows;
}

static void addTranslateWindow(BaseWindow* transWindow)
{
	m_transWindows.append(transWindow);
}

static void removeTranslateWindow(BaseWindow* transWindow)
{
	m_transWindows.removeOne(transWindow);
}

BaseWindow::BaseWindow(QWidget *parent): QWidget(parent)
{
	addTranslateWindow(this);
}

BaseWindow::~BaseWindow()
{
	removeTranslateWindow(this);
}

void BaseWindow::retranslateUi()
{
	qDebug() << "BaseWindow::retranslateUi: " << this;
}

void BaseWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void BaseWindow::mousePressEvent(QMouseEvent* ev)
{
	if (!m_bEnableMove)
	{
		QWidget::mousePressEvent(ev);
		return;
	}
	if (ev->pos().y() <= 30)
	{
		this->m_ptWindow = ev->globalPos() - geometry().topLeft();
		m_isMouseInTitle = true;
	}
}
void BaseWindow::mouseMoveEvent(QMouseEvent* ev)
{
	if ((!m_bEnableMove) || (!m_isMouseInTitle))
	{
		QWidget::mouseMoveEvent(ev);
		return;
	}
	if (isMaximized())
	{
		showNormal();
	}
	this->move(ev->globalPos() - m_ptWindow);
}
void BaseWindow::mouseReleaseEvent(QMouseEvent* ev)
{
	if ((!m_bEnableMove) || (!m_isMouseInTitle))
	{
		QWidget::mouseReleaseEvent(ev);
		return;
	}
	m_isMouseInTitle = false;
	m_ptWindow = QPoint();
}


void BaseWindow::changeEvent(QEvent* ev)
{
	switch (ev->type()) {
	case QEvent::LanguageChange:
	{
		for (auto transWidget : getAllTransWindows())
		{
			transWidget->retranslateUi();
		}
		break;
	}
	default:
		break;
	}
}