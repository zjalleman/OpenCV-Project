/* 
 * Zachary Alleman
 * 
 * Modified from MakeSiftDictionary.cpp and UseSiftDictionary.cpp from moodle
 * Also used SVM sample code from moodle
 */
#include <iostream> //require for opening and reading files
#include <fstream>  //required for opening and reading files
#include <cstdlib>  //C++ std library
#include "opencv2/highgui/highgui.hpp" //must have for core openCV matrices
#include "opencv2/ml/ml.hpp"      //must have for machine learning
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;
#define CLASScount 5



void useDict(int fileCounts[5], Mat &features, Mat &responses, int startNumber) {
    string labels[CLASScount] = {"FACE", "CAR", "PLANE", "MOTORCYCLE", "LEAF"};

    string dirName = "C:\\Users\\Zach\\Documents\\NetBeansProjects\\Zach Alleman CV Final Project\\";
    string fileTemplates[CLASScount] = {
        "images\\faces\\image_%04d.jpg",
        "images\\cars\\image_%04d.jpg",
        "images\\planes\\%04d.jpg",
        "images\\motorcycles\\%04d.jpg",
        "images\\leaves\\image_%04d.jpg"};
    

    char *filename = new char[200];
    Mat currentImage;
    vector<KeyPoint> keypoints; //hold keypoints  
    //prepare BOW descriptor extractor from the dictionary    
    Mat dictionary;
    
    FileStorage dictionaryFile("siftDictionary3.yml", FileStorage::READ);
    dictionaryFile["vocabulary"] >> dictionary;
    dictionaryFile.release();

    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
  
    //You can change the type of feature extracter here
    
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
    
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor, matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);
    
    
    cout << "Loading..." << endl;
    for (int classification = 0; classification < 5; classification++) {
        
        for (int imageNumber = startNumber; imageNumber < startNumber + fileCounts[classification]; imageNumber++) {
            string tempName = dirName + fileTemplates[classification];
            sprintf(filename, tempName.c_str(), imageNumber);
            //cout << "file name = " << filename << endl;
            currentImage = imread(filename, 0); //Load as grayscale                
            //detect feature points
            detector->detect(currentImage, keypoints);
            //compute the descriptors for each keypoint
            //To store the BoW (or BoF) representation of the image
             Mat bowDescriptor;
            //extract BoW (or BoF) descriptor from given image
            bowDE.compute(currentImage, keypoints, bowDescriptor); 
            features.push_back(bowDescriptor); //place descriptors in matrix    
            responses.push_back((float)classification);
        }//for each image
    }//for each classification
    cout << "feature cols = " << features.cols << endl;
    cout << "feature rows = " << features.rows << endl;
    //cout << features << endl;
    //cout << responses <<endl;
}


CvSVM* createSVM(Mat features, Mat responses){
    //cout << features << endl;
    //cout << responses << endl;
    float weights[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
    Mat weightMat(5, 1, CV_32F, weights);
    CvMat weightMat2 = weightMat;
    CvSVMParams params(CvSVM::C_SVC,  //CvSVM::C_SVC C-Support Vector Classification. 
                                      //CvSVM::NU_SVC nu-Support Vector Classification. set nu 
                                      //CvSVM::ONE_CLASS Distribution Estimation (One-class SVM). All the training data are from the same class, SVM builds a boundary that separates the class from the rest of the feature space.
                                      //CvSVM::EPS_SVR \epsilon-Support Vector Regression. set p
                                      //CvSVM::NU_SVR \nu-Support Vector Regression. set nu
                       CvSVM::RBF, //kernel type
                                      //CvSVM::LINEAR Linear kernel. No mapping is done, linear discrimination (or regression) is done in the original feature space. It is the fastest option. K(x_i, x_j) = x_i^T x_j.
                                      //CvSVM::POLY Polynomial kernel: K(x_i, x_j) = (\gamma x_i^T x_j + coef0)^{degree}, \gamma > 0.
                                      //CvSVM::RBF Radial basis function (RBF), a good choice in most cases. K(x_i, x_j) = e^{-\gamma ||x_i - x_j||^2}, \gamma > 0.
                                      //CvSVM::SIGMOID Sigmoid kernel: K(x_i, x_j) = \tanh(\gamma x_i^T x_j + coef0). 
                       2,//degree use for POLY kernel 
                       2, //gamma use for POLY or SIGMOID
                       2, //Parameter coef0 of a kernel function (POLY / SIGMOID)
                       2,  //must be greater than 1 Parameter C of a SVM optimization problem (C_SVC / EPS_SVR / NU_SVR). 
                       0.0,  //Parameter \nu of a SVM optimization problem (NU_SVC / ONE_CLASS / NU_SVR).
                       0.0, //Parameter \epsilon of a SVM optimization problem (EPS_SVR). 
                       &weightMat2,
                       cvTermCriteria(CV_TERMCRIT_EPS, 100, 1e-6 ));


    CvSVM* svm = new CvSVM; //this is a pointer to the CvSVM
    
    svm->train_auto( features, responses, Mat(), Mat(), params, 10, 
            CvSVM::get_default_grid(params.C), CvSVM::get_default_grid(params.gamma),
            CvSVM::get_default_grid(params.p), CvSVM::get_default_grid(params.nu), 
            CvSVM::get_default_grid(params.coef0), CvSVM::get_default_grid(params.degree), false);
   
    return svm;

}//createTree


void svmPredict(CvSVM* svm, Mat features, Mat responses){
    // compute hit-rate on the training database, demonstrates predict usage.
    float sum = 0.0;
    float numCorrect = 0.0;
    
    float sum0 = 0.0;
    float sum1 = 0.0;
    float sum2 = 0.0;
    float sum3 = 0.0;
    float sum4 = 0.0;
    float numCorrect0 = 0.0;
    float numCorrect1 = 0.0;
    float numCorrect2 = 0.0;
    float numCorrect3 = 0.0;
    float numCorrect4 = 0.0;
    for(int i = 0; i < features.rows; i++ )
    {
        Mat sample = features.row(i);
       
        float svmResponse = svm->predict(sample, false);
        float delta = fabs(svmResponse - responses.at<float>(i));
        //fprintf(stdout, "TestCase #%d:  Target-> %1.2f   SVM-> %1.2f\n",  i, responses.at<float>(i), svmResponse);
        if (responses.at<float>(i) == 0) {
                sum0++;
            }
            if (responses.at<float>(i) == 1) {
                sum1++;
            }
            if (responses.at<float>(i) == 2) {
                sum2++;
            }
            if (responses.at<float>(i) == 3) {
                sum3++;
            }
            if (responses.at<float>(i) == 4) {
                sum4++;
            }
        if (delta == 0.0) {
            numCorrect++;
            if (responses.at<float>(i) == 0) {
                numCorrect0++;
            }
            if (responses.at<float>(i) == 1) {
                numCorrect1++;
            }
            if (responses.at<float>(i) == 2) {
                numCorrect2++;
            }
            if (responses.at<float>(i) == 3) {
                numCorrect3++;
            }
            if (responses.at<float>(i) == 4) {
                numCorrect4++;
            }
        }
        sum = sum + delta*delta;
    }
    fprintf(stdout, "Mean Square Error: %1.5f\n", sum/features.rows);
    fprintf(stdout, "Number Correct: %1.5f\n", numCorrect);
    fprintf(stdout, "Classification Rate: %1.5f\n", (numCorrect/features.rows) * 100);
    cout << numCorrect0/sum0 << endl;
    cout << numCorrect1/sum1 << endl;
    cout << numCorrect2/sum2 << endl;
    cout << numCorrect3/sum3 << endl;
    cout << numCorrect4/sum4 << endl;
}


int main(int argc, char** argv) {
    cout << "TRAINING" << endl;
    Mat trainFeatures, trainResponses;
    int fileCounts[] = {50, 50, 50, 50, 50};
    useDict(fileCounts, trainFeatures, trainResponses, 1);
    CvSVM* svm = createSVM(trainFeatures, trainResponses);
    svmPredict(svm, trainFeatures, trainResponses);
  
    cout << endl;
    cout << "TESTING" << endl;
    int fileCounts2[] = {100,100,100,100,100};
    Mat testFeatures, testResponses;
    useDict(fileCounts2, testFeatures, testResponses, 51);
    svmPredict(svm, testFeatures, testResponses);
    
    return 0;


}//main