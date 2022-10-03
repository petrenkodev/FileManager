#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void openFile();
  void changeToAscii();
  void changeToHex();

private:
  enum FileFormat { ascii, hex };

  Ui::MainWindow *ui;
  FileFormat showFormat;
  QString fileName;
  QByteArray buffer;

  void loadFile(QString fileName);
  FileFormat checkFormat();
  void showFile();
  void showAsAscii();
  void showAsHex();
};
#endif // MAINWINDOW_H
