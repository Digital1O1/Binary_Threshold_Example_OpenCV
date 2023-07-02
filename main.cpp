#include <opencv2/opencv.hpp>

std::string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
        std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
        "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
        std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main() 
{
    // Jetson Nano parameters
    /*
    int capture_width = 1280;
    int capture_height = 720;
    int display_width = 1280;
    int display_height = 720;
    int framerate = 30;
    int flip_method = 2;

    std::string pipeline = gstreamer_pipeline(capture_width,
        capture_height,
        display_width,
        display_height,
        framerate,
        flip_method);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";

    cv::VideoCapture videoCapture(pipeline, cv::CAP_GSTREAMER);

    */

    // Laptop parameters
    cv::VideoCapture videoCapture(0);  // 0 for camera, or provide a video file path

    if (!videoCapture.isOpened()) {
        std::cout << "Failed to open video source." << std::endl;
        return -1;
    }

    cv::namedWindow("Original Frame", cv::WINDOW_NORMAL);
    cv::namedWindow("Thresholded Frame", cv::WINDOW_NORMAL);

    while (true) {
        cv::Mat frame;
        videoCapture >> frame;

        if (frame.empty()) {
            std::cout << "End of video stream" << std::endl;
            break;
        }

        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        cv::Mat thresholdedFrame;
        cv::threshold(grayFrame, thresholdedFrame, 128, 255, cv::THRESH_BINARY);

        cv::imshow("Original Frame", frame);
        cv::imshow("Thresholded Frame", thresholdedFrame);

        if (cv::waitKey(1) == 'q') 
        {
            break;
        }
    }

    videoCapture.release();
    cv::destroyAllWindows();

    return 0;
}
