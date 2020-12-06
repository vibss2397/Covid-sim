#include <vector>
#include <random>
#include <unordered_map>

class Person{
public:
    int16_t bu_id;
    int16_t age;
    int16_t covid_state; // 0: safe(potential), 1 : in-risk, 2 : quarantine(infected), -1 : recovery
    int16_t wears_mask; // 0 : off, 1 : on
    int16_t university_status; // 0 : undergrad, 1: grad, 2 : faculty, 3 : other staff
    int16_t covid_counter;
    Person(int16_t a = -1, int16_t b = -1, int16_t c = 0, int16_t d = 0, int16_t e = -1, int16_t f = -1){
        university_status = intialize_uni_status(e);
        age = initialize_age(b);    
        if(a==-1){
            bu_id = rand(); //  sets bu_id to a random number.
        }else{
            bu_id = a;
        }
        covid_state = c; //  sets state to 'potential' if not initialized.
        wears_mask = d; // default false
        if(covid_state==2) covid_counter = 0;  // if a person is infected at start, 
        else covid_counter = -1;
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
    }
    int16_t initialize_age(int16_t b){
        if(b==-1){
            if(university_status==0 || university_status==1){
                return rand()%11 + 15; // age for uni students is between 15 and 27-28
            }else{
                return rand()%43 + 27; // age for faculty and staff is between 27-70
            }
        }else{
            return b;
        }
    }
    void set_mask(){
        wears_mask = (wears_mask == 1)? 0 : 1;
    }
    void set_state(int16_t temp){
        covid_state = temp;
    }
    std::unordered_map<std::string, int16_t> get_details(){
        std::unordered_map<std::string, int16_t> stats;
        stats.insert(std::pair<std::string, int16_t>("bu_id", bu_id));
        stats.insert(std::pair<std::string, int16_t>("age", age));
        stats.insert(std::pair<std::string, int16_t>("state", covid_state));
        stats.insert(std::pair<std::string, int16_t>("mask_on", wears_mask));
        stats.insert(std::pair<std::string, int16_t>("university_member", university_status));
        return stats;
    }
    void get_covid_state(){
        // uses poisson distribution with lambda = 10
        // function which uses the pdf of poisson to determine if person has covid in this time_step or not
    }

    void calculate_covid(){
        // uses factors such as : neighbors, age etc for calculating if a person has covid or not
    }
    
};