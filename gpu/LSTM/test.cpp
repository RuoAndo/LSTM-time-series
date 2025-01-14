#include <vector>
#include <iostream>
#include <chrono>

#include <sys/resource.h>

#include "graph.h"
#include "variable.h"
#include "model.h"
#include "batchdata.h"
#include "optimizer_adam.h"
#include "optimizer_sgd.h"
#include "optimizer_sgd_moment.h"
#include "optimizer_adagrad.h"

// #include "word_embed.h"

using namespace std;

MallocCounter mallocCounter;

// extern map<Variable *, bool> obj_pool2;


void use_getrusage(void)
{
    struct rusage r;
    if (getrusage(RUSAGE_SELF, &r) != 0) {
        /*Failure*/
    }
    printf("maxrss=%ld\n", r.ru_maxrss);
}


void toPVariable(PVariable x1, float *X){
    x1->data.memSetHost(X);
}

void createSinData(float data[], int steps_per_cycle, int number_of_cycles){
    for (int j=0; j<number_of_cycles; j++){
        for (int i=0; i<steps_per_cycle; i++){
            float v = std::sin(i * 2 * std::atan(1) * 4 /  steps_per_cycle);
            data[steps_per_cycle * j + i] = v;
        }
    }
}

Model model;

//LSTM model
PVariable forward_one_step(PVariable x, PVariable d){


    PVariable s_h = model.G("g_lstm")->forward(x);

    PVariable s_t = model.G("tanh")->forward(s_h);

    PVariable s_y = model.G("w_hy")->forward(s_t);

    PVariable loss = model.G("g_mean_squared_error")->forward(s_y, d);

    return loss;
}

PVariable predict(PVariable x){

    PVariable s_h = model.G("g_lstm")->forward(x);

    PVariable s_t = model.G("tanh")->forward(s_h);

    PVariable s_y = model.G("w_hy")->forward(s_t);

    return s_y;
}

int main(){

    int steps_per_cycle = 50;
    int number_of_cycles = 100;

    float sin_raw_data[steps_per_cycle*number_of_cycles];

    createSinData(sin_raw_data, steps_per_cycle, number_of_cycles);

    int epoch_size = 1000;
    int batch_size = 100;
    int bprop_len = 100;
    float learning_rate = 0.001; //Optimizer Adam
    float sdg_momentum = 0.7;

    int i_size = 1;
    int n_size = 5;
    int o_size = 1;

    cout << "batch_size:" << batch_size << " epoch_size:" << epoch_size << " bprop_len:" << bprop_len << endl;
    cout << "n_size:" << n_size << endl;

    int whole_len = steps_per_cycle * number_of_cycles;
    int jump = whole_len/batch_size;
    cout << whole_len << " " << batch_size << " " << jump << endl;

    cout << "create model..." << endl;
    model.putG("g_lstm", new FullLSTM2(n_size, i_size));
    model.putG("tanh", new Tanh());
    model.putG("w_hy", new Linear(o_size, n_size));
    model.putG("g_mean_squared_error", new MeanSquaredError());
    model.putG("g_loss_plus", new Plus());

    OptimizerAdam optimizer(&model, learning_rate);
    optimizer.init();

    std::chrono::system_clock::time_point  start, end;


    int epoch = 0;
    float cur_log_perp = 0;

    PVariable loss_sum(new Variable(1, 1));

    start = std::chrono::system_clock::now();

    cout << "going to train " << jump*epoch_size << " iterations" << " epoch:" << epoch_size << endl;

    for (int i=0; i<jump * epoch_size; i++){


        BatchData bdata(1, 1, batch_size);

        for (int j=0; j<batch_size; j++){
            int idx = (jump * j + i) % whole_len;
            int idx_1 = (jump * j + i + 1) % whole_len;
            bdata.X[j] = sin_raw_data[idx];
            bdata.D[j] = sin_raw_data[idx_1];
        }

        PVariable x(new Variable(1, batch_size, false));
        PVariable d(new Variable(1, batch_size, false));
        toPVariable(x, bdata.X);
        toPVariable(d, bdata.D);

        PVariable loss =  forward_one_step(x, d);

        cur_log_perp += loss->val();
        loss_sum = model.G("g_loss_plus")->forward(loss_sum, loss);

        if ((i+1) % bprop_len == 0){
            loss_sum->backward();
            optimizer.update();

            loss_sum->zeros();
            model.zero_grads();
            model.unchain();
            // We use Tensor-Flow Style BPTT as below
            // http://r2rt.com/styles-of-truncated-backpropagation.html
            model.G("g_lstm")->reset_state();
        }
        if ((i+1) % 1000 == 0){
            end = std::chrono::system_clock::now();
            int elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

            cout << "iter:" << (i+1) << "/jump:" << jump << "/epoch:" << epoch << "/total iters:" << jump * epoch_size
                 << " " << ((float) (i+1))/(float(jump * epoch_size))*100 << "%"
                 << " loss:" << cur_log_perp/1000
                << " iters/sec:" << ((float)1000)/((float)elapsed) << " timesec:" << elapsed << endl;


            start = std::chrono::system_clock::now();
            cur_log_perp = 0;
        }

        if (i % jump == 0){
            epoch++;
        }


    } //for epoch loop


    // prepare predict data
    cout << "prepare predict data" << endl;
    int input_seq = 50;
    int pre_length = 100;

    vector<float> predict_data;
    float final_pre = 0.0;
    for (int i=0; i<input_seq; i++) {

        predict_data.push_back(sin_raw_data[i]);

    }
    /*
    cout << "previus data --------------------------" << endl;
    for(int i=0; i<input_seq; i++){
        cout << predict_data[i] << ",";
    }
    cout << endl;
    */




    // predcit 1
    cout << "predict 1" << endl;
    model.G("g_lstm")->reset_state();
    model.unchain();

    PVariable r;
    for (int i=0; i<input_seq; i++) {

        BatchData bdata(1, 1, 1);

        bdata.X[0] = sin_raw_data[i];


        PVariable x(new Variable(1, 1, false));
        toPVariable(x, bdata.X);

        r = predict(x);

        final_pre = r->data(0,0);
        cout << sin_raw_data[i+1] << "," << final_pre << endl;


        model.unchain();
    }




    // predcit 2
    cout << "predict 2" << endl;
    model.G("g_lstm")->reset_state();
    model.unchain();


    for (int j=0; j<pre_length; j++) {

        for (int i=0; i<input_seq; i++) {

            BatchData bdata(1, 1, 1);

            bdata.X[0] = predict_data[i];


            PVariable x(new Variable(1, 1, false));
            toPVariable(x, bdata.X);


            r = predict(x);

            final_pre = r->data(0,0);


            model.unchain();
        }

        predict_data.erase(predict_data.begin());
        predict_data.push_back(final_pre);
        cout << (sin_raw_data[input_seq + j]) << "," << final_pre << endl;
    }
}

