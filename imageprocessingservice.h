#ifndef IMAGEPROCESSINGSERVICE_H
#define IMAGEPROCESSINGSERVICE_H

#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QBuffer>
#include <QPainter>

class ImageProcessingService
{
public:
    // Perform edge detection on the image
    static QImage performEdgeDetection(const QImage &image, int th1);

    // Convert image to grayscale
    static QImage convertToGrayscale(const QImage &image);

    // Apply thresholding to the image
    static QImage applyThreshold(const QImage &image, int thresholdValue);

    // Adjust brightness of the image
    static QImage adjustBrightness(const QImage &image, int brightnessValue);

    // Adjust contrast of the image
    static QImage adjustContrast(const QImage &image, int contrastValue);

    // Apply blur to the image
    static QImage applyBlur(const QImage &image, int blurValue);

    // Draw histogram of the image
    static QImage drawHistogram(const QImage &image);

    // Convert QImage to cv::Mat
    static cv::Mat QImageToMat(const QImage &image);

    // Convert cv::Mat to QImage
    static QImage MatToQImage(const cv::Mat &mat);

private:
    // Helper function to convert QImage to cv::Mat
    static cv::Mat convertQImageToMat(const QImage &image);

    // Helper function to convert cv::Mat to QImage
    static QImage convertMatToQImage(const cv::Mat &mat);
};

#endif
