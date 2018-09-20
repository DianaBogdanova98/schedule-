#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
private slots:
    void replyFinishedT(QNetworkReply *);
    void on_pushButton_clicked();
    void setItem (QString str, int &rowCount);
    void getText();
    void table();
    void getSchedule();

private:
    QTextCodec *codec;
};

#endif // MAINWINDOW_H
