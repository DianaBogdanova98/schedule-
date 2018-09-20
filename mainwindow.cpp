#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QTextCodec>
#include <QFileInfo>
#include <QString>



MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    getText();
    table();
    getSchedule();

}
void MainWindow::replyFinishedT(QNetworkReply *reply)
{
    QByteArray content= reply->readAll();
    QTextCodec *codec = QTextCodec::codecForLocale();
    ui->html->setText(codec->toUnicode(content.data()) );
}

void MainWindow::setItem(QString str, int &rowCount ){
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(str);
    ui->tableWidget->setItem(0,rowCount,item);
    this->rowCount++;
}

void MainWindow::getText()
{
    QNetworkAccessManager *managerT = new QNetworkAccessManager(this);
    QUrl url("http://bseu.by/schedule/");
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QByteArray postData;
    QUrl params1;
    QUrlQuery params;

    params.addQueryItem("someFake","7");
    params.addQueryItem("faculty","7");
    params.addQueryItem("form","10");
    params.addQueryItem("course","3");
    params.addQueryItem("group","6857");
    params.addQueryItem("period","2");
    params.addQueryItem("__act","__id.25.main.inpFldsA.GetSchedule__sp.7.results__fp.4.main");


    params1.setQuery(params);
    postData = params1.toEncoded(QUrl::RemoveFragment);


    QObject::connect(managerT, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinishedT(QNetworkReply*)));
    managerT->post(request, postData);
}

void MainWindow::table()
{

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(35);
    ui->tableWidget->setShowGrid(true);//сетка
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//выделение 1 элемента
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//выделение строки
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Время"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Дисциплина, преподаватель"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("к./ауд."));
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setStyleSheet("QHeaderView::section { background-color:blue }");

}

void MainWindow::getSchedule()
{

    QString b = ui->html->text();
    int pos = b.indexOf("/th");
    b = b.right(b.length() - pos);
    QString Trow=b;
    int rowCount = 0;
    int colCount = 0;
    while(pos!=-1){
        pos = b.indexOf("<tr");
        b = b.right(b.length() - pos);
        pos = b.indexOf("/tr>");
        Trow = b.left(pos+4);
        if(Trow.indexOf("wday")!=-1)
        {
            QRegExp rx (".*<td colspan=\"5\" class=\"wday\">(.*)<\/td>");
            rx.setMinimal(true);
            rx.exactMatch(Trow);
            QString matched = rx.cap(1);
            ui->tableWidget->setSpan(colCount, 0,1, ui->tableWidget-> columnCount());
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(matched);
            item->setTextAlignment( Qt::AlignCenter );
            ui->tableWidget->setItem(0,rowCount,item);
            ui->tableWidget->item(colCount, 0)->setBackgroundColor(QColor(85, 255, 255));
            //rowCount+=3;
            colCount++;
        }
        else if (Trow.indexOf("td colspan=\"2\"")!=-1)
        {
            QRegExp rx (".*<td class=\"right\">(.*)<\/td>.*<td colspan=\"2\">(.*)<span class=\"distype\">(.*)<\/span>.*<span class=\"teacher\">(.*)<\/span>.*<td class=\"right\">(.*)<\/td>");
            rx.setMinimal(true);
            rx.exactMatch(Trow);
            QString time = rx.cap(1);
            QString object = rx.cap(2)+rx.cap(3)+rx.cap(4);
            QString room = rx.cap(5);
            setItem(time, rowCount);
            //rowCount++;
            setItem(object, rowCount);
            //rowCount++;
            setItem(room, rowCount);
           // rowCount++;
            colCount++;
        }
        else if (Trow.indexOf("td colspan=\"3\"")!=-1)
        {
            QRegExp rx (".*<td rowspan=\"4\">(.*)<\/td>.*<td colspan=\"3\">(.*)<span class=\"distype\">(.*)<\/span>");
            QRegExp rxRu (".*<td rowspan=\"2\">(.*)<\/td>.*<td colspan=\"3\">(.*)<span class=\"distype\">(.*)<\/span>");
            rx.setMinimal(true);
            rxRu.setMinimal(true);
            rx.exactMatch(Trow);
            rxRu.exactMatch(Trow);

            QString time = rx.cap(1);
            QString object = rx.cap(2)+rx.cap(3);
            QString timeRu = rxRu.cap(1);
            QString objectRu = rxRu.cap(2)+rxRu.cap(3);
            if(time!=""){
            setItem(time, rowCount);
            //rowCount++;
            setItem(object, rowCount);
            //rowCount+=2;
            ui->tableWidget->setSpan(colCount, 1,1, ui->tableWidget-> columnCount());
            colCount++;
            } else
            {
            setItem(timeRu, rowCount);
            //rowCount++;
            setItem(objectRu, rowCount);
            //rowCount+=2;
            ui->tableWidget->setSpan(colCount, 1,1, ui->tableWidget-> columnCount());
            colCount++;
            }


        }

        else if (Trow.indexOf("td class=\"sg\" rowspan=\"1\"")!=-1)
        {
            QRegExp rx (".*<td class=\"sg\" rowspan=\"1\">(.*)<\/td>.*<span class=\"teacher\">(.*)<\/span>.*<td rowspan=\"1\">(.*)<!--.*>");
            rx.setMinimal(true);
            rx.exactMatch(Trow);
            QString gr = rx.cap(1);
            QString teather = rx.cap(2);
            QString room = rx.cap(3);
            setItem(gr, rowCount);
            //rowCount++;
            setItem(teather, rowCount);
           // rowCount++;
            setItem(room, rowCount);
           // rowCount++;
            colCount++;
        }
        b = b.right(b.length() - pos - 4);
    }

}
