///* 
// * File:   main.cpp
// * Author: Archeress1966
// *
// * Created on November 5, 2014, 5:52 AM
// * Based on code found here:  http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
// */
//#include <iostream> //require for opening and reading files
//#include <fstream>  //required for opening and reading files
//#include <cstdlib>  //C++ std library
//#include "opencv2/highgui/highgui.hpp" //must have for core openCV matrices
//#include "opencv2/ml/ml.hpp"      //must have for machine learning
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"
//
//using namespace cv;
//using namespace std;
//#define CLASScount 5
//
//int main(int argc, char** argv) {
//
//    string labels[CLASScount] = {"FACE", "CAR", "PLANE", "MOTORCYCLE", "LEAF"};
//
//    //string dirName = "C:\\Users\\Zach\\Documents\\NetBeansProjects\\Zach Alleman CV Final Project\\";
//    string fileTemplates[CLASScount] ={"images\\faces\\image_%04d.jpg", 
//                                       "images\\cars\\image_%04d.jpg", 
//                                       "images\\planes\\%04d.jpg", 
//                                       "images\\motorcycles\\%04d.jpg", 
//                                       "images\\leaves\\image_%04d.jpg"};
//    int fileCounts[] = {100, 100, 100, 100, 100};
//
//    char *filename = new char[100];
//    Mat currentImage;
//
//
//    vector<KeyPoint> keypoints; //hold keypoints   
//    Mat descriptor; //hold descriptors
//
//    Mat features;
//    //The SIFT feature extractor and descriptor
//    SiftDescriptorExtractor detector;
//
//    for (int classification = 0; classification < 5; classification++) {
//        for (int imageNumber = 1; imageNumber <= fileCounts[classification]; imageNumber++) {
//            sprintf(filename, fileTemplates[classification].c_str(), imageNumber);
//            cout << "file name = "  << filename << endl;
//            currentImage = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale                
//            //detect feature points
//            detector.detect(currentImage, keypoints);
//            //compute the descriptors for each keypoint
//            detector.compute(currentImage, keypoints, descriptor);
//            features.push_back(descriptor); //place descriptors in matrix          
//        }//for each image
//    }//for each classification
//
//    //Construct BOWKMeansTrainer
//    //the number of bags
//    int dictionarySize = 200;
//    //define Term Criteria
//    TermCriteria criteriaForLooping(CV_TERMCRIT_ITER, 100, 0.001);
// 
//    int retries = 1;  
//    int flags = KMEANS_PP_CENTERS;
//    
//    BOWKMeansTrainer bowTrainer(dictionarySize, criteriaForLooping, retries, flags);
//    
//    //cluster the feature vectors
//    
//    Mat dictionary = bowTrainer.cluster(features);
//    //store the vocabulary
//    FileStorage siftDictionary("siftDictionary2.yml", FileStorage::WRITE);
//    siftDictionary << "vocabulary" << dictionary;
//    siftDictionary.release();
//
//
//}//main