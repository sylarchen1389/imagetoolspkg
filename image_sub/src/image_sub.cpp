#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<opencv2/highgui/highgui.hpp>
#include<cv_bridge/cv_bridge.h>
#include<time.h>
#include<iostream>
#include<string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

double start_t,end_t,pass_cost;
char *save_dir = "/home/sylar/dataset/";
// string dir = "~/dataset";

#define SAVE_IMG


void imageCallback(const sensor_msgs::ImageConstPtr& msg){
    start_t = clock();
    try{
        cv::Mat img = cv_bridge::toCvShare(msg,"bgr8")->image;
        cv::Mat frame;
        cout<<img.cols<<","<<img.rows<<endl;
        float scaleL = img.rows/1080.0;
        float scaleR = img.cols/1920.0;
        float scale = scaleL>scaleR?scaleL:scaleR;
        cout<<scale<<endl;
        cout<<(int)(img.cols/scale)<<","<<(int)(img.rows/scale)<<endl;
        cv::resize(img, frame,cv::Size((int)(img.cols/scale),(int)(img.rows/scale)), cv::INTER_AREA);
        cv::imshow("view",frame);
#ifdef SAVE_IMG
        time_t timep;
        time (&timep);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d",localtime(&timep) );
        char path[100];
        strcpy(path, save_dir);
        strcat(path, tmp);
        cout<<"save path: "<<path<<endl;
        if (0 != access(path, 0))
            if (mkdir(path,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO) == -1){
                ROS_ERROR("mkdir error"); 
                exit(1);
            }
        time (&timep);
        strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H:%M:%S",localtime(&timep) ); 
        char img_path[200];
        strcpy(img_path, path);
        strcat(img_path, "/");
        strcat(img_path, tmp);
        strcat(img_path, ".jpg");
        cout<<"image path: "<<img_path<<endl;
        cv::imwrite(img_path,img);
#endif            
        //cv::waitKey(30);
    }catch(cv_bridge::Exception e  ){
        ROS_ERROR("Could not convert");
    }
    end_t = clock();
    std::cout<<"[Time] using time:"<<(end_t-start_t)/CLOCKS_PER_SEC<<std::endl;
}



int main(int argc,char **argv){
    ros::init(argc,argv,"image_sub");
    ros::NodeHandle nh;
    cv::namedWindow("view");
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("camera/leftRaw",1,imageCallback);
    // image_transport::Subscriber sub = it.subscribe("/tensorrt/detection_image",1,imageCallback);
    ros::spin();
    cv::destroyWindow("view");
    return 0;
}