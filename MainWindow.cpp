// mainwindow.cpp

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <future>

#include <QWindow>
#include <QPushButton>
#include <QStringList>
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QToolTip>

void getAllFocusableWidgets(QWidget* parent, QVector<QWidget*>& focusableWidgets) {
	if (!parent)
		return;

	// Iterate through all child widgets
	const QObjectList& children = parent->children();
	for (QObject* obj : children) {
		if (QWidget* widget = qobject_cast<QWidget*>(obj)) {
			if (widget->focusPolicy() != Qt::NoFocus) {
				// If the widget can accept focus, add it to the list
				focusableWidgets.append(widget);
			}
			// Recursively search for focusable widgets in child widgets
			getAllFocusableWidgets(widget, focusableWidgets);
		}
	}
}

template<typename T>
QStringList findMissingTooltip(const QWidget* const root)
{
	QStringList children;
	for (const auto* c : root->findChildren<T*>())
	{
		children << c->objectName();
	}
	return children;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>())
{
	ui->setupUi(this);
	connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::showMessage);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showMessage() {
	// QMessageBox::information(this, "Message", "Button clicked!");	
	std::future<QStringList> tooltipButtons = std::async(std::launch::async, findMissingTooltip<QPushButton>, this);
	qDebug() << tooltipButtons.get();

	QVector<QWidget*> focusableWidgets;
	getAllFocusableWidgets(this, focusableWidgets);
	for (auto* widget : focusableWidgets)
	{		
		const auto pos = widget->mapToGlobal(QPoint(0, 0));
		const auto text = widget->toolTip().toStdString();
		QToolTip::showText(pos, text.c_str(), widget, {}, 2000);
	}
}