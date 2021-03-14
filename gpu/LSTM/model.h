/*
 * function_set.h
 *
 */


#ifndef FUNCTION_SET_H_
#define FUNCTION_SET_H_

#include <typeinfo>
#include <vector>
#include <map>

#include <fstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "variable.h"
#include "graph.h"

using namespace std;

class UpdateParams {
public:
    vector<Variable *> params;


    void add(Variable *v){
        params.push_back(v);
    }
};

class Model {
private:


    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
        ar & graphs;
    }


public:
    map<string, Graph *> graphs;
    vector<UpdateParams *> updateParams;

    ~Model(){
        for (int i=0; i<updateParams.size(); i++){
            delete updateParams.at(i);
        }
    }

    void putG(string name, Graph *f){
        graphs[name] = f;
    }
    Graph *G(string name){
        return graphs.at(name);
    }

    vector<UpdateParams *> &getUpdateParams(){
        for(auto gs : graphs){
            Graph *g = gs.second;

            const type_info& id = typeid(*g);

            if (typeid(Linear) == id){
                UpdateParams *p = new UpdateParams();
                Variable *w = ((Linear *)g)->w;
                p->add(w);
                if (!((Linear *)g)->noBias){
                    Variable *b = ((Linear *)g)->b;
                    p->add(b);
                }
                updateParams.push_back(p);

            }else if (typeid(FullLSTM2) == id){

                UpdateParams *p = new UpdateParams();
                FullLSTM2 *lstm = (FullLSTM2 *)g;

                p->add(lstm->f_c_w); p->add(lstm->f_h_w); p->add(lstm->f_x_w); p->add(lstm->f_x_b);
                p->add(lstm->i_c_w); p->add(lstm->i_h_w); p->add(lstm->i_x_w); p->add(lstm->i_x_b);
                p->add(lstm->o_c_w); p->add(lstm->o_h_w); p->add(lstm->o_x_w); p->add(lstm->o_x_b);
                p->add(lstm->g_h_w); p->add(lstm->g_x_w); p->add(lstm->g_x_b);

                p->add(lstm->gamma_f); p->add(lstm->beta_f);
                p->add(lstm->gamma_i); p->add(lstm->beta_i);
                p->add(lstm->gamma_g); p->add(lstm->beta_g);
                p->add(lstm->gamma_o); p->add(lstm->beta_o);

                updateParams.push_back(p);

            }
        }
        return updateParams;
    }


    void save(string path){
        for(auto gs : graphs){
            Graph *g = gs.second;

            const type_info& id = typeid(*g);

            if (typeid(Linear) == id){
                ((Linear *)g)->toHostArray();
            }else if (typeid(FullLSTM2) == id){
                ((FullLSTM2 *)g)->toHostArray();
            } else if (typeid(BatchNorm) == id){
                ((BatchNorm *)g)->toHostArray();
            }



        }

        std::ofstream ofs(path);
        boost::archive::binary_oarchive oa(ofs);
        oa.register_type<Linear>(); // add if you define new function
        // oa.register_type<LSTM>(); // add if you define new function
        // oa.register_type<FullLSTM>(); // add if you define new function
        oa.register_type<FullLSTM2>(); // add if you define new function
        oa.register_type<Tanh>(); // add if you define new function
        oa.register_type<MeanSquaredError>(); // add if you define new function
        oa.register_type<Plus>(); // add if you define new function
        // oa.register_type<BatchNorm>(); // add if you define new function
        oa << *this;

        ofs.close();
    }
    void load(string path){
        std::ifstream ifs(path);
        boost::archive::binary_iarchive ia(ifs);
        ia.register_type<Linear>(); // add if you define new function
        // ia.register_type<LSTM>(); // add if you define new function
        // ia.register_type<FullLSTM>(); // add if you define new function
        ia.register_type<FullLSTM2>(); // add if you define new function
        ia.register_type<Tanh>(); // add if you define new function
        ia.register_type<MeanSquaredError>(); // add if you define new function
        ia.register_type<Plus>(); // add if you define new function
        // ia.register_type<BatchNorm>(); // add if you define new function
        ia >> *this;

        ifs.close();

        for(auto gs : graphs){

            Graph *g = gs.second;

            const type_info& id = typeid(*g);

            if (typeid(Linear) == id){
                ((Linear *)g)->fromHostArray();
            }
	    /*
            else if (typeid(LSTM) == id){
                ((LSTM *)g)->fromHostArray();
            }
	    */
	    /*
            else if (typeid(FullLSTM) == id){
                ((FullLSTM *)g)->fromHostArray();
            }
	    */
            else if (typeid(FullLSTM2) == id){
                ((FullLSTM2 *)g)->fromHostArray();
            }
	    /*
            else if (typeid(BatchNorm) == id){
                ((BatchNorm *)g)->fromHostArray();
            }
	    */
        }

        getUpdateParams();
    }

    void unchain(){
        for(auto gs : graphs) {

            Graph *g = gs.second;

            g->remove_chain();
        }
    }

    void zero_grads(){
        for(auto gs : graphs) {

            Graph *g = gs.second;

            g->zero_grads();
        }
    }
};

#endif /* FUNCTION_SET_H_ */
