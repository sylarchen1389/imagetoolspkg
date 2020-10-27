#include<iostream>
#include<ros/ros.h>
#include<opencv2/highgui/highgui.hpp>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<string>
#include<time.h>
#include<chrono>
#include <dirent.h>
#include<vector>


using namespace std;

// string img_dir  = "/home/sylar/catkin_ws/src/image_pub/data/1296.jpg";
// string img_dir="/home/sylar/dataset/2020-08-13-catImg/";
// string img_dir = "/media/sylar/DATA/Code_D/model/dataset/BIT_FSACOCO/VOCdevkit/VOC2018/JPEGImages/";
string img_dir = "/home/sylar/dataset/2020-08-31-cat/";
string img_out_dir = "/home/sylar/dataset/out/";
// string img_out_dir = "/home/sylar/catkin_ws/src/image_pub/out";

std::vector<std::string>readFolder(const std::string &image_path)
{
    std::vector<std::string> image_names;
    auto dir = opendir(image_path.c_str());
 
    if ((dir) != NULL)
    {
        struct dirent *entry;
        entry = readdir(dir);
        while (entry)
        {
            auto temp = image_path + entry->d_name;
            if (strcmp(entry->d_name, "") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                entry = readdir(dir);
                continue;
            }
            std::cout << image_path + "/" + entry->d_name << std::endl;
            image_names.push_back(temp);
            entry = readdir(dir);
        }
    }
    return image_names;
}

int main(int argc,char** argv){
    ros::init(argc,argv,"image_pub");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/leftRaw",3);
    // cv::Mat image = cv::imread(img_dir,CV_LOAD_IMAGE_COLOR);
    // cv::waitKey(10);
    // sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(),"bgr8",image).toImageMsg();
    ros::Rate r(1);

    double start,end;
    ros::Rate loop_rate(10);
    // while (nh.ok())
    // {
        for(string image_name : readFolder(img_dir)){
            if(!nh.ok())
                continue
            auto t_start = std::chrono::high_resolution_clock::now();
            cv::Mat image = cv::imread(image_name,CV_LOAD_IMAGE_COLOR);
            cout<<image_name<<endl;
            std::cout<< image.cols <<","<<image.rows<<std::endl;
            //cv::waitKey(10);
            sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(),"bgr8",image).toImageMsg();
            pub.publish(msg);
            ros::spinOnce();
            //loop_rate.sleep();
            auto t_end = std::chrono::high_resolution_clock::now();
            auto total = std::chrono::duration<float, std::milli>(t_end - t_start).count();
            std::cout << "[Time] taken for image pub is " << total << " ms." << std::endl;
            bool met = r.sleep();
            ros::spinOnce();
        }
    // }
}