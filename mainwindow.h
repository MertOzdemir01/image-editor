#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMimeData>
#include <QToolButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QSlider>
#include <QImage>
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>
#include <opencv2/core.hpp>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onLoadImage();
    void deleteImage();
    void resetControls();
    void saveImage();
    void onRadioButtonToggled(bool checked);
    void onCheckBoxToggled(bool checked);
    void onSliderValueChanged(int value);
    void onValueChanged(int value);
    void setDefaultImage();

private:
    void loadImage(const QString &fileName);
    void updateImage();
    void enableImageControls(bool enable);
    void disableImageControls();
    bool saveImageToDatabase(const QString &fileName, const QImage &image, const QStringList &operations);
    QImage performEdgeDetection(const QImage &image, int th1);
    QImage convertToGrayscale(const QImage &image);
    QImage applyThreshold(const QImage &image, int thresholdValue);
    QImage adjustBrightness(const QImage &image, int brightnessValue);
    QImage adjustContrast(const QImage &image, int contrastValue);
    QImage applyBlur(const QImage &image, int blurValue);
    QImage drawHistogram(const QImage &image);

    cv::Mat QImageToMat(const QImage &image);
    QImage MatToQImage(const cv::Mat &mat);
    QByteArray imageToByteArray(const QImage &image);

    // UI Elements
    QToolButton *selectButton;
    QToolButton *deleteButton;
    QToolButton *saveButton;
    QLabel *imageLabel;
    QVBoxLayout *layout;
    QHBoxLayout *buttonLayout;
    QVBoxLayout *leftSideLayout;
    QVBoxLayout *rightSideLayout;
    QRadioButton *defaultRadioButton;
    QRadioButton *edgeDetectionRadioButton;
    QSlider *th1Slider;
    QRadioButton *grayscaleRadioButton;
    QCheckBox *brightnessCheckBox;
    QCheckBox *contrastCheckBox;
    QCheckBox *blurCheckBox;
    QRadioButton *thresholdRadioButton;
    QRadioButton *histogramRadioButton;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSlider *blurSlider;
    QSlider *thresholdSlider;

    QImage loadedImage;
    QString loadedImagePath;
    QImage edgesImage;
    QImage grayscaleImage;
    QSqlDatabase db;
};

#endif
