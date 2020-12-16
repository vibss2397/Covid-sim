#include <vector>
#include <random>
#include <unordered_map>
#include <ctime>
#include "Graph.hpp"
#include "Helper_Func.hpp"
#include "Person.hpp"
#include <stdio.h>

Person::Person(int16_t id, int16_t uni_Status, int16_t age_arg, int16_t cov_state, int16_t mask_state){
    if(id==-1){
        // sets bu_id to a random number.
        bu_id = rand(); 
    }else{
        bu_id = id;
    }
    university_status = intialize_uni_status(uni_Status);
    age = initialize_age(age_arg);    
    // sets state to 'potential' if not initialized.
    covid_state[0] = cov_state; 
    // sets state to 'potential' if not initialized.
    covid_state[1] = cov_state; 
    // default false
    wears_mask = mask_state; 
    // if a person is infected at start, 
    if(covid_state[1]==1) covid_counter = 1;  
    else covid_counter = -1;
    covid_mu_factor = set_covid_mu();
};

int16_t Person::intialize_uni_status(int16_t e){
    if(e==-1){
        // Undergrad : 17000(37%), grad : 15000(33%), faculty : 4000(9%), other : 10000(21%), total : 46000.
        int rand_nb = rand() %100;
        if(rand_nb<=36) {return 0;}
        else if(rand_nb>36 && rand_nb<70) {return 1;}
        else if(rand_nb>=70 && rand_nb<79) {return 2;}
        else {return 3;}
    }else{
        return e;
    }
};

int16_t Person::initialize_age(int16_t b){
    if(b==-1){
        if(university_status==0 || university_status==1){
            // age for uni students is between 15 and 28-29
            return rand()%14 + 15; 
        }else{
            // age for faculty and staff is between 30-80
            return rand()%54 + 30; 
        }
    }else{
        return b;
    }
};

void Person::set_mask(){
    wears_mask = 1;
};

void Person::set_state(int16_t temp){
    covid_state[1] = temp;
};

/**
 * @brief This function calculates the covid_mu_factor for determining severity of covid based on age, since that is an inherent factor.
 * People with more age will be more susceptible to contracting severe covid symptoms than younger people.
 * Data for setting covid_mu obtained from : https://www.cdc.gov/coronavirus/2019-ncov/hcp/duration-isolation.html.
 * For mild cases, people remain infected for up to 10 days.
 * For severe cases, people remain infected for up to 20 days.
 * @return int16_t 
 */
int16_t Person::set_covid_mu(){
    int16_t temp;
    // younger students have mu from [5, 10] (will help in recovering early, less death).
    if(age<30) temp = rand()%(10-5+1) + 5;
    else if(age>=30 && age<40)temp = 10;
    else if(age>=40 && age<50) temp = 11;
    else if(age>=50 && age<60) temp = 12;
    else if(age>=60 && age<70) temp = 13;
    else if(age>=70) temp = 14;
    return temp;
};

/**
 * @brief When called, returns a map object containing person's information.
 * 
 * @return std::unordered_map<std::string, int16_t> 
 */
std::unordered_map<std::string, int16_t> Person::get_details(){
    std::unordered_map<std::string, int16_t> stats;
    stats.insert(std::pair<std::string, int16_t>("bu_id", bu_id));
    stats.insert(std::pair<std::string, int16_t>("age", age));
    stats.insert(std::pair<std::string, int16_t>("state", covid_state[1]));
    stats.insert(std::pair<std::string, int16_t>("mask_on", wears_mask));
    stats.insert(std::pair<std::string, int16_t>("university_member", university_status));
    return stats;
};

/**
 * @brief called at the end of timestep. uses poisson distribution with lambda = covid_mu (initialized in constructor).
 * function uses the pdf of poisson to determine which state of covid person is in and make possible transitions.
 * Uses data provided by CDC : https://www.cdc.gov/nchs/nvss/vsrr/covid_weekly/index.htm to determine death rate for age groups.
 */
void Person::update_covid_state(){ 
    covid_state[0] = covid_state[1];
    // If person is infected, symptomatic or quarantine, only then update state.
    if(covid_state[0]==1||covid_state[0]==2 || covid_state[0]==3){ 
        double pdf_value = poisson_pdf(covid_mu_factor, covid_counter);
        if(pdf_value>=0.01){
            if(covid_state[1]==1){covid_state[1] = 2;}
            else if(covid_state[1]==2 || covid_state[1]==3){
                // Determine death based on day
                if(covid_counter>=16 && covid_counter<20){
                    // based on probability see if they can be killed.
                    float rand_nb = ((float)rand()*100)/(float) RAND_MAX;
                    if(covid_mu_factor<10){
                        if(rand_nb<=1)
                            covid_state[1] = -1;
                    }else if(covid_mu_factor==10){
                        if(rand_nb<=2)
                            covid_state[1] = -1;
                    }else if(covid_mu_factor==11){
                        if(rand_nb<=5)
                            covid_state[1] = -1;
                    }else if(covid_mu_factor==12){
                        if(rand_nb<=12)
                            covid_state[1] = -1;
                    }else if(covid_mu_factor==13){
                        if(rand_nb<=20)
                            covid_state[1] = -1;
                    }else if(covid_mu_factor==14){
                        if(rand_nb<=30)
                            covid_state[1] = -1;
                    }

                }else if(covid_counter>=20){
                    // Kill high risk group and recover everyone else.
                    if(covid_mu_factor>=13){
                        covid_state[1] = -1;
                    }else{
                        covid_state[1] = 4; 
                    }
                }
            }
        }
        else{
            if(covid_state[0]==2 || covid_state[0]==3){
                // recover everyone here.
                covid_state[1] = 4;
            }
        }
    }
    covid_counter += 1;
}

/**
 * @brief called at the end of each timestep(after update_covid_step()). 
 * uses factors such as : neighbors, mask_on/mask_off etc for calculating if a person has covid or not.
 * Data for figuring out how much do masks reduce transmission : https://www.ucdavis.edu/coronavirus/news/your-mask-cuts-own-risk-65-percent/
 * 
 * @param graph_instance 
 */
void Person::calculate_covid(Graph graph_instance){ 
    std::vector<std::pair<Person*, float>> neighbors = graph_instance.get_neighbors(bu_id);
    
    float probs_not_getting_covid = 1.0;

    float weighted_probs = 1.0;
    for(auto neighbor:neighbors){
        float covid_factor;
        if(neighbor.first->covid_state[0]==0||neighbor.first->covid_state[0]==3||neighbor.first->covid_state[0]==4||neighbor.first->covid_state[0]==-1){
            covid_factor = 0; // covid factor is 0 for quarantine, recovered and safe
        }else if(neighbor.first->covid_state[0]==1){
            covid_factor = 0.3; // 0.3 for when person is not symptomatic but infected(low amount of viral load).
        }else{
            covid_factor = 1; // 1 for when person is symptomatic.
        }
        float mask_factor = (neighbor.first->wears_mask==true&&(covid_factor==0.3 || covid_factor==1))?0.35:1.0;
        float pr_getting_covid_from_neighbor = neighbor.second*(mask_factor*covid_factor);
        probs_not_getting_covid*=(1 - pr_getting_covid_from_neighbor);
    }
    float probs_getting_covid = 1 - probs_not_getting_covid;
    float rand_covid_die = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // std::cout<<bu_id<<"::"<<rand_covid_die<<"::"<<probs_getting_covid<<std::endl;
    if(rand_covid_die<=probs_getting_covid && covid_state[0] == 0){
        // the person gets infected with a chance of product of weighted_probs
        covid_state[1] = 1; 
        covid_counter = 1;
    }
}