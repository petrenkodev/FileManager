#include "fsutils.h"

#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QIcon>
#include <QMessageBox>
#include <QProgressDialog>

bool FSUtils::inProgress = false;

FSUtils::FSUtils(QObject *parent)
    : QObject{parent}
{}

bool FSUtils::copy(QStringList paths, QString dest)
{
    if (inProgress)
        return false;
    inProgress = true;
    QProgressDialog progress("", "Отмена", 0, 0);
    progress.setMinimumDuration(0);
    progress.setFixedWidth(300);
    progress.setWindowTitle("Копирование");
    progress.setWindowIcon(QIcon(":/icons/copy.ico"));
    progress.show();

    if (dest.isEmpty() || paths.isEmpty()) {
        inProgress = false;
        return false;
    }
    if (paths.contains(dest)) {
        warning("Нельзя скопровать каталог сам в себя");
        inProgress = false;
        return false;
    }
    int pos = QFileInfo(paths[0]).path().size();
    QFileInfoList copyList = getList(paths);
    std::sort(copyList.begin(), copyList.end(), [](auto d1, auto d2) {
        return d1.path() < d2.path();
    });
    int count = 1;

    progress.setMaximum(copyList.count());
    if (progress.wasCanceled()) {
        inProgress = false;
        return false;
    }

    for (QFileInfo &entry : copyList) {
        QString sName = entry.filePath();
        QString dName = dest + QDir::separator() + sName.mid(pos);

        progress.setLabelText(sName + " -> " + dName);
        progress.setValue(count++);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        if (entry.isDir()) {
            bool res = QDir().mkdir(dName);
            qDebug() << "mkdir" << dName << (res ? "OK" : "ERROR");
        } else {
            bool res = QFile::copy(sName, dName);
            qDebug() << "copy" << sName << dName << (res ? "OK" : "ERROR");
        }
    }
    inProgress = false;
    return true;
}

bool FSUtils::move(QStringList paths, QString dest)
{
    copy(paths, dest);
    remove(paths);
    return true;
}

bool FSUtils::mkdir(QString newDir)
{
    QDir dir(newDir);
    if (dir.exists()) {
        warning("Каталог с таким именем уже существует!");
        return false;
    }
    bool res = dir.mkpath(".");
    qDebug() << "mkdir" << newDir << (res ? "OK" : "ERROR");
    return res;
}

bool FSUtils::remove(QStringList paths)
{
    int count = 1;
    QProgressDialog progress("", "Отмена", 0, paths.size());
    progress.setFixedWidth(300);
    progress.setWindowTitle("Удаление");
    progress.setWindowIcon(QIcon(":/icons/delete.ico"));
    progress.show();

    for (QString &file : paths) {
        progress.setLabelText(file);
        progress.setValue(count++);
        qApp->processEvents();
        bool res = QFile::moveToTrash(file);
        qDebug() << "remove" << file << (res ? "OK" : "ERROR");

        if (progress.wasCanceled()) {
            return false;
            break;
        }
    }
    return true;
}

void FSUtils::warning(QString message)
{
    QMessageBox(QMessageBox::Warning, qApp->applicationName(), message, QMessageBox::Ok).exec();
}

QFileInfoList FSUtils::getList(QStringList paths)
{
    QFileInfoList list;
    for (QString &path : paths) {
        list << QFileInfo(path);
        QDirIterator di(path, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (di.hasNext()) {
            di.next();
            if (di.fileName() != "." && di.fileName() != "..") {
                list << di.fileInfo();
            }
            qApp->processEvents();
        }
    }
    return list;
}
