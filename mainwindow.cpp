#include "mainwindow.h"
#include <QtCore/QDebug>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Widgets
    address = new QLineEdit;
    search = new QPushButton("Search");
    webView = new QWebView;

    // Layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(address, 0, 0);
    layout->addWidget(search, 0, 1);
    layout->addWidget(webView, 1, 0, 1, 2);

    // Set main window content
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    // Prepare network manager for search form
    network = new QNetworkAccessManager(this);
    connect(network, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGoogleMapsReply(QNetworkReply*)));
    connect(search, SIGNAL(clicked()), this, SLOT(onSearch()));
    connect(address, SIGNAL(returnPressed()), this, SLOT(onSearch()));

    // Set webview content
    initWebView();
}

MainWindow::~MainWindow()
{
}

// Set webview content
void MainWindow::initWebView()
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    webView->setUrl(QUrl("qrc:/html/google_maps.html"));

    frame = webView->page()->mainFrame();
    frame->addToJavaScriptWindowObject("coordinates", &coordinates);
}

// Search button has been clicked
void MainWindow::onSearch()
{
    QUrl url(QString("http://maps.googleapis.com/maps/api/geocode/json?address=%1&sensor=true").arg(address->text()));
    network->get(QNetworkRequest(url));
}

// Method fetched when a googlemaps location search request has got its reply
void MainWindow::onGoogleMapsReply(QNetworkReply* reply)
{
    // Parse JSON response
    QScriptEngine engine;
    QString json = reply->readAll();
    QScriptValue jsonVariant = engine.evaluate("(" + json + ")");

    if (!jsonVariant.isObject()) {
        qDebug() << "Reply is not json";
        return;
    }
    QVariant result = jsonVariant.toVariant();

    // Retrieve first result in list
    QVariantList placeMarks = result.toMap()["results"].toList();
    if(placeMarks.count() == 0) {
        qDebug() << "Reply has not 'results' object in dict";
        return;
    }

    // Read coords
    QVariant coords = placeMarks[0].toMap()["geometry"].toMap()["location"];
    double lat = coords.toMap()["lat"].toDouble();
    double lng  = coords.toMap()["lng"].toDouble();

    // Prepare set coordinates js code
    QString str =
        QString("var newLoc = new google.maps.LatLng(%1, %2);").arg(lat).arg(lng) +
        QString("map.setCenter(newLoc);") +
        QString("placeMarker(newLoc);");

    // Set coordinates
    frame->documentElement().evaluateJavaScript(str);
}


void JsObject::jsCallback(double lat, double lng, double alt)
{
    QTextStream out(stdout);

    out << "Retrieved values :" << endl;
    out << QString(" - Latitude  = %1 degrees").arg(lat) << endl;
    out << QString(" - Longitude = %1 degrees").arg(lng) << endl;
    out << QString(" - Altitude  = %1 meters").arg(alt) << endl;
}
