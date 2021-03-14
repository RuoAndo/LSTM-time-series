/*
 * function.cpp
 *
 */

#include <list>
#include <map>
#include <random>
#include <iostream>
#include <chrono>
#include <math.h>
#include <boost/pool/object_pool.hpp>
#include <sstream>

#include "function.h"


using namespace std;

int func_id = 0;

PVariable variable_construct_for_function(Function *f, int rows, int cols) {
    PVariable r = PVariable(variable_construct(rows, cols), variable_destroy);
    r->creator = f;

    return r;
}


// Function class //////////////////////////////////////////////////////////////
Function::Function(){
    name = "Function";
    this->id = func_id;
    func_id++;
    count_function++;
}
Function::~Function(){
    init();
    count_function--;

}
void Function::init() {

    inputs.clear();
    outputs.clear();
}



PVariable Function::forward(vector<PVariable> &inputs, vector<PVariable > &outputs){return NULL;}
void Function::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){}


PVariable Function::forward(PVariable v){

    v->forward_count++;

    inputs.push_back(v);
    PVariable r = forward(inputs, outputs);

    return r;
}


PVariable Function::forward(PVariable v1, PVariable v2){

    v1->forward_count++;
    v2->forward_count++;

    inputs.push_back(v1);
    inputs.push_back(v2);
    PVariable r = forward(inputs, outputs);

    return r;
}

PVariable Function::forward(PVariable v1, PVariable v2, PVariable v3){

    v1->forward_count++;
    v2->forward_count++;
    v3->forward_count++;

    inputs.push_back(v1);
    inputs.push_back(v2);
    inputs.push_back(v3);
    PVariable r = forward(inputs, outputs);

    return r;
}

PVariable Function::forward(PVariable v1, PVariable v2, PVariable v3, PVariable v4){
    v1->forward_count++;
    v2->forward_count++;

    inputs.push_back(v1);
    inputs.push_back(v2);
    inputs.push_back(v3);
    inputs.push_back(v4);
    PVariable r = forward(inputs, outputs);

    return r;
}

PVariable Function::forward(PVariable v1, PVariable v2, PVariable v3, PVariable v4,
                            PVariable v5, PVariable v6, PVariable v7, PVariable v8,
                            PVariable v9, PVariable v10, PVariable v11, PVariable v12
){
    v1->forward_count++;
    v2->forward_count++;


    inputs.push_back(v1);
    inputs.push_back(v2);
    inputs.push_back(v3);
    inputs.push_back(v4);
    inputs.push_back(v5);
    inputs.push_back(v6);
    inputs.push_back(v7);
    inputs.push_back(v8);
    inputs.push_back(v9);
    inputs.push_back(v10);
    inputs.push_back(v11);
    inputs.push_back(v12);
    PVariable r = forward(inputs, outputs);

    return r;
}



void Function::backward(cuMat &p_grad){

    backward(p_grad, inputs, outputs);
}

void Function::clip_grad(Variable *v){
    float clip_grad_threshold = 5.0;
    float sq = v->grad.l2();
    float rate = clip_grad_threshold/sq;
    if (rate < 1.){
        v->grad.mul(rate, v->grad);
    }
}

void Function::reset_state(){}



FunctionPlus::FunctionPlus() : Function() {
    name = "FunctionPlus";
}



PVariable FunctionPlus::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable v1 = inputs.at(0);
    PVariable v2 = inputs.at(1);

    PVariable r = variable_construct_for_function(this, v1->data.rows, v1->data.cols);

    v1->data.plus(v2->data, r->data);

    float *tmp;
    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    free(tmp);
    
    return r;
}
void FunctionPlus::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable v1 = inputs.at(0);
    PVariable v2 = inputs.at(1);
    //v1->grad += p_grad*1.0;
    //v2->grad += p_grad*1.0;
    if (v1->isGetGrad) p_grad.mul_plus(1.0, v1->grad);
    if (v2->isGetGrad) p_grad.mul_plus(1.0, v2->grad);
}

FunctionMul::FunctionMul() : Function() {
    name = "FunctionMul";
}
PVariable FunctionMul::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable v1 = inputs.at(0);
    PVariable v2 = inputs.at(1);

    PVariable r = variable_construct_for_function(this, v1->data.rows, v1->data.cols);

    outputs.push_back(r);
    v1->data.mul(v2->data, r->data);

    float *tmp;
    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    free(tmp);
    
    return r;

}
void FunctionMul::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable v1 = inputs.at(0);
    PVariable v2 = inputs.at(1);

    //v1->grad += p_grad * v2->data;
    //v2->grad += p_grad * v1->data;
    if (v1->isGetGrad) p_grad.mul_plus(v2->data, v1->grad, 1.0, 1.0);
    if (v2->isGetGrad) p_grad.mul_plus(v1->data, v2->grad, 1.0, 1.0);
}

FunctionLinear::FunctionLinear() : Function() {
    name = "FunctionLinear";
}
FunctionLinear::FunctionLinear(Variable *w, Variable *b,  bool isTranspose) : Function() {
    name = "FunctionLinear";
    this->w  = w;
    this->b = b;
    this->isTranspose = isTranspose;
}
FunctionLinear::FunctionLinear(Variable *w,  bool isTranspose) : Function() {
    name = "FunctionLinear";
    noBias = true;
    this->w  = w;
    this->isTranspose = isTranspose;

}
FunctionLinear::FunctionLinear(int output_size, int input_size) : Function() {
    name = "FunctionLinear";

    this->w = new Variable(output_size, input_size);
    this->b = new Variable(output_size, 1);
    this->w->randoms(0., sqrt((1./(float)input_size)));

}

FunctionLinear::FunctionLinear(int output_size, int input_size, bool no_bias) : Function() {
    name = "FunctionLinear";

    noBias = no_bias;

    this->w = new Variable(output_size, input_size);
    this->w->randoms(0., sqrt((1./(float)input_size)));

    if (!noBias){
        this->b = new Variable(output_size, 1);
    }
}


void FunctionLinear::toHostArray(){
    i1.toHostArray();
    w->data.toHostArray();
    w->grad.toHostArray();
    if (!noBias){
        b->data.toHostArray();
        b->grad.toHostArray();
    }
}
void FunctionLinear::fromHostArray(){
    i1.fromHostArray();
    w->data.fromHostArray();
    w->grad.fromHostArray();
    if (!noBias){
        b->data.fromHostArray();
        b->grad.fromHostArray();
    }
}


PVariable FunctionLinear::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){


    PVariable x = inputs.at(0);

    int w_size = w->data.rows;

    /*
    if (isTranspose)
      w_size = w->data.cols;
    */

    float *tmp; 
    tmp = (float*)calloc(x->data.cols * x->data.rows, sizeof(float));
    cublasGetVector(x->data.rows * x->data.cols, sizeof(float), x->data.mDevice, 1, tmp, 1 );

    float *tmp_w; 
    tmp_w = (float*)calloc(w->data.cols * w->data.rows, sizeof(float));
    cublasGetVector(w->data.rows * w->data.cols, sizeof(float), w->data.mDevice, 1, tmp_w, 1 );

    free(tmp);
    free(tmp_w);
    
    PVariable r = PVariable(new Variable(this, w_size, x->data.cols));

    r->data.mHost = (float*)calloc(w_size * x->data.cols, sizeof(float));  
    
    if (i1.cols == 0 || i1.cols != x->data.cols){
        i1 = cuMat(1, x->data.cols);
        i1.ones();
    }

    tmp = (float*)calloc(i1.cols * i1.rows, sizeof(float));
    cublasGetVector(i1.rows * i1.cols, sizeof(float), i1.mDevice, 1, tmp, 1 );
    free(tmp);

    /* could be bug 2021-01-05 */
    if (!noBias)
      {
	b->data.dot(i1, r->data);

	/*
	for(int i = 0; i < x->data.cols; i++)
	  r->data.mHost[i] = b->data.mHost[i] * 1;
	*/
      }
	
    if (!isTranspose)
      w->data.dot_plus(x->data, r->data);

    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    free(tmp);

    /*
    else
      w->data.transpose().dot_plus(x->data, r->data);
    */    

    //r->data = w->data.dot(x->data) + b->data.dot(i1);

    return r;
}
void FunctionLinear::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable x = inputs.at(0);

    if (x->isGetGrad){
        if (!isTranspose) w->data.transpose_dot_plus(p_grad, x->grad);
        else w->data.transpose().transpose_dot_plus(p_grad, x->grad);
    }
    //x->grad += w->data.transpose().dot(p_grad);

    if (!isTranspose) p_grad.dot_transpose_plus(x->data, w->grad);
    else {
        cuMat tmpwg = w->grad.transpose();
        tmpwg *= 0;
        p_grad.dot_transpose_plus(x->data, tmpwg);
        w->grad += tmpwg.transpose();
    }
    //w->grad += p_grad.dot(x->data.transpose());


    if (!noBias){
        p_grad.dot_transpose_plus(i1, b->grad);
    }

    float *tmp;
    tmp = (float*)calloc(w->grad.cols * w->grad.rows, sizeof(float));
    cublasGetVector(w->grad.rows * w->grad.cols, sizeof(float), w->grad.mDevice, 1, tmp, 1 );
    free(tmp);
    
    //b->grad += p_grad.dot(i1.transpose());
}

FunctionSigmoid::FunctionSigmoid() : Function() {
    name = "FunctionSigmoid";
}
PVariable FunctionSigmoid::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable x = inputs.at(0);

    PVariable r = variable_construct_for_function(this, x->data.rows, x->data.cols);


    outputs.push_back(r);
    x->data.sigmoid(r->data);

    float *tmp;
    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    free(tmp);
    
    return r;
}

void FunctionSigmoid::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){
    PVariable x = inputs.at(0);


    if (rr.get() == NULL || rr->data.cols != x->data.cols){
        rr = PVariable(new Variable(x->data.rows, x->data.cols));
    }
    x->data.sigmoid_d(rr->data);

    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);
}

FunctionTanh::FunctionTanh() : Function() {
    name = "FunctionTanh";
}
PVariable FunctionTanh::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable x = inputs.at(0);

    PVariable r = variable_construct_for_function(this, x->data.rows, x->data.cols);

    outputs.push_back(r);

    x->data.tanh(r->data);

    float *tmp;
    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    free(tmp);
    
    return r;
}
void FunctionTanh::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){
    PVariable x = inputs.at(0);


    if (rr.get() == NULL || rr->data.cols != x->data.cols){
        rr = PVariable(new Variable(x->data.rows, x->data.cols));
    }
    x->data.tanh_d(rr->data);

    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);
}

FunctionMeanSquaredError::FunctionMeanSquaredError() : Function() {
    name = "FunctionMeanSquaredError";
    loss = cuMat(1,1);
    loss.ones();
}

PVariable FunctionMeanSquaredError::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){

    PVariable x = inputs.at(0);
    PVariable t = inputs.at(1);

    float *tmp_x;
    tmp_x = (float*)calloc(x->data.cols * x->data.rows, sizeof(float));
    cublasGetVector(x->data.rows * x->data.cols, sizeof(float), x->data.mDevice, 1, tmp_x, 1 );

    float *tmp_t;
    tmp_t = (float*)calloc(t->data.cols * t->data.rows, sizeof(float));
    cublasGetVector(t->data.rows * t->data.cols, sizeof(float), t->data.mDevice, 1, tmp_t, 1 );

    free(tmp_x);
    free(tmp_t);
    
    if (rr.get() == NULL || rr->data.cols != x->data.cols){
        rr = PVariable(new Variable(x->data));
    }

    x->data.minus(t->data, rr->data);

    rr->data.mul(rr->data, rr->data);

    float sum = rr->data.sum();
    sum /= (2*rr->data.cols);

    PVariable r = variable_construct_for_function(this, loss.rows, loss.cols);

    outputs.push_back(r);

    loss.mul(sum, r->data);

    float *tmp;
    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));
    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );
    memcpy(r->data.mHost, tmp, r->data.rows * r->data.cols * sizeof(float));
    free(tmp);
    
    return r;
}
void FunctionMeanSquaredError::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){
    PVariable x = inputs.at(0);
    PVariable t = inputs.at(1);

    if (rr.get() == NULL || rr->data.cols != x->data.cols){

        rr = PVariable(new Variable(x->data));
    }

    x->data.minus(t->data, rr->data);
    float batch_size = rr->data.cols;
    if (x->isGetGrad) x->grad.plus(rr->data, x->grad);
}

