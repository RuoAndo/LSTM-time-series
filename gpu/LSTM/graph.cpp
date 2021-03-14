#include "graph.h"

using namespace std;


Graph::Graph(){
}

Graph::~Graph() {
    init();
}

void Graph::init(){
    remove_chain();
}

void Graph::remove_chain(){

    funcs_chain.clear();
}

vector<Variable *> Graph::getParams() {
    vector < Variable * > params;

    return params;
}

PVariable Graph::forward(PVariable input) {

}
PVariable Graph::forward(PVariable x, PVariable t) {

}

void Graph::zero_grads() {}

void Graph::reset_state() {}


void Graph::toHostArray(){}
void Graph::fromHostArray(){}



Linear::Linear() : Graph() {

}
Linear::Linear(int output_size, int input_size, bool no_bias) : Graph() {

        noBias = no_bias;

        this->w = new Variable(output_size, input_size);
        this->w->randoms(0., sqrt(2.0/((float)input_size)));

        if (!noBias){
            this->b = new Variable(output_size, 1);
        }

    }

Linear::Linear(Variable *w, bool isTranspose) : Graph() {
    this->w = w;
    this->isTranpose = isTranspose;

    int output_size = w->data.rows;
    if (isTranspose){
        output_size = w->data.cols;
    }
    this->b = new Variable(output_size, 1);
}

Linear::~Linear(){
    if (this->w != NULL) delete this->w;
    if (this->b != NULL) delete this->b;
}


vector<Variable *> Linear::getParams(){
    vector<Variable *> params;
    params.push_back(w);
    if (!noBias) params.push_back(b);

    return params;
}

PVariable Linear::forward(PVariable v){

    Function *f;
    if (noBias)
        f = new FunctionLinear(w, isTranpose);
    else
        f = new FunctionLinear(w, b, isTranpose);

    PFunction pf(f);

    funcs_chain.push_back(pf);

    return pf->forward(v);
}

void Linear::zero_grads() {
    w->zero_grad();
    if (!noBias) b->zero_grad();
}

void Linear::toHostArray(){
    w->data.toHostArray();
    if (!noBias) b->data.toHostArray();
}
void Linear::fromHostArray(){
    w->data.fromHostArray();
    if (!noBias) b->data.fromHostArray();

}

PVariable Linear::forward(PVariable x, PVariable t){}

Sigmoid::Sigmoid() : Graph() {

}
PVariable Sigmoid::forward(PVariable v){
    Function *f = new FunctionSigmoid();
    PFunction pf(f);
    funcs_chain.push_back(pf);
    return pf->forward(v);
}

Tanh::Tanh() : Graph() {

}
PVariable Tanh::forward(PVariable v){
    Function *f = new FunctionTanh();
    PFunction pf(f);
    funcs_chain.push_back(pf);
    return pf->forward(v);
}

MeanSquaredError::MeanSquaredError() : Graph() {

}

PVariable MeanSquaredError::forward(PVariable v1, PVariable v2) {

    Function *f = new FunctionMeanSquaredError();
    PFunction pf(f);
    funcs_chain.push_back(pf);

    return pf->forward(v1, v2);
}


Plus::Plus() {

}
PVariable Plus::forward(PVariable v1, PVariable v2) {
    Function *f = new FunctionPlus();
    PFunction pf(f);
    funcs_chain.push_back(pf);

    return pf->forward(v1, v2);
}

FullLSTM2::FullLSTM2() : Graph() {

}
FullLSTM2::FullLSTM2(int output_size, int input_size) {

    this->output_size = output_size;
    this->input_size = input_size;


    //------------------------------------------------------
    f_c_w = new Variable(output_size, output_size);
    f_h_w = new Variable(output_size, output_size);
    f_x_w = new Variable(output_size, input_size);
    f_x_b = new Variable(output_size, 1);

    f_c_w->randoms(0., sqrt((1./(float)output_size)));
    f_h_w->randoms(0., sqrt((1./(float)output_size)));
    f_x_w->randoms(0., sqrt((1./(float)input_size)));
    // initialize forget gate bias to 1
    // for more detail, http://jmlr.org/proceedings/papers/v37/jozefowicz15.pdf
    f_x_b->data.ones();

    //------------------------------------------------------
    i_c_w = new Variable(output_size, output_size);
    i_h_w = new Variable(output_size, output_size);
    i_x_w = new Variable(output_size, input_size);
    i_x_b = new Variable(output_size, 1);

    i_c_w->randoms(0., sqrt((1./(float)input_size)));
    i_h_w->randoms(0., sqrt((1./(float)input_size)));
    i_x_w->randoms(0., sqrt((1./(float)input_size)));

    //------------------------------------------------------
    o_c_w = new Variable(output_size, output_size);
    o_h_w = new Variable(output_size, output_size);
    o_x_w = new Variable(output_size, input_size);
    o_x_b = new Variable(output_size, 1);

    o_c_w->randoms(0., sqrt((1./(float)output_size)));
    o_h_w->randoms(0., sqrt((1./(float)output_size)));
    o_x_w->randoms(0., sqrt((1./(float)input_size)));

    //------------------------------------------------------
    g_h_w = new Variable(output_size, output_size);
    g_x_w = new Variable(output_size, input_size);
    g_x_b = new Variable(output_size, 1);

    g_h_w->randoms(0., sqrt((1./(float)output_size)));
    g_x_w->randoms(0., sqrt((1./(float)input_size)));


    //------------------------------------------------------
    gamma_f = new Variable(output_size, 1);
    gamma_i = new Variable(output_size, 1);
    gamma_g = new Variable(output_size, 1);
    gamma_o = new Variable(output_size, 1);
    beta_f = new Variable(output_size, 1);
    beta_i = new Variable(output_size, 1);
    beta_g = new Variable(output_size, 1);
    beta_o = new Variable(output_size, 1);

    gamma_f->randoms(0., sqrt((1./(float)output_size)));
    gamma_i->randoms(0., sqrt((1./(float)output_size)));
    gamma_g->randoms(0., sqrt((1./(float)output_size)));
    gamma_o->randoms(0., sqrt((1./(float)output_size)));


    x_mean_f = new Variable(output_size, 1);
    x_mean_i = new Variable(output_size, 1);
    x_mean_g = new Variable(output_size, 1);
    x_mean_o = new Variable(output_size, 1);
    x_var_f = new Variable(output_size, 1);
    x_var_i = new Variable(output_size, 1);
    x_var_g = new Variable(output_size, 1);
    x_var_o = new Variable(output_size, 1);

}

FullLSTM2::~FullLSTM2() {
    delete f_c_w; delete f_h_w; delete f_x_w; delete f_x_b;
    delete i_c_w; delete i_h_w; delete i_x_w; delete i_x_b;
    delete o_c_w; delete o_h_w; delete o_x_w; delete o_x_b;
    delete g_h_w; delete g_x_w; delete g_x_b;

    delete gamma_f;
    delete gamma_i;
    delete gamma_g;
    delete gamma_o;
    delete beta_f;
    delete beta_i;
    delete beta_g;
    delete beta_o;

    delete x_mean_f;
    delete x_mean_i;
    delete x_mean_g;
    delete x_mean_o;
    delete x_var_f;
    delete x_var_i;
    delete x_var_g;
    delete x_var_o;


}


vector<Variable *> FullLSTM2::getParams(){
    vector<Variable *> params;

    params.push_back(f_c_w);
    params.push_back(f_h_w);
    params.push_back(f_x_w);
    params.push_back(f_x_b);

    params.push_back(i_c_w);
    params.push_back(i_h_w);
    params.push_back(i_x_w);
    params.push_back(i_x_b);

    params.push_back(o_c_w);
    params.push_back(o_h_w);
    params.push_back(o_x_w);
    params.push_back(o_x_b);

    params.push_back(g_h_w);
    params.push_back(g_x_w);
    params.push_back(g_x_b);

    params.push_back(gamma_f);
    params.push_back(gamma_i);
    params.push_back(gamma_g);
    params.push_back(gamma_o);
    params.push_back(beta_f);
    params.push_back(beta_i);
    params.push_back(beta_g);
    params.push_back(beta_o);

    params.push_back(x_mean_f);
    params.push_back(x_mean_i);
    params.push_back(x_mean_g);
    params.push_back(x_mean_o);
    params.push_back(x_var_f);
    params.push_back(x_var_i);
    params.push_back(x_var_g);
    params.push_back(x_var_o);

    return params;
}


void FullLSTM2::set_train_status(bool status){
    is_train = status;
}

PVariable FullLSTM2::forward(PVariable x) {


    // prepare function
    PFunction p_f_x(new FunctionLinear(f_x_w, f_x_b));
    funcs_chain.push_back(p_f_x);
    PFunction p_f_h(new FunctionLinear(f_h_w));
    funcs_chain.push_back(p_f_h);
    PFunction p_f_c(new FunctionLinear(f_c_w));
    funcs_chain.push_back(p_f_c);
    PFunction p_f_sig(new FunctionSigmoid());
    funcs_chain.push_back(p_f_sig);
    PFunction p_f_sum1(new FunctionPlus());
    funcs_chain.push_back(p_f_sum1);
    PFunction p_f_sum2(new FunctionPlus());
    funcs_chain.push_back(p_f_sum2);

    PFunction p_i_x(new FunctionLinear(i_x_w, i_x_b));
    funcs_chain.push_back(p_i_x);
    PFunction p_i_h(new FunctionLinear(i_h_w));
    funcs_chain.push_back(p_i_h);
    PFunction p_i_c(new FunctionLinear(i_c_w));
    funcs_chain.push_back(p_i_c);
    PFunction p_i_sig(new FunctionSigmoid());
    funcs_chain.push_back(p_i_sig);
    PFunction p_i_sum1(new FunctionPlus());
    funcs_chain.push_back(p_i_sum1);
    PFunction p_i_sum2(new FunctionPlus());
    funcs_chain.push_back(p_i_sum2);

    PFunction p_g_x(new FunctionLinear(g_x_w, g_x_b));
    funcs_chain.push_back(p_g_x);
    PFunction p_g_h(new FunctionLinear(g_h_w));
    funcs_chain.push_back(p_g_h);
    PFunction p_g_tanh(new FunctionTanh());
    funcs_chain.push_back(p_g_tanh);
    PFunction p_g_sum(new FunctionPlus());
    funcs_chain.push_back(p_g_sum);

    PFunction p_c_mul1(new FunctionMul());
    funcs_chain.push_back(p_c_mul1);
    PFunction p_c_mul2(new FunctionMul());
    funcs_chain.push_back(p_c_mul2);
    PFunction p_c_plus(new FunctionPlus());
    funcs_chain.push_back(p_c_plus);

    PFunction p_o_x(new FunctionLinear(o_x_w, o_x_b));
    funcs_chain.push_back(p_o_x);
    PFunction p_o_h(new FunctionLinear(o_h_w));
    funcs_chain.push_back(p_o_h);
    PFunction p_o_c(new FunctionLinear(o_c_w));
    funcs_chain.push_back(p_o_c);
    PFunction p_o_sig(new FunctionSigmoid());
    funcs_chain.push_back(p_o_sig);
    PFunction p_o_sum1(new FunctionPlus());
    funcs_chain.push_back(p_o_sum1);
    PFunction p_o_sum2(new FunctionPlus());
    funcs_chain.push_back(p_o_sum2);

    PFunction p_h_tanh(new FunctionTanh());
    funcs_chain.push_back(p_h_tanh);
    PFunction p_h_mul(new FunctionMul());
    funcs_chain.push_back(p_h_mul);


    //--------------------------------------------


    if (c.get() == NULL || c->data.rows == 0 || c->data.cols != x->data.cols) {
        c = PVariable(variable_construct(output_size, x->data.cols), variable_destroy);
    }


    if (h.get() == NULL || h->data.rows == 0 || h->data.cols != x->data.cols) {
        h = PVariable(variable_construct(output_size, x->data.cols), variable_destroy);
        h->opt = id;

        last_opt = id;

        h->last_opt = &last_opt;
        h->is_last_backward = &is_last_backward;
        id++;
    }

    PVariable f_x, i_x, g_x, o_x;

    f_x = p_f_x->forward(x);
    i_x = p_i_x->forward(x);
    g_x = p_g_x->forward(x);
    o_x = p_o_x->forward(x);

    PVariable f_sum = p_f_sum1->forward(p_f_sum2->forward(f_x, p_f_h->forward(h)), p_f_c->forward(c));
    PVariable f = p_f_sig->forward(f_sum);

    PVariable i_sum = p_i_sum1->forward(p_i_sum2->forward(i_x, p_i_h->forward(h)), p_i_c->forward(c));
    PVariable i = p_i_sig->forward(i_sum);

    PVariable g_sum = p_g_sum->forward(g_x, p_g_h->forward(h));
    PVariable g = p_g_tanh->forward(g_sum);


    c = p_c_plus->forward(
            p_c_mul1->forward(i, g),
            p_c_mul2->forward(f, c)
    );



    PVariable o_sum = p_o_sum1->forward(p_o_sum2->forward(o_x, p_o_h->forward(h)), p_o_c->forward(c));
    PVariable o = p_o_sig->forward(o_sum);


    h = p_h_mul->forward(o, p_h_tanh->forward(c));

    float *tmp;
    tmp = (float*)calloc(h->data.cols * h->data.rows, sizeof(float));
    cublasGetVector(h->data.rows * h->data.cols, sizeof(float), h->data.mDevice, 1, tmp, 1 );
    free(tmp);

    h->opt = id;

    last_opt = id;

    h->last_opt = &last_opt;
    h->is_last_backward = &is_last_backward;
    id++;

    return h;
}


void FullLSTM2::reset_state(){

    if (f_c_w->grad.mDevice != NULL) f_c_w->grad *= 0;
    if (f_h_w->grad.mDevice != NULL) f_h_w->grad *= 0;
    if (f_x_w->grad.mDevice != NULL) f_x_w->grad *= 0;
    if (f_x_b->grad.mDevice != NULL) f_x_b->grad *= 0;

    if (i_c_w->grad.mDevice != NULL) i_c_w->grad *= 0;
    if (i_h_w->grad.mDevice != NULL) i_h_w->grad *= 0;
    if (i_x_w->grad.mDevice != NULL) i_x_w->grad *= 0;
    if (i_x_b->grad.mDevice != NULL) i_x_b->grad *= 0;

    if (o_c_w->grad.mDevice != NULL) o_c_w->grad *= 0;
    if (o_h_w->grad.mDevice != NULL) o_h_w->grad *= 0;
    if (o_x_w->grad.mDevice != NULL) o_x_w->grad *= 0;
    if (o_x_b->grad.mDevice != NULL) o_x_b->grad *= 0;

    if (g_h_w->grad.mDevice != NULL) g_h_w->grad *= 0;
    if (g_x_w->grad.mDevice != NULL) g_x_w->grad *= 0;
    if (g_x_b->grad.mDevice != NULL) g_x_b->grad *= 0;


    if (gamma_f->grad.mDevice != NULL) gamma_f->grad *= 0;
    if (gamma_i->grad.mDevice != NULL) gamma_i->grad *= 0;
    if (gamma_g->grad.mDevice != NULL) gamma_g->grad *= 0;
    if (gamma_o->grad.mDevice != NULL) gamma_o->grad *= 0;
    if (beta_f->grad.mDevice != NULL) beta_f->grad *= 0;
    if (beta_i->grad.mDevice != NULL) beta_i->grad *= 0;
    if (beta_g->grad.mDevice != NULL) beta_g->grad *= 0;
    if (beta_o->grad.mDevice != NULL) beta_o->grad *= 0;

    c->zeros();
    c->unchain();

    h->zeros();
    h->unchain();

    last_opt = 0;
    is_last_backward = false;

    id = 0;

    h->opt = id;

    last_opt = id;

    h->last_opt = &last_opt;
    h->is_last_backward = &is_last_backward;
    id++;
}

void FullLSTM2::zero_grads() {
    f_c_w->zero_grad();
    f_h_w->zero_grad();
    f_x_w->zero_grad();
    f_x_b->zero_grad();

    i_c_w->zero_grad();
    i_h_w->zero_grad();
    i_x_w->zero_grad();
    i_x_b->zero_grad();

    o_c_w->zero_grad();
    o_h_w->zero_grad();
    o_x_w->zero_grad();
    o_x_b->zero_grad();

    g_h_w->zero_grad();
    g_x_w->zero_grad();
    g_x_b->zero_grad();

    gamma_f->zero_grad();
    gamma_i->zero_grad();
    gamma_g->zero_grad();
    gamma_o->zero_grad();
    beta_f->zero_grad();
    beta_i->zero_grad();
    beta_g->zero_grad();
    beta_o->zero_grad();


    c->zero_grad();
    h->zero_grad();

}


void FullLSTM2::toHostArray(){


    f_c_w->data.toHostArray();
    f_h_w->data.toHostArray();
    f_x_w->data.toHostArray();
    f_x_b->data.toHostArray();

    i_c_w->data.toHostArray();
    i_h_w->data.toHostArray();
    i_x_w->data.toHostArray();
    i_x_b->data.toHostArray();

    o_c_w->data.toHostArray();
    o_h_w->data.toHostArray();
    o_x_w->data.toHostArray();
    o_x_b->data.toHostArray();

    g_h_w->data.toHostArray();
    g_x_w->data.toHostArray();
    g_x_b->data.toHostArray();

    x_mean_f->data.toHostArray();
    x_mean_i->data.toHostArray();
    x_mean_g->data.toHostArray();
    x_mean_o->data.toHostArray();
    x_var_f->data.toHostArray();
    x_var_i->data.toHostArray();
    x_var_g->data.toHostArray();
    x_var_o->data.toHostArray();

    gamma_f->data.toHostArray();
    beta_f->data.toHostArray();
    gamma_i->data.toHostArray();
    beta_i->data.toHostArray();
    gamma_g->data.toHostArray();
    beta_g->data.toHostArray();
    gamma_o->data.toHostArray();
    beta_o->data.toHostArray();


}
void FullLSTM2::fromHostArray(){

    f_c_w->data.fromHostArray();
    f_h_w->data.fromHostArray();
    f_x_w->data.fromHostArray();
    f_x_b->data.fromHostArray();

    i_c_w->data.fromHostArray();
    i_h_w->data.fromHostArray();
    i_x_w->data.fromHostArray();
    i_x_b->data.fromHostArray();

    o_c_w->data.fromHostArray();
    o_h_w->data.fromHostArray();
    o_x_w->data.fromHostArray();
    o_x_b->data.fromHostArray();

    g_h_w->data.fromHostArray();
    g_x_w->data.fromHostArray();
    g_x_b->data.fromHostArray();

    x_mean_f->data.fromHostArray();
    x_mean_i->data.fromHostArray();
    x_mean_g->data.fromHostArray();
    x_mean_o->data.fromHostArray();
    x_var_f->data.fromHostArray();
    x_var_i->data.fromHostArray();
    x_var_g->data.fromHostArray();
    x_var_o->data.fromHostArray();

    gamma_f->data.fromHostArray();
    beta_f->data.fromHostArray();
    gamma_i->data.fromHostArray();
    beta_i->data.fromHostArray();
    gamma_g->data.fromHostArray();
    beta_g->data.fromHostArray();
    gamma_o->data.fromHostArray();
    beta_o->data.fromHostArray();

}

void BatchNorm::setTrainStatus(bool status) {
    is_train = status;
}

void BatchNorm::toHostArray() {
    gamma->data.toHostArray();
    beta->data.toHostArray();
}

void BatchNorm::fromHostArray() {
    gamma->data.fromHostArray();
    beta->data.fromHostArray();
}

void BatchNorm::zero_grads() {
    gamma->zero_grad();
    beta->zero_grad();
}



