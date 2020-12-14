#include <vector>
#include <random>
#include <unordered_map>
#include <math.h>
#include<iostream>
#include <utility>
#include "Graph.cpp"

double factorial(int16_t n){
    // calculates n! = n*(n-1)*(n-2).....*1
    double temp = 1;
    for(int16_t i = 1;i<=n;i++){
        temp*=i;
    }
    return temp;
}
double poisson_pdf(int16_t mu, int16_t x){
    double num = exp(-mu)*pow(mu, x);
    double den = factorial(x);
    return num/den;
}

class Person{
public:
    int16_t bu_id;
    int16_t age;
    int16_t covid_state[2]; // 0: safe(potential), 1 : infected, 2 : symptomatic, 3 : quarantine, 4: recovery, -1 : dead
    int16_t wears_mask; // 0 : off, 1 : on
    int16_t university_status; // 0 : undergrad, 1: grad, 2 : faculty, 3 : other staff
    int16_t covid_counter;
    int16_t covid_mu_factor;
    Person(int16_t id = -1, int16_t age_arg = -1, int16_t cov_state = 0, int16_t mask_state = 0, int16_t uni_Status = -1){
        university_status = intialize_uni_status(uni_Status);
        age = initialize_age(age_arg);    
        if(id==-1){
            bu_id = rand(); //  sets bu_id to a random number.
        }else{
            bu_id = id;
        }
        covid_state[0] = cov_state; //  sets state to 'potential' if not initialized.
        covid_state[1] = cov_state; //  sets state to 'potential' if not initialized.
        wears_mask = mask_state; // default false
        if(covid_state[1]==1) covid_counter = 1;  // if a person is infected at start, 
        else covid_counter = -1;
        covid_mu_factor = set_covid_mu();
    };
    int16_t intialize_uni_status(int16_t e){
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
    int16_t initialize_age(int16_t b){
        if(b==-1){
            if(university_status==0 || university_status==1){
                return rand()%14 + 15; // age for uni students is between 15 and 28-29
            }else{
                return rand()%54 + 30; // age for faculty and staff is between 30-80
            }
        }else{
            return b;
        }
    };
    void set_mask(){
        wears_mask = (wears_mask == 1)? 0 : 1;
    };
    void set_state(int16_t temp){
        covid_state[1] = temp;
    };
    int16_t set_covid_mu(){
        // Data for setting covid_mu obtained from : https://www.cdc.gov/coronavirus/2019-ncov/hcp/duration-isolation.html.
        // For mild cases, people remain infected for upto 10 days.
        // For severe cases, people remain infected for upto 20 days.
        // This function calculates the covid_mu_factor for determining severity of covid based on age, since that is an inherent factor.
        // People with more age will be more susceptible to contracting severe covid symptoms than younger people.
        int16_t temp;
        if(university_status==0 || university_status==1){ 
            temp = rand()%(10-5+1) + 5; //undergrad or grad students
        }
        else{ // Faculty and other staff
            if(age>=30 && age<40)temp = 10;
            else if(age>=40 && age<49) temp = 11;
            else if(age>=50 && age<59) temp = 12;
            else if(age>=60 && age<69) temp = 13;
            else if(age>=70 && age<80) temp = 14;
        }
        return temp;
    };
    std::unordered_map<std::string, int16_t> get_details(){
        // When called, returns a map object containing person's information.
        std::unordered_map<std::string, int16_t> stats;
        stats.insert(std::pair<std::string, int16_t>("bu_id", bu_id));
        stats.insert(std::pair<std::string, int16_t>("age", age));
        stats.insert(std::pair<std::string, int16_t>("state", covid_state[1]));
        stats.insert(std::pair<std::string, int16_t>("mask_on", wears_mask));
        stats.insert(std::pair<std::string, int16_t>("university_member", university_status));
        return stats;
    };
    void update_covid_state(){ // called at the end of timestep.

        // uses poisson distribution with lambda = covid_mu (initialized in constructor).
        // function uses the pdf of poisson to determine which state of covid person is in and make possible transitions.
        // Uses data provided by CDC : https://www.cdc.gov/nchs/nvss/vsrr/covid_weekly/index.htm to determine death rate for age groups.
        covid_state[0] = covid_state[1];
        if(covid_state[0]==0 || covid_state[0]==-1){
            // If safe or dead, do nothing.
        }
        double pdf_value = poisson_pdf(covid_mu_factor, covid_counter);
        if(pdf_value>=0.01){
            if(covid_state[1]==1){covid_state[1] = 2;}
            else if(covid_state[1]==2 || covid_state[1]==3){
                // Determine death based on day
                if(covid_counter>=16){
                    // based on probability see if they can be killed.
                    float rand_nb = (float)rand()/(float)(RAND_MAX/100);
                    if(covid_mu_factor<10){
                        if(rand_nb<=1){
                            covid_state[1] = 4;
                        }
                    }else if(covid_mu_factor==10){
                        if(rand_nb<=2){
                            covid_state[1] = 4;
                        }

                    }else if(covid_mu_factor==11){
                        if(rand_nb<=5){
                            covid_state[1] = 4;
                        }

                    }else if(covid_mu_factor==12){
                        if(rand_nb<=12){
                            covid_state[1] = 4;
                        }
                    }else if(covid_mu_factor==13){
                        if(rand_nb<=20){
                            covid_state[1] = 4;
                        }

                    }else if(covid_mu_factor==14){
                        if(rand_nb<=30){
                            covid_state[1] = 4;
                        }
                    }

                }else if(covid_counter>=20){
                    // Kill high risk group and recover everyone else.
                    if(covid_mu_factor>=18){ // do i call sever all bonds here
                        covid_state[1] = -1;
                    }else{
                        covid_state[1] = 4; 
                    }
                }
            }
        }else{
            if(covid_state[1]==2 || covid_state[1]==3){
                // recover everyone here.
                covid_state[1] = 4;
            }
        }
        covid_counter += 1;
    }

    void calculate_covid(Graph graph_instance){ // called at the end of each timestep(after update_covid_step()).
        // uses factors such as : neighbors, mask_on/mask_off etc for calculating if a person has covid or not.
        // Data for figuring out how much do masks save : https://www.ucdavis.edu/coronavirus/news/your-mask-cuts-own-risk-65-percent/
        // 
        std::vector<std::pair<Person, float>> neighbors = graph_instance.get_neighbors();
        float numerator = 0.0;
        float denominator = 0.0;
        for(auto neighbor:neighbors){
            int covid_factor = (neighbor.first.covid_state[0]==1||neighbor.first.covid_state[0]==2)?2:1; //neighbors who are infected or symptomatic weighed more than others
            float mask_factor = (neighbor.first.wears_mask==true&&covid_factor==2)?0.35:1.0;
            numerator+=neighbor.second*mask_factor*covid_factor;
            denominator+=neighbor.second;
        }
        float weighted_covid_state = numerator/denominator;
        if(local_covid_state>)
    };
    
};

int main(){
    Person p(300, 50, 1, 0, 2);
    Person p2(320, 60, 1, 0, 2);
    for(int i = 0;i<20;i++){
        p.update_covid_state();
        p2.update_covid_state();
    }
    return 0;
}