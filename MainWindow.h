/// @file

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {class MainWindow;}

/// @brief Example enumeration
enum Rick
{
	A, ///< @todo
	B, ///< Yolo
	C  ///< @todo
};

/// @brief Example Documentation
class MainWindow : public QMainWindow {
	Q_OBJECT
	/// @brief Propert documentation
	Q_PROPERTY(bool example READ example WRITE setExample NOTIFY exampleChanged)
public:
	/// @todo
	/// @param parent
	MainWindow(QWidget* parent = nullptr);

	/// @cond DISABLE_DOXYGEN
	~MainWindow();
	/// @endcond

	/// @returns Returns a new QWidget
	/// @ownercaller
	QWidget* create(){ return nullptr; }

	/// @getter{example}
	bool example() const { return true; }
	/// @setter example
	/// @param value The property's new value
	void setExample(bool value){}

signals:
	/// @signal example
	void exampleChanged(bool);

private slots:
	/// @todo
	void showMessage();

private:
	/// @todo
	std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_H