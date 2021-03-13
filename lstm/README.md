<pre>
1. plus / mul_plus
2. mul / mul_plus
3. dot / dot_plus
4. dot_transpose_plus / dot
5. sigmoid / sigmoid_d
6. tanh / tanh_d
7. mul / minus

1. plus
2. mul_plus
3. mul
4. dot
5. dot_transpose_plus
6. minus
7. sigmoid
8. sigmoid_d
9. tanh
10. tanh_d

#####

1. plus / mul_plus

155PVariable FunctionPlus::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){   
162    v1->data.plus(v2->data, r->data); 

171void FunctionPlus::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){ 
177    if (v1->isGetGrad) p_grad.mul_plus(1.0, v1->grad);                                                                                                                

2. mul / mul_plus

184PVariable FunctionMul::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){ 
192    v1->data.mul(v2->data, r->data); 

202void FunctionMul::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                                                                                                                                                  
209    if (v1->isGetGrad) p_grad.mul_plus(v2->data, v1->grad, 1.0, 1.0);                                                                                                 
210    if (v2->isGetGrad) p_grad.mul_plus(v1->data, v2->grad, 1.0, 1.0);                                                                                                 

3. dot / dot_plus

272PVariable FunctionLinear::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){ 
311        b->data.dot(i1, r->data);    
319    if (!isTranspose)                                                                                                                                                 
320      w->data.dot_plus(x->data, r->data);  

4. dot_transpose_plus / dot

335void FunctionLinear::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){  
345    if (!isTranspose) p_grad.dot_transpose_plus(x->data, w->grad);                                                                                                    
                                                                                                                                                          
347        cuMat tmpwg = w->grad.transpose();                                                                                                                            
348        tmpwg *= 0;                                                                                                                                                   
349        p_grad.dot_transpose_plus(x->data, tmpwg);                                                                                                                    
350        w->grad += tmpwg.transpose();                                                                                                                                 
351    }                                                                                                                                                                 
352    //w->grad += p_grad.dot(x->data.transpose());                                                                                                                     
353                                                                                                                                                                      
354                                                                                                                                                                      
355    if (!noBias){                                                                                                                                                     
356        p_grad.dot_transpose_plus(i1, b->grad);                                                                                                                       
357    }   

5. sigmoid / sigmoid_d

370PVariable FunctionSigmoid::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                          
371                                                                                                                                                                                                                                                                                                                                      
377    outputs.push_back(r);                                                                                                                                             
378    x->data.sigmoid(r->data); 

388void FunctionSigmoid::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                               
394    }                                                                                                                                                                 
395    x->data.sigmoid_d(rr->data);                                                                                                                                      
396                                                                                                                                                                      
397    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);                                                                                                   
398}   

6. tanh / tanh_d

403PVariable FunctionTanh::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                             
404                                                                                                                                                                      
405    PVariable x = inputs.at(0);                                                                                                                                       
406                                                                                                                                                                      
407    PVariable r = variable_construct_for_function(this, x->data.rows, x->data.cols);                                                                                  
408                                                                                                                                                                      
409    outputs.push_back(r);                                                                                                                                             
410                                                                                                                                                                      
411    x->data.tanh(r->data); 

420void FunctionTanh::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                  
421    PVariable x = inputs.at(0);                                                                                                                                       
422                                                                                                                                                                                                                                                                                                                                      
427    x->data.tanh_d(rr->data);                                                                                                                                         
428                                                                                                                                                                      
429    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);                                                                                                   
430}    

7. mul / minus

   438PVariable FunctionMeanSquaredError::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                                                                                                                                                                             
   458    x->data.minus(t->data, rr->data);                                                                                                                                 
   459                                                                                                                                                                      
   460    rr->data.mul(rr->data, rr->data);                                                                                                                                                                                                                                                                           
   469    loss.mul(sum, r->data);                                                                                                                                           
   470                                                                                                                                                                                                                                                                                                                             
   476                                                                                                                                                                      
   477    return r;                                                                                                                                                         
   478}       

   479void FunctionMeanSquaredError::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                      
   480    PVariable x = inputs.at(0);                                                                                                                                       
   481    PVariable t = inputs.at(1);                                                                                                                                       
   482                                                                                                                                                                                                                                                                                                                                      
   488    x->data.minus(t->data, rr->data);                                                                                                                                 
   489    float batch_size = rr->data.cols;                                                                                                                                 
   490    if (x->isGetGrad) x->grad.plus(rr->data, x->grad);                                                                                                                
   491}    
   
#######################

   155PVariable FunctionPlus::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                             
   156                                                                                                                                                                      
   157    PVariable v1 = inputs.at(0);                                                                                                                                      
   158    PVariable v2 = inputs.at(1);                                                                                                                                      
   159                                                                                                                                                                      
   160    PVariable r = variable_construct_for_function(this, v1->data.rows, v1->data.cols);                                                                                
   161                                                                                                                                                                      
   162    v1->data.plus(v2->data, r->data);                                                                                                                                 
   163                                                                                                                                                                      
   164    float *tmp;                                                                                                                                                       
   165    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   166    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   167    free(tmp);                                                                                                                                                        
   168                                                                                                                                                                      
   169    return r;                                                                                                                                                         
   170}                                    
                                                                                                                                    
   171void FunctionPlus::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                  
   172                                                                                                                                                                      
   173    PVariable v1 = inputs.at(0);                                                                                                                                      
   174    PVariable v2 = inputs.at(1);                                                                                                                                      
   175    //v1->grad += p_grad*1.0;                                                                                                                                         
   176    //v2->grad += p_grad*1.0;                                                                                                                                         
   177    if (v1->isGetGrad) p_grad.mul_plus(1.0, v1->grad);                                                                                                                
   178    if (v2->isGetGrad) p_grad.mul_plus(1.0, v2->grad);                                                                                                                
   179}           

   184PVariable FunctionMul::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                              
   185                                                                                                                                                                      
   186    PVariable v1 = inputs.at(0);                                                                                                                                      
   187    PVariable v2 = inputs.at(1);                                                                                                                                      
   188                                                                                                                                                                      
   189    PVariable r = variable_construct_for_function(this, v1->data.rows, v1->data.cols);                                                                                
   190                                                                                                                                                                      
   191    outputs.push_back(r);                                                                                                                                             
   192    v1->data.mul(v2->data, r->data);                                                                                                                                  
   193                                                                                                                                                                      
   194    float *tmp;                                                                                                                                                       
   195    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   196    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   197    free(tmp);                                                                                                                                                        
   198                                                                                                                                                                      
   199    return r;                                                                                                                                                         
   200                                 
                                                                                                                                           201}                                                                                                                                                                     
   202void FunctionMul::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                   
   203                                                                                                                                                                      
   204    PVariable v1 = inputs.at(0);                                                                                                                                      
   205    PVariable v2 = inputs.at(1);                                                                                                                                      
   206                                                                                                                                                                      
   207    //v1->grad += p_grad * v2->data;                                                                                                                                  
   208    //v2->grad += p_grad * v1->data;                                                                                                                                  
   209    if (v1->isGetGrad) p_grad.mul_plus(v2->data, v1->grad, 1.0, 1.0);                                                                                                 
   210    if (v2->isGetGrad) p_grad.mul_plus(v1->data, v2->grad, 1.0, 1.0);                                                                                                 
   211}
   
   272PVariable FunctionLinear::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                           
   273                                                                                                                                                                      
   274                                                                                                                                                                         
   275    PVariable x = inputs.at(0);                                                                                                                                       
   276                                                                                                                                                                      
   277    int w_size = w->data.rows;                                                                                                                                        
   278                                                                                                                                                                      
   279    /*                                                                                                                                                                
   280    if (isTranspose)                                                                                                                                                  
   281      w_size = w->data.cols;                                                                                                                                          
   282    */                                                                                                                                                                
   283                                                                                                                                                                      
   284    float *tmp;                                                                                                                                                       
   285    tmp = (float*)calloc(x->data.cols * x->data.rows, sizeof(float));                                                                                                 
   286    cublasGetVector(x->data.rows * x->data.cols, sizeof(float), x->data.mDevice, 1, tmp, 1 );                                                                         
   287                                                                                                                                                                      
   288    float *tmp_w;                                                                                                                                                     
   289    tmp_w = (float*)calloc(w->data.cols * w->data.rows, sizeof(float));                                                                                               
   290    cublasGetVector(w->data.rows * w->data.cols, sizeof(float), w->data.mDevice, 1, tmp_w, 1 );                                                                       
   291                                                                                                                                                                      
   292    free(tmp);                                                                                                                                                        
   293    free(tmp_w);  
   294                                                                                                                                                                      
   295    PVariable r = PVariable(new Variable(this, w_size, x->data.cols));                                                                                                
   296                                                                                                                                                                      
   297    r->data.mHost = (float*)calloc(w_size * x->data.cols, sizeof(float));                                                                                             
   298                                                                                                                                                                      
   299    if (i1.cols == 0 || i1.cols != x->data.cols){                                                                                                                     
   300        i1 = cuMat(1, x->data.cols);                                                                                                                                  
   301        i1.ones();                                                                                                                                                    
   302    }                                                                                                                                                                 
   303                                                                                                                                                                      
   304    tmp = (float*)calloc(i1.cols * i1.rows, sizeof(float));                                                                                                           
   305    cublasGetVector(i1.rows * i1.cols, sizeof(float), i1.mDevice, 1, tmp, 1 );                                                                                        
   306    free(tmp);                                                                                                                                                        
   307                                                                                                                                                                      
   308    /* could be bug 2021-01-05 */                                                                                                                                     
   309    if (!noBias)                                                                                                                                                      
   310      {                                                                                                                                                               
   311        b->data.dot(i1, r->data);                                                                                                                                     
   312                                                                                                                                                                      
   313        /*                                                                                                                                                            
   314        for(int i = 0; i < x->data.cols; i++)                                                                                                                         
   315          r->data.mHost[i] = b->data.mHost[i] * 1;                                                                                                                    
   316        */                          
   317      }                                                                                                                                                               
   318                                                                                                                                                                      
   319    if (!isTranspose)                                                                                                                                                 
   320      w->data.dot_plus(x->data, r->data);                                                                                                                             
   321                                                                                                                                                                      
   322    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   323    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   324    free(tmp);                                                                                                                                                        
   325                                                                                                                                                                      
   326    /*                                                                                                                                                                
   327    else                                                                                                                                                              
   328      w->data.transpose().dot_plus(x->data, r->data);                                                                                                                 
   329    */                                                                                                                                                                
   330                                                                                                                                                                      
   331    //r->data = w->data.dot(x->data) + b->data.dot(i1);                                                                                                               
   332                                                                                                                                                                      
   333    return r;                                                                                                                                                         
   334}          


   335void FunctionLinear::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                
   336                                                                                                                                                                      
   337    PVariable x = inputs.at(0);                                                                                                                                       
   338                                                                                                                                                                      
   339    if (x->isGetGrad){                                                                                                                                                
   340        if (!isTranspose) w->data.transpose_dot_plus(p_grad, x->grad);                                                                                                
   341        else w->data.transpose().transpose_dot_plus(p_grad, x->grad);                                                                                                 
   342    }                                                                                                                                                                    
   343    //x->grad += w->data.transpose().dot(p_grad);                                                                                                                     
   344                                                                                                                                                                      
   345    if (!isTranspose) p_grad.dot_transpose_plus(x->data, w->grad);                                                                                                    
   346    else {                                                                                                                                                            
   347        cuMat tmpwg = w->grad.transpose();                                                                                                                            
   348        tmpwg *= 0;                                                                                                                                                   
   349        p_grad.dot_transpose_plus(x->data, tmpwg);                                                                                                                    
   350        w->grad += tmpwg.transpose();                                                                                                                                 
   351    }                                                                                                                                                                 
   352    //w->grad += p_grad.dot(x->data.transpose());                                                                                                                     
   353                                                                                                                                                                      
   354                                                                                                                                                                      
   355    if (!noBias){                                                                                                                                                     
   356        p_grad.dot_transpose_plus(i1, b->grad);                                                                                                                       
   357    }                                                                                                                                                                 
   358                                                                                                                                                                      
   359    float *tmp;                                                                                                                                                       
   360    tmp = (float*)calloc(w->grad.cols * w->grad.rows, sizeof(float));                                                                                                 
   361    cublasGetVector(w->grad.rows * w->grad.cols, sizeof(float), w->grad.mDevice, 1, tmp, 1 );                                                                         
   362    free(tmp);                                                                                                                                                        
   363                                                                                                                                                                      
   364    //b->grad += p_grad.dot(i1.transpose());                                                                                                                          
   365}             
   

   370PVariable FunctionSigmoid::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                          
   371                                                                                                                                                                      
   372    PVariable x = inputs.at(0);                                                                                                                                       
   373                                                                                                                                                                      
   374    PVariable r = variable_construct_for_function(this, x->data.rows, x->data.cols);                                                                                  
   375                                                                                                                                                                      
   376                                                                                                                                                                      
   377    outputs.push_back(r);                                                                                                                                             
   378    x->data.sigmoid(r->data);                                                                                                                                         
   379                                                                                                                                                                      
   380    float *tmp;                                                                                                                                                       
   381    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   382    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   383    free(tmp);                                                                                                                                                        
   384                                                                                                                                                                      
   385    return r;                                                                                                                                                         
   386}         
   

   388void FunctionSigmoid::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                               
   389    PVariable x = inputs.at(0);                                                                                                                                       
   390                                                                                                                                                                      
   391                                                                                                                                                                      
   392    if (rr.get() == NULL || rr->data.cols != x->data.cols){                                                                                                           
   393        rr = PVariable(new Variable(x->data.rows, x->data.cols));                                                                                                        394    }                                                                                                                                                                 
   395    x->data.sigmoid_d(rr->data);                                                                                                                                      
   396                                                                                                                                                                      
   397    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);                                                                                                   
   398}                                                                                                                                                                     
   399                    
   

   400FunctionTanh::FunctionTanh() : Function() {                                                                                                                           
   401    name = "FunctionTanh";                                                                                                                                            
   402}                                                                                                                                                                     
   403PVariable FunctionTanh::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                             
   404                                                                                                                                                                      
   405    PVariable x = inputs.at(0);                                                                                                                                       
   406                                                                                                                                                                      
   407    PVariable r = variable_construct_for_function(this, x->data.rows, x->data.cols);                                                                                  
   408                                                                                                                                                                      
   409    outputs.push_back(r);                                                                                                                                             
   410                                                                                                                                                                      
   411    x->data.tanh(r->data);                                                                                                                                            
   412                                                                                                                                                                      
   413    float *tmp;                                                                                                                                                          414    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   415    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   416    free(tmp);                                                                                                                                                        
   417                                                                                                                                                                      
   418    return r;                                                                                                                                                         
   419}                                                                                                                                                                     
   420void FunctionTanh::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                  
   421    PVariable x = inputs.at(0);                                                                                                                                       
   422                                                                                                                                                                      
   423                                                                                                                                                                      
   424    if (rr.get() == NULL || rr->data.cols != x->data.cols){                                                                                                           
   425        rr = PVariable(new Variable(x->data.rows, x->data.cols));                                                                                                     
   426    }                                                                                                                                                                 
   427    x->data.tanh_d(rr->data);                                                                                                                                         
   428                                                                                                                                                                      
   429    if (x->isGetGrad) rr->data.mul_plus(p_grad, x->grad, 1.0, 1.0);                                                                                                   
   430}              
   

   432FunctionMeanSquaredError::FunctionMeanSquaredError() : Function() {                                                                                                   
   433    name = "FunctionMeanSquaredError";                                                                                                                                   434    loss = cuMat(1,1);                                                                                                                                                
   435    loss.ones();                                                                                                                                                      
   436}                                                                                                                                                                     
   437                                                                                                                                                                      
   438PVariable FunctionMeanSquaredError::forward(vector<PVariable > &inputs, vector<PVariable > &outputs){                                                                 
   439                                                                                                                                                                      
   440    PVariable x = inputs.at(0);                                                                                                                                       
   441    PVariable t = inputs.at(1);                                                                                                                                       
   442                                                                                                                                                                      
   443    float *tmp_x;                                                                                                                                                     
   444    tmp_x = (float*)calloc(x->data.cols * x->data.rows, sizeof(float));                                                                                               
   445    cublasGetVector(x->data.rows * x->data.cols, sizeof(float), x->data.mDevice, 1, tmp_x, 1 );                                                                       
   446                                                                                                                                                                      
   447    float *tmp_t;                                                                                                                                                     
   448    tmp_t = (float*)calloc(t->data.cols * t->data.rows, sizeof(float));                                                                                               
   449    cublasGetVector(t->data.rows * t->data.cols, sizeof(float), t->data.mDevice, 1, tmp_t, 1 );                                                                       
   450                                                                                                                                                                      
   451    free(tmp_x);                                                                                                                                                      
   452    free(tmp_t);                                                                                                                                                      
   453                                                                                                                                                                      
   454    if (rr.get() == NULL || rr->data.cols != x->data.cols){    
   455        rr = PVariable(new Variable(x->data));                                                                                                                        
   456    }                                                                                                                                                                 
   457                                                                                                                                                                      
   458    x->data.minus(t->data, rr->data);                                                                                                                                 
   459                                                                                                                                                                      
   460    rr->data.mul(rr->data, rr->data);                                                                                                                                 
   461                                                                                                                                                                      
   462    float sum = rr->data.sum();                                                                                                                                       
   463    sum /= (2*rr->data.cols);                                                                                                                                         
   464                                                                                                                                                                      
   465    PVariable r = variable_construct_for_function(this, loss.rows, loss.cols);                                                                                        
   466                                                                                                                                                                      
   467    outputs.push_back(r);                                                                                                                                             
   468                                                                                                                                                                      
   469    loss.mul(sum, r->data);                                                                                                                                           
   470                                                                                                                                                                      
   471    float *tmp;                                                                                                                                                       
   472    tmp = (float*)calloc(r->data.cols * r->data.rows, sizeof(float));                                                                                                 
   473    cublasGetVector(r->data.rows * r->data.cols, sizeof(float), r->data.mDevice, 1, tmp, 1 );                                                                         
   474    memcpy(r->data.mHost, tmp, r->data.rows * r->data.cols * sizeof(float));                                                                                          
   475    free(tmp);                                                                                                                                                        
   476                                                                                                                                                                      
   477    return r;                                                                                                                                                         
   478}       
   
   479void FunctionMeanSquaredError::backward(cuMat &p_grad, vector<PVariable > &inputs, vector<PVariable > &outputs){                                                      
   480    PVariable x = inputs.at(0);                                                                                                                                       
   481    PVariable t = inputs.at(1);                                                                                                                                       
   482                                                                                                                                                                      
   483    if (rr.get() == NULL || rr->data.cols != x->data.cols){                                                                                                           
   484                                                                                                                                                                      
   485        rr = PVariable(new Variable(x->data));                                                                                                                        
   486    }                                                                                                                                                                 
   487                                                                                                                                                                      
   488    x->data.minus(t->data, rr->data);                                                                                                                                 
   489    float batch_size = rr->data.cols;                                                                                                                                 
   490    if (x->isGetGrad) x->grad.plus(rr->data, x->grad);                                                                                                                
   491}    
  </pre>
