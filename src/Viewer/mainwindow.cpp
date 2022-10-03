#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->open, &QAction::triggered, this, &MainWindow::openFile);
  connect(ui->ascii, &QAction::triggered, this, &MainWindow::changeToAscii);
  connect(ui->hex, &QAction::triggered, this, &MainWindow::changeToHex);
  connect(ui->exit, &QAction::triggered, qApp, QApplication::exit);

  QStringList cmd = qApp->arguments();
  if (cmd.size() == 2 && !cmd[1].isEmpty())
    loadFile(cmd[1]);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Открыть");
  loadFile(fileName);
}

void MainWindow::changeToAscii() {
  showFormat = ascii;
  showFile();
}

void MainWindow::changeToHex() {
  showFormat = hex;
  showFile();
}

void MainWindow::loadFile(QString fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
    return;
  buffer = file.readAll();
  file.close();
  setWindowTitle("Viewer - " + file.fileName());
  showFormat = checkFormat();
  showFile();
}

MainWindow::FileFormat MainWindow::checkFormat() {
  for (int i = 0; i < 100; i++) {
    if (i >= buffer.size())
      break;
    unsigned char ch = buffer[i];
    if (ch < 32 && ch != 10 && ch != 13)
      return hex;
  }
  return ascii;
}

void MainWindow::showFile() {
  ui->plainTextEdit->clear();

  switch (showFormat) {
  case ascii:
    showAsAscii();
    break;
  case hex:
    showAsHex();
    break;
  default:
    showAsHex();
  }
}

void MainWindow::showAsAscii() { ui->plainTextEdit->setPlainText(buffer); }

void MainWindow::showAsHex() {
  QString out;
  int cursor = 0;
  while (cursor < buffer.size()) {
    out.append(QString("%1  ").arg(cursor, 8, 16, QChar('0')));
    for (int i = 0; i < 16; i++) {
      if (cursor + i >= buffer.size()) {
        out.append("   ");
      } else {
        out.append(QString("%1 ").arg((unsigned char)buffer[cursor + i], 2, 16,
                                      QChar('0')));
      }
      if (i == 7)
        out += ' ';
    }

    out.append(' ');

    for (int i = 0; i < 16; i++) {
      if (cursor + i >= buffer.size())
        out.append(" ");
      else
        out.append((buffer[cursor + i] < ' ') ? '.' : buffer[cursor + i]);
    }

    cursor += 16;
    out += '\n';
  }
  ui->plainTextEdit->appendPlainText(out);
  ui->plainTextEdit->moveCursor(QTextCursor::Start);
}
