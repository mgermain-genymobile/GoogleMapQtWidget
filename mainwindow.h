#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkAccessManager>

class JsObject : public QObject
{
    Q_OBJECT
 public:
     Q_INVOKABLE void jsCallback(double lat, double lng, double alt);
 };

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Widgets
    QLineEdit *address;
    QPushButton *search;
    // Network manager
    QNetworkAccessManager *network;
    // Web parts
    QWebView *webView;
    QWebFrame *frame; // shortcut to mainframe

    JsObject coordinates;

    // Set webview content
    void initWebView();

private slots:
    // Search button has been clicked
    void onSearch();

    // Method fetched when a googlemaps location search request has got its reply
    void onGoogleMapsReply(QNetworkReply*);
};

#endif // MAINWINDOW_H
