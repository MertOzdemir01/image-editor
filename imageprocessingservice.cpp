#include "imageprocessingservice.h"

QImage ImageProcessingService::performEdgeDetection(const QImage &image, int th1)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat edges;
    cv::Canny(mat, edges, 0, th1);
    return MatToQImage(edges);
}

QImage ImageProcessingService::convertToGrayscale(const QImage &image)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat gray;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    return MatToQImage(gray);
}

QImage ImageProcessingService::applyThreshold(const QImage &image, int thresholdValue)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat gray, thresholded;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, thresholded, thresholdValue, 255, cv::THRESH_BINARY);
    return MatToQImage(thresholded);
}

QImage ImageProcessingService::adjustBrightness(const QImage &image, int brightnessValue)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat adjusted;
    mat.convertTo(adjusted, -1, brightnessValue / 100.0, 0);
    return MatToQImage(adjusted);
}

QImage ImageProcessingService::adjustContrast(const QImage &image, int contrastValue)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat adjusted;
    double contrastFactor = contrastValue / 100.0;
    mat.convertTo(adjusted, -1, contrastFactor, 0);
    return MatToQImage(adjusted);
}

QImage ImageProcessingService::applyBlur(const QImage &image, int blurValue)
{
    cv::Mat mat = QImageToMat(image);
    cv::Mat blurred;
    cv::blur(mat, blurred, cv::Size(blurValue, blurValue));
    return MatToQImage(blurred);
}

QImage ImageProcessingService::drawHistogram(const QImage &image)
{
    // Convert QImage to cv::Mat
    cv::Mat mat = QImageToMat(image);

    // Split channels
    std::vector<cv::Mat> bgr_planes;
    cv::split(mat, bgr_planes);

    // Histogram parameters
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    // Calculate histograms
    cv::Mat hist[3];
    for (int i = 0; i < 3; ++i) {
        cv::calcHist(&bgr_planes[i], 1, 0, cv::Mat(), hist[i], 1, &histSize, &histRange, true, false);
        cv::normalize(hist[i], hist[i], 0, 400, cv::NORM_MINMAX);
    }

    // Create histogram image
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    // Draw histograms
    cv::Scalar colors[3] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255) };
    for (int i = 1; i < histSize; i++) {
        for (int j = 0; j < 3; ++j) {
            cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist[j].at<float>(i - 1))),
                     cv::Point(bin_w * i, hist_h - cvRound(hist[j].at<float>(i))),
                     colors[j], 2, 8, 0);
        }
    }

    return MatToQImage(histImage);
}

cv::Mat ImageProcessingService::QImageToMat(const QImage &image)
{
    return convertQImageToMat(image);
}

QImage ImageProcessingService::MatToQImage(const cv::Mat &mat)
{
    return convertMatToQImage(mat);
}

cv::Mat ImageProcessingService::convertQImageToMat(const QImage &image)
{
    switch (image.format()) {
    case QImage::Format_RGB32: {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone();
    }
    case QImage::Format_RGB888: {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::Mat matBGR;
        cv::cvtColor(mat, matBGR, cv::COLOR_RGB2BGR);
        return matBGR.clone();
    }
    case QImage::Format_Grayscale8: {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone();
    }
    default:
        return cv::Mat();
    }
}

QImage ImageProcessingService::convertMatToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC4: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        return image.copy();
    }
    case CV_8UC3: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    default:
        return QImage();
    }
}
